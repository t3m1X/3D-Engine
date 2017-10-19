#ifndef __MODULETEXTURES__H__
#define __MODULETEXTURES__H__

#include "Module.h"

enum TEXTURE_TYPE {
	DIFFUSE = 0,
	UNKNOWN
};

struct Texture {

public:
	Texture();
	virtual ~Texture();
	int Getid();
	int Getwidth();
	int Getheight();
	void SetId(uint id);
	void SetWidth(int w);
	void Setheight(int h);
	void SetTextureType(TEXTURE_TYPE t);
	TEXTURE_TYPE GetType();

private:
	uint id = 0;
	TEXTURE_TYPE type;
	int width = 0;
	int height = 0;
};


class ModuleTextures : public Module {
public:
	ModuleTextures(bool start_enabled = true);
	~ModuleTextures();

	bool Init(JSON_File* conf);
	bool Start();
	update_status Update();
	bool CleanUp();
	Texture* GetTexture();
	void Clear();
	bool Empty();

	Texture* LoadTexture(const char* path);

private:
	std::list<Texture*>textures;
};



#endif
