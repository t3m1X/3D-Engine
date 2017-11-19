#pragma once
#ifndef __ModuleImporter_H__
#define __ModuleImporter_H__

#include "Module.h"

struct aiScene;

class ModuleImporter : public Module {

public:
	ModuleImporter(bool start_enabled = true);
	~ModuleImporter();
	bool Init();
	update_status Update(float dt);
	bool CleanUp();
	void Load(char* path);
	void LoadScene(aiScene* scene);
	void CheckAssets();

};

#endif
