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

};
class ModuleLoader : public Module {

	public:
		ModuleLoader();
		~ModuleLoader();
		bool Init();
		bool Update();
		bool CleanUp();
		void LoadFBX(char* path);
	public:

		std::list<Mesh*>meshes;

};

#endif
