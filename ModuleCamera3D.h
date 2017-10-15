#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "ModuleLoader.h"

#define CAMERA_PROPORTION 1.5;
class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();
	void Focus(const vec3 target, const float dist);
	void Orbit(const vec3& orbit_center, const float& motion_x, const float& motion_y);
	void Rotate(const float& motion_x, const float& motion_y);
	void ImGuiDraw();
	//void FocusMesh(Mesh* m);

private:

	void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z, Position, Reference;

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;
	float speed = 3.0f;
	float Sensitivity = 0.25f;
};