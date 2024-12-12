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

	translate = std::make_unique<float3>(0.0f, 1.0f, -4.0f);
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
		const unsigned char* buffer_ptr = &(position_buffer.data[position_accessor.byteOffset + position_view.byteOffset]);
		size_t i = position_buffer.data.size();
		LOG("Positions buffer raw data size: %d", i);
		LOG("Positions byte stride: %d", position_view.byteStride);

		int stride = 3;
		bool uvs_exist = false;

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
			LOG("Uvs buffer raw data size: %d", j);
			LOG("Uvs byte stride: %d", uvs_view.byteStride);

			stride = 5;

			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			int buffer_size = sizeof(float) * ((3 * vertex_count) + (2 * uvs_count));
			glBufferData(GL_ARRAY_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);

			float* vbo_pos_ptr = reinterpret_cast<float*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
			for (size_t i = 0; i < (3 * vertex_count) + (2 * uvs_count); i += stride)
			{
				// Vertex data
				float3 vtx_pos = *reinterpret_cast<const float3*>(buffer_ptr);

				vbo_pos_ptr[i] = vtx_pos.x;
				vbo_pos_ptr[i + 1] = vtx_pos.y;
				vbo_pos_ptr[i + 2] = vtx_pos.z;
				
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
					buffer_ptr += sizeof(float) * 3;
				else
					buffer_ptr += position_view.byteStride;

				// Uvs data
				float2 uvs_pos = *reinterpret_cast<const float2*>(uvs_buffer_ptr);
				vbo_pos_ptr[i + 3] = uvs_pos.x;
				vbo_pos_ptr[i + 4] = uvs_pos.y;
				
				if (uvs_view.byteStride == 0)
					uvs_buffer_ptr += sizeof(float) * 2;
				else
					uvs_buffer_ptr += uvs_view.byteStride;
			}
			glUnmapBuffer(GL_ARRAY_BUFFER);

			*mesh_center = float3((max_positions_local->x + min_positions_local->x) / 2, (max_positions_local->y + min_positions_local->y) / 2, (max_positions_local->z + min_positions_local->z));
			LOG("Max positions: %f, %f, %f", max_positions_local->x, max_positions_local->y, max_positions_local->z);
			LOG("Min positions: %f, %f, %f", min_positions_local->x, min_positions_local->y, min_positions_local->z);
		}
		else
		{
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			int buffer_size = sizeof(float) * ((3 * vertex_count) + (2 * uvs_count));
			glBufferData(GL_ARRAY_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);

			float* vbo_pos_ptr = reinterpret_cast<float*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
			for (size_t i = 0; i < 3 * vertex_count; i += 3)
			{
				 float3 vtx_pos = *reinterpret_cast<const float3*>(buffer_ptr);
				 vbo_pos_ptr[i] = vtx_pos.x;
				 vbo_pos_ptr[i + 1] = vtx_pos.y;
				 vbo_pos_ptr[i + 2] = vtx_pos.z;

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
					buffer_ptr += sizeof(float) * 3;
				else
					buffer_ptr += position_view.byteStride;
			}
			glUnmapBuffer(GL_ARRAY_BUFFER);
		}
		float4x4 model = math::float4x4::FromTRS(*translate, float4x4::RotateZ(0), float3(1.0f, 1.0f, 1.0f));
		float4 max_pos = model * float4(*max_positions_local, 1.0f);
		*max_positions_world = float3(max_pos.x, max_pos.y, max_pos.z);

		float4 min_pos = model * float4(*min_positions_local, 1.0f);
		*min_positions_world = float3(min_pos.x, min_pos.y, min_pos.z);

		*world_position = *translate;

		LOG("Max positions world: %f, %f, %f", max_positions_world->x, max_positions_world->y, max_positions_world->z);
		LOG("Min positions world: %f, %f, %f", min_positions_world->x, min_positions_world->y, min_positions_world->z);
		LOG("World pos: %f, %f, %f", world_position->x, world_position->y, world_position->z);
	}
	LOG("Load mesh with vbo index: %d. vertex count: %d. uvs count: %d", vbo, vertex_count, uvs_count);
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float) * 2, (void*)(0));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float) * 2, (void*)(sizeof(float) * 3));

	glBindVertexArray(0);

}

void Mesh::Render(unsigned int program, const std::vector<unsigned int>& textures) const
{
	float4x4 projection = App->GetCamera()->GetProjectionMatrix();
	float4x4 view = App->GetCamera()->GetViewMatrix();
	//float4x4 model = math::float4x4::FromTRS(float3(0.0f, 1.0f, -4.0f), float4x4::RotateZ(0), float3(100.0f, 100.0f, 100.0f));
	float4x4 model = math::float4x4::FromTRS(*translate, float4x4::RotateZ(0), float3(1.0f, 1.0f, 1.0f));

	glUseProgram(program);
	glUniformMatrix4fv(0, 1, GL_TRUE, &projection[0][0]);
	glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(2, 1, GL_TRUE, &model[0][0]);

	*world_position = *translate;

	float4 max_pos = model * float4(*max_positions_local, 1.0f);
	*max_positions_world = float3(max_pos.x, max_pos.y, max_pos.z);
	
	float4 min_pos = model * float4(*min_positions_local, 1.0f);
	*min_positions_world = float3(min_pos.x, min_pos.y, min_pos.z);

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