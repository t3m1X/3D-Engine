#include "ModuleLoader.h"
#include "glew\include\GL\glew.h"
#include "Application.h"
#include "Console.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "GameObject.h"
#include "ModuleSceneIntro.h"
#include "ModuleImGui.h"
#include "ModuleCamera3D.h"
#include "Mesh.h"
#include "Transform.h"
#include "Material.h"
#include <queue>

#pragma comment (lib, "Assimp/libx86/assimp.lib")


ModuleLoader::ModuleLoader(bool start_enabled) : Module(start_enabled)
{
	SetName("Loader");
}

ModuleLoader::~ModuleLoader()
{
}

bool ModuleLoader::Init()
{

	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return true;
}

update_status ModuleLoader::Update(float dt)
{
	
	/*for (list<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it) {
		(*it)->Render(texture);
	}*/

	
	return update_status::UPDATE_CONTINUE;
}

bool ModuleLoader::CleanUp()
{
	aiDetachAllLogStreams();
	if (!meshes.empty()) {
		for (list<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it) {
		//	UnloadMesh(*it);
			delete (*it);
		}
		meshes.clear();
	}
	LOG("All meshes deleted");
	return true;
}

void ModuleLoader::LoadFBX(char* path)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	LOG("FBX Load: Loading %s", path);
	if (scene->mRootNode != nullptr) {
		std::queue<pair<aiNode*, GameObject*>> nodes;
		nodes.push({ scene->mRootNode, nullptr });
		while (!nodes.empty())
		{
			pair<aiNode*, GameObject*> cnode = nodes.front();
			LOG("FBX Load: Loading node with %d meshes", cnode.first->mNumMeshes);
			//Creating new gameobject
			GameObject* new_obj = new GameObject(cnode.first->mName.C_Str(), cnode.second);
			if (cnode.first == scene->mRootNode) {
				App->scene_intro->AddObject(new_obj);
				//App->scene_intro->SetobjSelected(new_obj);
			}

			//Adding childs to queue
			LOG("FBX Load: Queueing %d child nodes", cnode.first->mNumChildren);
			for (int i = 0; i < cnode.first->mNumChildren; ++i)
				nodes.push({ cnode.first->mChildren[i], new_obj });

			GameObject* parent = new_obj;
			// Use scene->mNumMeshes to iterate on scene->mMeshes array
			for (int i = 0; i < cnode.first->mNumMeshes; i++, new_obj = new GameObject(cnode.first->mName.C_Str(), i, parent)) {

				//Transform
				float3 pos(0, 0, 0);
				float3 scale(1, 1, 1);
				Quat rot({ 1,0,0 }, 0);

				if (new_obj == parent)
				{
					LOG("Loading transform");
					aiVector3D translation;
					aiVector3D scaling;
					aiQuaternion rotation;

					cnode.first->mTransformation.Decompose(scaling, rotation, translation);
					pos = { translation.x, translation.y, translation.z };
					scale = { scaling.x, scaling.y, scaling.z };
					rot = Quat( rotation.x, rotation.y, rotation.z, rotation.w );
				}

				Transform* trans = new Transform(new_obj);
				trans->SetRotation(rot);
				trans->SetPosition(pos);
				trans->SetScale(scale);
				new_obj->AddComponent(trans);

				//Vertices
				//App->imgui->curr_obj = new_obj;
				aiMesh* m = scene->mMeshes[cnode.first->mMeshes[i]];
				Mesh* new_mesh = new Mesh(new_obj);
				new_mesh->num_faces = m->mNumFaces;
				new_mesh->num_vertices = m->mNumVertices;
				new_mesh->vertices = new float[new_mesh->num_vertices * 3];
				memcpy(new_mesh->vertices, m->mVertices, sizeof(float) * new_mesh->num_vertices * 3);
				LOG("New mesh with %d vertices", new_mesh->num_vertices);
				glGenBuffers(1, (GLuint*) &(new_mesh->id_vertices));
				glBindBuffer(GL_ARRAY_BUFFER, new_mesh->id_vertices);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * new_mesh->num_vertices * 3, new_mesh->vertices, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				LOG("New mesh sent to VRAM");

				//Indices

				if (m->HasFaces()) {
					new_mesh->num_indices = m->mNumFaces * 3;
					new_mesh->indices = new uint[new_mesh->num_indices];
					for (uint i = 0; i < m->mNumFaces; ++i)
					{
						if (m->mFaces[i].mNumIndices != 3) {
							LOG("WARNING, geometry face with != 3 indices!");
						}
						else
							memcpy(&new_mesh->indices[i * 3], m->mFaces[i].mIndices, 3 * sizeof(uint));
					}

				}
				LOG("New mesh with %d indices", new_mesh->num_indices);
				glGenBuffers(1, (GLuint*) &(new_mesh->id_indices));
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->id_indices);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * new_mesh->num_indices, new_mesh->indices, GL_STATIC_DRAW);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				//UVS
				if (m->HasTextureCoords(0))
				{
					new_mesh->num_uv = m->mNumVertices;
					new_mesh->UVs = new float[new_mesh->num_uv * 3];
					memcpy(new_mesh->UVs, m->mTextureCoords[0], sizeof(float)*new_mesh->num_uv * 3);

					glGenBuffers(1, (GLuint*)&(new_mesh->id_uv));
					glBindBuffer(GL_ARRAY_BUFFER, (GLuint)new_mesh->id_uv);
					glBufferData(GL_ARRAY_BUFFER, sizeof(float) * new_mesh->num_uv * 3, new_mesh->UVs, GL_STATIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, 0);

					LOG("Loading UVs succesfully");
				}
				else
					LOG("FBX Load: No texture coordinates found");


				meshes.push_back(new_mesh);
				LOG("FBX Load: Mesh loaded with %d vertices and %d indices", new_mesh->num_vertices, new_mesh->num_indices);

				new_obj->AddComponent(new_mesh);
			
				new_obj->RecalculateAABB();
				new_obj->Enable();
				LOG("Created new object");

				////////Material
				aiString tex_path;
				//////Just diffuse for now
				if (scene->mMaterials[m->mMaterialIndex]->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
					scene->mMaterials[m->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, m->mMaterialIndex, &tex_path);
					Texture* diffuse = App->tex->LoadTexture(tex_path.C_Str());
					if (diffuse != nullptr) {
						Material* mat = new Material(new_obj);
						mat->AddTexture(diffuse);
						new_obj->AddComponent(mat);
					}
				}
			}
			nodes.pop();
		}
		aiReleaseImport(scene);
	}
	else
		LOG("FBX Load: Error loading scene %s", path);
}

void ModuleLoader::UnloadMesh(Mesh * m)
{
	for (list<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it) 
	{
		if ((*it) == m)
		{
			// Unload from memory
			if ((*it)->id_vertices != 0)
			{
				glDeleteBuffers(1, (GLuint*)(*it)->id_vertices);
			}

			if ((*it)->id_indices != 0)
			{
				glDeleteBuffers(1, (GLuint*)(*it)->id_indices);
			}

			if ((*it)->id_uv != 0)
			{
				glDeleteBuffers(1, (GLuint*)(*it)->id_uv);
			}

			(*it)->CleanUp();
			meshes.erase(it);
			break;
		}
	}
}

void ModuleLoader::SetWire(bool w)
{
	for (list<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it) {
		(*it)->SetWire(w);
	}
}



