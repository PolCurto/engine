#ifndef __MESH_H__
#define __MESH_H__

#include "Globals.h"

namespace tinygltf
{
	class Model;
	struct Mesh;
	struct Primitive;
}

class Mesh
{
public:
	Mesh();
	~Mesh();

	void LoadVBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void CreateVAO();
	void Render(const unsigned int program, const std::vector<unsigned int>& textures) const;
	void Delete() const;

private:
	unsigned int vbo = 0;
	unsigned int ebo = 0;
	unsigned int vao = 0;
	unsigned int material_index = 0;

	size_t vertex_count = 0;
	size_t indices_count = 0;
	size_t uvs_count = 0;
};
#endif // __MESH_H__