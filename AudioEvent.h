#pragma once
#ifndef _AUDIOEVENT_H_
#define _AUDIOEVENT_H_

#include "ModuleAudio.h"

#include <string>
#include "SoundBank.h"
#include "ModuleJson.h"
class SoundBank;
class AudioEvent {

public:

	AudioEvent();
	~AudioEvent();
	void UnLoad();


	void Load(JSON_File* file, SoundBank* p);
	std::string name;
	uint id = 0;
	bool in_3D = false;

	AkPlayingID playing_id = 0L;
	AkCallbackFunc event_call_back = nullptr;
	SoundBank* parent = nullptr;
	

};










#endif