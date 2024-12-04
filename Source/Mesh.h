#ifndef __MESH_H__
#define __MESH_H__

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

	void Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void Render() const;

private:
	unsigned int vbo = 0;
	unsigned int ebo = 0;
	unsigned int material_index = 0;

	unsigned int vertex_count = 0;
};
#endif // __MESH_H__