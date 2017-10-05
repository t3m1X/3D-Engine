#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"
//#include "MathGeoLib\include\MathGeoLib.h"
//#include "MathGeoLib\include\Geometry\Sphere.h"


struct PhysBody3D;
struct PhysMotor3D;
struct PhysVehicle3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	//void ModuleSceneIntro::OnCollision(PhysBody3D * body1, PhysBody3D * body2);
	void AddBody(Primitive*body);
	void AddSphere(float x, float y, float z, float radius);
	void AddCube(float x, float y, float z, float posx, float posy, float posz);
	void AddCylinder(float radius, float height, float posx, float posy, float posz);
	void AddPlane(float x, float y, float z, float d, float posx, float posy, float posz);
	void Wireframe(bool b);
	void Draw();
public:
	std::list<Primitive*> bodies;
	float vertices[36 * 3] = {
		0.f + 4, 0.f, 0.f,
		1.f + 4, 0.f, 0.f,
		0.f + 4, 1.f, 0.f,
		1.f + 4, 1.f, 0.f,
		0.f + 4, 1.f, 0.f,
		1.f + 4, 0.f, 0.f,

		0.f + 4, 0.f, -1.f,
		0.f + 4, 0.f, 0.f,
		0.f + 4, 1.f, -1.f,
		0.f + 4, 1.f, 0.f,
		0.f + 4, 1.f, -1.f,
		0.f + 4, 0.f, 0.f,

		1.f + 4, 0.f, 0.f,
		1.f + 4, 0.f, -1.f,
		1.f + 4, 1.f, -1.f,
		1.f + 4, 1.f, 0.f,
		1.f + 4, 0.f, 0.f,
		1.f + 4, 1.f, -1.f,

		0.f + 4, 0.f, -1.f,
		0.f + 4, 1.f, -1.f,
		1.f + 4, 0.f, -1.f,
		1.f + 4, 1.f, -1.f,
		1.f + 4, 0.f, -1.f,
		0.f + 4, 1.f, -1.f,

		0.f + 4, 1.f, 0.f,
		1.f + 4, 1.f, 0.f,
		0.f + 4, 1.f, -1.f,
		1.f + 4, 1.f, -1.f,
		0.f + 4, 1.f, -1.f,
		1.f + 4, 1.f, 0.f ,
		0.f + 4, 0.f, 0.f,
		0.f + 4, 0.f, -1.f,
		1.f + 4, 0.f, 0.f,
		1.f + 4, 0.f, -1.f,
		1.f + 4, 0.f, 0.f,
		0.f + 4, 0.f, -1.f
	};
};