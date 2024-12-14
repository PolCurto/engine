#include "ModuleEditor.h"
#include "Imgui/imgui.h"
#include "ImGui/imgui_impl_sdl2.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "GL/glew.h"
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"
#include "ModuleEditorCamera.h"
#include "ModuleHardware.h"
#include "ModuleRenderExercise.h"
#include "DirectXTex.h"
#include "tiny_gltf.h"
#include "Model.h"
#include "SDL.h"


ModuleEditor::ModuleEditor()
{
	quit = false;
}

ModuleEditor::~ModuleEditor()
{

}

bool ModuleEditor::PreInit()
{
	logs = new ImGuiTextBuffer();

	return true;
}

bool ModuleEditor::Init()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui_ImplSDL2_InitForOpenGL(App->GetWindow()->window, App->GetOpenGL()->GetContext());
	ImGui_ImplOpenGL3_Init();

	return true;
}

update_status ModuleEditor::PreUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update()
{
	Draw();

	if (quit)
		return UPDATE_STOP;

	// Renders at the end
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Needed for the viewports flag to work
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	delete logs;
	logs = nullptr;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void ModuleEditor::AddLog(const char* new_log) const
{
	if (logs != nullptr)
		logs->append(new_log);
}

void ModuleEditor::Draw()
{
	MainMenu();

	if (show_settings)
		SettingsMenu();

	if (show_demo)
		ImGui::ShowDemoWindow();

	if (show_about)
		AboutWindow();

	if (show_console)
		Console();
}

void ModuleEditor::MainMenu()
{
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("General"))
	{
		if (ImGui::MenuItem("Geometry properties"))
			show_properties = !show_properties;

		if (ImGui::MenuItem("Console"))
			show_console = !show_console;

		if (ImGui::MenuItem("Quit"))
			quit = true;

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Settings"))
	{
		if (ImGui::MenuItem("Editor settings"))
			show_settings = !show_settings;

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("Gui Demo"))
			show_demo = !show_demo;

		if (ImGui::MenuItem("Documentation"))
			App->RequestBrowser("https://github.com/PolCurto/engine");

		if (ImGui::MenuItem("Download Latest"))
			App->RequestBrowser("https://github.com/PolCurto/engine");

		if (ImGui::MenuItem("Report a bug"))
			App->RequestBrowser("https://github.com/PolCurto/engine");

		if (ImGui::MenuItem("About"))
			show_about = !show_about;

		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
}

void ModuleEditor::SettingsMenu()
{
	ImGui::Begin("Editor settings");

	ImGui::SeparatorText("Ms and Fps Graph");
	FPSCount();
	ImGui::Spacing();

	ImGui::SeparatorText("Modules Configuration");
	if (ImGui::CollapsingHeader("Window"))
	{
		WindowConfig();
	}
	ImGui::Spacing();
	if (ImGui::CollapsingHeader("Editor camera"))
	{
		CameraConfig();
	}
	ImGui::Spacing();
	if (ImGui::CollapsingHeader("OpenGL"))
	{
		OpenGLConfig();
	}
	ImGui::Spacing();
	if (ImGui::CollapsingHeader("Textures"))
	{
		TexturesConfig();
	}
	ImGui::Spacing();
	if (ImGui::CollapsingHeader("Model"))
	{
		ModelConfig();
	}
	ImGui::Spacing();

	ImGui::SeparatorText("Hardware Info");
	App->GetHardware()->ShowHardwareInfo();

	ImGui::End();
}

void ModuleEditor::WindowConfig()
{
	static bool borderless = false;
	static bool full_desktop = false;

	// Brightness Slider
	float brightness = App->GetWindow()->GetBrightness();
	if (ImGui::SliderFloat("Brightness", &brightness, 0, 1))
		App->GetWindow()->SetBrightness(brightness);

	// Width Slider
	int width = App->GetWindow()->GetWidth();
	if (ImGui::SliderInt("Width", &width, 0, 2000))
		App->GetWindow()->SetWidth(width);

	// Height Slider
	int height = App->GetWindow()->GetHeight();
	if (ImGui::SliderInt("Height", &height, 0, 2000))
		App->GetWindow()->SetHeight(height);

	// Set Fullscreen
	if (ImGui::Checkbox("Fullscreen", &fullscreen))
		App->GetWindow()->SetFullscreen(fullscreen);
	ImGui::SameLine();

	// Set Resizable
	if (ImGui::Checkbox("Resizable", &resizable))
		App->GetWindow()->SetResizable(resizable);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Restart to apply");

	// Set Borderless
	if (ImGui::Checkbox("Borderless", &borderless))
		App->GetWindow()->SetBorderless(borderless);
	ImGui::SameLine();

	// Set Full Desktop
	if (ImGui::Checkbox("Full Desktop", &full_desktop))
		App->GetWindow()->SetFullDesktop(full_desktop);
}

void ModuleEditor::CameraConfig()
{
	// Camera position
	float3 camera_pos = App->GetCamera()->GetCameraPosition();
	float pos[] = { camera_pos.x, camera_pos.y, camera_pos.z };
	if (ImGui::InputFloat3("Camera position", pos))
	{
		App->GetCamera()->SetPosition(pos[0], pos[1], pos[2]);
	}

	// Camera FOV
	float fov = App->GetCamera()->GetFOV();
	if (ImGui::SliderFloat("FOV", &fov, 30.0f, 180.0f))
	{
		App->GetCamera()->SetFOV(fov);
	}

	// Camera sensitivity
	float sensitivity = App->GetCamera()->GetSensitivity();
	if (ImGui::SliderFloat("Sensitivity", &sensitivity, 0.0f, 10.0f))
	{
		App->GetCamera()->SetSensitivity(sensitivity);
	}

	// Camera free movement speed
	float free_movement_speed = App->GetCamera()->GetFreeMovementSpeed();
	if (ImGui::SliderFloat("Free movement speed", &free_movement_speed, 0.0f, 100.0f))
	{
		App->GetCamera()->SetFreeMovementSpeed(free_movement_speed);
	}

	// Camera zoom speed
	float zoom_speed = App->GetCamera()->GetZoomSpeed();
	if (ImGui::SliderFloat("Zoom speed", &zoom_speed, 0.0f, 100.0f))
	{
		App->GetCamera()->SetZoomSpeed(zoom_speed);
	}

	// Camera zoom speed
	float drag_speed = App->GetCamera()->GetDragSpeed();
	if (ImGui::SliderFloat("Drag speed", &drag_speed, 0.0f, 20.0f))
	{
		App->GetCamera()->SetDragSpeed(drag_speed);
	}

	// Frustum planes
	float near_plane_dist = App->GetCamera()->GetNearPlaneDist();
	if (ImGui::InputFloat("Near plane distance", &near_plane_dist))
	{
		App->GetCamera()->SetPlaneDistances(near_plane_dist, App->GetCamera()->GetFarPlaneDist());
	}
	float far_plane_dist = App->GetCamera()->GetFarPlaneDist();
	if (ImGui::InputFloat("Far plane distance", &far_plane_dist))
	{
		App->GetCamera()->SetPlaneDistances(App->GetCamera()->GetNearPlaneDist(), far_plane_dist);
	}
}

void ModuleEditor::OpenGLConfig()
{
	static bool depth_test = true;
	if (ImGui::Checkbox("Depth test", &depth_test))
	{
		App->GetOpenGL()->SetDepthTest(depth_test);
	}
	
	static bool face_cull = true;
	if (ImGui::Checkbox("Face cull", &face_cull))
	{
		App->GetOpenGL()->SetFaceCull(face_cull);
	}

	static int front_face_mode = GL_CCW;
	bool changed = false;
	ImGui::Text("Front face mode");
	if (ImGui::RadioButton("Counter clock-wise", &front_face_mode, GL_CCW))
	{
		App->GetOpenGL()->SetFrontFaceMode(front_face_mode);
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Clock-wise", &front_face_mode, GL_CW))
	{
		App->GetOpenGL()->SetFrontFaceMode(front_face_mode);
	}	
}

void ModuleEditor::TexturesConfig() 
{
	static int wrap_mode = GL_CLAMP;
	static int mag_filter = GL_NEAREST;
	static int min_filter = GL_NEAREST;

	for (int i = 0; i < App->GetRenderExercise()->model->GetTexturesIDCount(); ++i)
	{
		ImGui::Text("Texture %d", i + 1);

		ImGui::Text("Wrap mode");
		ImGui::RadioButton("Repeat##2", &wrap_mode, GL_REPEAT);
		ImGui::SameLine();
		ImGui::RadioButton("Mirrored repeat##2", &wrap_mode, GL_MIRRORED_REPEAT);
		ImGui::RadioButton("Clamp##2", &wrap_mode, GL_CLAMP);
		ImGui::SameLine();
		ImGui::RadioButton("Clamp to border##2", &wrap_mode, GL_CLAMP_TO_BORDER);

		ImGui::Text("Min Filter");
		ImGui::RadioButton("Nearest##2", &min_filter, GL_NEAREST);
		ImGui::SameLine();
		ImGui::RadioButton("Linear##2", &min_filter, GL_LINEAR);

		ImGui::Text("Mag Filter");
		ImGui::RadioButton("Nearest##1", &mag_filter, GL_NEAREST);
		ImGui::SameLine();
		ImGui::RadioButton("Linear##1", &mag_filter, GL_LINEAR);

		App->GetRenderExercise()->model->SetTextureParameters({ i, wrap_mode, min_filter, mag_filter });
	}
}

void ModuleEditor::ModelConfig()
{
	float3 model_pos = *App->GetRenderExercise()->model->world_position;
	float pos[] = { model_pos.x, model_pos.y, model_pos.z };
	if (ImGui::InputFloat3("Model position", pos))
	{
		float3 new_pos(pos[0], pos[1], pos[2]);
		App->GetRenderExercise()->model->SetPosition(new_pos);
	}
}

void ModuleEditor::FPSCount()
{
	int vectors_length = 50;
	if (ms_log.size() < vectors_length)
	{
		ms_log.push_back(App->delta);
	}
	else
	{
		ms_log.pop_front();
		ms_log.emplace_back(App->delta);		
	}
	char title[25];
	std::vector<float> ms_log_vector(ms_log.begin(), ms_log.end());
	sprintf_s(title, 25, "Milliseconds %0.1f", ms_log.back());
	ImGui::PlotHistogram("##milliseconds", &ms_log_vector[0], ms_log_vector.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));

	std::vector<float> fps_log_vector(fps_log.begin(), fps_log.end());
	sprintf_s(title, 25, "Framerate %.1f", fps_log.back());
	ImGui::PlotHistogram("##framerate", &fps_log_vector.front(), fps_log_vector.size(), 0, title, 0.0f, 200.0f, ImVec2(310, 100));
}

void ModuleEditor::AddFPS(float new_fps)
{
	int vectors_length = 50;

	if (fps_log.size() < vectors_length)
	{
		fps_log.push_back(new_fps);
	}
	else
	{
		fps_log.pop_front();
		fps_log.emplace_back(new_fps);
	}
}

void ModuleEditor::AboutWindow() const
{
	ImGui::Begin("About");

	SDL_version ver;
	SDL_GetVersion(&ver);

	ImGui::Text("Engine name: %s", TITLE);
	ImGui::Text("Description: Made from scratch with C++ in the UPC School Advanced Programming for AAA games master's degree");
	ImGui::Text("Author: Pol Curto");
	ImGui::Text("Libraries:");
	ImGui::BulletText("SDL v%u.%u.%u", ver.major, ver.minor, ver.patch);
	ImGui::BulletText("Glew v2.1.0");
	ImGui::BulletText("MathGeoLib v1.5");
	ImGui::BulletText("ImGui v1.91.4");
	ImGui::BulletText("DirectXTex vOct2024");
	ImGui::BulletText("TinyGltf v2.9.3");
	ImGui::Text("License: MIT License");

	ImGui::End();
}

void ModuleEditor::ShowModelProperties(const std::vector<std::vector<int>>& meshes_data, const std::vector<std::vector<int>>& textures_data) const
{
	if (show_properties)
	{
		ImGui::Begin("Properties");

		ImGui::SeparatorText("Geometry properties");
		for (int i = 0; i < meshes_data.size(); ++i)
		{
			ImGui::Text("Mesh %d", i + 1);
			ImGui::Text("Number of vertices: %d", meshes_data[i][0]);
			ImGui::Text("Number of triangles: %d", meshes_data[i][1]);
			ImGui::Spacing();
		}

		ImGui::SeparatorText("Texture properties");
		for (int i = 0; i < textures_data.size(); ++i)
		{
			ImGui::Text("Texture %d", i + 1);
			ImGui::Text("Texture width: %d", textures_data[i][0]);
			ImGui::Text("Texture height: %d", textures_data[i][1]);
			ImGui::Spacing();
		}

		ImGui::End();
	}	
}

void ModuleEditor::Console() const
{
	ImGui::Begin("Console");

	ImGui::TextUnformatted(logs->begin(), logs->end());

	// Autoscroll only if the scroll is in the bottom position
	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())    
	{
		ImGui::SetScrollHereY(1.0f);
	}

	ImGui::End();
}

