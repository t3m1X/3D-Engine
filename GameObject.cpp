#include "GameObject.h"
#include "Application.h"
#include "ModuleLoader.h"
#include "Material.h"
#include "glew\include\GL\glew.h"
#include "ModuleImGui.h"
#include "Transform.h"


GameObject::GameObject(std::string name, GameObject* _parent) : parent(_parent)
{
	/*std::string n = name + std::to_string(id);*/
	SetName(name);

	if (_parent != nullptr)
		_parent->AddChild(this);

	boundingbox.SetNegativeInfinity();
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

GameObject::GameObject(std::string name, uint id, GameObject* _parent) : parent(_parent)
{
	/*std::string n = name + std::to_string(id);*/
	name += "_" + std::to_string(id);
	SetName(name);
	//else
	//	SetName("");

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
	Transform* trans = (Transform*)FindComponentbyType(TRANSFORM);
	if (trans != nullptr) {
		LOG("Transform bounding box");
		boundingbox.Transform(trans->GetGlobalTransform());
	}

	for (uint i = 0; i < children.size(); i++) {
		children[i]->Update();
	}

	for (uint i = 0; i < components.size(); i++) {
		components[i]->Update();
	}
	
}

void GameObject::Draw()
{

	bool has_mesh = false;
	bool has_material = false;
	Transform* tr = nullptr;


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

		DrawBox();

		glMatrixMode(GL_MODELVIEW);
		
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
		
	}
	
	
	for (uint i = 0; i < children.size(); i++)
			children[i]->Draw();
	glPopMatrix();
	
	DrawBox();
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

void GameObject::RecalculateAABB()
{
	bool has_mesh = false;
	Mesh* m;
	Transform* trans = (Transform*)FindComponentbyType(TRANSFORM);
	for (uint i = 0; i < components.size() && !has_mesh; i++) 
		if (components[i]->GetType() == MESH) {
			has_mesh = true; /////Assuming there's one mesh per game object
			m = (Mesh*)components[i];
		}
	if (has_mesh)
		boundingbox.Transform(trans->GetLocalTransform());
		boundingbox.Enclose((float3*)m->vertices, m->num_vertices);
}

vector<GameObject*> GameObject::GetChild()
{
	return children;
}

void GameObject::DrawBox()
{
	float3 corners[8];
	boundingbox.GetCornerPoints(corners);
	const int s = 24;

	float3* lines = new float3[s];
	float3* colors = new float3[s];

	lines[0] = float3(corners[0].x, corners[0].y, corners[0].z);
	lines[1] = float3(corners[2].x, corners[2].y, corners[2].z);

	lines[2] = float3(corners[2].x, corners[2].y, corners[2].z);
	lines[3] = float3(corners[6].x, corners[6].y, corners[6].z);

	lines[4] = float3(corners[4].x, corners[4].y, corners[4].z);
	lines[5] = float3(corners[6].x, corners[6].y, corners[6].z);

	lines[6] = float3(corners[4].x, corners[4].y, corners[4].z);
	lines[7] = float3(corners[0].x, corners[0].y, corners[0].z);

	//

	lines[8] = float3(corners[1].x, corners[1].y, corners[1].z);
	lines[9] = float3(corners[3].x, corners[3].y, corners[3].z);

	lines[10] = float3(corners[3].x, corners[3].y, corners[3].z);
	lines[11] = float3(corners[7].x, corners[7].y, corners[7].z);

	lines[12] = float3(corners[5].x, corners[5].y, corners[5].z);
	lines[13] = float3(corners[7].x, corners[7].y, corners[7].z);

	lines[14] = float3(corners[5].x, corners[5].y, corners[5].z);
	lines[15] = float3(corners[1].x, corners[1].y, corners[1].z);

	//

	lines[16] = float3(corners[0].x, corners[0].y, corners[0].z);
	lines[17] = float3(corners[1].x, corners[1].y, corners[1].z);

	lines[18] = float3(corners[2].x, corners[2].y, corners[2].z);
	lines[19] = float3(corners[3].x, corners[3].y, corners[3].z);

	lines[20] = float3(corners[4].x, corners[4].y, corners[4].z);
	lines[21] = float3(corners[5].x, corners[5].y, corners[5].z);

	lines[22] = float3(corners[6].x, corners[6].y, corners[6].z);
	lines[23] = float3(corners[7].x, corners[7].y, corners[7].z);

	for (int i = 0; i < s; i++)
	{
		colors[i] = float3(60, 1, 1);
	}

	//	DrawLinesList(lines, s, 5, colors);

	glLineWidth((float)5);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, (float*)lines->ptr());

	if (colors != nullptr)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, (float*)colors->ptr());
	}

	glDrawArrays(GL_LINES, 0, s);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glLineWidth(1);

	delete[] lines;
	delete[] colors;
}


