#ifndef __MESH_H__
#define __MESH_H__

#include "Globals.h"
#include <memory>

namespace tinygltf
{
	class Model;
	struct Mesh;
	struct Primitive;
}

namespace math
{
	class float3;
	class float4x4;
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
	void Move(const math::float3& distance);

	size_t GetVertexCount() const { return vertex_count; }
	size_t GetTrianglesCount() const { return vertex_count / 3; }
	size_t GetIndicesCount() const { return vertex_count; }
	float GetKs() const { return ks; }
	float GetKd() const { return kd; }
	float GetShininess() const { return shininess; }

	void SetModelMatrix(const std::vector<double>& model);
	void SetModelMatrix(const std::vector<double>& translation, const std::vector<double>& rotation, const std::vector<double>& scale);
	void SetKs(float new_ks) { ks = new_ks; }
	void SetKd(float new_kd) { kd = new_kd; }
	void SetShininess(float new_shininess) { shininess = new_shininess; }


	std::unique_ptr<math::float3> mesh_center;
	std::unique_ptr<math::float3> world_position;
	std::unique_ptr<math::float3> max_positions_world;
	std::unique_ptr<math::float3> min_positions_world;

private:
	unsigned int vbo = 0;
	unsigned int ebo = 0;
	unsigned int vao = 0;
	int material_index = -1;

	float ks = 0.2f;
	float kd = 0.9f;
	float shininess = 250;

	size_t vertex_count = 0;
	size_t indices_count = 0;
	size_t uvs_count = 0;
	size_t normals_count = 0;

	std::unique_ptr<math::float3> max_positions_local;
	std::unique_ptr<math::float3> min_positions_local;

	std::unique_ptr<math::float4x4> model_matrix;
};
#endif // __MESH_H__