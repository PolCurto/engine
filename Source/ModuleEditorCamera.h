#pragma once
#include "Module.h"
#include "MathGeoLib.h"

class ModuleEditorCamera : public Module
{
public:

	ModuleEditorCamera();
	~ModuleEditorCamera();

	bool Init();
	update_status Update();
	bool CleanUp();

	float4x4 GetProjectionMatrix() const { return projection_matrix; }
	float4x4 GetViewMatrix() const { return view_matrix; }

	void SetFOV(float new_fov);
	void SetAspectRatio(float new_aspect_ratio);
	void SetPlaneDistances(float near_plane, float far_plane);
	void SetPosition(const float3& new_position);


private:
	void SetFrustum();
	void ProcessInput();

	Frustum frustum;
	float4x4 projection_matrix = float4x4::zero;
	float4x4 view_matrix = float4x4::zero;
	float3 camera_position = float3::zero;

	float aspect_ratio = 0;

	float movement_speed = 0;
	float sensitivity = 0;
};