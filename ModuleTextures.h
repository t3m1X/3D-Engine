#ifndef __MODULETEXTURES__H__
#define __MODULETEXTURES__H__

#include "Module.h"
class JSON_File;

enum TEXTURE_TYPE {
	DIFFUSE = 0,
	_UNKNOWN
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
	void SetId(uint id);
	void SetWidth(int w);
	void Setheight(int h);
	void SetTextureType(TEXTURE_TYPE t);
	TEXTURE_TYPE GetType();
	void SetFormat(const int& f);
	void SetName(const char* n);
	std::string GetPath()const;
	int GetFormat()const;
	const char* GetName()const;
	void Serialize(JSON_File* doc);

public:

	uint id = 0;
	TEXTURE_TYPE type;
	int width = 0;
	int height = 0;
	int format = 0;
	const char* name = nullptr;
	std::string path;
	
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

	int ImportTexture(const char* path, std::string& output_file);
	Texture* LoadDDSTexture(const char* path);
	Texture* LoadToDDS(const char* path, std::string& output_file);
	Texture* LoadToDDS(const char* path);

private:

	bool Import(const char* path, std::string& output_file);
	bool Import(const void* buffer, uint size, std::string& output_file, const char* file_name);
	Texture* Load(const char* exported_file);
	Texture* Load(const char* exported_file, Texture* tex);

private:
	std::vector<Texture*>textures;
	int clamp = clampingTexType_ClampRepeat;
	int interpolation_type = interpolationTexType_Linear;
	char* clamping_str = nullptr;
	char* interpolate_str = nullptr;
public: 
	int state = 0;
	
};



#endif
