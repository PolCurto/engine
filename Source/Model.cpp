#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"

#include "Model.h"
#include "Mesh.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ImGui/imgui.h"
#include "DirectXTex.h"
#include "GL/glew.h"
#include "ModuleEditor.h"
#include "Math/MathAll.h"

Model::Model()
{
	local_position = std::make_unique<float3>();
	world_position = std::make_unique<float3>();
	max_positions = std::make_unique<float3>();
	min_positions = std::make_unique<float3>();
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
	//UpdatePosition();
	LoadMaterials(model);
}

void Model::LoadMaterials(const tinygltf::Model& src_model)
{
	LOG("Loading materials");

	for (const tinygltf::Material& src_material : src_model.materials)
	{
		DirectX::ScratchImage scratch_image;
		unsigned int texture_id = 0;
		if (src_material.pbrMetallicRoughness.baseColorTexture.index >= 0)
		{
			const tinygltf::Texture& texture = src_model.textures[src_material.pbrMetallicRoughness.baseColorTexture.index];
			const tinygltf::Image& image = src_model.images[texture.source];

			std::string texture_folder = "./Textures/";
			texture_id = App->GetTextures()->LoadFile(texture_folder.append(image.uri).c_str(), scratch_image);
		}
		textures_id.push_back(texture_id);
		textures_data.emplace(std::make_pair(texture_id, std::make_unique<DirectX::ScratchImage>(std::move(scratch_image))));
	}

	LOG("Materials loaded");
}

void Model::Render(const unsigned int program)
{
	for (const std::unique_ptr<Mesh>& mesh : meshes)
	{
		mesh->Render(program, textures_id);
	}
	UpdatePosition();
}

void Model::ShowModelInformation() const
{
	std::vector<int> data;

	std::vector<std::vector<int>> meshes_data;
	for (const std::unique_ptr<Mesh>& mesh : meshes)
	{
		data.emplace_back(mesh->GetVertexCount());
		data.emplace_back(mesh->GetTrianglesCount());
		meshes_data.emplace_back(data);
		data.clear();
	}

	std::vector<std::vector<int>> textures_metadata;
	for (const auto& tex_data : textures_data)
	{
		DirectX::TexMetadata metadata = tex_data.second->GetMetadata();
		data.emplace_back(metadata.width);
		data.emplace_back(metadata.height);
		textures_metadata.emplace_back(data);
		data.clear();
	}

	App->GetEditor()->ShowModelProperties(meshes_data, textures_metadata);
}

void Model::SetTextureParameters(const std::vector<int>&& params)
{
	glBindTexture(GL_TEXTURE_2D, textures_id[params[0]]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params[3]);
}

void Model::Delete()
{
	for (const std::unique_ptr<Mesh>& mesh : meshes)
		mesh->Delete();

	for (const unsigned int& texture : textures_id)
		App->GetTextures()->DestroyTexture(texture);
	
	meshes.clear();
	textures_id.clear();
	textures_data.clear();
}

void Model::UpdatePosition()
{
	for (int i = 0; i < meshes.size(); ++i)
	{
		if (i == 0)
		{
			*world_position = *meshes[i]->world_position;
			*local_position = *world_position + *meshes[i]->mesh_center;
			*max_positions = *meshes[i]->max_positions_world;
			*min_positions = *meshes[i]->min_positions_world;
		}
		else
		{

			if (meshes[i]->max_positions_world->x > max_positions->x)
				max_positions->x = meshes[i]->max_positions_world->x;

			if (meshes[i]->min_positions_world->x < min_positions->x)
				min_positions->x = meshes[i]->min_positions_world->x;

			if (meshes[i]->max_positions_world->y > max_positions->y)
				max_positions->y = meshes[i]->max_positions_world->y;

			if (meshes[i]->min_positions_world->y < min_positions->y)
				min_positions->y = meshes[i]->min_positions_world->y;

			if (meshes[i]->max_positions_world->z > max_positions->z)
				max_positions->z = meshes[i]->max_positions_world->z;

			if (meshes[i]->min_positions_world->z < min_positions->z)
				min_positions->z = meshes[i]->min_positions_world->z;
		}
	}

	//LOG("Max model positions: %f, %f, %f", max_positions->x, max_positions->y, max_positions->z);
	//LOG("Min model positions: %f, %f, %f", min_positions->x, min_positions->y, min_positions->z);
}
