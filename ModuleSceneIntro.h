#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include "ModulePlayer.h"
#include "GameObject.h"
#include "Octree.h"
//#include "MathGeoLib\include\MathGeoLib.h"
//#include "MathGeoLib\include\Geometry\Sphere.h"




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
	void IntersectAABB(LineSegment &picking, std::vector<GameObject*>& DistanceList);
	GameObject* SelectObject(LineSegment picking);
	
	void RecalculateOctree();
	const char* Serialize(const char* name);
	const char* LoadScene(const char* scene_name);
	
public:
//	std::list<Primitive*> bodies;
	GameObject* root;
	GameObject* selected =nullptr;
	Octree* octree = nullptr;
	std::vector<GameObject*>all_objects;
	std::list<GameObject*>non_static_objects;
	bool hierarchy = true;
	std::string scene_name = "Scene1";
	GameObject* emmiter = nullptr;
	Timer songs;
	float curr_time = 0;
	bool change = false;
};