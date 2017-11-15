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

void Component::UI_draw()
{
}


void Component::Setname(const char * n)
{
	name = n;
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

void Component::SetOwner( GameObject * go)
{
	this->owner = go;
}

const char * Component::GetName()
{
	return name;
}
