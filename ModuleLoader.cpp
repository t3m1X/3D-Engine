#include "ModuleLoader.h"
#include "glew\include\GL\glew.h"
#include "Application.h"
#include "Console.h"
#include "GameObject.h"
#include "ModuleSceneIntro.h"
#include "ModuleImGui.h"
#include "ModuleCamera3D.h"
#include "Mesh.h"
#include "Transform.h"
#include "Material.h"
#include <queue>
#include "ModuleFileSystem.h"





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
	LOG_OUT("All meshes deleted");
	return true;
}

void ModuleLoader::LoadFBX(const char* path)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	LOG_OUT("FBX Load: Loading %s", path);
	if (scene->mRootNode != nullptr) {
		std::queue<pair<aiNode*, GameObject*>> nodes;
		nodes.push({ scene->mRootNode, nullptr });
		while (!nodes.empty())
		{
			pair<aiNode*, GameObject*> cnode = nodes.front();
			LOG_OUT("FBX Load: Loading node with %d meshes", cnode.first->mNumMeshes);
			//Creating new gameobject
			GameObject* new_obj = new GameObject(cnode.first->mName.C_Str(), cnode.second);
			new_obj->SetStatic(false);

			App->scene_intro->all_objects.push_back(new_obj);
			App->scene_intro->non_static_objects.push_back(new_obj);
			if (cnode.first == scene->mRootNode) {
				
				new_obj->SetStatic(false);
				App->scene_intro->AddObject(new_obj);
				
				
				
			}

			//Adding childs to queue
			LOG_OUT("FBX Load: Queueing %d child nodes", cnode.first->mNumChildren);
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
					LOG_OUT("Loading transform");
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


				/////Load Mesh

				std::string mesh_path;
				Mesh* new_mesh = nullptr;
				new_mesh = Loadrmesh(scene->mMeshes[cnode.first->mMeshes[i]], mesh_path);
				if (new_mesh != nullptr) {
					new_mesh->SetPath(mesh_path.c_str());
					new_mesh->SetOwner(new_obj);
					new_obj->AddComponent(new_mesh);
					
					LOG_OUT("Mesh no es null");
				}

				
				
				
			
				new_obj->RecalculateAABB();
				new_obj->Enable();
				
				
					
				
				LOG_OUT("Created new object");

				

				aiMaterial** materials = nullptr;
				if (scene->HasMaterials())
					materials = scene->mMaterials;
				else
					LOG_OUT("Scene without materials");

				if (materials != nullptr && new_mesh != nullptr) {

					Material* new_material = LoadMaterial(materials[(int)scene->mMeshes[cnode.first->mMeshes[i]]->mMaterialIndex]);
					if (new_material != nullptr) {
						new_material->SetOwner(new_obj);
						new_obj->AddComponent(new_material);
					}
				}
			}
			nodes.pop();
		}
		aiReleaseImport(scene);
	}
	else
		LOG_OUT("FBX Load: Error loading scene %s", path);
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

bool ModuleLoader::Import(const aiMesh * m, std::string & output)
{
	bool ret = false;

	if (m == nullptr) {
		LOG_OUT("ERROR Loading mesh -> Mesh is NULLPTR");
		return false;
	}

	Mesh* new_mesh = nullptr;

	new_mesh = new Mesh();
	//Nums
	new_mesh->material_id = m->mMaterialIndex;
	new_mesh->num_faces = m->mNumFaces;
	new_mesh->num_vertices = m->mNumVertices;
	//Vertices
	new_mesh->vertices = new float[new_mesh->num_vertices * 3];
	memcpy(new_mesh->vertices, m->mVertices, sizeof(float) * new_mesh->num_vertices * 3);

	//Indices
	if (m->HasFaces()) {
		new_mesh->num_indices = m->mNumFaces * 3;
		new_mesh->indices = new uint[new_mesh->num_indices];

		for (uint i = 0; i < m->mNumFaces; ++i)
		{
			if (m->mFaces[i].mNumIndices != 3) {
				LOG_OUT("Mesh face with != 3 indices!");
			}
			else
				memcpy(&new_mesh->indices[i * 3], m->mFaces[i].mIndices, 3 * sizeof(uint));
		}

	}
	else {
		LOG_OUT("Mesh without Faces");
	}

	//UVS
	if (m->HasTextureCoords(0))
	{
		new_mesh->num_uv = m->mNumVertices;
		new_mesh->UVs = new float[new_mesh->num_uv * 3];
		memcpy(new_mesh->UVs, m->mTextureCoords[0], sizeof(float)*new_mesh->num_uv * 3);

	}
	else
	{
		LOG_OUT("No Texture Coords found");
	}

	//Name
	new_mesh->Setname(m->mName.C_Str());
	if (new_mesh->GetName() == " ") {
		new_mesh->Setname("Unamed");
	}
	new_mesh->SetPath(output.c_str());

	if (new_mesh != nullptr) {
		ret = Save(new_mesh, output);
	}

	if (new_mesh != nullptr) {
		delete new_mesh;
	}

	return ret;
}

bool ModuleLoader::Save(Mesh * m, std::string & output)
{
	bool ret = false;

	int bufferId[3] = { 
		m->num_vertices,
		m->num_indices,
		m->num_uv
	};

	int Size = 0;	
	Size += sizeof(bufferId);
	Size += sizeof(float) * m->num_vertices * 3;
	Size += sizeof(unsigned int) * m->num_indices;
	Size += sizeof(float) * m->num_uv * 3;

	char* buffer = new char[Size]; // Creating the Buffer
	char* it = buffer;
	std::memcpy(buffer, bufferId, sizeof(bufferId)); // Allocating Indexes
	it += sizeof(bufferId);
	std::memcpy(it, m->vertices, sizeof(float) * m->num_vertices * 3); // Allocating vertices
	it += sizeof(float) * m->num_vertices * 3;
	std::memcpy(it, m->indices, sizeof(unsigned int) * m->num_indices); // Allocating indices
	if (m->num_uv > 0) {
		it += sizeof(unsigned int) * m->num_indices;
		std::memcpy(it, m->UVs, sizeof(float) * m->num_uv * 3); // Allocating UVs
	}

	ret = App->fs->SaveUnique(LIBRARY_MESHES, buffer, m->GetName(), MESH_FORMAT, Size, output, true);

	if (buffer != nullptr) {
		delete[] buffer;
	}
	else {
		LOG_OUT("Buffer es nullptr");
	}

	return ret;
}

Mesh * ModuleLoader::Loadrmesh(const char * file)
{
	Mesh* new_mesh = nullptr;

	if (file == nullptr) {
		LOG_OUT("Unvalid Path");
		return false;
	}
	const char* str = GetCExtension(file);
	if (std::strcmp(GetCExtension(file), MESH_FORMAT) == false) {
		LOG_OUT("ERROR not '%s' extension file", MESH_FORMAT);
		return false;
	}

	char* buffer = nullptr;
	int Size = App->fs->Load(file, &buffer);
	if (buffer != nullptr) {
		if (Size > 0) {
			// [FORMAT] nV nI nUV - Vs Is UVs
			new_mesh = new Mesh();
			memcpy(&new_mesh->num_vertices, buffer, sizeof(uint));
			memcpy(&new_mesh->num_indices, buffer + sizeof(uint), sizeof(uint));
			memcpy(&new_mesh->num_uv, buffer + 2 * sizeof(uint), sizeof(uint));
			char* it = buffer;
			// Vertices
			it = buffer + sizeof(uint) * 3; // it = buffer[Vs]
			new_mesh->vertices = new float[new_mesh->num_vertices * 3];
			memcpy(new_mesh->vertices, it, sizeof(float) * new_mesh->num_vertices * 3);
			glGenBuffers(1, (GLuint*)&(new_mesh->id_vertices));
			glBindBuffer(GL_ARRAY_BUFFER, new_mesh->id_vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * new_mesh->num_vertices * 3, new_mesh->vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// Indices
			it += sizeof(float) * new_mesh->num_vertices * 3; // it = buffer[Is]
			new_mesh->indices = new uint[new_mesh->num_indices];
			memcpy(new_mesh->indices, it, sizeof(uint) * new_mesh->num_indices);
			glGenBuffers(1, (GLuint*) &(new_mesh->id_indices));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->id_indices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * new_mesh->num_indices, new_mesh->indices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			// UVs
			if (new_mesh->num_uv > 0) {
				it += sizeof(uint) * new_mesh->num_indices; // it = buffer[UVs]
				new_mesh->UVs = new float[new_mesh->num_uv * 3];
				memcpy(new_mesh->UVs, it, sizeof(float) * new_mesh->num_uv * 3);

				glGenBuffers(1, (GLuint*)&(new_mesh->id_uv));
				glBindBuffer(GL_ARRAY_BUFFER, (GLuint)new_mesh->id_uv);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * new_mesh->num_uv * 3, new_mesh->UVs, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			else
				LOG_OUT("Mesh without Uvs");
			// Name
			new_mesh->Setname(GetFile(file, false).c_str());
			new_mesh->Setfbxpath(file);

			if (new_mesh != nullptr) {
				LOG_OUT("Loaded mesh with %d vertices %d indices %d UVs", new_mesh->num_vertices, new_mesh->num_indices, new_mesh->num_uv);
			}
		}
	}

	return new_mesh;
}

Mesh * ModuleLoader::Loadrmesh(const aiMesh * m)
{
	Mesh* new_mesh = nullptr;

	std::string output_file;
	if (Import(m, output_file)) {
		new_mesh = Loadrmesh(output_file.c_str());
		if (new_mesh == nullptr)
			LOG_OUT("ERROR Loading Mesh from '%s' format", MESH_FORMAT);
	}
	else {
		LOG_OUT("ERROR Importing Mesh to '%s' format", MESH_FORMAT);
	}

	if (new_mesh != nullptr) {
		new_mesh->path = output_file;
		meshes.push_back(new_mesh);
	}

	return new_mesh;
}

Mesh * ModuleLoader::Loadrmesh(const aiMesh * m, std::string & output)
{
	Mesh* new_mesh = nullptr;

	if (Import(m, output)) {
		new_mesh = Loadrmesh(output.c_str());
		if (new_mesh == nullptr)
			LOG_OUT("ERROR Loading Mesh from '%s' format", MESH_FORMAT);
	}
	else {
		LOG_OUT("ERROR Importing Mesh to '%s' format", MESH_FORMAT);
	}

	if (new_mesh != nullptr) {
		new_mesh->path = output;
		meshes.push_back(new_mesh);
	}

	return new_mesh;
}

Mesh * ModuleLoader::Loadrmeshfile(const char * buffer, const char * filename)
{
	Mesh* rmesh = nullptr;

	std::string out;

	if (App->fs->SaveUnique(LIBRARY_MESHES, buffer, filename, MESH_FORMAT, strlen(buffer) * sizeof(char), out, true))
		rmesh = Loadrmesh(out.c_str());

	return rmesh;
}

Mesh * ModuleLoader::LoadaiMesh(const aiMesh * m)
{
	Mesh* new_mesh = nullptr;

	new_mesh = new Mesh();
	new_mesh->material_id = m->mMaterialIndex;
	new_mesh->num_faces = m->mNumFaces;
	new_mesh->num_vertices = m->mNumVertices;
	//Vertices
	new_mesh->vertices = new float[new_mesh->num_vertices * 3];
	memcpy(new_mesh->vertices, m->mVertices, sizeof(float) * new_mesh->num_vertices * 3);
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
				LOG_OUT("Mesh face with != 3 indices!");
			}
			else
				memcpy(&new_mesh->indices[i * 3], m->mFaces[i].mIndices, 3 * sizeof(uint));
		}

	}
	else {
		LOG_OUT("Mesh with no Faces");
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
		glBindBuffer(GL_ARRAY_BUFFER, (GLuint)new_mesh->id_uv);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * new_mesh->num_uv * 3, new_mesh->UVs, GL_STATIC_DRAW);
	}
	else
	{
		LOG_OUT("No Texture Coords found");
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Name
	new_mesh->Setname(m->mName.C_Str());

	LOG_OUT("Loaded mesh with %d vertices %d indices %d UVs", new_mesh->num_vertices, new_mesh->num_indices, new_mesh->num_uv);

	meshes.push_back(new_mesh);

	return new_mesh;
}

Material * ModuleLoader::LoadMaterial(const aiMaterial * mat)
{
	Material* new_material = nullptr;
	new_material = new Material();

	

	// Diffuse
	if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
		for (uint i = 0; i < mat->GetTextureCount(aiTextureType_DIFFUSE); i++) {
			aiString m_path;
			mat->GetTexture(aiTextureType_DIFFUSE, i, &m_path);
			if (m_path.length > 0) {
				string fullPath = "Assets\\";
				fullPath.append(m_path.C_Str());
				std::string tex_path;
				Texture* tmp_tex;
				tmp_tex = App->tex->LoadToDDS(fullPath.c_str(), tex_path);
				if (tmp_tex != nullptr) {
					tmp_tex->path = tex_path.c_str();
					tmp_tex->SetTextureType(DIFFUSE);
					if (new_material != nullptr) {
						new_material->AddTexture(tmp_tex);
						return new_material;
					}

				}
				
			}
			else {
				LOG_OUT("Unvalid Path from texture: %s", m_path.C_Str());
			}
		}
	}
	else {
		LOG_OUT("No Diffuse found");
	}
	//All other texture types...

	
		if (new_material != nullptr) {
			new_material->CleanUp();
			delete new_material;
			new_material = nullptr;
		}
	

	return new_material;
}







