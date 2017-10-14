#include "GameObject.h"
#include "Application.h"
#include "ModuleLoader.h"
GameObject::GameObject(int _id, Mesh * m)
{
	mesh = m;
	this->id = _id;
	SetName("Game Object");
	position = (0, 0, 0);
	rotation = (0, 0, 0);
	scale = (1, 1, 1);
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	/////// Just for now, since it only supports one texture at a time
	this->tex = App->loader->texture;
}

void GameObject::Draw()
{
	
	this->mesh->Render(this->tex);
	
}

void GameObject::Enable()
{
	enabled = true;
}

void GameObject::Disable()
{
	enabled = false;
}

const bool GameObject::GetEnabled() const
{
	return enabled;
}

void GameObject::SetEnabled(const bool & set)
{
	set ? Enable() : Disable();
}

void GameObject::CleanUp()
{
}

const char * GameObject::GetName() const
{
	return name.c_str();
}

void GameObject::SetName(const std::string & set)
{
	name = set;
}

void GameObject::SetSelected(const bool & set)
{
	selected = set;
}

void GameObject::SetTexture(Texture * tex)
{
	this->texture = tex;
}

const int GameObject::GetId() const
{
	return id;
}

const bool GameObject::GetSelected() const
{
	return selected;
}
