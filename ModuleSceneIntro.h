#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"
#include "GameObject.h"
#include "Octree.h"
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
	void Draw();
	void AddObject(GameObject* obj);
	void SetobjSelected(GameObject* obj);
	GameObject* GetSelected()const;
	void DrawHierarchy()const;
	void Clear();
	
public:
//	std::list<Primitive*> bodies;
	GameObject* root;
	GameObject* selected =nullptr;
	Octree* octree = nullptr;
	bool hierarchy = true;


};