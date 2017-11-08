#ifndef __MODULETEXTURES__H__
#define __MODULETEXTURES__H__

#include "Module.h"
#define DEFAULT_TEXTURE_NAME "__default_texture_name__"

enum TEXTURE_TYPE {
	DIFFUSE = 0,
	UNKNOWN
};
enum clampingTexType {
	clampingTexType_ClampToEdge = 0,
	clampingTexType_ClampRepeat,
	clampingTexType_ClampMirroredRepeat
};
enum interpolationTexType {
	interpolationTexType_Nearest = 0,
	interpolationTexType_Linear
};

struct Texture {

public:
	Texture();
	virtual ~Texture();
	int Getid();
	int Getwidth();
	int Getheight();
	const char* GetPath()const;
	const char*GetName()const;
	int GetFormat()const;

	void SetId(uint id);
	void SetWidth(int w);
	void Setheight(int h);
	void SetTextureType(TEXTURE_TYPE t);
	void SetPath(const char* path);
	void SetName(const char* name);
	void SetFormat(int f);
	TEXTURE_TYPE GetType();

private:
	uint id = 0;
	TEXTURE_TYPE type;
	int width = 0;
	int height = 0;
	const char* path = nullptr;
	const char* name = nullptr;

	int format = 0;
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

public:
	std::list<Texture*>textures;
private:
	int clamp_type = clampingTexType_ClampRepeat;
	int interpolation_type = interpolationTexType_Linear;
	char* clamping_str = nullptr;
	char* interpolate_str = nullptr;
};



#endif
