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

	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.horizontalFov = DegToRad(120.0);

	aspect_ratio = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
	frustum.verticalFov = 2.0f * atanf(tanf(frustum.horizontalFov * 0.5f) * (1 / aspect_ratio));
	frustum.pos = camera_position;
	projection_matrix = frustum.ProjectionMatrix();

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
		pitch_deg += 1;
	if (keys[SDL_SCANCODE_DOWN])
		pitch_deg -= 1;
	if (keys[SDL_SCANCODE_LEFT])
		yaw_deg += 1;
	if (keys[SDL_SCANCODE_RIGHT])
		yaw_deg -= 1;

	// Rotate around local X axis
	if ((pitch_deg < 0 && frustum.front.y > -0.99f) || (pitch_deg > 0 && frustum.front.y < 0.99f))
	{
		float4x4 pitch_rotation = float4x4::RotateAxisAngle(frustum.WorldRight(), math::DegToRad(pitch_deg));
		float3 oldFront = frustum.front.Normalized();
		float3 oldUp = frustum.up.Normalized();
		frustum.up = pitch_rotation.MulDir(oldUp);
		frustum.front = pitch_rotation.MulDir(oldFront);
	}

	// Rotate around world Y axis
	if (yaw_deg)
	{
		float4x4 yaw_rotation = float4x4::RotateY(math::DegToRad(yaw_deg));
		float3 oldFront = frustum.front.Normalized();
		float3 oldUp = frustum.up.Normalized();
		frustum.up = yaw_rotation.MulDir(oldUp);
		frustum.front = yaw_rotation.MulDir(oldFront);
	}

	LOG("[Front] x: %f - y: %f - z: %f", frustum.front.x, frustum.front.y, frustum.front.z);
	LOG("[Up] x: %f - y: %f - z: %f", frustum.up.x, frustum.up.y, frustum.up.z);
}

void ModuleEditorCamera::SetFrustum()
{
	// Get all the data using the frustrum class
	frustum.pos = camera_position;

	//LOG("Aspect ration: %f. Width: %d. Height: %d.", aspect_ratio, App->GetOpenGL()->GetWindowWidth(), App->GetOpenGL()->GetWindowHeight());

	view_matrix = static_cast<float4x4>(frustum.ViewMatrix());
	// TODO: Implement my own LookAt function

}

void ModuleEditorCamera::SetFOV(float new_fov)
{
	frustum.horizontalFov = DegToRad(new_fov);
	frustum.verticalFov = 2.0f * atanf(tanf(frustum.horizontalFov * 0.5f) * (1 / aspect_ratio));
	projection_matrix = frustum.ProjectionMatrix();
}

void ModuleEditorCamera::SetAspectRatio(float new_aspect_ratio)
{
	aspect_ratio = new_aspect_ratio;
	frustum.verticalFov = 2.0f * atanf(tanf(frustum.horizontalFov * 0.5f) * (1 / aspect_ratio));
	projection_matrix = frustum.ProjectionMatrix();
}

void ModuleEditorCamera::SetPlaneDistances(float near_plane_dist, float far_plane_dist)
{
	frustum.nearPlaneDistance = near_plane_dist;
	frustum.farPlaneDistance = far_plane_dist;
	projection_matrix = frustum.ProjectionMatrix();
}

void ModuleEditorCamera::SetPosition(const float3& new_position)
{
	frustum.pos = camera_position = new_position;
}

