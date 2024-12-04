#include "Mesh.h"
#include "tiny_gltf.h"
#include "SDL.h"
#include "GL/glew.h"
#include <map>
#include <Math/MathAll.h>	


Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

void Mesh::Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
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
		const unsigned char* buffer_position = &(position_buffer.data[position_accessor.byteOffset + position_view.byteOffset]);

		glGenBuffers(1, &vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * vertex_count, nullptr, GL_STATIC_DRAW);
		float3* vbo_data_ptr = reinterpret_cast<float3*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		for (size_t i = 0; i < vertex_count; ++i)
		{
			vbo_data_ptr[i] = *reinterpret_cast<const float3*>(buffer_position);
			buffer_position += position_view.byteStride;
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
}

void Mesh::Render() const
{
	//glUseProgram();
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, vertex_count);
}