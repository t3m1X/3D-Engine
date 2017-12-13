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
	AkGameObjectID GetID()const;

private:

	std::vector<AudioEvent*>events;
	SoundBank* soundbank = nullptr;
	Wwise::SoundObject* obj;
};


#endif
