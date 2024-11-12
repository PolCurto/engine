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

	float4x4 GetProjectionMatrix() { return projection_matrix; }
	float4x4 GetViewMatrix() { return view_matrix; }
	float4x4 GetModelMatrix() { return model_matrix; }

private:
	void SetFrustum();
	void ProcessInput();

	float4x4 projection_matrix = float4x4::zero;
	float4x4 view_matrix = float4x4::zero;
	float4x4 model_matrix = float4x4::zero;
	float3 camera_position = float3::zero;;
};