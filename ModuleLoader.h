#ifndef __MODULELOADER__H__
#define __MODULELOADER__H__

#include "Module.h"
#include "Globals.h"
#include "ModuleTextures.h"
#include "Mesh.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "Material.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

#define MESH_FORMAT "rmesh"




class ModuleLoader : public Module {

	public:
		ModuleLoader(bool start_enabled = true);
		~ModuleLoader();
		bool Init();
		update_status Update(float dt);
		bool CleanUp();
		void LoadFBX(const char* path);
		void UnloadMesh(Mesh* m);
		void SetWire(bool w);

		bool Import(const aiMesh* m, std::string& output);
		bool Save(Mesh*m, std::string& output);
		Mesh* Loadrmesh(const char*file);
		Mesh* Loadrmesh(const aiMesh*m);
		Mesh* Loadrmesh(const aiMesh* m, std::string& output);
		Mesh* Loadrmeshfile(const char* buffer, const char* filename);
		Mesh*LoadaiMesh(const aiMesh*m);
		Material* LoadMaterial(const aiMaterial* mat);
		

		
	public:
		uint texture = 0;
		std::list<Mesh*>meshes;
		bool wire = false;
		uint obj_count = 0;

};

#endif
