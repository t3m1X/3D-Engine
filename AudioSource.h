#pragma once
#ifndef _AUDIOSOURCE_H_
#define _AUDIOSOURCE_H_

#include "Component.h"



class AudioSource : public Component {

public :

	AudioSource(GameObject* own);
	AudioSource();
	~AudioSource();

	void Update();
	void PlayMusic(double audio_id);
	void PlayMusic(const char* name);
	void PlayEvent(uint id);
	void PlayEvent(const char* event_name);
	void StopEvent(uint id);
	double GetID()const;


private:

	
	double id;
};


#endif
