#ifndef __MESH_H__
#define __MESH_H__

#include "Globals.h"
#include <Math/MathAll.h>	

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

	size_t GetVertexCount() const { return vertex_count; }
	size_t GetTrianglesCount() const { return vertex_count / 3; }
	size_t GetIndicesCount() const { return vertex_count; }

	std::string name;

private:
	unsigned int vbo = 0;
	unsigned int ebo = 0;
	unsigned int vao = 0;
	int material_index = -1;

	float3 max_positions = float3::zero;
	float3 min_positions = float3::zero;

	size_t vertex_count = 0;
	size_t indices_count = 0;
	size_t uvs_count = 0;
};
#endif // __MESH_H__