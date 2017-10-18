#ifndef __MODULELOADER__H__
#define __MODULELOADER__H__

#include "Module.h"
#include "Globals.h"
#include "ModuleTextures.h"
#include "Mesh.h"

class ModuleLoader : public Module {

	public:
		ModuleLoader(bool start_enabled = true);
		~ModuleLoader();
		bool Init();
		update_status Update(float dt);
		bool CleanUp();
		void LoadFBX(char* path);
		void UnloadMesh(Mesh* m);
		void SetWire(bool w);
	public:
		uint texture = 0;
		std::list<Mesh*>meshes;
		bool wire = false;

};

#endif
