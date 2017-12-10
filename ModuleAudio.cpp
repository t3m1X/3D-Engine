#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"

#include "Wwise/SDK/samples/SoundEngine/Win32/AkFilePackageLowLevelIOBlocking.h" 


CAkFilePackageLowLevelIOBlocking g_lowLevelIO;



ModuleAudio::ModuleAudio(bool start_enabled) : Module(start_enabled)
{
	SetName("Audio Engine");
}

ModuleAudio::~ModuleAudio()
{
}

bool ModuleAudio::Init(JSON_File * config)
{

	// Create and initialize an instance of the default memory manager. 
	AkMemSettings memSettings;
	memSettings.uMaxNumPools = 20;

	if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{
		LOG_OUT("Could not create the memory manager.");
		return false;
	}

	// Create and initialize an instance of the default streaming manager. 

	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);

	// Customize the Stream Manager settings here.

	if (!AK::StreamMgr::Create(stmSettings))
	{
		LOG_OUT("Could not create the Streaming Manager");
		return false;
	}

	
	// Create a streaming device with blocking low-level I/O handshaking.
	
	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	// Customize the streaming device settings here.

	
	if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		LOG_OUT("Could not create the streaming device and Low-Level I/O system");
		return false;
	}

	// Create the Sound Engine
	// Using default initialization parameters
	

	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

	if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
	{
		LOG_OUT("Could not initialize the Sound Engine.");
		return false;
	}

	// Initialize the music engine
	// Using default initialization parameters
	

	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);
	

	if (AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		LOG_OUT("Could not initialize the Music Engine.");
		return false;
	}

#ifndef AK_OPTIMIZED
	
	// Initialize communications (not in release build!)
	
	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);
	if (AK::Comm::Init(commSettings) != AK_Success)
	{
		LOG_OUT("Could not initialize communication.");
		return false;
	}
#endif // AK_OPTIMIZED




	return true;
}

bool ModuleAudio::Start()
{
	return true;
}


update_status ModuleAudio::PostUpdate()
{
	AK::SoundEngine::RenderAudio();
	return UPDATE_CONTINUE;
}

bool ModuleAudio::CleanUp()
{
#ifndef AK_OPTIMIZED
	AK::Comm::Term();
#endif // AK_OPTIMIZED

	AK::MusicEngine::Term();
	AK::SoundEngine::Term();

	g_lowLevelIO.Term();

	if (AK::IAkStreamMgr::Get())
		AK::IAkStreamMgr::Get()->Destroy();

	AK::MemoryMgr::Term();

	

	return true;
}

void ModuleAudio::RegisterGO(uint id)
{
	AK::SoundEngine::RegisterGameObj(id);
}

void ModuleAudio::UnRegisterGO(uint id)
{
	AK::SoundEngine::UnregisterGameObj(id);
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
