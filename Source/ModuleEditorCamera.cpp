#include "ModuleEditorCamera.h"
#include "ModuleOpenGL.h"
#include "ModuleInput.h"
#include "SDL.h"
#include "Model.h"
#include "ModuleRenderExercise.h"
#include "ModuleWindow.h"
#include "MathGeoLib.h"

ModuleEditorCamera::ModuleEditorCamera()
{
	projection_matrix = std::make_unique<float4x4>(float4x4::zero);
	view_matrix = std::make_unique<float4x4>(float4x4::zero);
	camera_position = std::make_unique<float3>(float3::zero);
	frustum = std::make_unique<Frustum>();
}

ModuleEditorCamera::~ModuleEditorCamera()
{

}

bool ModuleEditorCamera::Init()
{
	free_movement_speed = 15;
	zoom_speed = 50;
	drag_speed = 10;
	sensitivity = 0.4f;

	frustum->type = FrustumType::PerspectiveFrustum;
	frustum->front = math::float3(0, 0, -1);
	frustum->up = math::float3::unitY;

	frustum->nearPlaneDistance = 0.01f;
	frustum->farPlaneDistance = 100.0f;
	frustum->horizontalFov = DegToRad(90);

	aspect_ratio = static_cast<float>(App->GetWindow()->GetWidth()) / static_cast<float>(App->GetWindow()->GetHeight());
	frustum->verticalFov = 2.0f * atanf(tanf(frustum->horizontalFov * 0.5f) * (1 / aspect_ratio));
	frustum->pos = *camera_position;
	*projection_matrix = frustum->ProjectionMatrix();

	return true;
}

update_status ModuleEditorCamera::Update()
{	
	ProcessInput();
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

	if (keys[SDL_SCANCODE_F]) // Focus
	{
		FocusGeometry();
	}
	else if (App->GetInput()->GetMouseButtons()[MIDDLE_BUTTON]) // Drag
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		*camera_position += App->GetInput()->GetMouseMotionY() * drag_speed * frustum->up * (App->delta/1000.0f) * factor;
		right_speed -= App->GetInput()->GetMouseMotionX() * drag_speed;
	}
	else if (keys[SDL_SCANCODE_LALT] && App->GetInput()->GetMouseButtons()[RIGHT_BUTTON]) // Zoom with vertical mouse motion
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		front_speed -= App->GetInput()->GetMouseMotionY() * zoom_speed;
	}
	else if (keys[SDL_SCANCODE_LALT] && App->GetInput()->GetMouseButtons()[LEFT_BUTTON]) // Orbit around the selected object
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);

		float3 model_pos = *App->GetRenderExercise()->model->local_position;
		float3 model_front = (*camera_position - model_pos).Normalized();

		float yaw_rot = DegToRad(-App->GetInput()->GetMouseMotionX());
		float pitch_rot = DegToRad(-App->GetInput()->GetMouseMotionY());

		if (yaw_rot)
		{
			float4x4 yaw_rotation = float4x4::RotateY(yaw_rot);
			model_front = yaw_rotation.MulDir(model_front).Normalized();
		}

		if ((pitch_rot < 0 && frustum->front.y > -0.9f) || (pitch_rot > 0 && frustum->front.y < 0.9f))
		{
			float4x4 pitch_rotation = float4x4::RotateAxisAngle(frustum->WorldRight(), pitch_rot);
			model_front = pitch_rotation.MulDir(model_front).Normalized();
		}

		*camera_position = model_pos + model_front * (*camera_position - model_pos).Length();
		SetOrientation(model_pos);
	}
	else if (App->GetInput()->GetMouseButtons()[RIGHT_BUTTON]) // Enables WASD movement and mouse motion camera movement
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
			factor = 2;

		yaw_deg = -App->GetInput()->GetMouseMotionX() * sensitivity;
		pitch_deg = -App->GetInput()->GetMouseMotionY() * sensitivity;

		// Rotate around world Y axis
		if (yaw_deg)
		{
			float4x4 yaw_rotation = float4x4::RotateY(math::DegToRad(yaw_deg));
			frustum->up = yaw_rotation.MulDir(frustum->up).Normalized();
			frustum->front = yaw_rotation.MulDir(frustum->front).Normalized();
		}

		// Rotate around local X axis
		if ((pitch_deg < 0 && frustum->front.y > -0.9f) || (pitch_deg > 0 && frustum->front.y < 0.9f))
		{
			float4x4 pitch_rotation = float4x4::RotateAxisAngle(frustum->WorldRight(), math::DegToRad(pitch_deg));
			frustum->up = pitch_rotation.MulDir(frustum->up).Normalized();
			frustum->front = pitch_rotation.MulDir(frustum->front).Normalized();
		}
	}
	else // Mouse wheel zoom
	{
		front_speed += App->GetInput()->GetMouseWheel() * zoom_speed * 5;
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}

	*camera_position += (front_speed * frustum->front + right_speed * frustum->WorldRight() + up_speed * float3::unitY) * (App->delta / 1000.0f) * factor;
	frustum->pos = *camera_position;
	*view_matrix = static_cast<float4x4>(frustum->ViewMatrix());

}

void ModuleEditorCamera::FocusGeometry()
{
	float diagonal = App->GetRenderExercise()->model->max_positions->Distance(*App->GetRenderExercise()->model->min_positions);
	float3 pos = *App->GetRenderExercise()->model->local_position;

	camera_position->x = pos.x;
	camera_position->y = pos.y;
	camera_position->z = pos.z + diagonal;

	SetOrientation(pos);
}

void ModuleEditorCamera::SetFOV(const float new_fov)
{
	frustum->horizontalFov = DegToRad(new_fov);
	frustum->verticalFov = 2.0f * atanf(tanf(frustum->horizontalFov * 0.5f) * (1 / aspect_ratio));
	*projection_matrix = frustum->ProjectionMatrix();
}

void ModuleEditorCamera::SetAspectRatio(const float new_aspect_ratio)
{
	aspect_ratio = new_aspect_ratio;
	frustum->verticalFov = 2.0f * atanf(tanf(frustum->horizontalFov * 0.5f) * (1 / aspect_ratio));
	*projection_matrix = frustum->ProjectionMatrix();
}

void ModuleEditorCamera::SetPlaneDistances(const float near_plane_dist, const float far_plane_dist)
{
	frustum->nearPlaneDistance = near_plane_dist;
	frustum->farPlaneDistance = far_plane_dist;
	*projection_matrix = frustum->ProjectionMatrix();
}

void ModuleEditorCamera::SetPosition(const float3& new_position)
{
	frustum->pos = *camera_position = new_position;
	*view_matrix = static_cast<float4x4>(frustum->ViewMatrix());
}

void ModuleEditorCamera::SetPosition(const float x, const float y, const float z)
{
	float3 new_position(x, y, z);
	frustum->pos = *camera_position = new_position;
	*view_matrix = static_cast<float4x4>(frustum->ViewMatrix());
}

void ModuleEditorCamera::SetOrientation(const float3& target)
{
	frustum->front = (target - *camera_position).Normalized();
	
	float3 frustum_right = float3::unitY.Cross(frustum->front).Normalized();
	frustum->up = frustum->front.Cross(frustum_right).Normalized();
	*view_matrix = static_cast<float4x4>(frustum->ViewMatrix());
}

float ModuleEditorCamera::GetFOV() const
{
	return RadToDeg(frustum->horizontalFov);
}

float ModuleEditorCamera::GetNearPlaneDist() const
{
	return frustum->nearPlaneDistance;
}

float ModuleEditorCamera::GetFarPlaneDist() const
{
	return frustum->farPlaneDistance;
}
