#include "ModuleEditor.h"
#include "Imgui/imgui.h"
#include "ImGui/imgui_impl_sdl2.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "GL/glew.h"
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"

ModuleEditor::ModuleEditor()
{

}

ModuleEditor::~ModuleEditor()
{

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
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return true;
}

void ModuleEditor::Draw()
{
	MainMenu();
	
	ImGui::Begin("Configuration");
	ImGui::Text("Options");

	if (ImGui::CollapsingHeader("Application"))
	{
		FPSCount();
	}

	static bool borderless = false;
	static bool full_desktop = false;

	if (ImGui::CollapsingHeader("Window"))
	{
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

	ImGui::End();
	
	if (show_demo)
		ImGui::ShowDemoWindow();
}

void ModuleEditor::MainMenu()
{
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("help"))
	{
		if (ImGui::MenuItem("Gui Demo"))
			show_demo = !show_demo;

		if (ImGui::MenuItem("Documentation"))
			App->RequestBrowser("https://github.com/PolCurto/engine");

		if (ImGui::MenuItem("Download Latest"))
			App->RequestBrowser("https://github.com/PolCurto/engine");

		if (ImGui::MenuItem("Report a bug"))
			App->RequestBrowser("https://github.com/PolCurto/engine");

		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
}

void ModuleEditor::FPSCount()
{
	int vectors_length = 50;
	if (ms_log.size() < vectors_length)
		ms_log.emplace_back(App->GetDelta());
	else
	{
		ms_log.pop_front();
		ms_log.emplace_back(App->GetDelta());
	}

	char title[25];
	//sprintf_s(title, 25, "Framerate %.1f", fps_log.back());
	//ImGui::PlotHistogram("framerate", &fps_log.front(), fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
	sprintf_s(title, 25, "Milliseconds %0.1f", ms_log.back());
	ImGui::PlotHistogram("##milliseconds", &ms_log.front(), 50, 0, title, 0.0f, 40.0f, ImVec2(310, 100));
}