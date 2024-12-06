#ifndef __MODEL_H__
#define __MODEL_H__

#include "Globals.h"
#include <vector>

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
	void Delete();

private:
	std::vector<std::unique_ptr<Mesh>> meshes;
	std::vector<unsigned int> textures;
	
};

#endif // __MODEL_H__
