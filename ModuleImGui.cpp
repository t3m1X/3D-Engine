#include "ModuleImGui.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleLoader.h"
#include "Primitive.h"
#include "Geomath.h"
#include "imgui_impl_sdl.h"
#include "imgui_docking.h"
#include "MathGeoLib\include\Algorithm\Random\LCG.h"
#include <cmath>
#include <random>
#include "ModuleCamera3D.h"
#include "Transform.h"
#include "Geomath.h"
#include "imconfig.h"
#include "Panel.h"
#include "Console.h"
#include "Viewer.h"
#include "ConfigPanel.h"
#include "SDL/include/SDL_opengl.h"
#include "GameObject.h"
#include "About.h"
#include "Properties.h"
#include "Hierarchy.h"
#include "HardwarePanel.h"

ModuleImGui::ModuleImGui(bool start_enabled) : Module(start_enabled)
{
	SetName("ImGui");
	memset(layout_name, 0, sizeof(layout_name));
}

ModuleImGui::~ModuleImGui()
{

}

bool ModuleImGui::Init(JSON_File* conf)
{

	bool ret = true;

	ret = ImGui_ImplSdlGL2_Init(App->window->window);
	LoadStyle("blue_yellow");
	b_y = true;
	b_w = g = b_o = false;
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = "Settings/imgui.ini";
	console = new Console();
	configuration = new ConfigPanel();
	viewer = new Viewer();
	about = new About();
	properties = new Properties();
	hierarchy = new Hierarchy();
	hardware = new HardwarePanel(false);
	panels.push_back(console);
	panels.push_back(configuration);
	panels.push_back(viewer);
	panels.push_back(about);
	panels.push_back(properties);
	panels.push_back(hierarchy);
	panels.push_back(hardware);

	curr_operation = ImGuizmo::TRANSLATE;
	curr_mode = ImGuizmo::LOCAL;

	LoadLayouts();
	return ret;
}

update_status ModuleImGui::PreUpdate(float dt)
{
	ImGui_ImplSdlGL2_NewFrame(App->window->window);
	ImGuizmo::BeginFrame();

	igBeginWorkspace( false, ImVec2(-3, 21), ImVec2(App->window->GetWidth(), App->window->GetHeight() - 21),
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus);

	//Set Keyboard / Mouse bindings here

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleImGui::Update(float dt)
{

	for (list<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it)
		(*it)->Draw();

	igEndWorkspace();

	ImGuiStyle * style = &ImGui::GetStyle();

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		{
			curr_operation = ImGuizmo::TRANSLATE;
		}
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		{
			curr_operation = ImGuizmo::ROTATE;
		}
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		{
			curr_operation = ImGuizmo::SCALE;
		}

	if (App->scene_intro->selected!=nullptr)
		{
		App->scene_intro->selected->OnGuizmo();
		}
	//Menu///////////////////

	if (ImGui::BeginMainMenuBar()) {

		if (ImGui::BeginMenu("File")) {
			
			if (ImGui::MenuItem("Exit", "Alt+F4")) { return update_status::UPDATE_STOP; }

			ImGui::EndMenu();
		}
	
		if (ImGui::BeginMenu("View"))
		{
			
			ImGui::MenuItem("Configuration", "C", &configuration->Active);
			ImGui::MenuItem("Inspector", "P", &properties->Active);
			ImGui::MenuItem("Hierarchy", NULL, &hierarchy->Active);
			ImGui::MenuItem("Hardware", NULL, &hardware->Active);
			ImGui::MenuItem("Console", "0", &console->Active);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Layouts"))
		{
			ImGui::Text("Current: %s", current_layout);

			ImGui::Separator();
			for (list<string>::iterator it = layouts.begin(); it != layouts.end();)
			{
				ImGui::Text((*it).c_str());

				ImGui::SameLine();

				string id = (*it) + "set";
				ImGui::PushID(id.c_str());
				if (ImGui::SmallButton("Set"))
				{
					SaveCurrentLayout();
					SetCurrentLayout((*it).c_str());
				}
				ImGui::PopID();

				ImGui::SameLine();

				id = (*it) + "x";
				ImGui::PushID(id.c_str());
				if (ImGui::SmallButton("x"))
				{
					SetCurrentLayout("default");
					DeleteLayout((*it).c_str());
					it = layouts.erase(it);
					SaveLayouts();
				}
				else
					++it;

				ImGui::PopID();
			}
			ImGui::Separator();

			ImGui::InputText("", layout_name, 25);
			if (ImGui::Button("Save layout"))
			{
				string lname = layout_name;
				if (lname != "")
				{
					NewLayout(layout_name);
					SaveLayouts();
				}
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Documentation")) {
				App->RequestBrowser("https://github.com/rogerbusquets97/3D-Engine/wiki");
			}
			if (ImGui::MenuItem("Report bug")) {
				App->RequestBrowser("https://github.com/rogerbusquets97/3D-Engine/issues");
			}
			if (ImGui::MenuItem("Github repository")) {
				App->RequestBrowser("https://github.com/rogerbusquets97/3D-Engine");
			}
			if (ImGui::MenuItem("Download latest")) {
				App->RequestBrowser("https://github.com/rogerbusquets97/3D-Engine/releases");
			}

			ImGui::MenuItem("About", NULL, &about->Active);
			ImGui::EndMenu();
		}
		

		if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN) {
			configuration->ToggleActive();
		}

		

	ImGui::EndMainMenuBar();
}

	//for (list<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it)
	//{
	//	if (App->input->GetKey(it._Ptr->_Myval->GetShortcut()) == KEY_DOWN) {
	//		if (it._Ptr->_Myval->IsActive()) {
	//			it._Ptr->_Myval->Disable();
	//		}
	//		else {
	//			it._Ptr->_Myval->Enable();
	//		}
	//	}

	//	if (it._Ptr->_Myval->IsActive()) {
	//		ImGui::SetNextWindowPos(ImVec2((float)it._Ptr->_Myval->x, (float)it._Ptr->_Myval->y), ImGuiSetCond_Always);
	//		ImGui::SetNextWindowSize(ImVec2((float)it._Ptr->_Myval->w, (float)it._Ptr->_Myval->h), ImGuiSetCond_Always);
	//		it._Ptr->_Myval->Draw();
	//	}
	//}
	///////////////////////

	

	// Test window imgui
	if (show_test_window)
	{
		ImGui::ShowTestWindow();
	}

	/*if (geometry) {

		
	}*/

	//if (properties) {
	//	curr_obj = App->scene_intro->selected;
	//	ImGui::Begin("Properties");
	//	
	//	if (curr_obj != nullptr) {
	//		curr_obj->DrawComponents();
	//	}
	//	
	//	ImGui::End();
	//}
			
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		properties->ToggleActive();
	}
	

	//Draw();


	return update_status::UPDATE_CONTINUE;
}

update_status ModuleImGui::PostUpdate(float dt)
{
	return update_status::UPDATE_CONTINUE;
}

bool ModuleImGui::CleanUp() {
	SaveLayouts();
	for (list<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it)
	{
		(*it)->CleanUp();
		delete *it;
	}

	console = nullptr;
	configuration = nullptr;
	viewer = nullptr;
	about = nullptr;
	properties = nullptr;
	hierarchy = nullptr;
	hardware = nullptr;

	LOG_OUT("Shutting down ImGui");
	ImGui_ImplSdlGL2_Shutdown();

	return true;
}

void ModuleImGui::LoadStyle(char * name)
{
	ImGuiStyle * style = &ImGui::GetStyle();

	if (name == "default")
	{

	}
	else if (name == "black_white")
	{
		style->WindowPadding = ImVec2(15, 15);
		style->WindowRounding = 5.0f;
		style->FramePadding = ImVec2(5, 5);
		style->FrameRounding = 4.0f;
		style->ItemSpacing = ImVec2(12, 8);
		style->ItemInnerSpacing = ImVec2(8, 6);
		style->IndentSpacing = 25.0f;
		style->ScrollbarSize = 15.0f;
		style->ScrollbarRounding = 9.0f;
		style->GrabMinSize = 5.0f;
		style->GrabRounding = 3.0f;

		style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
		style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
		style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
		style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
		style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
	}
	else if (name == "gray")
	{
		style->ChildWindowRounding = 3.f;
		style->GrabRounding = 0.f;
		style->WindowRounding = 0.f;
		style->ScrollbarRounding = 3.f;
		style->FrameRounding = 3.f;
		style->WindowTitleAlign = ImVec2(0.5f, 0.5f);

		style->Colors[ImGuiCol_Text] = ImVec4(0.73f, 0.73f, 0.73f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.26f, 0.26f, 0.26f, 0.95f);
		style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_ComboBg] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style->Colors[ImGuiCol_CloseButton] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
		style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.32f, 0.52f, 0.65f, 1.00f);
		style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
	}
	else if (name == "black_orange")
	{
		style->WindowPadding = ImVec2(15, 15);
		style->WindowRounding = 5.0f;
		style->FramePadding = ImVec2(5, 5);
		style->FrameRounding = 4.0f;
		style->ItemSpacing = ImVec2(12, 8);
		style->ItemInnerSpacing = ImVec2(8, 6);
		style->IndentSpacing = 25.0f;
		style->ScrollbarSize = 15.0f;
		style->ScrollbarRounding = 9.0f;
		style->GrabMinSize = 5.0f;
		style->GrabRounding = 3.0f;

		style->Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.90f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.80f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.20f, 0.20f, 0.20f, 0.00f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.34f, 0.34f, 0.34f, 0.80f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.47f, 0.47f, 0.47f, 0.40f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.66f, 0.66f, 0.66f, 0.45f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(1.00f, 0.44f, 0.30f, 0.80f);
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.27f, 0.27f, 0.27f, 0.80f);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.88f, 0.22f, 0.06f, 0.80f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.00f, 0.20f, 0.00f, 0.80f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.90f, 0.44f, 0.32f, 0.80f);
		style->Colors[ImGuiCol_ComboBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.99f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.86f, 0.20f, 0.04f, 0.80f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 0.20f, 0.00f, 0.80f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_Separator] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.89f, 0.24f, 0.08f, 0.80f);
		style->Colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 0.20f, 0.00f, 0.80f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
		style->Colors[ImGuiCol_CloseButton] = ImVec4(1.00f, 1.00f, 1.00f, 0.80f);
		style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
		style->Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
	}

	else if ("blue_yellow")
	{
		style->WindowPadding = ImVec2(15, 15);
		style->WindowRounding = 5.0f;
		style->FramePadding = ImVec2(5, 5);
		style->FrameRounding = 4.0f;
		style->ItemSpacing = ImVec2(12, 8);
		style->ItemInnerSpacing = ImVec2(8, 6);
		style->IndentSpacing = 25.0f;
		style->ScrollbarSize = 15.0f;
		style->ScrollbarRounding = 9.0f;
		style->GrabMinSize = 5.0f;
		style->GrabRounding = 3.0f;

		style->Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.16f, 0.17f, 1.00f);
		style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.16f, 0.18f, 0.20f, 1.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.15f, 0.16f, 0.17f, 1.00f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.15f, 0.16f, 0.17f, 1.00f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.15f, 0.16f, 0.17f, 1.00f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.34f, 0.34f, 0.34f, 0.80f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.47f, 0.47f, 0.47f, 0.40f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.66f, 0.66f, 0.66f, 0.45f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.50f, 0.88f, 1.00f);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.14f, 0.50f, 0.88f, 1.00f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.13f, 0.52f, 0.94f, 1.00f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.21f, 0.29f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.21f, 0.29f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.13f, 0.52f, 0.94f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_ComboBg] = ImVec4(0.13f, 0.52f, 0.94f, 1.00f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.81f, 0.49f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.87f, 0.54f, 0.05f, 1.00f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.11f, 0.43f, 0.77f, 1.00f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.88f, 0.53f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_Separator] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.90f, 0.55f, 0.02f, 1.00f);
		style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.89f, 0.54f, 0.01f, 1.00f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
		style->Colors[ImGuiCol_CloseButton] = ImVec4(1.00f, 1.00f, 1.00f, 0.80f);
		style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
		style->Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.14f, 0.50f, 0.88f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.14f, 0.50f, 0.88f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
	}

}





void ModuleImGui::ImGuiInput(SDL_Event* ev) {
	// ImGui Input
	ImGui_ImplSdlGL2_ProcessEvent(ev);
}



float ModuleImGui::GetRandomValue(float range_1, float range_2) {
	{
		random_device rd;
		mt19937 gen(rd());
		uniform_real_distribution<> dis(range_1, range_2);

		return dis(gen);
	}

}

void ModuleImGui::Draw() {
	ImGui::Render();
}

void ModuleImGui::LogFps(float fps, float ms) {
	
		if (configuration != nullptr)
			configuration->FPS(fps, ms);
	
}

void ModuleImGui::AddPanel(Panel * panel)
{
	panels.push_back(panel);
}

void ModuleImGui::ImGuiDraw()
{
	if (ImGui::CollapsingHeader("User Interface")) {
		ImGui::Text("UI Style:");
		if (ImGui::Checkbox("Black and white", &b_w) && b_w) {
			LoadStyle("black_white");
			g = b_o = b_y = false;
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Grey", &g) && g)
		{
			LoadStyle("grey");
			b_w = b_o = b_y = false;
		}

		if (ImGui::Checkbox("Black and orange", &b_o) && b_o)
		{
			LoadStyle("black_orange");
			b_w = g = b_y = false;
		}

		ImGui::SameLine();
		if (ImGui::Checkbox("Blue and yellow", &b_y) && b_y)
		{
			LoadStyle("blue_yellow");
			b_w = g = b_o = false;
		}
	}
}

bool ModuleImGui::HoveringWindow()
{
	return ImGui::IsMouseHoveringAnyWindow();
}

Console * ModuleImGui::GetConsole() const
{
	return console;
}

void ModuleImGui::LoadLayouts()
{
	if (layout_config == nullptr)
		layout_config = App->json->LoadJSON("layouts.json");

	if (layout_config != nullptr)
	{
		int size = layout_config->ArraySize("layouts");
		for (int i = 0; i < size; i++)
		{
			string current = layout_config->ArrayGetString("layouts", i);
			bool found = false;
			for (list<string>::iterator it = layouts.begin(); it != layouts.end() && !found; it++)
				found = *it == current;

			if (!found) layouts.push_back(current);
		}

		SetCurrentLayout(layout_config->GetString("editor.current_layout", ""));
	}
	else
	{
		SetCurrentLayout("default");
	}
}

void ModuleImGui::SaveLayouts()
{
	if (layout_config == nullptr)
		layout_config = App->json->LoadJSON("layouts.json");

	if (layout_config == nullptr)
		layout_config = App->json->CreateJSON("layouts.json");

	if (layout_config != nullptr)
	{
		layout_config->SetString("editor.current_layout", current_layout);

		layout_config->ClearArray("layouts");
		layout_config->AddArray("layouts");
		for (list<string>::iterator it = layouts.begin(); it != layouts.end(); it++)
		{
			layout_config->ArrayAddString("layouts", (*it).c_str());
		}
	}

	SaveCurrentLayout();
}

void ModuleImGui::SetCurrentLayout(const char * name)
{
	string clay = name;
	current_layout = "default";
	for (list<string>::iterator it = layouts.begin(); it != layouts.end(); it++)
	{
		if (*it == clay)
		{
			current_layout = name;
			break;
		}
	}
	ReloadCurrentLayout();
}

void ModuleImGui::ReloadCurrentLayout()
{
	if (layout_config != nullptr)
	{
		layout_config->RootObject();
		getDockContext()->LoadLayout(layout_config, current_layout);
	}
}

void ModuleImGui::SaveCurrentLayout()
{
	if (layout_config != nullptr)
	{
		layout_config->RootObject();

		getDockContext()->SaveLayout(layout_config, current_layout);
		layout_config->Save();
	}
}

void ModuleImGui::NewLayout(const char * name)
{
	string nlay = name;
	bool found = false;
	for (list<string>::iterator it = layouts.begin(); it != layouts.end() && !found; ++it)
		found = nlay == *it;

	if (!found)
	{
		layouts.push_back(name);
		current_layout = name;
		SaveCurrentLayout();
	}
}

void ModuleImGui::DeleteLayout(const char * name)
{
	if (layout_config != nullptr)
	{
		layout_config->RemoveObject(name);
		layout_config->Save();
	}
}
