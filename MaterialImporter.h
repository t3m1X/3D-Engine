#pragma once
#ifndef __MATERIALIMPORTER_H__
#define __MATERIALIMPORTER_H__

#include "Importer.h"
#include "ModuleTextures.h"

class MaterialImporter : public Importer {
public:
	MaterialImporter();
	~MaterialImporter();
public:
	bool Import(const char* path, std::string& output_file);
	bool Import(const void* buffer, uint size, std::string& output_file);
	bool Load(const char* exported_file);
	bool LoadCheckers(Texture* resource);
};

#endif 
