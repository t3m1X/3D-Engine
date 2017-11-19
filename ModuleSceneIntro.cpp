#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"
#include "ModuleImGui.h"
#include "Transform.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include <vector>
#include "ModuleFileSystem.h"
#include "ModuleTextures.h"

#define MIN_DISTANCE 9999

ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
	root = new GameObject("root", nullptr);
	root->SetName("Game");

	

	selected = nullptr;
}

ModuleSceneIntro::~ModuleSceneIntro()
{
	
}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG_OUT("Loading Intro assets");
	bool ret = true;

	GameObject* cam_obj = new GameObject("Camera", root);

	Quat rot = Quat::identity;
	float3 scale;
	float3 pos;
	scale.Set(1, 1, 1);
	pos.Set(0, 0, 0);
	Transform* trans = new Transform(cam_obj);
	trans->SetRotation(rot);
	trans->SetPosition(pos);
	trans->SetScale(scale);
	cam_obj->AddComponent(trans);
	ComponentCamera* cam = new ComponentCamera(cam_obj);
	cam_obj->AddComponent(cam);


	float3 max_point;
	float3 min_point;

	max_point.Set(10, 10, 10);
	min_point.Set(-10, 0, -10);

	octree = new Octree();
	octree->Create(max_point, min_point);


	
	App->camera->SetCurrentCamera(cam->GetCamera());
	selected = cam_obj;
	
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG_OUT("Unloading Intro scene");

	delete root;

	return true;
}


void ModuleSceneIntro::AddObject(GameObject * obj)
{

	
	root->AddChild(obj);


	if (obj->GetStatic()) {
		octree->InsertGO(obj);
		all_objects.push_back(obj);
		LOG_OUT("Insterted into the octree");
	}
	else {
		non_static_objects.push_back(obj);
		all_objects.push_back(obj);
		LOG_OUT("Insterted in non static objects");
	}

	
}


void ModuleSceneIntro::Draw()
{


	if (App->camera->GetCurrentCamera()->GetCulling()) {
		std::list<GameObject*> intersections_list;
		octree->CollectIntersections(intersections_list, App->camera->GetCurrentCamera());
		for (std::list<GameObject*>::iterator it = intersections_list.begin(); it != intersections_list.end(); it++) {
			(*it)->Draw();
			//LOG_OUT("Drawing static objects");
		}


		for (std::list<GameObject*>::iterator it = non_static_objects.begin(); it != non_static_objects.end(); it++) {
			(*it)->Draw();
			//LOG_OUT("Drawing non static objects");
		}
	}
	else {
		root->Draw();
	}

//	root->Draw();

}



void ModuleSceneIntro::SetobjSelected(GameObject * obj)
{
	selected = obj;
}

GameObject * ModuleSceneIntro::GetSelected() const
{
	if (selected != nullptr) {
		return selected;
	}
	else {
		return nullptr;
	}
}

void ModuleSceneIntro::DrawHierarchy() const
{

	ImGui::Begin("Hierarchy");
	root->UIDraw();
	

	ImGui::End();
}

void ModuleSceneIntro::Clear()
{
	//root->children.clear();

	
	delete octree;
	delete root;

}

void ModuleSceneIntro::IntersectAABB(LineSegment & picking, std::vector<GameObject*>& DistanceList)
{

	std::list<GameObject*> distance_list;
	octree->CollectIntersections(distance_list,picking);

	for (std::list<GameObject*>::iterator it = distance_list.begin(); it != distance_list.end(); it++) {
		if (picking.Intersects((*it)->GetBoundingBox())) {
			DistanceList.push_back((*it));
			LOG_OUT("AABB hit");
		}
	}

	for (std::list<GameObject*>::iterator it = non_static_objects.begin(); it != non_static_objects.end(); it++) {
		if (picking.Intersects((*it)->GetBoundingBox())) {
			DistanceList.push_back((*it));
			LOG_OUT("AABB hit");
		}
	}

	/*for (int i = 0; i < all_objects.size(); i++) {
		if (picking.Intersects(all_objects[i]->GetBoundingBox())) {
			DistanceList.push_back(all_objects[i]);
			LOG_OUT("AABB hit");
		}
	}*/

	/*for (uint i = 0; i < root->children.size(); i++) {
		if (root->children[i]->children.size() > 0) {
			for (uint j = 0; j < root->children[i]->children.size(); j++) {
				if (picking.Intersects(root->children[i]->children[j]->GetBoundingBox())) {
					DistanceList.push_back(root->children[i]->children[j]);
					LOG_OUT("AABB hit");
				}
			}
		}
		else {
			if (picking.Intersects(root->children[i]->GetBoundingBox())) {
				DistanceList.push_back(root->children[i]);
				LOG_OUT("AABB hit");
			}
		}
	}*/
}

GameObject * ModuleSceneIntro::SelectObject(LineSegment picking)
{
	GameObject* closest = nullptr;
	std::vector<GameObject*>DistanceList;


	IntersectAABB(picking, DistanceList);

	if (DistanceList.size() > 0) {
		float last_distance = inf;

		for (uint i = 0; i < DistanceList.size(); i++) {

			Mesh* m = (Mesh*)DistanceList[i]->FindComponentbyType(MESH);
			float dist = inf;
			float3 hitpoint;
			if (m != nullptr) {
				bool hit = m->TriCheck(picking, dist, hitpoint);
				if (hit) {
					LOG_OUT("HIT!!");
					if (dist < last_distance) {
						last_distance = dist;
						closest = DistanceList[i];
					}
				}
			}
		}

	}

	return closest;
}



void ModuleSceneIntro::RecalculateOctree()
{
	float3 min_point = { 0, 0, 0 };
	float3 max_point = { 0, 0, 0 };

	for (int i = 0; i < all_objects.size(); i++)
	{
		if ((all_objects[i] != nullptr)&&(all_objects[i]->GetStatic()))
		{
			if (all_objects[i]->GetBoundingBox().minPoint.x < min_point.x)
				min_point.x = all_objects[i]->GetBoundingBox().minPoint.x;

			if (all_objects[i]->GetBoundingBox().minPoint.y < min_point.y)
				min_point.y = all_objects[i]->GetBoundingBox().minPoint.y;

			if (all_objects[i]->GetBoundingBox().minPoint.z < min_point.z)
				min_point.z = all_objects[i]->GetBoundingBox().minPoint.z;

			if (all_objects[i]->GetBoundingBox().maxPoint.x > max_point.x)
				max_point.x = all_objects[i]->GetBoundingBox().maxPoint.x;

			if (all_objects[i]->GetBoundingBox().maxPoint.y > max_point.y)
				max_point.y = all_objects[i]->GetBoundingBox().maxPoint.y;

			if (all_objects[i]->GetBoundingBox().maxPoint.z > max_point.z)
				max_point.z = all_objects[i]->GetBoundingBox().maxPoint.z;
		}
	}

	if (octree != nullptr) {
		delete octree;
	}

	octree = new Octree();
	octree->Create(max_point, min_point);

	for (int i = 0; i < all_objects.size(); i++) {
		if (all_objects[i]->GetStatic()) {
			octree->InsertGO(all_objects[i]);
		}
	}
	

	
}

const char* ModuleSceneIntro::Serialize(const char * name)
{
	App->fs->CreateFolder("Assets", "Scenes");
	std::string file = "Assets/Scenes/";
	file += name;
	std::string test = GetExtension(name);
	if (strcmp(test.c_str(), "json") != 0)
		file += ".json";

	JSON_File* scene_file = App->json->LoadJSON(file.c_str());

	scene_file->SetString("scene.name", name);
	scene_file->AddArray("gameobjects");
	scene_file->AddArray("components");

	if (root != nullptr) {
		root->Serialize(scene_file);
	}

	scene_file->Save();

	return file.c_str();

}

const char * ModuleSceneIntro::LoadScene(const char * scene_name)
{

	std::string file = "Assets/Scenes/";
	file += scene_name;

	JSON_File* scene_doc = App->json->LoadJSON(file.c_str());

	if (scene_doc == nullptr) {
		LOG_OUT("ERROR Loading Scene");
		return "";
	}

	Clear();

	root = new GameObject("Game", nullptr);

	float3 max_point;
	float3 min_point;

	max_point.Set(10, 10, 10);
	min_point.Set(-10, 0, -10);

	octree = new Octree();
	octree->Create(max_point, min_point);


	std::vector<GameObject*> tmp_gos;
	scene_doc->RootObject();
	int n_gos = scene_doc->ArraySize("gameobjects");
	for (int i = 0; i < n_gos; i++) {
		scene_doc->RootObject();
		scene_doc->MoveToInsideArray("gameobjects", i);

		GameObject* go = new GameObject(scene_doc->GetString("name"));
		if (go != nullptr) {
			go->SetUID(scene_doc->GetNumber("UID"));
			go->SetParentUID(scene_doc->GetNumber("parentUID"));
			go->SetStatic(scene_doc->GetBool("static"));
			tmp_gos.push_back(go);


		}
		else
			LOG_OUT("ERROR Loading gameobject '%s'", scene_doc->GetString("name"));
	}
	scene_doc->RootObject();

	// GameObjects Connections
	for (int i = 0; i < tmp_gos.size(); i++) {
		GameObject* curr = tmp_gos[i];
		for (int k = 0; k < tmp_gos.size(); k++) {
			GameObject* checked = tmp_gos[k];
			if (curr->GetParentUID() == checked->GetUID()) {
				curr->SetParent(checked);
				checked->children.push_back(curr);
				break;
			}
		}
		if (tmp_gos[i]->GetParentUID() == -1) {
			App->scene_intro->AddObject(tmp_gos[i]);
		}
		else {
			App->scene_intro->all_objects.push_back(tmp_gos[i]);
		}
	}

	scene_doc->RootObject();
	int nComponents = scene_doc->ArraySize("components");
	std::vector<Component*> tmp_cs;
	for (int i = 0; i < nComponents; i++) {
		scene_doc->RootObject();
		scene_doc->MoveToInsideArray("components", i);
		int type = scene_doc->GetNumber("type");// COMPONENT_TYPE
		Component* c = nullptr;
		Material* mat = nullptr;
		Mesh* cmesh = nullptr;
		int aux = 0;
		const char* mpath = nullptr;

		switch (type) {
		case MESH:
			scene_doc->MoveToInsideArray("components", i);
			mpath = scene_doc->GetString("path");
			if (mpath != nullptr) {
				if (App->fs->exists(mpath))
					cmesh = App->loader->Loadrmesh(mpath);
				else
					cmesh = App->loader->Loadrmesh(scene_doc->GetString("fbx_path"));
				if (cmesh != nullptr) {
					cmesh->path = scene_doc->GetString("path");
					cmesh->fbx_path = scene_doc->GetString("fbx_path");
					cmesh->Setname(scene_doc->GetString("name"));
					App->loader->meshes.push_back(cmesh);
					c = cmesh;
				}
			}
			else {
				LOG_OUT("Component mesh with malformed path");
			}
			break;
			if (cmesh != nullptr) {
				cmesh->path = scene_doc->GetString("path");
				cmesh->fbx_path = scene_doc->GetString("fbx_path");
				App->loader->meshes.push_back(cmesh);
				c = cmesh;
			}
			break;
		case MATERIAL:
			LOG_OUT("Checking material");
			scene_doc->MoveToInsideArray("components", i);
			mat = new Material();
			aux = scene_doc->ArraySize("textures");
			for (int i = 0; i < aux; i++) {
				scene_doc->MoveToInsideArray("textures", i);
				if (App->fs->exists(scene_doc->GetString("path"))) {
					Texture* tmp_tex = App->tex->LoadDDSTexture(scene_doc->GetString("path"));
					tmp_tex->SetTextureType(DIFFUSE);//just diffuse for now
					mat->AddTexture(tmp_tex);
					LOG_OUT("Material created");
				}
				else {
					Texture* tmp_tex = App->tex->LoadDDSTexture(scene_doc->GetString("path"));
					tmp_tex->SetTextureType(DIFFUSE);// just diffuse for now
					mat->AddTexture(tmp_tex);
					LOG_OUT("Material created");
				}

				if (mat != nullptr && !mat->HasTextures()) {
					c = mat;
				}
			}

			/*if (mat != nullptr) {
				mat->CleanUp();
				delete mat;
			}*/

			scene_doc->RootObject();
			scene_doc->MoveToInsideArray("components", i);
			break;
		case TRANSFORM:
			float3 p = { (float)scene_doc->GetNumber("position.x"), (float)scene_doc->GetNumber("position.y"), (float)scene_doc->GetNumber("position.z") };
			Quat r = { (float)scene_doc->GetNumber("rotation.x"), (float)scene_doc->GetNumber("rotation.y"), (float)scene_doc->GetNumber("rotation.z"), (float)scene_doc->GetNumber("rotation.w") };
			float3 s = { (float)scene_doc->GetNumber("scale.x"), (float)scene_doc->GetNumber("scale.y"), (float)scene_doc->GetNumber("scale.z") };
			Transform* trans = new Transform(s, r, p);
			c = trans;
				break;
			}
	

			if (c != nullptr) {
				
					c->SetOwnerUID(scene_doc->GetNumber("ownerUID"));
					tmp_cs.push_back(c);
				

			}
		}
			// Components Link
			for (int i = 0; i < tmp_gos.size(); i++) {
				for (int k = 0; k < tmp_cs.size(); k++) {
					if (tmp_gos[i]->GetUID() == tmp_cs[k]->GetOwnerUID()) {
						tmp_gos[i]->AddComponent(tmp_cs[k]);
						if (tmp_cs[k]->GetType() == MESH) {
							tmp_gos[i]->RecalculateAABB();
						}
						if (tmp_cs[k]->GetType() == TRANSFORM) {
							Transform* tr = (Transform*)tmp_cs[k];
							tr->RecalculateTransform();
						}
					}
				}
			}
			/*for (int i = 0; i < tmp_gos.size(); i++) {
				if (tmp_gos[i]->GetUID() == c->GetOwnerUID()) {
					tmp_gos[i]->AddComponent(c);
					if (tmp_gos[i]->HasMesh()) {
						tmp_gos[i]->RecalculateAABB();
						App->scene_intro->AddObject(tmp_gos[i]);
					}
				}

			}*/




			for (uint i = 0; i < App->scene_intro->all_objects.size(); ++i)
			{
				if (App->scene_intro->all_objects[i]->GetStatic())
				{
					App->scene_intro->octree->InsertGO(App->scene_intro->all_objects[i]);
				}
			}


			RecalculateOctree();
			return file.c_str();

		}
	



// Update
update_status ModuleSceneIntro::Update(float dt)
{
	bPlane p(0, 1, 0, 0);
	p.axis = true;
	
	if (octree->need_update) {
		
		RecalculateOctree();

	}
	DrawHierarchy();
	root->Update();
	//root->Draw();
	octree->DebugDraw();
	p.Render();

	
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
		octree->Divide();
	}
	
	return UPDATE_CONTINUE;
}