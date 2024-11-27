#include "ModuleTextures.h"

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

/*
bool ModuleTextures::LoadFile(const wchar_t* filename, DirectX::ScratchImage image)
{
	HRESULT hr;
	hr = DirectX::LoadFromDDSFile(filename, DirectX::DDS_FLAGS_NONE, nullptr, image);
	
	if (FAILED(hr))
	{
		LOG("Could not load file %s from DDS. Trying with TGA...", filename);
		hr = DirectX::LoadFromTGAFile(filename, nullptr, image);
	
		if (FAILED(hr))
		{
			LOG("Could not load file %s from TGA. Trying with WIC...", filename);
			hr = DirectX::LoadFromWICFile(filename, DirectX::WIC_FLAGS_NONE, nullptr, image);
	
			if (FAILED(hr))
			{
				LOG("Could not load file %s from WIC. No more methods implemented.", filename);
				return false;
			}
		}
	}

	return true;
}
*/