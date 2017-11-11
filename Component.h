#ifndef __COMPONENT__H__
#define __COMPONENT__H__

#include "GameObject.h"

class GameObject;

enum COMPONENT_TYPE {
	MESH = 0,
	MATERIAL,
	TRANSFORM,
	CAMERA
};

class Component {

	public:

		Component(GameObject* own);
		virtual ~Component();
		virtual void Update();
		virtual void Enable();
		virtual void Disable();
		virtual void SetEnabled(const bool& set);
		virtual bool IsActive();
		virtual void UI_draw();
		virtual void Setname(const char* n);
		const char* GetName() const;

		COMPONENT_TYPE GetType();
		void SetType(COMPONENT_TYPE t);
		GameObject* GetOwner()const;

	protected:
		const char* name;
		COMPONENT_TYPE type;
		bool active;
		GameObject* owner;








};

#endif
