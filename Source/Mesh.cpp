#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include "tiny_gltf.h"

#include "Mesh.h"
#include "SDL.h"
#include "GL/glew.h"
#include "ModuleEditorCamera.h"
#include "ImGui/imgui.h"
#include <Math/MathAll.h>	


Mesh::Mesh()
{
	mesh_center = std::make_unique<float3>();
	world_position = std::make_unique<float3>();

	max_positions_local = std::make_unique<float3>();
	min_positions_local = std::make_unique<float3>();
	max_positions_world = std::make_unique<float3>();
	min_positions_world = std::make_unique<float3>();

	model_matrix = std::make_unique<float4x4>();
}

Mesh::~Mesh()
{

}

void Mesh::LoadVBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
{
	// Load vertex positions to vbo
	const auto& it_pos = primitive.attributes.find("POSITION");
	if (it_pos != primitive.attributes.end())
	{
		const tinygltf::Accessor& position_accessor = model.accessors[it_pos->second];
		SDL_assert(position_accessor.type == TINYGLTF_TYPE_VEC3);
		SDL_assert(position_accessor.componentType == GL_FLOAT);
		vertex_count = position_accessor.count;

		const tinygltf::BufferView& position_view = model.bufferViews[position_accessor.bufferView];
		const tinygltf::Buffer& position_buffer = model.buffers[position_view.buffer];
		const unsigned char* positions_buffer_ptr = &(position_buffer.data[position_accessor.byteOffset + position_view.byteOffset]);
		size_t i = position_buffer.data.size();
		LOG("Positions byte stride: %d", position_view.byteStride);

		int stride = 3;

		// Load UVs positions to vbo
		const auto& it_tex = primitive.attributes.find("TEXCOORD_0");
		if (it_tex != primitive.attributes.end())
		{
			material_index = primitive.material;
			const tinygltf::Accessor& uvs_accessor = model.accessors[it_tex->second];
			SDL_assert(uvs_accessor.type == TINYGLTF_TYPE_VEC2);
			SDL_assert(uvs_accessor.componentType == GL_FLOAT);
			uvs_count = uvs_accessor.count;

			const tinygltf::BufferView& uvs_view = model.bufferViews[uvs_accessor.bufferView];
			const tinygltf::Buffer& uvs_buffer = model.buffers[uvs_view.buffer];
			const unsigned char* uvs_buffer_ptr = &(uvs_buffer.data[uvs_accessor.byteOffset + uvs_view.byteOffset]);
			size_t j = uvs_buffer.data.size();
			LOG("Uvs byte stride: %d", uvs_view.byteStride);

			stride = 5;

			const auto& it_norm = primitive.attributes.find("NORMAL");
			if (it_norm != primitive.attributes.end()) // If mesh has everything
			{
				const tinygltf::Accessor& normals_accesor = model.accessors[it_norm->second];
				SDL_assert(normals_accesor.type == TINYGLTF_TYPE_VEC3);
				SDL_assert(normals_accesor.componentType == GL_FLOAT);
				normals_count = normals_accesor.count;

				const tinygltf::BufferView& normals_view = model.bufferViews[normals_accesor.bufferView];
				const tinygltf::Buffer& normals_buffer = model.buffers[normals_view.buffer];
				const unsigned char* normals_buffer_ptr = &(normals_buffer.data[normals_accesor.byteOffset + normals_view.byteOffset]);
				size_t j = uvs_buffer.data.size();
				LOG("Normals byte stride: %d", uvs_view.byteStride);

				stride = 8;

				glGenBuffers(1, &vbo);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				int buffer_size = sizeof(float) * ((3 * vertex_count) + (2 * uvs_count) + (3 * normals_count));
				glBufferData(GL_ARRAY_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);

				float* vbo_ptr = reinterpret_cast<float*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
				for (size_t i = 0; i < buffer_size / sizeof(float); i += stride)
				{
					// Vertex data
					float3 vtx_pos = *reinterpret_cast<const float3*>(positions_buffer_ptr);

					vbo_ptr[i] = vtx_pos.x;
					vbo_ptr[i + 1] = vtx_pos.y;
					vbo_ptr[i + 2] = vtx_pos.z;

					if (i == 0)
					{
						*max_positions_local = vtx_pos;
						*min_positions_local = vtx_pos;
					}
					else
					{
						if (vtx_pos.x > max_positions_local->x)
							max_positions_local->x = vtx_pos.x;

						if (vtx_pos.x < min_positions_local->x)
							min_positions_local->x = vtx_pos.x;

						if (vtx_pos.y > max_positions_local->y)
							max_positions_local->y = vtx_pos.y;

						if (vtx_pos.y < min_positions_local->y)
							min_positions_local->y = vtx_pos.y;

						if (vtx_pos.z > max_positions_local->z)
							max_positions_local->z = vtx_pos.z;

						if (vtx_pos.z < min_positions_local->z)
							min_positions_local->z = vtx_pos.z;
					}

					if (position_view.byteStride == 0)
						positions_buffer_ptr += sizeof(float) * 3;
					else
						positions_buffer_ptr += position_view.byteStride;

					// Uvs data
					float2 uvs_pos = *reinterpret_cast<const float2*>(uvs_buffer_ptr);
					vbo_ptr[i + 3] = uvs_pos.x;
					vbo_ptr[i + 4] = uvs_pos.y;

					if (uvs_view.byteStride == 0)
						uvs_buffer_ptr += sizeof(float) * 2;
					else
						uvs_buffer_ptr += uvs_view.byteStride;

					// Normals data
					float3 normals_pos = *reinterpret_cast<const float3*>(normals_buffer_ptr);
					vbo_ptr[i + 5] = normals_pos.x;
					vbo_ptr[i + 6] = normals_pos.y;
					vbo_ptr[i + 7] = normals_pos.z;

					if (normals_view.byteStride == 0)
						normals_buffer_ptr += sizeof(float) * 3;
					else
						normals_buffer_ptr += normals_view.byteStride;
				}
				glUnmapBuffer(GL_ARRAY_BUFFER);
			}
			else // If mesh has no normals
			{
				glGenBuffers(1, &vbo);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				int buffer_size = sizeof(float) * ((3 * vertex_count) + (2 * uvs_count));
				glBufferData(GL_ARRAY_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);

				float* vbo_ptr = reinterpret_cast<float*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
				for (size_t i = 0; i < (3 * vertex_count) + (2 * uvs_count); i += stride)
				{
					// Vertex data
					float3 vtx_pos = *reinterpret_cast<const float3*>(positions_buffer_ptr);

					vbo_ptr[i] = vtx_pos.x;
					vbo_ptr[i + 1] = vtx_pos.y;
					vbo_ptr[i + 2] = vtx_pos.z;

					if (i == 0)
					{
						*max_positions_local = vtx_pos;
						*min_positions_local = vtx_pos;
					}
					else
					{
						if (vtx_pos.x > max_positions_local->x)
							max_positions_local->x = vtx_pos.x;

						if (vtx_pos.x < min_positions_local->x)
							min_positions_local->x = vtx_pos.x;

						if (vtx_pos.y > max_positions_local->y)
							max_positions_local->y = vtx_pos.y;

						if (vtx_pos.y < min_positions_local->y)
							min_positions_local->y = vtx_pos.y;

						if (vtx_pos.z > max_positions_local->z)
							max_positions_local->z = vtx_pos.z;

						if (vtx_pos.z < min_positions_local->z)
							min_positions_local->z = vtx_pos.z;
					}

					if (position_view.byteStride == 0)
						positions_buffer_ptr += sizeof(float) * 3;
					else
						positions_buffer_ptr += position_view.byteStride;

					// Uvs data
					float2 uvs_pos = *reinterpret_cast<const float2*>(uvs_buffer_ptr);
					vbo_ptr[i + 3] = uvs_pos.x;
					vbo_ptr[i + 4] = uvs_pos.y;

					if (uvs_view.byteStride == 0)
						uvs_buffer_ptr += sizeof(float) * 2;
					else
						uvs_buffer_ptr += uvs_view.byteStride;
				}
				glUnmapBuffer(GL_ARRAY_BUFFER);
			}
		}
		else // If the mesh has no uvs
		{
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			int buffer_size = sizeof(float) * ((3 * vertex_count) + (2 * uvs_count));
			glBufferData(GL_ARRAY_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);

			float* vbo_ptr = reinterpret_cast<float*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
			for (size_t i = 0; i < 3 * vertex_count; i += 3)
			{
				 float3 vtx_pos = *reinterpret_cast<const float3*>(positions_buffer_ptr);
				 vbo_ptr[i] = vtx_pos.x;
				 vbo_ptr[i + 1] = vtx_pos.y;
				 vbo_ptr[i + 2] = vtx_pos.z;

				 if (i == 0)
				 {
					 *max_positions_local = vtx_pos;
					 *min_positions_local = vtx_pos;
				 }
				 else
				 {
					 if (vtx_pos.x > max_positions_local->x)
						 max_positions_local->x = vtx_pos.x;

					 if (vtx_pos.x < min_positions_local->x)
						 min_positions_local->x = vtx_pos.x;

					 if (vtx_pos.y > max_positions_local->y)
						 max_positions_local->y = vtx_pos.y;

					 if (vtx_pos.y < min_positions_local->y)
						 min_positions_local->y = vtx_pos.y;

					 if (vtx_pos.z > max_positions_local->z)
						 max_positions_local->z = vtx_pos.z;

					 if (vtx_pos.z < min_positions_local->z)
						 min_positions_local->z = vtx_pos.z;
				 }

				if (position_view.byteStride == 0)
					positions_buffer_ptr += sizeof(float) * 3;
				else
					positions_buffer_ptr += position_view.byteStride;
			}
			glUnmapBuffer(GL_ARRAY_BUFFER);
		}
	}
	LOG("Load mesh with vbo index: %d. vertex count: %d. uvs count: %d", vbo, vertex_count, uvs_count);
}

void Mesh::SetModelMatrix(const std::vector<double>& model)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			model_matrix->At(j, i) = static_cast<float>(model[i]);
		}
	}
}

void Mesh::SetModelMatrix(const std::vector<double>& trans, const std::vector<double>& rot, const std::vector<double>& sc)
{
	float3 translation = float3::zero;
	float4x4 rotation = float4x4::identity;
	float3 scale = float3::one;

	// Translation
	if (trans.size() > 0)
	{
		for (int i = 0; i < trans.size(); ++i)
		{
			translation[i] = static_cast<float>(trans[i]);
		}
	}

	// Rotation
	if (rot.size() > 0)
	{
		Quat quaternion(static_cast<float>(rot[0]), static_cast<float>(rot[1]), static_cast<float>(rot[2]), static_cast<float>(rot[3]));
		rotation = quaternion.ToFloat4x4();
	}
	
	// Scale
	if (sc.size() > 0)
	{
		for (int i = 0; i < sc.size(); ++i)
		{
			scale[i] = static_cast<float>(sc[i]);
		}
	}

	*model_matrix = float4x4::FromTRS(translation, rotation, scale);

	float4 max_pos = *model_matrix * float4(*max_positions_local, 1.0f);
	*max_positions_world = float3(max_pos.x, max_pos.y, max_pos.z);
	
	float4 min_pos = *model_matrix * float4(*min_positions_local, 1.0f);
	*min_positions_world = float3(min_pos.x, min_pos.y, min_pos.z);
	
	*world_position = model_matrix->TranslatePart();
	LOG("Max positions world: %f, %f, %f", max_positions_world->x, max_positions_world->y, max_positions_world->z);
	LOG("Min positions world: %f, %f, %f", min_positions_world->x, min_positions_world->y, min_positions_world->z);
	LOG("World pos: %f, %f, %f", world_position->x, world_position->y, world_position->z);

	*mesh_center = float3((max_positions_world->x + min_positions_world->x) / 2, (max_positions_world->y + min_positions_world->y) / 2, (max_positions_world->z + min_positions_world->z));
	LOG("Max positions: %f, %f, %f", max_positions_local->x, max_positions_local->y, max_positions_local->z);
	LOG("Min positions: %f, %f, %f", min_positions_local->x, min_positions_local->y, min_positions_local->z);
}

void Mesh::LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
{
	if (primitive.indices >= 0)
	{
		LOG("Loading EBO");
		const tinygltf::Accessor& indices_accessor = model.accessors[primitive.indices];
		indices_count = indices_accessor.count;
		const tinygltf::BufferView& indices_view = model.bufferViews[indices_accessor.bufferView];
		const tinygltf::Buffer& indices_buffer = model.buffers[indices_view.buffer];
		const unsigned char* buffer_ptr = &(indices_buffer.data[indices_accessor.byteOffset + indices_view.byteOffset]);

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices_accessor.count, nullptr, GL_STATIC_DRAW);
		unsigned int* ebo_ptr = reinterpret_cast<unsigned int*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));

		if (indices_accessor.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT)
		{
			const uint32_t* buffer_int = reinterpret_cast<const uint32_t*>(buffer_ptr);
			for (size_t i = 0; i < indices_count; ++i)
			{
				ebo_ptr[i] = buffer_int[i];
			}
		}
		if (indices_accessor.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT)
		{
			const uint16_t* buffer_short = reinterpret_cast<const uint16_t*>(buffer_ptr);
			for (size_t i = 0; i < indices_count; ++i)
			{
				ebo_ptr[i] = buffer_short[i];
			}
		}
		if (indices_accessor.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE)
		{
			const uint8_t* buffer_byte = reinterpret_cast<const uint8_t*>(buffer_ptr);
			for (size_t i = 0; i < indices_count; ++i)
			{
				ebo_ptr[i] = buffer_byte[i];
			}
		}
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}
}

void Mesh::CreateVAO()
{
	LOG("Creating VAO");

	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float) * 2 + sizeof(float) * 3, (void*)(0));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float) * 2 + sizeof(float) * 3, (void*)(sizeof(float) * 3));
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float) * 2 + sizeof(float) * 3, (void*)(sizeof(float) * 3 + sizeof(float) * 2));

	glBindVertexArray(0);

}

void Mesh::Render(unsigned int program, const std::vector<unsigned int>& textures) const
{
	float4x4 projection = App->GetCamera()->GetProjectionMatrix();
	float4x4 view = App->GetCamera()->GetViewMatrix();
	float4x4 model = *model_matrix;

	glUseProgram(program);
	glUniformMatrix4fv(0, 1, GL_TRUE, &projection[0][0]);
	glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(2, 1, GL_TRUE, &model[0][0]);


	float light_dir[3] = { 50, 100, 50 };
	float3 camera_pos = App->GetCamera()->GetCameraPosition();
	float view_position[3] = { camera_pos.x, camera_pos.y, camera_pos.z };

	float ambient_color[3] = { 0.1f, 0.1f, 0.1f };
	float light_color[3] = { 1.0f, 1.0f, 1.0f };

	float ks = 0.4f;
	float kd = 1.0f;
	float shininess = 50;

	glUniform3fv(3, 1, &light_dir[0]);
	glUniform3fv(4, 1, &view_position[0]);
	glUniform3fv(5, 1, &ambient_color[0]);
	glUniform3fv(6, 1, &light_color[0]);
	
	glUniform1f(7, ks);
	glUniform1f(8, kd);
	glUniform1f(9, shininess);

	if (material_index >= 0) // Only if mesh has texture
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[material_index]);
	}

	if (indices_count > 0) // If mesh has indices
	{
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_count), GL_UNSIGNED_INT, 0);
	}
	else // If mesh has no indices
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float) * 2, (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float) * 2, (void*)(sizeof(float) * 3));
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertex_count));
	}
}

void Mesh::Delete() const
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

void Mesh::Move(const float3& distance)
{
	model_matrix->SetTranslatePart(distance);

	// Update the boundaries of the mesh
	*world_position = model_matrix->TranslatePart();

	float4 max_pos = *model_matrix * float4(*max_positions_local, 1.0f);
	*max_positions_world = float3(max_pos.x, max_pos.y, max_pos.z);

	float4 min_pos = *model_matrix * float4(*min_positions_local, 1.0f);
	*min_positions_world = float3(min_pos.x, min_pos.y, min_pos.z);

	*mesh_center = float3((max_positions_world->x + min_positions_world->x) / 2, (max_positions_world->y + min_positions_world->y) / 2, (max_positions_world->z + min_positions_world->z));

}