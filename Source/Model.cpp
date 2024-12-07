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
		DirectX::ScratchImage scratch_image;
		unsigned int texture_id = 0;
		if (src_material.pbrMetallicRoughness.baseColorTexture.index >= 0)
		{
			const tinygltf::Texture& texture = src_model.textures[src_material.pbrMetallicRoughness.baseColorTexture.index];
			const tinygltf::Image& image = src_model.images[texture.source];

			std::string texture_folder = "./Textures/";
			texture_id = App->GetTextures()->LoadFile(texture_folder.append(image.uri).c_str(), scratch_image);
			//texture_id = App->GetTextures()->LoadFile("./Textures/Baboon.dds");
		}
		textures_id.push_back(texture_id);
		textures_data.emplace(std::make_pair(texture_id, std::make_unique<DirectX::ScratchImage>(std::move(scratch_image))));
	}
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
	ImGui::Begin("Properties");

	for (const std::unique_ptr<Mesh>& mesh : meshes)
	{
		ImGui::SeparatorText("Geometry properties");
		ImGui::Text("Mesh name: %s", mesh->name);
		ImGui::Text("Number of vertices: %d", mesh->GetVertexCount());
		ImGui::Text("Number of triangles: %d", mesh->GetTrianglesCount());
	}

	ImGui::SeparatorText("Texture properties");

	for (const auto& data : textures_data)
	{
		DirectX::TexMetadata metadata = data.second->GetMetadata();
		ImGui::Text("Texture width: %d", metadata.width);
		ImGui::Text("Texture height: %d", metadata.height);
	}

	ImGui::End();
}

void Model::TextureOptions()
{
	ImGui::Begin("Textures parameters");

	for (const unsigned int& id : textures_id)
	{
		glBindTexture(GL_TEXTURE_2D, id);

		static int wrap_mode = GL_CLAMP;
		if (ImGui::CollapsingHeader("Wrap mode"))
		{
			ImGui::RadioButton("Repeat##2", &wrap_mode, GL_REPEAT);
			ImGui::SameLine();
			ImGui::RadioButton("Mirrored repeat##2", &wrap_mode, GL_MIRRORED_REPEAT);
			ImGui::RadioButton("Clamp##2", &wrap_mode, GL_CLAMP);
			ImGui::SameLine();
			ImGui::RadioButton("Clamp to border##2", &wrap_mode, GL_CLAMP_TO_BORDER);
		}

		static int mag_filter = GL_NEAREST;
		if (ImGui::CollapsingHeader("Mag Filter"))
		{
			ImGui::RadioButton("Nearest##1", &mag_filter, GL_NEAREST);
			ImGui::SameLine();
			ImGui::RadioButton("Linear##1", &mag_filter, GL_LINEAR);
		}

		static int min_filter = GL_NEAREST;
		if (ImGui::CollapsingHeader("Min Filter"))
		{
			ImGui::RadioButton("Nearest##2", &min_filter, GL_NEAREST);
			ImGui::SameLine();
			ImGui::RadioButton("Linear##2", &min_filter, GL_LINEAR);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_mode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_mode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	}

	ImGui::End();	
}

void Model::Delete() const
{
	for (const std::unique_ptr<Mesh>& mesh : meshes)
	{
		mesh->Delete();
	}
	for (const unsigned int& texture : textures_id)
	{
		App->GetTextures()->DestroyTexture(texture);
	}
}
