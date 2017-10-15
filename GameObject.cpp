#include "GameObject.h"
#include "Application.h"
#include "ModuleLoader.h"

GameObject::GameObject(int _id, std::list<Mesh*>& m)
{
	mesh = m;
	id = _id;
	SetName("Game Object");
	position = { 0, 0, 0 };
	rotation = { 0, 0, 0 };
	scale = { 0, 0, 0 };

	vertices = 0;
	tris = 0;

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
	for (list<Mesh*>::iterator it = mesh.begin(); it != mesh.end(); ++it) {
		(*it)->Render(tex);
	}
	
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
	for (list<Mesh*>::iterator it = mesh.begin(); it != mesh.end(); ++it) {
		(*it)->CleanUp();
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

void GameObject::SetTexture(Texture * tex)
{
	texture = tex;
}

void GameObject::SetPosition(float3 pos)
{
	position = pos;
}

void GameObject::SetScale(float3 scale)
{
	this->scale = scale;
}

void GameObject::SetRotation(float3 rot)
{
	rotation = rot;
}

void GameObject::SetTransform(const float4x4 & matrix)
{
	transform.Set(matrix);

	float3x3 rot_mat;
	transform.Decompose(position, rot_mat, scale);
	rotation = rot_mat.ToEulerXYZ();
}

const int GameObject::GetId() const
{
	return id;
}

const bool GameObject::GetSelected() const
{
	return selected;
}

const float3 GameObject::GetPosition()const
{
	return position;
}

const float3 GameObject::GetRotation()const
{
	return rotation;
}

const float3 GameObject::GetScale()const
{
	return scale;
}
