#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"
#include "Include_Wwise.h"
#include "Geomath.h"
#include "Wwise.h"

class SoundBank;

#define DEFAULT_VOLUME 50



class ModuleAudio : public Module {

public:

	ModuleAudio(bool start_enabled = true);
	virtual ~ModuleAudio();
	bool Init(JSON_File* config);
	bool Start();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	SoundBank* LoadSoundBank(string path);
	unsigned int GetBankInfo(string path, SoundBank* &bank);
	//static void EventCallBack(AkCallbackType type, AkCallbackInfo *info);

	//Game Objects 
	Wwise::SoundObject* CreateSoundObject(const char* name, math::float3 position);
	Wwise::SoundObject* CreateListener(const char* name, math::float3 position);
	void SetRTPvalue(const char* rtpc, float value);
	void ImGuiDraw();
	//Listener

	Wwise::SoundObject* camera_listener = nullptr;
	std::list <Wwise::SoundObject*> sound_obj;
	std::list<SoundBank*> soundbanks;

	unsigned long listener_id = 1;
	unsigned long last_go_id = 100;

	Wwise::SoundObject*  emmiter;
	SoundBank* soundbank = nullptr;
	bool listener_created = false;
	int volume = DEFAULT_VOLUME;
	




};




































/*#include "SDL_mixer\include\SDL_mixer.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

class ModuleAudio : public Module
{
public:

	ModuleAudio(bool start_enabled = true);
	~ModuleAudio();

	bool Init(JSON_File*conf);
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);
	void ImGuiDraw();

private:

	Mix_Music*			music;
	list<Mix_Chunk*>	fx;
};*/

#endif // __ModuleAudio_H__