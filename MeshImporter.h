#pragma once
#ifndef __MESHIMPORTER_H__
#define __MESHIMPORTER_H__

#include "Importer.h"

class MeshImporter : Importer {
public:
	MeshImporter();
	~MeshImporter();
public:
	bool Import(const char* path, std::string& output_file);
	bool Import(const void* buffer, uint size, std::string& output_file);
	bool Load(const char* exported_file);
};

#endif 

