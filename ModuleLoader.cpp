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
	return true;
}

void ModuleLoader::LoadFBX(char* path)
{
	/*if (!meshes.empty()) {
		for (list<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it) {
			//UnloadMesh(*it);
			delete (*it);
		}
		meshes.clear();
		App->scene_intro->Clear();
		if (texture != 0) {
			glDeleteTextures(1, &texture);
			texture = 0;
			App->tex->Clear();
		}

	}*/
	
	

	
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	LOG("FBX Load: Loading %s", path);
	if (scene != nullptr && scene->HasMeshes())
	{
		LOG("FBX Load: Loading %d meshes", scene->mNumMeshes);
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for ( int i = 0; i < scene->mNumMeshes; i++) {
			//Vertices
			
			GameObject* new_obj = new GameObject("Game Object ", i );
			aiMesh* m = scene->mMeshes[i];
			Mesh* new_mesh = new Mesh(new_obj);
			new_mesh->num_faces = m->mNumFaces;
			new_mesh->num_vertices = m->mNumVertices;
			new_mesh->vertices = new float[new_mesh->num_vertices * 3];
			memcpy(new_mesh->vertices, m->mVertices, sizeof(float) * new_mesh->num_vertices * 3);
			LOG("New mesh with %d vertices", new_mesh->num_vertices);
			glGenBuffers(1, (GLuint*) &(new_mesh->id_vertices));
			glBindBuffer(GL_ARRAY_BUFFER,new_mesh->id_vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * new_mesh->num_vertices * 3, new_mesh->vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			LOG("New mesh sended to VRAM");
			
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
			new_obj->Enable();
			LOG("Created new object");
			aiNode* root = scene->mRootNode;
			aiVector3D translation;
			aiVector3D scaling;
			aiQuaternion rotation;

			root->mTransformation.Decompose(scaling, rotation, translation);

			float3 pos(translation.x, translation.y, translation.z);
			float3 scale(scaling.x, scaling.y, scaling.z);
			Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

			Transform* trans = new Transform(scale, rot, pos, new_obj);
			new_obj->AddComponent(trans);

			App->scene_intro->AddObject(new_obj);
			App->scene_intro->SetobjSelected(new_obj);

			////////Materials
			if (scene->HasMaterials()) {
				for (uint i = 0; i < scene->mNumMaterials; i++) {
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
				
			}

		}

		//App->camera->FocusMesh(new_mesh);
		/*App->camera->Move(vec3(0, new_obj->boundingbox.r.y * 2, new_obj->boundingbox.r.z * 2) - App->camera->Position);

		App->camera->Move(vec3(0, new_obj->boundingbox.r.y + 5, new_obj->boundingbox.r.z - 5) - App->camera->Position);

		App->camera->LookAt(vec3(0, 0, 0));*/
		
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



