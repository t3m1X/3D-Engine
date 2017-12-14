#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"

#include "Wwise/IO/Win32/AkFilePackageLowLevelIOBlocking.h"
#include "GameObject.h"
#include "ModuleInput.h"
#include ".\mmgr\mmgr.h"
#include "Wwise.h"
#include <corecrt_wstring.h>
#include "ModuleCamera3D.h"
#include "Wwise.h"
#include "ModuleJson.h"
#include "SoundBank.h"

//CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

#define BANK_BASE_PATH "SoundBanks/"


ModuleAudio::ModuleAudio(bool start_enabled) : Module(start_enabled)
{
	SetName("Audio Engine");
}

ModuleAudio::~ModuleAudio()
{
}

bool ModuleAudio::Init(JSON_File * config)
{

	LOG_OUT("Loading Wwished library");

	//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	//std::wstring base_path = converter.from_bytes("SoundBanks");

	bool ret = Wwise::InitWwise("English(US)");

	//Wwise::LoadBank("SoundBanks/Test.bnk");

	LoadSoundBank("Blend.bnk");

	
	return true;
}

bool ModuleAudio::Start()
{

	/*float3 cam_up = App->camera->GetCurrentCamera()->GetUp();
	float3 cam_front = App->camera->GetCurrentCamera()->GetFront();
	float3 cam_pos = App->camera->GetCurrentCamera()->GetPos();

	camera_listener = Wwise::CreateSoundObj(0, "Camera_Listener", cam_pos.x, cam_pos.y, cam_pos.z, true);
	camera_listener->SetPosition(cam_pos.x, cam_pos.y, cam_pos.z, cam_front.x, cam_front.y, cam_front.z, cam_up.x, cam_up.y, cam_up.z);

	emmiter = Wwise::CreateSoundObj(100, "Emmiter", 1, 1, 1, false);*/


	return true;
}

update_status ModuleAudio::PreUpdate(float dt)
{
	

	return UPDATE_CONTINUE;
		
}


update_status ModuleAudio::PostUpdate(float dt)
{

	AK::SoundEngine::RenderAudio();
	
	return UPDATE_CONTINUE;
}

bool ModuleAudio::CleanUp()
{
	LOG_OUT("Unloading Wwished library");
	delete camera_listener;
	return Wwise::CloseWwise();

	return true;
}

SoundBank* ModuleAudio::LoadSoundBank(string path)
{
	SoundBank* new_bank = new SoundBank();
	string bank_path = BANK_BASE_PATH + path;
	Wwise::LoadBank(bank_path.c_str());

	std::string json_file = bank_path.substr(0, bank_path.find_last_of('.')) + ".json"; // Changing .bnk with .json
	GetBankInfo(json_file,new_bank);
	soundbanks.push_back(new_bank);
	soundbank = new_bank;
	return new_bank;
}

unsigned int ModuleAudio::GetBankInfo(string path, SoundBank* &bank)
{

	unsigned int ret = 0;
	JSON_File * bank_file = App->json->LoadJSON(path.c_str());
	bank_file->RootObject();

	if (bank_file == nullptr) {
		LOG_OUT("Error reading bank json file");
	}
	else {
		bank_file->ChangeObject("SoundBanksInfo");
		int n_banks = bank_file->ArraySize("SoundBanks");
		for (int i = 0; i < n_banks; i++) {
			//bank_file->RootObject();
			bank_file->MoveToInsideArray("SoundBanks", i);
			ret = bank->id = bank_file->GetNumber("Id");
			bank->name = bank_file->GetString("ShortName");
			bank->path = bank_file->GetString("Path");

			//bank_file->RootObject();
			int n_events = bank_file->ArraySize("IncludedEvents");
			for (int i = 0; i < n_events; i++) {
				//bank_file->RootObject();
				bank_file->MoveToInsideArray("IncludedEvents", i);
				//create new event and load it
				AudioEvent* new_event = new AudioEvent();
				new_event->Load(bank_file, bank);
				bank->events.push_back(new_event);
			}
		}

	}
	return ret;
}


Wwise::SoundObject * ModuleAudio::CreateSoundObject(const char * name, float3 position)
{
	Wwise::SoundObject* ret = Wwise::CreateSoundObj(last_go_id++, name, position.x, position.y, position.z);
	sound_obj.push_back(ret);

	return ret;
}

Wwise::SoundObject * ModuleAudio::CreateListener(const char * name, math::float3 position)
{
	Wwise::SoundObject* ret;

	if (!listener_created) {

		float3 cam_up = App->camera->GetCurrentCamera()->GetUp();
		float3 cam_front = App->camera->GetCurrentCamera()->GetFront();
		float3 cam_pos = App->camera->GetCurrentCamera()->GetPos();

		ret = Wwise::CreateSoundObj(0, "Listener", cam_pos.x, cam_pos.y, cam_pos.z, true);
		ret->SetPosition(cam_pos.x, cam_pos.y, cam_pos.z, cam_front.x, cam_front.y, cam_front.z, cam_up.x, cam_up.y, cam_up.z);
		
		sound_obj.push_back(ret);
		listener_created = true;
	}
	else {
		LOG_OUT("It exist a listener already!");
		ret = nullptr;
	}

	return ret;
}

void ModuleAudio::SetRTPvalue(const char * rtpc, float value)
{
	AK::SoundEngine::SetRTPCValue(rtpc, value);
}












































/*
#include <algorithm>
#include "imgui.h"

#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

ModuleAudio::ModuleAudio(bool start_enabled) : Module(start_enabled), music(NULL)
{
	SetName("Audio");
}

// Destructor
ModuleAudio::~ModuleAudio()
{}

// Called before render is available
bool ModuleAudio::Init(JSON_File* conf)
{
	LOG_OUT("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG_OUT("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	// load support for the OGG format
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOG_OUT("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		ret = false;
	}

	//Initialize SDL_mixer
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG_OUT("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		ret = false;
	}

	return ret;
}

// Called before quitting
bool ModuleAudio::CleanUp()
{
	LOG_OUT("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	list<Mix_Chunk*>* item;

	for (list<Mix_Chunk*>::iterator it = fx.begin(); it != fx.end(); ++it) {
		Mix_FreeChunk((*it));
	}

	fx.clear();
	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
	return true;
}

// Play a music file
bool ModuleAudio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;
	
	if(music != NULL)
	{
		if(fade_time > 0.0f)
		{
			Mix_FadeOutMusic((int) (fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path);

	if(music == NULL)
	{
		LOG_OUT("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if(fade_time > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fade_time * 1000.0f)) < 0)
			{
				LOG_OUT("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				LOG_OUT("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOG_OUT("Successfully playing %s", path);
	return ret;
}

// Load WAV
unsigned int ModuleAudio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if(chunk == NULL)
	{
		LOG_OUT("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.push_back(chunk);
		ret = true;
	}

	return ret;
}

// Play WAV
bool ModuleAudio::PlayFx(unsigned int id, int repeat)
{
	bool ret = false;
	Mix_Chunk* chunk;

	{
		int i = 0;
		for (list<Mix_Chunk*>::iterator it = fx.begin(); it != fx.end(); ++it) {
			if (i == id - 1) {
				chunk = (*it);
			}
		}
	}
		Mix_PlayChannel(-1, chunk, repeat);
		ret = true;

	return ret;
}

void ModuleAudio::ImGuiDraw() {

	if (ImGui::CollapsingHeader(this->GetName())) {
	
	}
}*/
