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
	void Wireframe(bool b);
public:
	std::list<Primitive*> bodies;
};
