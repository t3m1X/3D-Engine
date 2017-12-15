#pragma once
#ifndef _AUDIOSOURCE_H_
#define _AUDIOSOURCE_H_

#include "Component.h"
#include "ModuleAudio.h"
#include "AudioEvent.h"
#include "SoundBank.h"
#include "Wwise.h"


class AudioSource : public Component {

public :

	AudioSource(GameObject* own);
	~AudioSource();

	void Update();
	void PlayMusic(double audio_id);
	void PlayMusic(const char* name);
	void PlayEvent(uint id);
	void PlayEvent(const char* event_name);
	void StopEvent(uint id);
	void SendEvent(const char* name);
	AkGameObjectID GetID()const;
	void UI_draw();
	void GetEvents();
public:
	SoundBank* soundbank = nullptr;
private:

	std::vector<AudioEvent*>events;

	std::vector<AudioEvent*>events_to_play;

	Wwise::SoundObject* obj;
};


#endif
