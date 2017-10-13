#ifndef __MODULELOADER__H__
#define __MODULELOADER__H__

#include "Module.h"
#include "Globals.h"
#include "ModuleTextures.h"

struct Mesh {
public:
	uint id_vertices = 0; // id in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;
	uint id_indices = 0; // id in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;
	float* UVs = nullptr;
	uint id_uv = 0;
	uint num_uv = 0;
	Texture* texture = nullptr;
public:
	void SetTexture(Texture* tex) {
		texture = tex;
	}
	void Render(uint id);

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
		uint texture = 0;
		std::list<Mesh*>meshes;

};

#endif
