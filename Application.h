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

class ModuleImGui;

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

private:

	Timer	ms_timer;
	float	dt;
	Timer	fps_timer;
	Uint32	frames;
	int		fps_counter;
	int		last_frame_ms;
	int		last_fps;
	int		capped_ms;
	list<Module*> list_modules;
	

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	bool Debug();
	void SetDebug(bool active);
	bool debug;
	uint GetFramerateLimit() const;
	void SetFramerateLimit(uint max_framerate);
private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};
extern Application* App;