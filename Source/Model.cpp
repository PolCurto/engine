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

void Model::Render(const unsigned int program) const
{
	for (const std::unique_ptr<Mesh>& mesh : meshes)
	{
		mesh->Render(program, textures_id);
	}
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

void Model::TextureOptions()
{
	ImGui::Begin("Textures parameters");

	for (const unsigned int& id : textures_id)
	{
		glBindTexture(GL_TEXTURE_2D, id);

		static int wrap_mode = GL_CLAMP;
		ImGui::SeparatorText("Wrap mode");
		ImGui::RadioButton("Repeat##2", &wrap_mode, GL_REPEAT);
		ImGui::SameLine();
		ImGui::RadioButton("Mirrored repeat##2", &wrap_mode, GL_MIRRORED_REPEAT);
		ImGui::RadioButton("Clamp##2", &wrap_mode, GL_CLAMP);
		ImGui::SameLine();
		ImGui::RadioButton("Clamp to border##2", &wrap_mode, GL_CLAMP_TO_BORDER);
		
		static int mag_filter = GL_NEAREST;
		ImGui::SeparatorText("Mag Filter");
		ImGui::RadioButton("Nearest##1", &mag_filter, GL_NEAREST);
		ImGui::SameLine();
		ImGui::RadioButton("Linear##1", &mag_filter, GL_LINEAR);
		
		static int min_filter = GL_NEAREST;
		ImGui::SeparatorText("Min Filter");
		ImGui::RadioButton("Nearest##2", &min_filter, GL_NEAREST);
		ImGui::SameLine();
		ImGui::RadioButton("Linear##2", &min_filter, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_mode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_mode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	}

	ImGui::End();	
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
