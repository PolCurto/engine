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
	camera_position.y = 1.0f;
	free_movement_speed = 15;
	zoom_speed = 50;
	drag_speed = 10;
	sensitivity = 0.4f;

	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.front = math::float3(0, 0, -1);
	frustum.up = math::float3::unitY;

	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.horizontalFov = DegToRad(90);

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

	// Add the mouse wheel motion to the movement

	float yaw_deg = 0;
	float pitch_deg = 0;

	if (keys[SDL_SCANCODE_F])
	{
		FocusGeometry();
	}
	else if (App->GetInput()->GetMouseButtons()[MIDDLE_BUTTON])
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		camera_position += App->GetInput()->GetMouseMotionY() * drag_speed * frustum.up * (App->delta/1000.0f) * factor;
		right_speed -= App->GetInput()->GetMouseMotionX() * drag_speed;
	}
	else if (keys[SDL_SCANCODE_LALT] && App->GetInput()->GetMouseButtons()[RIGHT_BUTTON])    // Zoom with vertical mouse motion
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		front_speed -= App->GetInput()->GetMouseMotionY() * zoom_speed;
	}
	else if (keys[SDL_SCANCODE_LALT] && App->GetInput()->GetMouseButtons()[LEFT_BUTTON])    // Orbit around the selected object
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		LOG("ALT");
	}
	else if (App->GetInput()->GetMouseButtons()[RIGHT_BUTTON])    // Enables WASD movement and mouse motion camera movement
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);

		if (keys[SDL_SCANCODE_W])
			front_speed += free_movement_speed;
		if (keys[SDL_SCANCODE_S])
			front_speed -= free_movement_speed;
		if (keys[SDL_SCANCODE_D])
			right_speed += free_movement_speed;
		if (keys[SDL_SCANCODE_A])
			right_speed -= free_movement_speed;
		if (keys[SDL_SCANCODE_Q])
			up_speed += free_movement_speed;
		if (keys[SDL_SCANCODE_E])
			up_speed -= free_movement_speed;
		if (keys[SDL_SCANCODE_LSHIFT])
			factor = 3;

		yaw_deg = -App->GetInput()->GetMouseMotionX() * sensitivity;
		pitch_deg = -App->GetInput()->GetMouseMotionY() * sensitivity;
	}
	else
	{
		front_speed += App->GetInput()->GetMouseWheel() * zoom_speed * 5;
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}

	camera_position += (front_speed * frustum.front + right_speed * frustum.WorldRight() + up_speed * float3::unitY) * (App->delta / 1000.0f) * factor;

	// Rotate around world Y axis
	if (yaw_deg)
	{
		float4x4 yaw_rotation = float4x4::RotateY(math::DegToRad(yaw_deg));
		float3 oldFront = frustum.front.Normalized();
		float3 oldUp = frustum.up.Normalized();
		frustum.up = yaw_rotation.MulDir(oldUp);
		frustum.front = yaw_rotation.MulDir(oldFront);
	}

	// Rotate around local X axis
	if ((pitch_deg < 0 && frustum.front.y > -0.9f) || (pitch_deg > 0 && frustum.front.y < 0.9f))
	{
		float4x4 pitch_rotation = float4x4::RotateAxisAngle(frustum.WorldRight(), math::DegToRad(pitch_deg));
		float3 oldFront = frustum.front.Normalized();
		float3 oldUp = frustum.up.Normalized();
		frustum.up = pitch_rotation.MulDir(oldUp);
		frustum.front = pitch_rotation.MulDir(oldFront);
	}
}

void ModuleEditorCamera::FocusGeometry()
{

}

void ModuleEditorCamera::SetFrustum()
{
	// Get all the data using the frustrum class
	frustum.pos = camera_position;
	//projection_matrix = frustum.ProjectionMatrix();
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

void ModuleEditorCamera::SetPosition(const float x, const float y, const float z)
{
	float3 new_position(x, y, z);
	frustum.pos = camera_position = new_position;
}

void ModuleEditorCamera::SetOrientation(const float3& new_front)
{
	frustum.front = new_front;
}



