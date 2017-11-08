#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include "Module.h"

class FileSystem : public Module
{
public:
	FileSystem(bool start_enabled = true);
	~FileSystem();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

	string GetBasePath() const;
	string GetSettingsPath() const;
	string GetAssetsPath() const;
	string GetLibraryPath() const;
	string GetMeshLibraryPath() const;
	string GetTextureLibraryPath() const;

	string CreateFolder(const char* path, const char* name);
	bool FSave(const char * path, const char * content, const char * name, const char * extension, int size);
	bool FDelete(const char* file);
	bool FMove(const char* filepath, const char* destination, bool overwrite);
	bool FCopy(const char* filepath, const char* destination, bool overwrite);

	vector<string> ListFiles(const char * path, const char* extension = "*") const;

	uint Load(const char* path, char** buffer);
	bool SaveUnique(const char* path, const char* file_content, const char* name, const char* extension, int size, std::string& output_file, bool gen_uid = true); 

	string GetFileFromPath(const char * path);
	inline bool exists(const std::string& path) {
		struct stat buffer;
		return (stat(path.c_str(), &buffer) == 0);
	}

public:
	string base_path;
	string settings_path;
	string assets_path;
	string library_path;
	string mesh_library_path;
	string texture_library_path;

};

#endif