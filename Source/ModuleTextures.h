#pragma once
#include "Module.h"

namespace DirectX
{
	class ScratchImage;
}


class ModuleTextures : public Module
{
public:

	ModuleTextures();
	~ModuleTextures();

	bool Init();
	update_status Update();
	bool CleanUp();

	bool LoadFile(const wchar_t* filename, DirectX::ScratchImage& image) const;

private:

};