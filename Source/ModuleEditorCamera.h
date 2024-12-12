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

	void FocusGeometry();
	void SetFOV(float new_fov);
	void SetAspectRatio(float new_aspect_ratio);
	void SetPlaneDistances(float near_plane, float far_plane);
	void SetPosition(const float3& new_position);
	void SetPosition(const float x, const float y, const float z);
	void SetOrientation(const float3& new_front);

	float3 GetCameraPosition() const { return camera_position; }
	float GetFOV() const { return RadToDeg(frustum.horizontalFov); }
	float GetSensitivity() const { return sensitivity; }
	float GetFreeMovementSpeed() const { return free_movement_speed; }
	float GetZoomSpeed() const { return zoom_speed; }
	float GetDragSpeed() const { return drag_speed; }
	float GetNearPlaneDist() const { return frustum.nearPlaneDistance; }
	float GetFarPlaneDist() const { return frustum.farPlaneDistance; }

	void SetSensitivity(float new_sens) { sensitivity = new_sens; }
	void SetFreeMovementSpeed(float new_speed) { free_movement_speed = new_speed; }
	void SetZoomSpeed(float new_speed) { zoom_speed = new_speed; }
	void SetDragSpeed(float new_speed) { drag_speed = new_speed; }


private:
	void SetFrustum();
	void ProcessInput();

	Frustum frustum;
	float4x4 projection_matrix = float4x4::zero;
	float4x4 view_matrix = float4x4::zero;
	float3 camera_position = float3::zero;

	float aspect_ratio = 0;

	float free_movement_speed = 0;
	float zoom_speed = 0;
	float drag_speed = 0;
	float sensitivity = 0;
	float distance_from_model = 0;
	float3 model_front = float3::zero;

	bool dummy = true;
	bool is_orbiting = false;
};