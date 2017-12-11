#ifndef __COMPONENT__H__
#define __COMPONENT__H__

#include "GameObject.h"

class GameObject;

enum COMPONENT_TYPE {
	MESH = 0,
	MATERIAL,
	TRANSFORM,
	CAMERA,
	LISTENER,
	AUDIO_SOURCE
};

class Component {

	public:

		Component(GameObject* own);
		Component() {};
		virtual ~Component();
		virtual void Update();
		virtual void Enable();
		virtual void Disable();
		virtual bool IsActive();
		virtual void UI_draw();
		virtual void Setname(const char* n);
		virtual void Serialize(JSON_File* doc);
		COMPONENT_TYPE GetType();
		void SetType(COMPONENT_TYPE t);
		GameObject* GetOwner()const;
		void SetOwner( GameObject* go);
		const char* GetName();
		void SetOwnerUID(double uid);
		double GetOwnerUID()const;


	protected:
		const char* name;
		COMPONENT_TYPE type;
		bool active;
		GameObject* owner;
		double owner_UID;








};

#endif
