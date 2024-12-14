#include "ModuleHardware.h"

#include "SDL.h"
#include "GL/glew.h"
#include "Imgui/imgui.h"

ModuleHardware::ModuleHardware()
{
}

ModuleHardware::~ModuleHardware()
{
}

bool ModuleHardware::Init()
{
	sdl_version = new SDL_version();
	SDL_GetVersion(sdl_version);

	return true;
}

update_status ModuleHardware::Update()
{
	
	return UPDATE_CONTINUE;
}

void ModuleHardware::ShowHardwareInfo() const
{
	ImGui::Text("CPUs: %i (Cache: %i KB)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
	ImGui::Text("System RAM: %.2f GB", static_cast<float>(SDL_GetSystemRAM()) / 1024);
	ImGui::Separator();

	ImGui::Text("GPU vendor: %s", glGetString(GL_VENDOR));
	ImGui::Text("GPU model: %s", glGetString(GL_RENDERER));

	if (glewIsExtensionSupported("GL_NVX_gpu_memory_info")) // For Nvidia GPUs
	{
		GLint total_memory = 0, dedicated_memory = 0, available_memory = 0;
		
		glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &total_memory);
		glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &available_memory);
		glGetIntegerv(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &dedicated_memory);
		
		ImGui::Text("Total VRAM: %.2f MB", static_cast<float>(total_memory) / 1024.0f);
		ImGui::Text("Dedicated VRAM: %.2f MB", static_cast<float>(dedicated_memory) / 1024.0f);
		ImGui::Text("Available VRAM: %.2f MB", static_cast<float>(available_memory) / 1024.0f);
	}

	if (glewIsExtensionSupported("GL_ATI_meminfo")) // For AMD GPUs
	{
		GLint vbo_memory[4];
		glGetIntegerv(GL_VBO_FREE_MEMORY_ATI, vbo_memory);

		ImGui::Text("Total VRAM: %.2f MB", static_cast<float>(vbo_memory[0]) / 1024.0f);
		ImGui::Text("Available VRAM: %.2f MB", static_cast<float>(vbo_memory[1]) / 1024.0f);
	}
}

bool ModuleHardware::CleanUp()
{
	delete sdl_version;

	return true;
}