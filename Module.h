#pragma once

#include <list>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "SDL\include\SDL.h"
#include "Globals.h"


using namespace std;

class Application;
struct PhysBody3D;
class JSON_File;

class Module
{
private:
	bool enabled;
	const char * name;
public:

	Module(bool start_enabled = true)
	{}

	virtual ~Module()
	{}

	virtual bool Init(JSON_File* conf)
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate (float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update (float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate (float dt)
	{
		return update_status::UPDATE_CONTINUE;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual void OnCollision(PhysBody3D* body1, PhysBody3D* body2)
	{}
	void SetName(const char* name) { this->name = name; }
	const char * GetName() { return name; };
	virtual void ImGuiDraw() {}
	virtual void Load(JSON_File* config) {}
	virtual void Save(JSON_File* c) {}
};