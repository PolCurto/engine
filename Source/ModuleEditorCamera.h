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

	void SetFOV();
	void SetAspectRatio();
	void SetPlaneDistances();


private:
	void SetFrustum();
	void ProcessInput();

	Frustum frustum;
	float4x4 projection_matrix = float4x4::zero;
	float4x4 view_matrix = float4x4::zero;
	float3 camera_position = float3::zero;;
};