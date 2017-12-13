#pragma once
#ifndef _AUDIOEVENT_H_
#define _AUDIOEVENT_H_

#include "ModuleAudio.h"

#include <string>
#include "SoundBank.h"

class SoundBank;
class AudioEvent {

public:

	AudioEvent();
	~AudioEvent();
	void UnLoad();

	std::string name;
	uint id = 0;
	bool in_3D = false;

	AkPlayingID playing_id = 0L;
	AkCallbackFunc event_call_back = nullptr;
	SoundBank* parent = nullptr;
	

};










#endif