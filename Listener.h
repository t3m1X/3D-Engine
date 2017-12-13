#pragma once
#ifndef _LISTENER_H_
#define _LISTENER_H_

#include "Component.h"
#include "ModuleAudio.h"

class Listener : public Component {

public:

	Listener(GameObject* own);
	Listener();
	~Listener();
	void Update();
	AkGameObjectID GetId()const;



private:

	Wwise::SoundObject* obj;



};


#endif // !_LISTENER_H_
