#include "ModuleTextures.h"
#include "DirectXTex.h"
#include "GL/glew.h"


ModuleTextures::ModuleTextures()
{

}

ModuleTextures::~ModuleTextures()
{

}

bool ModuleTextures::Init()
{
	return true;
}

update_status ModuleTextures::Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleTextures::CleanUp()
{
	return true;
}


int ModuleTextures::LoadFile(const char* filename, DirectX::ScratchImage& scratch_image) const
{
	// Convert char* to wchar_t*
	size_t size = strlen(filename) + 1;
	wchar_t* wc_filename = new wchar_t[size];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wc_filename, size, filename, _TRUNCATE);

	DirectX::ScratchImage image;
	bool load_ok = true;
	HRESULT hr;
	hr = DirectX::LoadFromDDSFile(wc_filename, DirectX::DDS_FLAGS_NONE, nullptr, image);
	
	if (FAILED(hr))
	{
		LOG("Could not load file '%s' from DDS.Trying with TGA...", filename);
		hr = DirectX::LoadFromTGAFile(wc_filename, nullptr, image);
	
		if (FAILED(hr))
		{
			LOG("Could not load file '%s' from TGA. Trying with WIC...", filename);
			hr = DirectX::LoadFromWICFile(wc_filename, DirectX::WIC_FLAGS_NONE, nullptr, image);
	
			if (FAILED(hr))
			{
				LOG("Could not load file '%s' from WIC. No more methods implemented.", filename);
				delete[] wc_filename;
				return -1;
			}
		}
	}

	delete[] wc_filename;

	// Generate textures
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	DirectX::TexMetadata metadata = image.GetMetadata();

	GLint internal_format;
	GLenum format;
	GLenum type;

	// Get the texture formats
	switch (metadata.format)
	{
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		case DXGI_FORMAT_R8G8B8A8_UNORM:
			internal_format = GL_RGBA8;
			format = GL_RGBA;
			type = GL_UNSIGNED_BYTE;
			break;
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
		case DXGI_FORMAT_B8G8R8A8_UNORM:
			internal_format = GL_RGBA8;
			format = GL_BGRA;
			type = GL_UNSIGNED_BYTE;
			break;
		case DXGI_FORMAT_B5G6R5_UNORM:
			internal_format = GL_RGB8;
			format = GL_BGR;
			type = GL_UNSIGNED_BYTE;
			break;
		default:
			assert(false && "Unsupported format");
	}

	// Load texture data to VRAM
	for (size_t i = 0; i < metadata.mipLevels; ++i)
	{
		const DirectX::Image* mip = image.GetImage(i, 0, 0);
		glTexImage2D(GL_TEXTURE_2D, i, internal_format, mip->width, mip->height, 0, format, type, mip->pixels);
	}

	if (metadata.mipLevels == 1)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	// Texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	if (metadata.mipLevels > 1)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, metadata.mipLevels - 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	scratch_image = std::move(image);
	return texture;
}

void ModuleTextures::DestroyTexture(unsigned int texture) const
{
	glDeleteTextures(1, &texture);
}