#ifndef __MODULELOADER__H__
#define __MODULELOADER__H__

#include "Module.h"
#include "Globals.h"

struct Mesh {

	uint id_vertices = 0; // id in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;
	uint id_indices = 0; // id in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;
	float* UVs = nullptr;
	uint id_uv = 0;
	uint num_uv = 0;

};
class ModuleLoader : public Module {

	public:
		ModuleLoader(Application* app, bool start_enabled = true);
		~ModuleLoader();
		bool Init();
		update_status Update(float dt);
		bool CleanUp();
		void LoadFBX(char* path);
	public:

		std::list<Mesh*>meshes;

};

#endif
