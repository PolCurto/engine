#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include "tiny_gltf.h"

#include "Mesh.h"
#include "SDL.h"
#include "GL/glew.h"
#include "ModuleEditorCamera.h"
#include <Math/MathAll.h>	
#include "ImGui/imgui.h"


Mesh::Mesh()
{
	ebo = 0;
	indices_count = 0;
}

Mesh::~Mesh()
{

}

void Mesh::LoadVBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
{
	name = mesh.name;

	// Load vertex positions to vbo
	const auto& it_pos = primitive.attributes.find("POSITION");
	if (it_pos != primitive.attributes.end())
	{
		const tinygltf::Accessor& position_accessor = model.accessors[it_pos->second];
		SDL_assert(position_accessor.type == TINYGLTF_TYPE_VEC3, "Accesor type not Vec3");
		SDL_assert(position_accessor.componentType == GL_FLOAT, "Accesor component type not float");
		vertex_count = position_accessor.count;

		const tinygltf::BufferView& position_view = model.bufferViews[position_accessor.bufferView];
		const tinygltf::Buffer& position_buffer = model.buffers[position_view.buffer];
		const unsigned char* buffer_ptr = &(position_buffer.data[position_accessor.byteOffset + position_view.byteOffset]);
		size_t i = position_buffer.data.size();
		LOG("Positions buffer raw data size: %d", i);
		LOG("Positions byte stride: %d", position_view.byteStride);

		// Load UVs positions to vbo
		const auto& it_tex = primitive.attributes.find("TEXCOORD_0");
		if (it_tex != primitive.attributes.end())
		{
			material_index = primitive.material;
			const tinygltf::Accessor& uvs_accessor = model.accessors[it_tex->second];
			SDL_assert(uvs_accessor.type == TINYGLTF_TYPE_VEC2, "Accesor type not Vec2");
			SDL_assert(uvs_accessor.componentType == GL_FLOAT, "Accesor component type not float");
			uvs_count = uvs_accessor.count;

			const tinygltf::BufferView& uvs_view = model.bufferViews[uvs_accessor.bufferView];
			const tinygltf::Buffer& uvs_buffer = model.buffers[uvs_view.buffer];
			const unsigned char* uvs_buffer_ptr = &(uvs_buffer.data[uvs_accessor.byteOffset + uvs_view.byteOffset]);
			size_t j = uvs_buffer.data.size();
			LOG("Uvs buffer raw data size: %d", j);
			LOG("Uvs byte stride: %d", uvs_view.byteStride);

			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			int buffer_size = sizeof(float) * ((3 * vertex_count) + (2 * uvs_count));
			glBufferData(GL_ARRAY_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);

			float3* vbo_pos_ptr = reinterpret_cast<float3*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
			for (size_t i = 0; i < vertex_count; ++i)
			{
				//LOG("pos loop i: %d", i);
				vbo_pos_ptr[i] = *reinterpret_cast<const float3*>(buffer_ptr);

				if (position_view.byteStride == 0)
				{
					buffer_ptr += sizeof(float) * 3;
				}
				else
				{
					buffer_ptr += position_view.byteStride;
				}
			}
			glUnmapBuffer(GL_ARRAY_BUFFER);

			float2* vbo_uvs_ptr = reinterpret_cast<float2*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
			size_t offset = (vertex_count * 3) / 2;
			for (i = offset; i < offset + uvs_count; ++i)
			{
				//LOG("uvs loop i: %d", i);
				vbo_uvs_ptr[i] = *reinterpret_cast<const float2*>(uvs_buffer_ptr);

				if (uvs_view.byteStride == 0)
				{
					uvs_buffer_ptr += sizeof(float) * 2;
				}
				else
				{
					uvs_buffer_ptr += uvs_view.byteStride;
				}
			}
			glUnmapBuffer(GL_ARRAY_BUFFER);
		}
		else
		{
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			int buffer_size = sizeof(float) * 3 * vertex_count;
			glBufferData(GL_ARRAY_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);

			float3* vbo_pos_ptr = reinterpret_cast<float3*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
			for (size_t i = 0; i < vertex_count; ++i)
			{
				vbo_pos_ptr[i] = *reinterpret_cast<const float3*>(buffer_ptr);

				if (position_view.byteStride == 0)
				{
					buffer_ptr += sizeof(float) * 3;
				}
				else
				{
					buffer_ptr += position_view.byteStride;
				}
			}
			glUnmapBuffer(GL_ARRAY_BUFFER);
		}
	}

	LOG("Load mesh with vbo index: %d. vertex count: %d. uvs count: %d", vbo, vertex_count, uvs_count);
}

void Mesh::LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
{
	if (primitive.indices >= 0)
	{
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
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * vertex_count));

	glBindVertexArray(0);

}

void Mesh::Render(unsigned int program, const std::vector<unsigned int>& textures) const
{
	float4x4 projection = App->GetCamera()->GetProjectionMatrix();
	float4x4 view = App->GetCamera()->GetViewMatrix();
	float4x4 model = math::float4x4::FromTRS(float3(0.0f, 1.0f, -4.0f), float4x4::RotateZ(0), float3(100.0f, 100.0f, 100.0f));
	//float4x4 model = math::float4x4::FromTRS(float3(0.0f, 1.0f, -4.0f), float4x4::RotateZ(0), float3(1.0f, 1.0f, 1.0f));

	glUseProgram(program);
	glUniformMatrix4fv(0, 1, GL_TRUE, &projection[0][0]);
	glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(2, 1, GL_TRUE, &model[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[material_index]);

	ImGui::Begin("Baboon Texture");
	
	ImGui::SeparatorText("Texture Data");
	
	//ImGui::Text("Texture width: %i", baboon_metadata.width);
	//ImGui::Text("Texture height: %i", baboon_metadata.height);
	//ImGui::Text("Format: %i", baboon_metadata.format);
	//ImGui::Text("Mipmaps: %i", baboon_metadata.mipLevels);
	
	ImGui::SeparatorText("Texture settings");
	
	static int wrap_mode = GL_CLAMP;
	if (ImGui::CollapsingHeader("Wrap mode"))
	{
		ImGui::RadioButton("Repeat##2", &wrap_mode, GL_REPEAT);
		ImGui::SameLine();
		ImGui::RadioButton("Mirrored repeat##2", &wrap_mode, GL_MIRRORED_REPEAT);
		ImGui::RadioButton("Clamp##2", &wrap_mode, GL_CLAMP);
		ImGui::SameLine();
		ImGui::RadioButton("Clamp to border##2", &wrap_mode, GL_CLAMP_TO_BORDER);
	}
	
	static int mag_filter = GL_NEAREST;
	if (ImGui::CollapsingHeader("Mag Filter"))
	{
		ImGui::RadioButton("Nearest##1", &mag_filter, GL_NEAREST);
		ImGui::SameLine();
		ImGui::RadioButton("Linear##1", &mag_filter, GL_LINEAR);
	}
	
	static int min_filter = GL_NEAREST;
	if (ImGui::CollapsingHeader("Min Filter"))
	{
		ImGui::RadioButton("Nearest##2", &min_filter, GL_NEAREST);
		ImGui::SameLine();
		ImGui::RadioButton("Linear##2", &min_filter, GL_LINEAR);
	}
	
	ImGui::End();
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);

	if (indices_count > 0)
	{
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * vertex_count));
		glDrawArrays(GL_TRIANGLES, 0, vertex_count);
	}
}

void Mesh::Delete() const
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}