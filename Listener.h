#pragma once
#ifndef _LISTENER_H_
#define _LISTENER_H_

#include "Component.h"

class Listener : public Component {

public:

	Listener(GameObject* own);
	Listener();
	~Listener();
	void Update();
	uint GetId()const;



private:

	uint id;


};


#endif // !_LISTENER_H_
