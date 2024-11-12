#include "ModuleEditorCamera.h"
#include "ModuleOpenGL.h"
#include "ModuleInput.h"
#include "SDL.h"

ModuleEditorCamera::ModuleEditorCamera()
{

}

ModuleEditorCamera::~ModuleEditorCamera()
{

}

bool ModuleEditorCamera::Init()
{
	//frustum.type = FrustumType::PerspectiveFrustum;
	return true;
}

update_status ModuleEditorCamera::Update()
{	
	ProcessInput();
	SetFrustum();
	return UPDATE_CONTINUE;
}

bool ModuleEditorCamera::CleanUp()
{
	return true;
}

void ModuleEditorCamera::ProcessInput()
{
	const uint8_t* keys = App->GetInput()->GetKeyboard();

	float front_speed = 0;
	float right_speed = 0;
	float up_speed = 0;

	if (keys[SDL_SCANCODE_S])
	{
		LOG("FORWARD");
		++front_speed;
	}
	if (keys[SDL_SCANCODE_W])
	{
		LOG("BACKWARD");
		--front_speed;
	}
	if (keys[SDL_SCANCODE_D])
	{
		LOG("RIGHT");
		++right_speed;
	}
	if (keys[SDL_SCANCODE_A])
	{
		LOG("LEFT");
		--right_speed;
	}
	if (keys[SDL_SCANCODE_Q])
	{
		LOG("UP");
		++up_speed;
	}
	if (keys[SDL_SCANCODE_E])
	{
		LOG("DOWN");
		--up_speed;
	}

	float factor = 0.05f;

	camera_position.x += right_speed * factor;
	camera_position.y += up_speed * factor;
	camera_position.z += front_speed * factor;

	LOG("[Position] x: %d - y: %d - z: %d", camera_position.x, camera_position.y, camera_position.z);
}

void ModuleEditorCamera::SetFrustum()
{
	Frustum frustum;
	frustum.type = FrustumType::PerspectiveFrustum;

	// Get all the data using the frustrum class
	frustum.pos = camera_position;
	frustum.front = math::float3(0, 0, -1);
	frustum.up = math::float3::unitY;

	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;

	float aspect_ratio = static_cast<float>(App->GetOpenGL()->GetWindowWidth()) / static_cast<float>(App->GetOpenGL()->GetWindowHeight());
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect_ratio);

	projection_matrix = frustum.ProjectionMatrix();
	view_matrix = frustum.ViewMatrix();
	model_matrix = math::float4x4::FromTRS(float3(0.0f, 1.0f, -3.0f),
		float4x4::RotateZ(pi / 4.0f),
		float3(0.5f, 0.5f, 0.5f));

	// TODO: Implement my own LookAt function

}

