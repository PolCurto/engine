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

	int LoadFile(const char* filename, DirectX::ScratchImage& scratch_image) const;
	void DestroyTexture(unsigned int texture) const;

private:

};