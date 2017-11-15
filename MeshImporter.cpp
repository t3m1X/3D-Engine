#include "MeshImporter.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "Application.h"
#include "ModuleFilesystem.h"
#include "Mesh.h"
#include "glew\include\GL\glew.h"
#include <queue>

MeshImporter::MeshImporter()
{
}

MeshImporter::~MeshImporter()
{
}

bool MeshImporter::Import(const char * path, std::string & output_file)
{
	bool ret = false;

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene->mRootNode != nullptr && scene->HasMeshes())
	{
		std::queue<aiNode*> nodes;
		nodes.push(scene->mRootNode);
		while (!nodes.empty())
		{
			aiNode* cnode = nodes.front();
			for (int i = 0; i < cnode->mNumChildren; ++i)
				nodes.push(cnode->mChildren[i]);

			for (int i = 0; i < cnode->mNumMeshes; ++i)
				ImportMesh(scene->mMeshes[cnode->mMeshes[i]], output_file);
		}
	}
	aiReleaseImport(scene);
	return ret;
}


bool MeshImporter::Load(const char * exported_file, Mesh* m)
{
	bool ret = true;
	
	string path = App->fs->GetMeshLibraryPath();
	path += exported_file;
	char* data;
	ret = App->fs->Load(path.c_str(), &data) != 0 && m != nullptr;

	if (ret)
	{
		char* cursor = data;
		uint ranges[3];
		uint bytes = sizeof(ranges);
		memcpy(ranges, cursor, bytes);

		m->num_indices = ranges[0];
		m->num_faces = ranges[0] / 3;
		m->num_vertices = ranges[1];
		m->num_uv = ranges[2];

		//Load indices
		cursor += bytes;
		bytes = m->num_indices * sizeof(uint);
		m->indices = new uint(bytes);
		memcpy(m->indices, cursor, bytes);
		glGenBuffers(1, (GLuint*) &(m->id_indices));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * m->num_indices, m->indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//Load vertices
		cursor += bytes;
		bytes = m->num_vertices * 3 * sizeof(float);
		m->vertices = new float(bytes);
		memcpy(m->vertices, cursor, bytes);
		glGenBuffers(1, (GLuint*) &(m->id_vertices));
		glBindBuffer(GL_ARRAY_BUFFER, m->id_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->num_vertices * 3, m->vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Load UVs
		cursor += bytes;
		bytes = m->num_uv * 3 * sizeof(uint);
		m->UVs = new float(bytes); 
		memcpy(m->UVs, cursor, bytes);
		glGenBuffers(1, (GLuint*)&(m->id_uv));
		glBindBuffer(GL_ARRAY_BUFFER, (GLuint)m->id_uv);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->num_uv * 3, m->UVs, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	return ret;
}

bool MeshImporter::ImportMesh(aiMesh * mesh, std::string & output_file)
{
	bool ret = true;
	uint numUVs = 0;
	if (mesh->HasTextureCoords(0)) numUVs = mesh->mNumVertices;
	uint ranges[3] = { mesh->mNumFaces * 3, mesh->mNumVertices,  numUVs };
	uint size = sizeof(ranges) + sizeof(uint)*mesh->mNumFaces * 3 + sizeof(float)* mesh->mNumVertices * 3 * sizeof(float) * numUVs * 3;
	char* data = new char[size];
	char* cursor = data;

	uint bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);

	cursor += bytes;
	bytes = sizeof(uint) * mesh->mNumFaces * 3;
	memcpy(cursor, mesh->mFaces, bytes);

	cursor += bytes;
	bytes = sizeof(float) * mesh->mNumVertices * 3;
	memcpy(cursor, mesh->mVertices, bytes);

	cursor += bytes;
	bytes = sizeof(float) * numUVs * 3;
	memcpy(cursor, mesh->mTextureCoords, bytes);

	ret = App->fs->SaveUnique(App->fs->GetMeshLibraryPath().c_str(), data, output_file.c_str(), "mrse", size, output_file, true);
	delete[] data;

	return ret;

}
