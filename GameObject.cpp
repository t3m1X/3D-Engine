#include "GameObject.h"
#include "Application.h"
#include "ModuleLoader.h"

GameObject::GameObject(int _id, Mesh * m)
{
	mesh = m;
	id = _id;
	SetName("Game Object");
	position = (0, 0, 0);
	rotation = (0, 0, 0);
	scale = (1, 1, 1);


	boundingbox.r = { 0,0,0 };
	for (int i = 0; i < mesh->num_vertices * 3; ++i)
	{
		if (boundingbox.r.x < mesh->vertices[i])
			boundingbox.r.x = mesh->vertices[i];
		if (boundingbox.r.y < mesh->vertices[++i])
			boundingbox.r.y = mesh->vertices[i];
		if (boundingbox.r.z > mesh->vertices[++i])
			boundingbox.r.z = mesh->vertices[i];
	}
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	/////// Just for now, since it only supports one texture at a time
	tex = App->loader->texture;
	if (!App->tex->Empty()) {
		texture = App->tex->GetTexture();
	}
}

void GameObject::Draw()
{
	
	this->mesh->Render(tex);
	
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
	texture = tex;
}

const int GameObject::GetId() const
{
	return id;
}

const bool GameObject::GetSelected() const
{
	return selected;
}

const vec3 GameObject::GetPosition()
{
	return position;
}

const vec3 GameObject::GetRotation()
{
	return rotation;
}

const vec3 GameObject::GetScale()
{
	return scale;
}
