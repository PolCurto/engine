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
	void LoadMaterials(const tinygltf::Model& src_model);
	void Render(unsigned int program);
	void ShowModelInformation() const;
	void SetTextureParameters(const std::vector<int>&& params);
	void Delete();

	int GetTexturesIDCount() const { return textures_id.size(); }

	std::unique_ptr<math::float3> position;
	std::unique_ptr<math::float3> max_positions;
	std::unique_ptr<math::float3> min_positions;

private:
	void UpdatePosition();

	std::vector<std::unique_ptr<Mesh>> meshes;
	std::vector<unsigned int> textures_id;
	std::unordered_map<unsigned int, std::unique_ptr<DirectX::ScratchImage>> textures_data;

};

#endif // __MODEL_H__
