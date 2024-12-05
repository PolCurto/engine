#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include "tiny_gltf.h"

#include "Mesh.h"
#include "SDL.h"
#include "GL/glew.h"
#include "ModuleEditorCamera.h"
#include <map>
#include <Math/MathAll.h>	


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
		LOG("Buffer raw data size: %d", i);
		LOG("Byte stride: %d", position_view.byteStride);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * vertex_count, nullptr, GL_STATIC_DRAW);
		float3* vbo_ptr = reinterpret_cast<float3*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		for (size_t i = 0; i < vertex_count; ++i)
		{
			vbo_ptr[i] = *reinterpret_cast<const float3*>(buffer_ptr);

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
	LOG("Load mesh with vbo index: %d. vertex count: %d", vbo, vertex_count);
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

void Mesh::Render(unsigned program) const
{
	float4x4 projection = App->GetCamera()->GetProjectionMatrix();
	float4x4 view = App->GetCamera()->GetViewMatrix();
	float4x4 model = math::float4x4::FromTRS(float3(0.0f, 1.0f, -2.0f), float4x4::RotateZ(0), float3(1.0f, 1.0f, 1.0f));

	glUseProgram(program);
	glUniformMatrix4fv(0, 1, GL_TRUE, &projection[0][0]);
	glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(2, 1, GL_TRUE, &model[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * vertex_count));

	if (indices_count > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, vertex_count);
	}
}