#ifndef __COMPONENT__H__
#define __COMPONENT__H__

#include "GameObject.h"

enum COMPONENT_TYPE {
	MESH = 0,
	MATERIAL,
	TRANSFORM
};

class Component {

	public:

		Component(GameObject* own);
		virtual ~Component();
		virtual void Update();
		virtual void Enable();
		virtual void Disable();
		virtual bool IsActive();
		COMPONENT_TYPE GetType();

	private:

		COMPONENT_TYPE type;
		bool active;
		GameObject* owner;








};

#endif
