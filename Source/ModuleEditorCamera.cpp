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
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.front = math::float3(0, 0, -1);
	frustum.up = math::float3::unitY;
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
	float factor = 1;

	if (keys[SDL_SCANCODE_W])
		++front_speed;
	if (keys[SDL_SCANCODE_S])
		--front_speed;
	if (keys[SDL_SCANCODE_D])
		++right_speed;
	if (keys[SDL_SCANCODE_A])
		--right_speed;
	if (keys[SDL_SCANCODE_Q])
		++up_speed;
	if (keys[SDL_SCANCODE_E])
		--up_speed;
	if (keys[SDL_SCANCODE_LSHIFT])
		factor = 3;

	float delta = 0.05f;

	math::DegToRad(2);

	camera_position += (front_speed * frustum.front + right_speed * frustum.WorldRight() + up_speed * float3::unitY) * delta * factor;

	LOG("[Position] x: %f - y: %f - z: %f", camera_position.x, camera_position.y, camera_position.z);

	float pitch_deg = 0;
	float yaw_deg = 0;

	if (keys[SDL_SCANCODE_UP])
		pitch_deg += 0.1;
	if (keys[SDL_SCANCODE_DOWN])
		pitch_deg -= 0.1;
	if (keys[SDL_SCANCODE_RIGHT])
		yaw_deg += 0.1;
	if (keys[SDL_SCANCODE_LEFT])
		yaw_deg -= 0.1;

	float4x4 yaw_rotation = float4x4::RotateY(math::DegToRad(yaw_deg));

	// Rotate around X axis
	if (pitch_deg)
	{
		float4x4 pitch_rotation = float4x4::RotateX(math::DegToRad(pitch_deg));
		float3 oldFront = frustum.front.Normalized();
		float3 oldUp = frustum.up.Normalized();
		frustum.up = pitch_rotation.MulDir(oldUp);
		frustum.front = pitch_rotation.MulDir(oldFront);
	}

	if (yaw_deg)
	{

	}

	//float3 oldFront = frustum.front.Normalized();
	//float3 oldUp = frustum.front.Normalized();
	//frustum.front = yaw_rotation.MulDir(oldFront);
	//frustum.up = yaw_rotation.MulDir(oldUp);

	LOG("[Front] x: %f - y: %f - z: %f", frustum.front.x, frustum.front.y, frustum.front.z);
	LOG("[Up] x: %f - y: %f - z: %f", frustum.up.x, frustum.up.y, frustum.up.z);
}

void ModuleEditorCamera::SetFrustum()
{
	// Get all the data using the frustrum class
	frustum.pos = camera_position;

	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;

	float aspect_ratio = static_cast<float>(App->GetOpenGL()->GetWindowWidth()) / static_cast<float>(App->GetOpenGL()->GetWindowHeight());
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect_ratio);

	projection_matrix = frustum.ProjectionMatrix();
	view_matrix = static_cast<float4x4>(frustum.ViewMatrix());

	// TODO: Implement my own LookAt function

}

