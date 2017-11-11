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

void Component::SetEnabled(const bool& set)
{
	set ? Enable() : Disable();
}

bool Component::IsActive()
{
	return active;
}

void Component::UI_draw()
{
}


void Component::Setname(const char * n)
{
	name = n;
}

const char * Component::GetName() const
{
	return name;
}

COMPONENT_TYPE Component::GetType()
{
	return type;
}

void Component::SetType(COMPONENT_TYPE t)
{
	type = t;
}

GameObject * Component::GetOwner() const
{
	return owner;
}
