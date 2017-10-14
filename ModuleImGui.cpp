#include "ModuleImGui.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "Geomath.h"
#include "imgui_impl_sdl.h"
#include "MathGeoLib\include\Algorithm\Random\LCG.h"
#include <cmath>
#include <random>

ModuleImGui::ModuleImGui(bool start_enabled) : Module(start_enabled)
{
	
}

ModuleImGui::~ModuleImGui()
{

}

bool ModuleImGui::Init(JSON_File* conf)
{
	bool ret = true;

	ImGui_ImplSdl_Init(App->window->window);
	LoadStyle("blue_yellow");
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = "Settings/imgui.ini";
	//console = new Console();
	configuration = new ConfigPanel(App);
	//panels.push_back(console);
	panels.push_back(configuration);
	x = 0;
	y = 0;
	z = 0;
	r = 0;
	posx = 0;
	posy = 0;
	posz = 0;
	h = 0;
	return ret;
}

update_status ModuleImGui::PreUpdate(float dt)
{
	ImGui_ImplSdl_NewFrame(App->window->window);


	ImGuiIO& io = ImGui::GetIO();

	//Set Keyboard / Mouse bindings here

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleImGui::Update(float dt)
{
	

	ImGuiStyle * style = &ImGui::GetStyle();

	//Menu///////////////////

	if (ImGui::BeginMainMenuBar()) {

		if (ImGui::BeginMenu("File")) {
			
			if (ImGui::MenuItem("Exit", "Alt+F4")) { return update_status::UPDATE_STOP; }

			ImGui::EndMenu();
		}
	
		if (ImGui::BeginMenu("View"))
		{
			
			if (ImGui::MenuItem("Configuration", "C"))
			{
				if (!configuration->IsActive()) {
					configuration->Enable();
				}
				else {
					configuration->Disable();
				}
			}

			ImGui::MenuItem("Geometry", NULL, &geometry);
			

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
			if (ImGui::MenuItem("About")) {
				about = true;
			}


			ImGui::EndMenu();
		}

		

		if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN) {
			if (!configuration->IsActive()) {
				configuration->Enable();
			}
			else {
				configuration->Disable();
			}
		}

		

	ImGui::EndMainMenuBar();
}

	for (list<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it)
	{
		if (App->input->GetKey(it._Ptr->_Myval->GetShortcut()) == KEY_DOWN) {
			if (it._Ptr->_Myval->IsActive()) {
				it._Ptr->_Myval->Disable();
			}
			else {
				it._Ptr->_Myval->Enable();
			}
		}

		if (it._Ptr->_Myval->IsActive()) {
			ImGui::SetNextWindowPos(ImVec2((float)it._Ptr->_Myval->x, (float)it._Ptr->_Myval->y), ImGuiSetCond_Always);
			ImGui::SetNextWindowSize(ImVec2((float)it._Ptr->_Myval->w, (float)it._Ptr->_Myval->h), ImGuiSetCond_Always);
			it._Ptr->_Myval->Draw(App);
		}
	}
	///////////////////////

	

	// Test window imgui
	if (show_test_window)
	{
		ImGui::ShowTestWindow();
	}

	if (geometry) {

		ImGui::Begin("Geometry", &geometry);
		if (ImGui::CollapsingHeader("Spheres")) {
			ImGui::InputFloat("x", &posx);
			ImGui::InputFloat("y", &posy);
			ImGui::InputFloat("z", &posz);
			ImGui::InputFloat("Radius", &r);
			if (ImGui::Button("Create Sphere")) {
				App->scene_intro->AddSphere(posx, posy, posz, r);
			}
		}
		if (ImGui::CollapsingHeader("Cubes")) {
			ImGui::InputFloat("Size x", &x);
			ImGui::InputFloat("Size y", &y);
			ImGui::InputFloat("Size z", &z);

			ImGui::InputFloat("Pos x", &posx);
			ImGui::InputFloat("Pos y", &posy);
			ImGui::InputFloat("Pos z", &posz);

			if (ImGui::Button("Create Cube")) {
				App->scene_intro->AddCube(x, y, z, posx, posy, posz);
			}

		}
		if (ImGui::CollapsingHeader("Cylinder")) {
			ImGui::InputFloat("Height", &h);
			ImGui::InputFloat("Radius", &r);
			ImGui::InputFloat("Pos x", &posx);
			ImGui::InputFloat("Pos y", &posy);
			ImGui::InputFloat("Pos z", &posz);

			if (ImGui::Button("Create Cylinder")) {
				App->scene_intro->AddCylinder(r, h, posx, posy, posz);
			}

		}
		if (ImGui::CollapsingHeader("Plane")) {
			ImGui::InputFloat("Size x", &x);
			ImGui::InputFloat("Size y", &y);
			ImGui::InputFloat("Size z", &z);
			ImGui::InputFloat("D", &d);

			ImGui::InputFloat("Pos x", &posx);
			ImGui::InputFloat("Pos y", &posy);
			ImGui::InputFloat("Pos z", &posz);

			if (ImGui::Button("Create Plane")) {
				App->scene_intro->AddPlane(x, y, z, d, posx, posy, posz);
			}

		}
		ImGui::End();
	}
		if (properties) {
			curr_obj = App->scene_intro->objects.front();
			ImGui::Begin("Properties", &properties);
			if (ImGui::CollapsingHeader("Transform")) {
				float3 pos(curr_obj->GetPosition().x, curr_obj->GetPosition().y, curr_obj->GetPosition().z);
				float3 rot(curr_obj->GetRotation().x, curr_obj->GetRotation().y, curr_obj->GetRotation().z);
				float3 scale(curr_obj->GetScale().x, curr_obj->GetScale().y, curr_obj->GetScale().z);

				ImGui::Text("Position");
				ImGui::InputFloat("x", &pos.x);
				ImGui::InputFloat("y", &pos.y);
				ImGui::InputFloat("z", &pos.z);

				ImGui::Text("Rotation:");
				ImGui::InputFloat("x", &rot.x);
				ImGui::InputFloat("y", &rot.y);
				ImGui::InputFloat("z", &rot.z);

				ImGui::Text("Scale");
				ImGui::InputFloat("x", &scale.x);
				ImGui::InputFloat("y", &scale.y);
				ImGui::InputFloat("z", &scale.z);

			}
			if (ImGui::CollapsingHeader("Geometry")) {
				if (!App->loader->meshes.empty()) {
					ImGui::Text("Tris: %d", curr_obj->mesh->num_vertices / 3);
				}
				else {
					ImGui::Text("No geometry loaded yet");
				}
			}
			if (ImGui::CollapsingHeader("Texture")) {
				if (!App->tex->Empty()) {
					ImGui::Text("Width: %d", curr_obj->texture->Getwidth());
					ImGui::Text("Height: %d", curr_obj->texture->Getheight());
					ImTextureID img = (void*)App->tex->GetTexture()->Getid();
					
					ImGui::Image(img, ImVec2(100, 100));
				}
				else {
					ImGui::Text("No texture loaded.");
				}
					
			}
			ImGui::End();
		}

		if (about) {
			if (ImGui::Begin("About",&about))
			{
				ImGui::Text("Roger's Engine %s", App->GetVersion());
				ImGui::Text("About Roger's Engine:");
				ImGui::Text("3D engine developed as a project for UPC's video games degree by Roger Busquets Duran and Sergi Parra Ramirez.");
				ImGui::Separator();
				if (ImGui::CollapsingHeader("License")) {
					ImGui::Text("MIT License");
					ImGui::Text("Copyright(c) 2017 Roger Busquets Duran and Sergi Parra Ramirez");
					ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy");
					ImGui::Text("of this software and associated documentation files(the 'Software'), to deal");
					ImGui::Text("in the Software without restriction, including without limitation the rights");
					ImGui::Text("to use, copy, modify, merge, publish, distribute, sublicense, and / or sell");
					ImGui::Text("copies of the Software, and to permit persons to whom the Software is");
					ImGui::Text("furnished to do so, subject to the following conditions :");

					ImGui::TextColored({ 1, 0.2f, 0.2f, 1 }, "The above copyright notice and this permission notice shall be included in all");
					ImGui::TextColored({ 1, 0.2f, 0.2f, 1 }, "copies or substantial portions of the Software.");

					ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
					ImGui::Text("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,");
					ImGui::Text("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE");
					ImGui::Text("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER");
					ImGui::Text("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,");
					ImGui::Text("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE");
					ImGui::Text("SOFTWARE.");
				}
				if (ImGui::CollapsingHeader("Libraries")) {
					ImGui::Text("SDL : v2.0.4");
					ImGui::Text("MathGeoLib : v1.5");
					ImGui::Text("ImGui : v1.52");
					ImGui::Text("Parson : 2017 version");
					ImGui::Text("OpenGL : v2.1");
					ImGui::Text("Glew: v2.0");
					ImGui::Text("Devil : v1.7.8");
					ImGui::Text("assimp");
					ImGui::Text("mmgr");

				}


				
			}
			ImGui::End();
		}
			
		if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
			if (!App->loader->meshes.empty()) {
				if (properties) {
					properties = false;
				}
				else {
					properties = true;
				}
			}
		}
	

	//Draw();


	return update_status::UPDATE_CONTINUE;
}

update_status ModuleImGui::PostUpdate(float dt)
{
	return update_status::UPDATE_CONTINUE;
}

bool ModuleImGui::CleanUp()
{
	for (list<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it)
	{
		delete *it;
	}

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
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.70f);
		style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.22f, 0.22f, 0.22f, 0.00f);
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
	ImGui_ImplSdl_ProcessEvent(ev);
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

void ModuleImGui::Setproperties(bool set)
{
	properties = set;
}
