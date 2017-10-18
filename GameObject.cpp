#include "GameObject.h"
#include "Application.h"
#include "ModuleLoader.h"

GameObject::GameObject(const char* name, int id)
{
	this->name = name + std::to_string(id);
	SetName("Game Object");

	/*
	boundingbox.r = { 0,0,0 };

	for (list<Mesh*>::iterator it = mesh.begin(); it != mesh.end(); ++it) {
		for (int i = 0; i < (*it)->num_vertices * 3; ++i)
		{
			if (boundingbox.r.x < (*it)->vertices[i])
				boundingbox.r.x = (*it)->vertices[i];
			if (boundingbox.r.y < (*it)->vertices[++i])
				boundingbox.r.y = (*it)->vertices[i];
			if (boundingbox.r.z >(*it)->vertices[++i])
				boundingbox.r.z = (*it)->vertices[i];
		}
	}

	for (list<Mesh*>::iterator it = mesh.begin(); it != mesh.end(); ++it) {
		vertices += (*it)->num_vertices;
		tris += (*it)->num_faces;
	}*/


}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	
}

void GameObject::Draw()
{
	
	
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
	for (int i = 0; i < components.size(); i++) {
		delete components[i];
	}
	for (int i = 0; i < children.size(); i++) {
		delete children[i];
	}

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

void GameObject::AddComponent(Component * c)
{
	components.push_back(c);
}


const bool GameObject::GetSelected() const
{
	return selected;
}

