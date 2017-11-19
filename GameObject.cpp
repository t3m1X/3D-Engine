#include "GameObject.h"
#include "Application.h"
#include "ModuleLoader.h"
#include "Material.h"
#include "glew\include\GL\glew.h"
#include "ModuleImGui.h"
#include "Transform.h"
#include "ModuleCamera3D.h"
#include "ModuleSceneIntro.h"


GameObject::GameObject(std::string name, GameObject* _parent) : parent(_parent)
{
	/*std::string n = name + std::to_string(id);*/
	SetName(name);

	if (_parent != nullptr)
		_parent->AddChild(this);

	boundingbox.SetNegativeInfinity();
	UID = RandomNumber();
	if (parent != nullptr) {
		parent_UID = parent->UID;
	}
	
	//Static = false;
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
	boundingbox.SetNegativeInfinity();

	UID = RandomNumber();
	if (parent != nullptr) {
		parent_UID = parent->UID;
	}
	
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

	for (uint i = 0; i < components.size(); i++) {
		components[i]->Update();
	}
	
}

void GameObject::Draw()
{
	if (App->camera->GetCurrentCamera()->GetCulling()) {
		if (App->camera->GetCurrentCamera()->IsInside(GetBoundingBox())) {
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

				LOG_OUT("Puta");
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
					glDisable(GL_COLOR_MATERIAL);
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
				glEnable(GL_COLOR_MATERIAL);

			}


			for (uint i = 0; i < children.size(); i++)
				children[i]->Draw();
			glPopMatrix();

			DrawBox();
		}
	}
	else {
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
				glDisable(GL_COLOR_MATERIAL);
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
			glEnable(GL_COLOR_MATERIAL);
		}

		for (uint i = 0; i < children.size(); i++)
			children[i]->Draw();
		glPopMatrix();

		DrawBox();
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
	c->SetOwner(this);
	c->SetOwnerUID(this->UID);
}

void GameObject::AddChild(GameObject * child)
{
	children.push_back(child);
}

//
// void GameObject::RecalculateGlobalTrans()
//{
//	 Transform* trans = (Transform*)FindComponentbyType(TRANSFORM);
//	 if (trans != nullptr)
//	 {
//		 float4x4 temp;
//		 temp.SetIdentity();
//
//		 if (parent != nullptr && parent->HasComponent(TRANSFORM))
//			 temp = temp * ((Transform*)parent->FindComponentbyType(TRANSFORM))->GetGlobalTransform();
//
//		 temp = temp * trans->GetLocalTransform();
//		 trans->SetGlobalTransform(temp);
//
//		 for (int i = 0; i < children.size(); i++) {
//			 children[i]->RecalculateGlobalTrans();
//		 }
//	 }
//}

 bool GameObject::HasComponent(COMPONENT_TYPE type) const
 {
	 bool ret = false;
	 for (uint i = 0; i < components.size() && !ret; i++)
		 ret = components[i]->GetType() == type;

	 return ret;
 }

Component * GameObject::FindComponentbyType(COMPONENT_TYPE type)
{
	for (uint i = 0; i < components.size(); i++) {
		if (components[i]->GetType() == type) {
			return components[i];
		}
	}

	return nullptr;
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
			App->scene_intro->selected = this;
		}

		for (uint i = 0; i < children.size(); i++) {
			children[i]->UIDraw();
		}

		ImGui::TreePop();
	}

	
}

void GameObject::DrawComponents()
{
	if (ImGui::Checkbox("Static",&Static)) {

		if (GetStatic()) {
			
			App->scene_intro->octree->InsertGO(this);
			if (std::find(App->scene_intro->non_static_objects.begin(), App->scene_intro->non_static_objects.end(), this) != App->scene_intro->non_static_objects.end()) {
				App->scene_intro->non_static_objects.remove(this);
			}
			
			
		}
		else {
			
			App->scene_intro->octree->EraseGO(this);
			
			
		}
	}
	//ImGui::SameLine();
	
	

	if (!components.empty()) {
		for (uint i = 0; i < components.size(); i++) {
			components[i]->UI_draw();
		}
	}
}

void GameObject::RecalculateAABB()
{
	bbinit = true;
	bool has_mesh = false;
	Mesh* m = (Mesh*)this->FindComponentbyType(MESH);
	Transform* trans = (Transform*)this->FindComponentbyType(TRANSFORM);
	if (m!=nullptr) {
		if (trans != nullptr) {
			boundingbox = AABB::MinimalEnclosingAABB((float3*)m->vertices, m->num_vertices);
			boundingbox.Transform(trans->GetGlobalTransform());
		}
	}
}

void GameObject::RecalculateAABB(float4x4 transformation)
{
	if (bbinit)
		boundingbox.Transform(transformation);
}

vector<GameObject*> GameObject::GetChild() const
{
	return children;
}

GameObject * GameObject::GetParent() const
{
	return parent;
}

void GameObject::SetParent(GameObject * p)
{
	this->parent = p;
}

void GameObject::DrawBox()
{
	float3 corners[8];
	AABB box = GetBoundingBox();
	box.GetCornerPoints(corners);
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

void GameObject::OnGuizmo()
{
	Transform* trans = (Transform*)FindComponentbyType(TRANSFORM);
	if(trans!=nullptr)
	trans->OnGuizmo();
}

bool GameObject::GetStatic() const
{
	return Static;
}

void GameObject::SetStatic(bool  set)
{
	Static = set;
}

bool GameObject::HasMesh() const
{
	bool ret=false;

	for (uint i = 0; i < components.size(); i++) {
		if (components[i]->GetType() == MESH) {
			ret = true;
		}
	}
	return ret;
}

void GameObject::Serialize(JSON_File * doc)
{
	if (doc == nullptr) {
		return;
	}
	doc->RootObject();
	doc->AddArraySection("gameobjects");
	doc->MoveToInsideArray("gameobjects", doc->ArraySize("gameobjects") - 1);

	doc->SetBool("static", this->Static);
	doc->SetNumber("UID", this->UID);
	if (this->parent != nullptr) {
		doc->SetNumber("parentUID", parent->UID);
	}
	else {
		doc->SetNumber("parentUID", -1);
	}
	doc->SetString("name", this->name.c_str());
	for (uint i = 0; i < components.size(); i++) {
		doc->RootObject();
		doc->AddArraySection("components");
		doc->MoveToInsideArray("components", doc->ArraySize("components")-1);
		components[i]->Serialize(doc);

	}
	for (int i = 0; i < children.size(); i++) {
		children[i]->Serialize(doc);
	}

	doc->RootObject();
}

double GameObject::GetUID() const
{
	return UID;
}

double GameObject::GetParentUID() const
{
	return parent_UID;
}

void GameObject::SetUID(double  set)
{
	this->UID = set;
}

void GameObject::SetParentUID(double set)
{
	parent_UID = set;
}

AABB GameObject::GetBoundingBox() const
{
	return boundingbox.MinimalEnclosingAABB();
}


