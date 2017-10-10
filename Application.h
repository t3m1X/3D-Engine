#pragma once


#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModulePlayer.h"
#include "ModuleImGui.h"
#include "ModuleJson.h"
#include "ModuleLoader.h"
#include "Console.h"
#include "ModuleLoader.h"

class ModuleImGui;
class JSON_File;
class ModuleJSON;
class ModuleWindow;
class Console;

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	ModulePlayer* player;
	ModuleImGui* imgui;
	ModuleJSON* json;
	ModuleLoader* loader;
	list<Module*> list_modules;
	Console* con;
private:

	Timer	ms_timer;
	float	dt;
	Timer	fps_timer;
	Uint32	frames;
	int		fps_counter;
	int		last_frame_ms;
	int		last_fps;
	int		capped_ms;
	const char* name = nullptr;
	const char* organization = nullptr;
	const char* version = nullptr;
	JSON_File * config = nullptr;

public:

	Application();
	~Application();

	bool Init();
	bool Awake();
	update_status Update();
	bool CleanUp();
	bool Debug();
	void SetDebug(bool active);
	bool debug;
	uint GetFramerateLimit() const;
	void SetFramerateLimit(uint max_framerate);
	void RequestBrowser(const char* url) const;
	void SetName(const char* str);
	void SetOrg(const char* org);
	void SetVersion(const char* ver);
	const char* GetName()const;
	const char* GetOrg()const;
	const char* GetVersion()const;
	void LoadConfig(const char* path);
	void SaveConfig(Module*module);

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

};
