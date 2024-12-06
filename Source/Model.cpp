#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"

#include "Model.h"
#include "Mesh.h"
#include "Application.h"
#include "ModuleTextures.h"

Model::Model()
{
	
}

Model::~Model()
{

}

void Model::Load(const char* asset_filename)
{
	tinygltf::TinyGLTF gltfObject;
	tinygltf::Model model;
	std::string error;
	std::string warning;

	bool loaded_correct = gltfObject.LoadASCIIFromFile(&model, &error, &warning, asset_filename);

	if (!loaded_correct)
	{
		LOG("Error loading %s: %s", asset_filename, error);
		return;
	}

	if (warning.size() > 0)
		LOG("Warning loading %s: %s", asset_filename, warning);


	for (const tinygltf::Mesh& source_mesh : model.meshes)
	{
		for (const tinygltf::Primitive primitive : source_mesh.primitives)
		{
			std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
			mesh->LoadVBO(model, source_mesh, primitive);
			mesh->LoadEBO(model, source_mesh, primitive);
			mesh->CreateVAO();
			meshes.push_back(std::move(mesh));
		}
	}

	LoadMaterials(model);
}

void Model::LoadMaterials(const tinygltf::Model& src_model)
{
	for (const tinygltf::Material& src_material : src_model.materials)
	{
		unsigned int texture_id = 0;
		if (src_material.pbrMetallicRoughness.baseColorTexture.index >= 0)
		{
			const tinygltf::Texture& texture = src_model.textures[src_material.pbrMetallicRoughness.baseColorTexture.index];
			const tinygltf::Image& image = src_model.images[texture.source];

			std::string texture_folder = "./Textures/";
			texture_id = App->GetTextures()->LoadFile(texture_folder.append(image.uri).c_str());
			//texture_id = App->GetTextures()->LoadFile("./Textures/Baboon.dds");
		}
		textures.push_back(texture_id);
	}
}

void Model::Render(const unsigned int program)
{
	for (std::unique_ptr<Mesh>& mesh : meshes)
	{
		mesh->Render(program, textures);
	}
}

void Model::Delete()
{
	for (std::unique_ptr<Mesh>& mesh : meshes)
	{
		mesh->Delete();
	}
	for (unsigned int& texture : textures)
	{
		App->GetTextures()->DestroyTexture(texture);
	}
}
