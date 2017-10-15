#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"
#include "GameObject.h"
//#include "MathGeoLib\include\MathGeoLib.h"
//#include "MathGeoLib\include\Geometry\Sphere.h"


struct PhysBody3D;
struct PhysMotor3D;
struct PhysVehicle3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	//void ModuleSceneIntro::OnCollision(PhysBody3D * body1, PhysBody3D * body2);
	void AddBody(Primitive*body);
	void AddObject(GameObject* obj);
	void AddSphere(float x, float y, float z, float radius);
	void AddCube(float x, float y, float z, float posx, float posy, float posz);
	void AddCylinder(float radius, float height, float posx, float posy, float posz);
	void AddPlane(float x, float y, float z, float d, float posx, float posy, float posz);
	void Wireframe(bool b);
	void Draw();
	bool Empty();
	void Clear();
	
public:
	std::list<Primitive*> bodies;
	std::list<GameObject*>objects;
};