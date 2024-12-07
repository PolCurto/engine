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

class Mesh;

class Model
{
public:
	Model();
	~Model();

	void Load(const char* asset_filename);
	void LoadMaterials(const tinygltf::Model& src_model);
	void Render(unsigned int program) const;
	void ShowModelInformation() const;
	void TextureOptions();
	void Delete();
	void OnFileDropped();

private:
	std::vector<std::unique_ptr<Mesh>> meshes;
	std::vector<unsigned int> textures_id;
	std::unordered_map<unsigned int, std::unique_ptr<DirectX::ScratchImage>> textures_data;
	
};

#endif // __MODEL_H__
