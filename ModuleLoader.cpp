#include "ModuleLoader.h"
#include "glew\include\GL\glew.h"
#include "Application.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"


#pragma comment (lib, "Assimp/libx86/assimp.lib")


ModuleLoader::ModuleLoader(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	SetName("Loader");
}

ModuleLoader::~ModuleLoader()
{
}

bool ModuleLoader::Init()
{
	return true;
}

update_status ModuleLoader::Update(float dt)
{
	
	for (list<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it) {
		(*it)->Render(texture);
	}

	
	return update_status::UPDATE_CONTINUE;
}

bool ModuleLoader::CleanUp()
{
	aiDetachAllLogStreams();
	while (!meshes.empty())
	{
		delete[] meshes.front();
		meshes.pop_front();
	}
	return true;
}

void ModuleLoader::LoadFBX(char* path)
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for ( int i = 0; i < scene->mNumMeshes; i++) {
			//Vertices
			aiMesh* m = scene->mMeshes[i];
			Mesh* new_mesh = new Mesh;
			new_mesh->num_vertices = m->mNumVertices;
			new_mesh->vertices = new float[new_mesh->num_vertices * 3];
			memcpy(new_mesh->vertices, m->mVertices, sizeof(float) * new_mesh->num_vertices * 3);
			LOG("New mesh with %d vertices", new_mesh->num_vertices);
			glGenBuffers(1, (GLuint*) &(new_mesh->id_vertices));
			glBindBuffer(GL_ARRAY_BUFFER, new_mesh->id_vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * new_mesh->num_vertices * 3, new_mesh->vertices, GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
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
				glBindBuffer(GL_ARRAY_BUFFER, (GLuint) new_mesh->id_uv);
				glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * new_mesh->num_uv* 3, new_mesh->UVs, GL_STATIC_DRAW);
			}
			else
			{
				App->con->AddLog("No Texture Coords found");
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			meshes.push_back(new_mesh);
		}

		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene %s", path);
}

void Mesh::Render(uint id)
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);

	//Apply UV if exist
	if (num_uv != 0)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, id_uv);
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, (GLuint)id);

	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);

	

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//Unbind textures affter rendering
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
