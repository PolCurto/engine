#ifndef __MODEL_H__
#define __MODEL_H__

#include "Globals.h"
#include <vector>
#include "DirectXTex.h"

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
	void Render(unsigned int program);
	void ShowInformation();
	void Delete();

private:
	std::vector<std::unique_ptr<Mesh>> meshes;
	std::vector<unsigned int> textures;
	std::vector<DirectX::ScratchImage> images;
	
};

#endif // __MODEL_H__
