#ifndef __MESH_H__
#define __MESH_H__

#include "tiny_gltf.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void Load(tinygltf::Model src_model);
private:
	unsigned int vbo;
	unsigned int ebo;
	unsigned int material_index;
};
#endif