#pragma once

//#include "Module.h"
#include "Application.h"

#include "ImGui\imgui.h"
#include "ImGui\imconfig.h"
#include "ImGui\imgui_internal.h"

#include "imgui_impl_sdl.h"
#include "SDL/include/SDL_opengl.h"
#include "Geomath.h"

class Module;
class Application;

class ModuleImGui : public Module {
public:
	ModuleImGui(Application* app, bool start_enabled = true);
	~ModuleImGui();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	void LoadStyle(char* name);
	void Addsphere(float p1, float p2, float p3, float radius);////just for testing
	void GeoMathTest();
	void ImGuiInput(SDL_Event* ev);
	void CheckIntersecs(Sphere &sph);
	void CreateRandom();
	float GetRandomValue(float range_1, float range_2);
	

private:
	bool show_test_window = false;
	bool math_test = false;
	///just for testing mathgeolib///
	std::list<bSphere*>spheres;
	std::vector<Sphere*>col_spheres;
	float pos[3] = { 0.0f, 0.0f, 0.0f };
	float radius = 0;
	int contacts = 0;
};
	