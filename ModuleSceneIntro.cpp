#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"
#include "ModuleImGui.h"
#include "Transform.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include <vector>

#define MIN_DISTANCE 9999

ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
	root = new GameObject("root", nullptr);
	root->SetName("Game");

	

	selected = nullptr;
}

ModuleSceneIntro::~ModuleSceneIntro()
{
	
}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG_OUT("Loading Intro assets");
	bool ret = true;

	GameObject* cam_obj = new GameObject("Camera", root);

	Quat rot = Quat::identity;
	float3 scale;
	float3 pos;
	scale.Set(1, 1, 1);
	pos.Set(0, 0, 0);
	Transform* trans = new Transform(cam_obj);
	trans->SetRotation(rot);
	trans->SetPosition(pos);
	trans->SetScale(scale);
	cam_obj->AddComponent(trans);
	ComponentCamera* cam = new ComponentCamera(cam_obj);
	cam_obj->AddComponent(cam);


	float3 max_point;
	float3 min_point;

	max_point.Set(10, 10, 10);
	min_point.Set(-10, 0, -10);

	octree = new Octree();

	octree->Create(max_point, min_point);


	
	App->camera->SetCurrentCamera(cam->GetCamera());
	selected = cam_obj;
	
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG_OUT("Unloading Intro scene");

	delete root;

	return true;
}


void ModuleSceneIntro::AddObject(GameObject * obj)
{
	root->AddChild(obj);


	if (obj->GetStatic()) {
		octree->InsertGO(obj);
	//	static_objects.push_back(obj);
		LOG_OUT("Insterted in static objects");
	}
	else {
		//non_static_objects.push_back(obj);
		LOG_OUT("Insterted in non static objects");
	}

	
}


void ModuleSceneIntro::Draw()
{


	if (App->camera->GetCurrentCamera() != App->camera->GetEditorCamera()) {
		std::list<GameObject*> intersections_list;
		octree->CollectIntersections(intersections_list, App->camera->GetCurrentCamera());
		for (std::list<GameObject*>::iterator it = intersections_list.begin(); it != intersections_list.end(); it++) {
			(*it)->Draw();
			//LOG_OUT("Drawing static objects");
		}
	}

	for (std::list<GameObject*>::iterator it = non_static_objects.begin(); it != non_static_objects.end(); it++) {
		(*it)->Draw();
		//LOG_OUT("Drawing non static objects");
	}
//	root->Draw();

}



void ModuleSceneIntro::SetobjSelected(GameObject * obj)
{
	selected = obj;
}

GameObject * ModuleSceneIntro::GetSelected() const
{
	if (selected != nullptr) {
		return selected;
	}
	else {
		return nullptr;
	}
}

void ModuleSceneIntro::DrawHierarchy() const
{

	ImGui::Begin("Hierarchy");
	root->UIDraw();
	

	ImGui::End();
}

void ModuleSceneIntro::Clear()
{
	
}

void ModuleSceneIntro::IntersectAABB(LineSegment & picking, std::vector<GameObject*>& DistanceList)
{

	std::list<GameObject*> distance_list;
	octree->CollectIntersections(distance_list,picking);

	for (std::list<GameObject*>::iterator it = distance_list.begin(); it != distance_list.end(); it++) {
		if (picking.Intersects((*it)->boundingbox)) {
			DistanceList.push_back((*it));
			LOG_OUT("AABB hit");
		}
	}

	for (std::list<GameObject*>::iterator it = non_static_objects.begin(); it != non_static_objects.end(); it++) {
		if (picking.Intersects((*it)->boundingbox)) {
			DistanceList.push_back((*it));
			LOG_OUT("AABB hit");
		}
	}

	/*for (uint i = 0; i < root->children.size(); i++) {
		if (root->children[i]->children.size() > 0) {
			for (uint j = 0; j < root->children[i]->children.size(); j++) {
				if (picking.Intersects(root->children[i]->children[j]->boundingbox)) {
					DistanceList.push_back(root->children[i]->children[j]);
					LOG_OUT("AABB hit");
				}
			}
		}
		else {
			if (picking.Intersects(root->children[i]->boundingbox)) {
				DistanceList.push_back(root->children[i]);
				LOG_OUT("AABB hit");
			}
		}
	}*/
}

GameObject * ModuleSceneIntro::SelectObject(LineSegment picking)
{
	GameObject* closest = nullptr;
	std::vector<GameObject*>DistanceList;


	IntersectAABB(picking, DistanceList);

	if (DistanceList.size() > 0) {
		float last_distance = MIN_DISTANCE;

		for (uint i = 0; i < DistanceList.size(); i++) {

			Mesh* m = (Mesh*)DistanceList[i]->FindComponentbyType(MESH);
			float dist = inf;
			float3 hitpoint;
			if (m != nullptr) {
				bool hit = m->TriCheck(picking, dist, hitpoint);
				if (hit) {
					LOG_OUT("HIT!!");
					if (dist < last_distance) {
						last_distance = dist;
						closest = DistanceList[i];
					}
				}
			}
		}

	}

	return closest;
}

void ModuleSceneIntro::FillOctree()
{
	if (!static_objects.empty()) {
		for (std::list<GameObject*>::iterator it = static_objects.begin(); it != static_objects.end(); it++) {
			if ((*it)->GetStatic()) {
				octree->InsertGO((*it));
			}
		}
	}
}

void ModuleSceneIntro::RecalculateOctree()
{
	float3 new_min_point = octree->min_point;
	float3 new_max_point = octree->max_point;

	for (std::list<GameObject*>::iterator it = static_objects.begin(); it != static_objects.end(); it++) {

		if ((*it)->boundingbox.minPoint.x < new_min_point.x)
		{
			new_min_point.x = (*it)->boundingbox.minPoint.x;
		}
		if ((*it)->boundingbox.minPoint.y < new_min_point.y)
		{
			new_min_point.y = (*it)->boundingbox.minPoint.y;
		}
		if ((*it)->boundingbox.minPoint.z < new_min_point.z)
		{
			new_min_point.z = (*it)->boundingbox.minPoint.z;
		}
		if ((*it)->boundingbox.maxPoint.x > new_max_point.x)
		{
			new_max_point.x = (*it)->boundingbox.maxPoint.x;
		}
		if ((*it)->boundingbox.maxPoint.y > new_max_point.y)
		{
			new_max_point.y = (*it)->boundingbox.maxPoint.y;
		}
		if ((*it)->boundingbox.maxPoint.z > new_max_point.z)
		{
			new_max_point.z = (*it)->boundingbox.maxPoint.z;
		}

	}

	delete octree;
	octree = new Octree();
	octree->Create(new_max_point, new_min_point);
	octree->need_update = false;
}


// Update
update_status ModuleSceneIntro::Update(float dt)
{
	bPlane p(0, 1, 0, 0);
	p.axis = true;
	
	if (octree->need_update) {
		
		RecalculateOctree();
	}
	DrawHierarchy();
	root->Update();
	//root->Draw();
	octree->DebugDraw();
	p.Render();

	
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
		octree->Divide();
	}
	
	return UPDATE_CONTINUE;
}