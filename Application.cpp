#include "Application.h"
#include <iostream> 
#include <sstream> 


Application::Application()
{

	frames = 0;
	last_frame_ms = -1;
	last_fps = -1;
	capped_ms = 1000 / 60;
	fps_counter = 0;
	name = nullptr;
	organization = nullptr;
	version = nullptr;
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	physics = new ModulePhysics3D(this);
	player = new ModulePlayer(this);
	imgui = new ModuleImGui(this);
	json = new ModuleJSON(this);


	con = new Console();
	imgui->AddPanel(con);
	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(json);
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);
	

	// Scenes
	AddModule(scene_intro);
	AddModule(player);

	// Renderer last!
	AddModule(renderer3D);
	AddModule(imgui);
	
}

Application::~Application()
{
	for (std::list<Module*>::reverse_iterator rit = list_modules.rbegin(); rit != list_modules.rend(); ++rit)
	{
		if ((*rit) != nullptr)
			delete *rit;
	}

	list_modules.clear();

}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules

	for (list<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); ++it) {
		if ((*it) != nullptr && ret == true) {
			ret = (*it)->Init();
		}
	}


	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");

	for (list<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); ++it) {
		if ((*it) != nullptr && ret == true) {
			ret = (*it)->Start();
		}
	}
	ms_timer.Start();
	debug = true;
	return ret;
}

bool Application::Awake()
{
	bool ret = true;

	LoadConfig("config.json");

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	++frames;
	++fps_counter;

	if (fps_timer.Read() >= 1000)
	{
		last_fps = fps_counter;
		fps_counter = 0;
		fps_timer.Start();
	}

	last_frame_ms = ms_timer.Read();

	// cap fps
	if (capped_ms > 0 && (last_frame_ms < capped_ms))
		SDL_Delay(capped_ms - last_frame_ms);

	imgui->LogFps((float)last_fps, (float)last_frame_ms);
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();

	for (list<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); ++it) {
		if ((*it) != nullptr && ret == true) {
			ret = (*it)->PreUpdate(dt);
		}
	}

	for (list<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); ++it) {
		if ((*it) != nullptr && ret == true) {
			ret = (*it)->Update(dt);
		}
	}

	for (list<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); ++it) {
		if ((*it) != nullptr && ret == true) {
			ret = (*it)->PostUpdate(dt);
		}
	}
	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	for (list<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); ++it) {
		if ((*it) != nullptr && ret == true) {
			ret = (*it)->CleanUp();
		}
	}
	return ret;
}

bool Application::Debug()
{
	return debug;
}

void Application::SetDebug(bool active)
{
	debug = active;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

uint Application::GetFramerateLimit() const
{
	if (capped_ms > 0)
		return (uint)((1.0f / (float)capped_ms) * 1000.0f);
	else
		return 0;
}

void Application::SetFramerateLimit(uint max_framerate)
{
	if (max_framerate > 0)
		capped_ms = 1000 / max_framerate;
	else
		capped_ms = 0;
}
void Application::RequestBrowser(const char * url) const
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

void Application::SetName(const char * str)
{
	name = str;
}

void Application::SetOrg(const char * org)
{
	organization = org;
}

void Application::SetVersion(const char * ver)
{
	version = ver;
}

const char * Application::GetName()const
{
	return name;
}

const char * Application::GetOrg()const
{
	return organization;
}

const char * Application::GetVersion()const
{
	return version;
}

void Application::LoadConfig(const char * path)
{
	config = json->LoadJSON("config.json");
	if (config != nullptr) {
		const char* n = config->GetString("app.name");
		const char* o = config->GetString("app.organization");
		const char* v = config->GetString("app.version");

		SetName(n);
		SetOrg(o);
		SetVersion(v);

	}
	else {
		con->AddLog("Json did not load correctly");
	}

	for (list<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); ++it) {
		(*it)->Load(config);
	}
}

void Application::SaveConfig(Module*module)
{
	if (config != nullptr)
	{
		config->SetString("app.title", GetName());
		config->SetString("app.organization", GetOrg());
		config->SetString("app.version", GetVersion());

		for (list<Module*>::reverse_iterator it = list_modules.rbegin(); it != list_modules.rend(); it++)
		{
			if (module == nullptr)
				(*it)->Save(config);

			else if (module == (*it))
				(*it)->Save(config);
		}


		config->Save();
	}
}
