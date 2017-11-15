#pragma once
#ifndef __MESHIMPORTER_H__
#define __MESHIMPORTER_H__

#include "Importer.h"
#include "Globals.h"
#include <string>

struct aiMesh;
struct Mesh;

class MeshImporter : Importer {
public:
	MeshImporter();
	~MeshImporter();
public:
	bool Import(const char* path, std::string& output_file); //Imports all meshes on FBX, does not store hierarchy or transforms
	bool ImportMesh(aiMesh* mesh, std::string& output_file);
	bool Load(const char* exported_file, Mesh* m);
};

#endif 

