#ifndef __MODULEJSON__H__
#define __MODULEJSON__H__

#include "Module.h"
#include "Parson\parson.h"
#include "Application.h"



class JSON_File
{
public:
	JSON_File(JSON_Value* value, JSON_Object* object, const char* path);
	~JSON_File();

	void SetString(const char* name, const char* str);
	const char* GetString(const char* name);

	void SetBool(const char* name, bool b);
	bool GetBool(const char* b);

	void SetNumber(const char* name, double num);
	double GetNumber(const char* name);

	const char* GetPath();
	void Save();
	void CleanUp();


private:
	JSON_Value*  val = nullptr;
	JSON_Object* obj = nullptr;
	const char*  path;
};

class ModuleJSON : public Module
{
public:
	ModuleJSON(Application* app, bool start_enabled = true);
	~ModuleJSON();

	bool Awake();
	bool CleanUp();
	void ImGuiDraw();

	JSON_File* LoadJSON(const char* path);
	JSON_File* CreateJSON(const char* path);

private:
	std::list<JSON_File*> files;
};





#endif
