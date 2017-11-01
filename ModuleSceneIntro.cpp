#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"
#include "ModuleImGui.h"


ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
	root = new GameObject("root", 0);
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
	p.Render();
	
	return UPDATE_CONTINUE;
}