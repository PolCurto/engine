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

	void Load(const char* asset_filename, std::vector<std::unique_ptr<Mesh>>& meshes_vector);
	void LoadMaterials(const tinygltf::Model& src_model);

private:
	
};

#endif // __MODEL_H__
