#ifndef __MODEL_H__
#define __MODEL_H__

#include "Globals.h"
#include <vector>
#include <unordered_map>

namespace DirectX
{
	class ScratchImage;
}

namespace tinygltf
{
	class Model;
}

namespace math
{
	class float3;
}

class Mesh;

class Model
{
public:
	Model();
	~Model();

	void Load(const char* asset_filename);
	void Render(unsigned int program);
	void GetModelInformation(std::vector<std::vector<int>>& mesh_data, std::vector<std::vector<int>>& tex_metadata) const;
	void SetTextureParameters(const std::vector<int>&& params) const;
	void Delete();

	int GetTexturesIDCount() const { return textures_id.size(); }
	//std::vector<std::unique_ptr<Mesh>> GetMeshes() const { return meshes; }

	void SetPosition(const math::float3& new_position);

	std::unique_ptr<math::float3> local_position; // Center position
	std::unique_ptr<math::float3> world_position; // Pivot position
	std::unique_ptr<math::float3> max_positions;
	std::unique_ptr<math::float3> min_positions;

private:
	void LoadMaterials(const tinygltf::Model& src_model);
	void LoadMatrices(const tinygltf::Model& src_model);
	void UpdatePositions();

	std::vector<std::unique_ptr<Mesh>> meshes;
	std::vector<unsigned int> textures_id;
	std::unordered_map<unsigned int, std::unique_ptr<DirectX::ScratchImage>> textures_data;

};

#endif // __MODEL_H__
