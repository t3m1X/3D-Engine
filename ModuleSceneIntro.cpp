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
	LOG("Loading Intro assets");
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
	LOG("Unloading Intro scene");

	delete root;

	return true;
}


void ModuleSceneIntro::AddObject(GameObject * obj)
{
	root->AddChild(obj);
}


void ModuleSceneIntro::Draw()
{

	root->Draw();

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

	for (uint i = 0; i < root->children.size(); i++) {
		if (root->children[i]->children.size() > 0) {
			for (uint j = 0; j < root->children[i]->children.size(); j++) {
				if (picking.Intersects(root->children[i]->children[j]->boundingbox)) {
					DistanceList.push_back(root->children[i]->children[j]);
					LOG("AABB hit");
				}
			}
		}
		else {
			if (picking.Intersects(root->children[i]->boundingbox)) {
				DistanceList.push_back(root->children[i]);
				LOG("AABB hit");
			}
		}
	}
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
					LOG("HIT!!");
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


// Update
update_status ModuleSceneIntro::Update(float dt)
{
	bPlane p(0, 1, 0, 0);
	p.axis = true;
	
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