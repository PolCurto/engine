#pragma once
#include <memory>
#include "Module.h"

namespace math
{
	class Frustum;
	class float3;
	class float4x4;
}

class ModuleEditorCamera : public Module
{
public:

	ModuleEditorCamera();
	~ModuleEditorCamera();

	bool Init();
	update_status Update();
	bool CleanUp();

	math::float4x4& GetProjectionMatrix() const { return *projection_matrix; }
	math::float4x4& GetViewMatrix() const { return *view_matrix; }

	void FocusGeometry();
	void SetFOV(const float new_fov);
	void SetAspectRatio(const float new_aspect_ratio);
	void SetPlaneDistances(const float near_plane, const float far_plane);
	void SetPosition(const math::float3& new_position);
	void SetPosition(const float x, const float y, const float z);
	void SetOrientation(const math::float3& new_front);

	math::float3& GetCameraPosition() const { return *camera_position; }
	float GetFOV() const;
	float GetSensitivity() const { return sensitivity; }
	float GetFreeMovementSpeed() const { return free_movement_speed; }
	float GetZoomSpeed() const { return zoom_speed; }
	float GetDragSpeed() const { return drag_speed; }
	float GetNearPlaneDist() const;
	float GetFarPlaneDist() const;

	void SetSensitivity(float new_sens) { sensitivity = new_sens; }
	void SetFreeMovementSpeed(float new_speed) { free_movement_speed = new_speed; }
	void SetZoomSpeed(float new_speed) { zoom_speed = new_speed; }
	void SetDragSpeed(float new_speed) { drag_speed = new_speed; }

private:
	void ProcessInput();

	std::unique_ptr<math::Frustum> frustum; 
	std::unique_ptr<math::float4x4> projection_matrix;
	std::unique_ptr<math::float4x4> view_matrix;
	std::unique_ptr<math::float3> camera_position;

	float aspect_ratio = 0;

	float free_movement_speed = 0;
	float zoom_speed = 0;
	float drag_speed = 0;
	float sensitivity = 0;
};