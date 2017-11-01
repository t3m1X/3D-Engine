#include "GameObject.h"
#include "Application.h"
#include "ModuleLoader.h"
#include "Material.h"
#include "glew\include\GL\glew.h"
#include "ModuleImGui.h"
#include "Transform.h"


GameObject::GameObject(const char* name, GameObject* _parent) : parent(_parent)
{
	/*std::string n = name + std::to_string(id);*/
	SetName(name);

	if (_parent != nullptr)
		_parent->AddChild(this);
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

	for (uint i = 0; i < children.size(); i++) {
		children[i]->Update();
	}
	
}

void GameObject::Draw()
{
	bool has_mesh = false;
	bool has_material = false;
	Transform* tr;


	for (uint i = 0; i < components.size(); i++) {
		if (components[i]->GetType() == MESH) {
			has_mesh = true; /////Assuming there's one mesh per game object
		}
		if (components[i]->GetType() == MATERIAL) {
			has_material = true;
		}
		if (components[i]->GetType() == TRANSFORM) {
			tr = (Transform*)this->FindComponentbyType(TRANSFORM);
		}
		
	}
	if (has_mesh) {
		
		Mesh* m = (Mesh*)this->FindComponentbyType(MESH); 

		glPushMatrix();
		glMultMatrixf(tr->GetLocalTransform().Transposed().ptr());

		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, m->id_vertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->id_indices);

		//Apply UV if exist
		if (m->num_uv != 0)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, m->id_uv);
			glTexCoordPointer(3, GL_FLOAT, 0, NULL);
		}

		if (has_material) {
			Material* mat = (Material*)this->FindComponentbyType(MATERIAL);
			glBindTexture(GL_TEXTURE_2D, (GLuint)mat->FindtexturebyType(DIFFUSE)->Getid());
		}
		
		glDrawElements(GL_TRIANGLES, m->num_indices, GL_UNSIGNED_INT, NULL);



		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//Unbind textures affter rendering
		glBindTexture(GL_TEXTURE_2D, 0);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glPopMatrix();
		//LOG("Rendering object");
	}
	
	if (!children.empty()) {
		for (uint i = 0; i < children.size(); i++) {
			children[i]->Draw();
		}
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
	for (int i = 0; i < components.size(); i++) {
		delete components[i];
	}
	for (int i = 0; i < children.size(); i++) {
		children[i]->CleanUp();
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

void GameObject::AddChild(GameObject * child)
{
	children.push_back(child);
}


const bool GameObject::GetSelected() const
{
	return selected;
}

Component * GameObject::FindComponentbyType(COMPONENT_TYPE type)
{
	for (uint i = 0; i < components.size(); i++) {
		if (components[i]->GetType() == type) {
			return components[i];
		}
	}
}

void GameObject::UIDraw()
{
	uint flags = 0;
	if (children.size() == 0) {
		flags |= ImGuiTreeNodeFlags_Leaf;
	}
	if (App->imgui->curr_obj==this)
	{
		flags |= ImGuiTreeNodeFlags_Selected;
	}


	if (ImGui::TreeNodeEx(name.c_str(), flags)) {
		if (ImGui::IsItemClicked()) {
			App->imgui->curr_obj = this;
		}

		for (uint i = 0; i < children.size(); i++) {
			children[i]->UIDraw();
		}

		ImGui::TreePop();
	}

	
}

void GameObject::DrawComponents()
{
	if (!components.empty()) {
		for (uint i = 0; i < components.size(); i++) {
			components[i]->UI_draw();
		}
	}
}

vector<GameObject*> GameObject::GetChild()
{
	return children;
}


