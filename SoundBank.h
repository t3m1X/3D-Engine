#pragma once
#ifndef _SOUNDBANK_H_
#define _SOUNDBANK_H_

#include <string>
#include <vector>
#include "AudioEvent.h"

class AudioEvent;

class SoundBank {

public: 

	SoundBank(const char* name, const char* path);
	std::vector<AudioEvent*>events;
	std::string name;
	std::string path;

	unsigned int id = 0;
};

#endif
