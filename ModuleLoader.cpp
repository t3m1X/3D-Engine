#include "ModuleLoader.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"


#pragma comment (lib, "Assimp/libx86/assimp.lib")


bool ModuleLoader::CleanUp()
{
	aiDetachAllLogStreams();
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
			Mesh new_mesh;
			new_mesh.num_vertices = m->mNumVertices;
			new_mesh.vertices = new float[new_mesh.num_vertices * 3];
			memcpy(new_mesh.vertices, m->mVertices, sizeof(float) * new_mesh.num_vertices * 3);
			LOG("New mesh with %d vertices", new_mesh.num_vertices);
			//Indices

			if (m->HasFaces()) {
				new_mesh.num_indices = m->mNumFaces * 3;
				new_mesh.indices = new uint[new_mesh.num_indices];

				for (uint i = 0; i < m->mNumFaces; ++i)
				{
					if (m->mFaces[i].mNumIndices != 3) {
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
						memcpy(&new_mesh.indices[i * 3], m->mFaces[i].mIndices, 3 * sizeof(uint));
				}

			}

			meshes.push_back(&new_mesh);
			
		}

		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene %s", path);
}
