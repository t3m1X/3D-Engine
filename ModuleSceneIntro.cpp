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


// Update
update_status ModuleSceneIntro::Update(float dt)
{
	bPlane p(0, 1, 0, 0);
	p.axis = true;
	
	DrawHierarchy();
	root->Update();
	root->Draw();
	octree->DebugDraw();
	p.Render();


	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
		octree->Divide();
	}
	
	return UPDATE_CONTINUE;
}