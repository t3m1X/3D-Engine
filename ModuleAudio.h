#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"
#include "Include_Wwise.h"

#define MAX_LISTENERS 8


class ModuleAudio : public Module {

public:

	ModuleAudio(bool start_enabled = true);
	virtual ~ModuleAudio();
	bool Init(JSON_File* config);
	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	//Game Objects 
	void RegisterGO(uint id);
	void UnRegisterGO(uint id);

	//Listener
	
	uint AddListener();


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