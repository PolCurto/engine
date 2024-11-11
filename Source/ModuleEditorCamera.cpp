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
	GetInput();
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

	if (keys[SDL_SCANCODE_UP])
	{
		LOG("UP");
	}
}

void ModuleEditorCamera::SetFrustum()
{
	Frustum frustum;
	frustum.type = FrustumType::PerspectiveFrustum;

	// Get all the data using the frustrum class
	frustum.pos = math::float3(0, 0.5f, 5);
	frustum.front = math::float3(0, 0, -1);
	frustum.up = math::float3::unitY;

	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;

	float aspect_ratio = App->GetOpenGL()->GetWindowWidth() / App->GetOpenGL()->GetWindowHeight();
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect_ratio);

	projection_matrix = frustum.ProjectionMatrix();
	view_matrix = frustum.ViewMatrix();
	model_matrix = math::float4x4::FromTRS(float3(0.0f, 1.0f, -3.0f),
		float4x4::RotateZ(pi / 4.0f),
		float3(0.5f, 0.5f, 0.5f));

	// TODO: Implement my own LookAt function

}

