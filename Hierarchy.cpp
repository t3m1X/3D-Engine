#include "Hierarchy.h"
#include "imgui.h"
#include "imgui_docking.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

Hierarchy::Hierarchy() : Panel("Hierarchy", SDL_SCANCODE_P, true)
{
}

Hierarchy::~Hierarchy()
{
}

void Hierarchy::Start()
{
}

void Hierarchy::Draw()
{
	igBeginDock("Hierarchy", &Active, ImGuiWindowFlags_MenuBar);

	GameObject* root = App->scene_intro->root;

	root->UIDraw();

	igEndDock();
}
