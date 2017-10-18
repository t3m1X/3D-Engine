#include "Component.h"

Component::Component(GameObject* own)
{
	active = true;// they're active by default
	owner = own;
}

Component::~Component()
{
}

void Component::Update()
{

}

void Component::Enable()
{
	active = true;
}

void Component::Disable()
{
	active = false;
}

bool Component::IsActive()
{
	return active;
}

COMPONENT_TYPE Component::GetType()
{
	return type;
}
