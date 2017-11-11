#include "Properties.h"
#include "imgui.h"
#include "imgui_docking.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

Properties::Properties(): Panel("Properties", SDL_SCANCODE_P, true)
{
}

Properties::~Properties()
{
}

void Properties::Start()
{
}

void Properties::Draw()
{
	igBeginDock("Properties", &Active, ImGuiWindowFlags_MenuBar);

	GameObject* selected = App->scene_intro->selected;
	if (selected == nullptr)
		return;

	//TO BE ADDED
	//ImGui::BeginMenuBar();
	//if (ImGui::BeginMenu("Add Component"))
	//{
	//	if (ImGui::MenuItem("Add Material"))
	//		selected->AddComponent();
	//	if (ImGui::MenuItem("Add Mesh"))
	//		selected->AddComponent();
	//	if (ImGui::MenuItem("Add Camera"))
	//		selected->AddComponent();

	//	ImGui::EndMenu();
	//}
	//ImGui::EndMenuBar();

	// Text rename
	char name[25];
	sprintf_s(name, 25, selected->GetName());
	if (ImGui::InputText("Name", name, 25, ImGuiInputTextFlags_AutoSelectAll))
		selected->SetName(name);

	//ImGui::SameLine();

	//TO BE ADDED
	//bool stat = selected->GetStatic();
	//if (ImGui::Checkbox("Static", &stat))
	//	selected->SetStatic(stat);

	//ImGui::TextWrapped("Unique id: %f", selected->GetId());


	selected->DrawComponents();

	igEndDock();
}
