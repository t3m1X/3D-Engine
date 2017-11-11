#ifndef __MODULEIMGGUI_H__
#define __MODULEIMGGUI_H__

#include "Module.h"
#include "ImGui\ImGuizmo\ImGuizmo.h"
#include "ImGui\imconfig.h"
#include "imgui_impl_sdl.h"
#include "imgui.h"

class Panel;
class Console;
class ConfigPanel;
class About;
class ProfileViewer;
class EngineTest;
class HardwarePanel;
class Hierarchy;
class Properties;
class GameObject;
class Viewer;

class ModuleImGui : public Module {
public:
	ModuleImGui(bool start_enabled = true);
	~ModuleImGui();

	bool Init(JSON_File* conf);
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	void Draw();
	bool CleanUp();

	void LoadStyle(char* name);
	void ImGuiInput(SDL_Event* ev);
	float GetRandomValue(float range_1, float range_2);
	void LogFps(float fps, float ms);
	void AddPanel(Panel* panel);
	void ImGuiDraw();
	GameObject* curr_obj;
	bool HoveringWindow();

	Console* GetConsole() const;

	void LoadLayouts();
	void SaveLayouts();

	void SetCurrentLayout(const char* name);
	void ReloadCurrentLayout();
	void SaveCurrentLayout();

	void NewLayout(const char* name);
	void DeleteLayout(const char* name);


private:
	std::list<std::string> layouts;
	const char* current_layout = nullptr;
	JSON_File* layout_config = nullptr;
	char layout_name[26];

	bool show_test_window = false;
	bool geometry = false;
	std::list<Panel*> panels;
	About* about = nullptr;
	ConfigPanel* configuration = nullptr;
	Console* console = nullptr;
	Viewer* viewer = nullptr;
	Properties* properties = nullptr;
	Hierarchy* hierarchy = nullptr;
	HardwarePanel* hardware = nullptr;
	ImGuizmo::OPERATION curr_operation;
	ImGuizmo::MODE curr_mode;
	bool b_w, g, b_o, b_y;
	
	///just for testing mathgeolib///
};

#endif
	