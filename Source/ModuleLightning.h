#ifndef __MODULE_LIGHTNING_H__
#define __MODULE_LIGHTNING_H__

#include "Module.h"
#include <vector>

struct SDL_version;

class ModuleLightning : public Module
{
public:

	ModuleLightning();
	~ModuleLightning();

	void GetLightPosition(std::vector<float>& pos_to_fill) const { pos_to_fill = light_pos; }
	void GetLightColor(std::vector<float>& color_to_fill) const { color_to_fill = light_color; }
	void GetAmbientColor(std::vector<float>& color_to_fill) const { color_to_fill = ambient_color; }

	void SetLightPosition(const std::vector<float>& new_pos) { light_pos = new_pos; }
	void SetLightColor(const std::vector<float>& new_color) { light_color = new_color; }
	void SetAmbientColor(const std::vector<float>& new_color) { ambient_color = new_color; }
	
	void SetLightPosition(const float x, const float y, const float z) { light_pos = { x, y, z }; }
	void SetLightColor(const float x, const float y, const float z) { light_color = { x, y, z }; }
	void SetAmbientColor(const float x, const float y, const float z) { ambient_color = { x, y, z }; }

private:
	std::vector<float> light_pos = { 50, 100, 50 };
	std::vector<float> light_color = { 1.0f, 1.0f, 1.0f };
	std::vector<float> ambient_color = { 0.1f, 0.1f, 0.1f };

};

#endif // __MODULE_LIGHTNING_H__

