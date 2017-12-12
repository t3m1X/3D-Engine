#pragma once
#ifndef _AUDIOSOURCE_H_
#define _AUDIOSOURCE_H_

#include "Component.h"
#include "ModuleAudio.h"



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
	void SetPosition(float x = 0, float y = 0, float z = 0, float x_front = 1, float y_front = 0, float z_front = 0, float x_top = 0, float y_top = 1, float z_top = 0);


private:

	
	AkGameObjectID id;
	const char* n = nullptr;
	AkVector position;
	AkVector orient_top;
	AkVector orient_front;
};


#endif
