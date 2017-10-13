#ifndef __MODULETEXTURES__H__
#define __MODULETEXTURES__H__

#include "Module.h"

struct Texture {

public:
	Texture();
	virtual ~Texture();
	int Getid();
	int Getwidth();
	int Getheight();
private:
	int id = 0;
	int width = 0;
	int height = 0;
};

class ModuleTextures : public Module {
public:
	ModuleTextures(Application* app, bool start_enabled = true);
	~ModuleTextures();

	bool Init();
	bool Start();
	update_status Update();
	bool CleanUp();

	bool LoadTexture(const char* path);

private:
	std::list<Texture*>textures;
};



#endif
