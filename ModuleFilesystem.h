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

	std::string CreateFolder(const char* path, const char* name);
	bool FSave(const char * path, const char * content, const char * name, const char * extension, int size);
	bool FDelete(const char* file);
	bool FMove(const char* file, const char* destination, bool overwrite);
	bool FCopy(const char* file, const char* destination, bool overwrite);
};

#endif