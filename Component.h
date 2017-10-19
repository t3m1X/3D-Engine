#ifndef __COMPONENT__H__
#define __COMPONENT__H__

#include "GameObject.h"

class GameObject;

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
		virtual void UI_draw();
		virtual void Setname(const char* n);
		COMPONENT_TYPE GetType();
		void SetType(COMPONENT_TYPE t);

	private:
		const char* name;
		COMPONENT_TYPE type;
		bool active;
		GameObject* owner;








};

#endif
