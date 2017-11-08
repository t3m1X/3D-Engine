#include "ModuleFilesystem.h"
#include "Application.h"
#include <Windows.h>
#include <fstream>
#include "mmgr\mmgr.h"

void ReplaceCharacter(string& str, char old_c, char new_c)
{
	for (string::iterator it = str.begin(); it != str.end(); ++it)
		if (*it == old_c) *it = new_c;
}

FileSystem::FileSystem(bool start_enabled) : Module(start_enabled)
{
	SetName("FileSystem");

	base_path = SDL_GetBasePath();
	settings_path = CreateFolder(base_path.c_str(), "Settings");
	assets_path = CreateFolder(base_path.c_str(), "Assets");
	library_path = CreateFolder(base_path.c_str(), "Library");
	mesh_library_path = CreateFolder(library_path.c_str(), "Meshes");
	texture_library_path = CreateFolder(library_path.c_str() , "Textures");
}

FileSystem::~FileSystem()
{
}

bool FileSystem::Init()
{
	bool ret = true;

	return ret;
}

update_status FileSystem::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool FileSystem::CleanUp()
{
	bool ret = true;

	return ret;
}

string FileSystem::GetSettingsPath() const
{
	return settings_path;
}

string FileSystem::GetAssetsPath() const
{
	return assets_path;
}

string FileSystem::GetLibraryPath() const
{
	return library_path;
}

string FileSystem::GetMeshLibraryPath() const
{
	return mesh_library_path;
}

string FileSystem::GetTextureLibraryPath() const
{
	return texture_library_path;
}


string FileSystem::GetBasePath() const
{
	return base_path;
}

string FileSystem::CreateFolder(const char * path, const char * name)
{
	string ret;

	string filepath = path;
	ReplaceCharacter(filepath, '\\', '/');

	if (filepath[filepath.length() - 1] != '/')
		filepath += '/';

	filepath += name;

	DWORD error = GetLastError();

	if (CreateDirectory(filepath.c_str(), NULL) == 0)
	{
		error = GetLastError();
	}

	if (error == ERROR_PATH_NOT_FOUND)
	{
		LOG("Error creating folder (path not found): %s", path);
		return ret.c_str();
	}
	else if (error == ERROR_ALREADY_EXISTS)
	{
		LOG("Error creating folder (Folder aleady exists): %s", filepath.c_str())
	}

	ret = filepath + '/';

	return (ret.c_str());
}

bool FileSystem::FMove(const char* filepath, const char* destination, bool overwrite)
{
	bool ret = true;
	string new_filepath = destination;
	ReplaceCharacter(new_filepath, '\\', '/');

	if (new_filepath[new_filepath.length() - 1] != '/')
		new_filepath += '/';

	string _filepath = filepath;
	ReplaceCharacter(_filepath, '\\', '/');
	if (_filepath[_filepath.length() - 1] == '/')
		_filepath.pop_back();

	new_filepath += _filepath.substr(_filepath.find_last_of('/') + 1);

	if (!overwrite){
		if (MoveFile(filepath, new_filepath.c_str()))
		{
			LOG("Error moving file: %s to %s", filepath, new_filepath.c_str());
			ret = false;
		}
	}
	else if (MoveFileEx(filepath, new_filepath.c_str(), MOVEFILE_REPLACE_EXISTING))
	{
		LOG("Error moving file: %s to %s", filepath, new_filepath.c_str());
		ret = false;
	}

	return ret;
}

bool FileSystem::FCopy(const char* filepath, const char* destination, bool overwrite)
{
	bool ret = true;
	string new_filepath = destination;
	ReplaceCharacter(new_filepath, '\\', '/');

	if (new_filepath[new_filepath.length() - 1] != '/')
		new_filepath += '/';

	string _filepath = filepath;
	ReplaceCharacter(_filepath, '\\', '/');
	if (_filepath[_filepath.length() - 1] == '/')
		_filepath.pop_back();

	new_filepath += _filepath.substr(_filepath.find_last_of('/') + 1);

	if (CopyFile(destination, new_filepath.c_str(), !overwrite))
	{
		LOG("Error moving file: %s to %s", destination, new_filepath.c_str());
		ret = false;
	}

	return ret;
}

bool FileSystem::FDelete(const char * filepath)
{
	bool ret = true;
	if (DeleteFile(filepath) == 0)
	{
		ret = false;
		DWORD error = GetLastError();

		if (error == ERROR_FILE_NOT_FOUND)
		{
			LOG("Error deleting file (path not found)): %s", filepath);
		}
	}

	return ret;
}

bool FileSystem::FSave(const char * path, const char* file_content, const char* name, const char* extension, int size)
{
	bool ret = false;

	string file = path;
	file += name;
	file += ".";
	file += extension;

	std::ofstream;
	FILE* new_file = fopen(file.c_str(), "wb");

	if (new_file)
	{
		fwrite(file_content, sizeof(char), size, new_file);
		ret = true;
	}
	else
	{
		LOG("Error saving file %s: ", name);
	}

	fclose(new_file);

	return ret;
}

vector<string> FileSystem::ListFiles(const char * path, const char* extension) const
{
	vector<string> files;

	WIN32_FIND_DATA search_data;

	string path_ex = path;
	path_ex += "*.";
	path_ex += extension;

	HANDLE handle = FindFirstFile(path_ex.c_str(), &search_data);

	while (handle != INVALID_HANDLE_VALUE)
	{
		string path_new = path;
		path_new += search_data.cFileName;
		files.push_back(path_new);

		if (FindNextFile(handle, &search_data) == FALSE)
			break;
	}

	FindClose(handle);

	return files;
}

uint FileSystem::Load(const char * path, char ** buffer)
{
	uint ret = 0;

	std::ofstream;
	FILE* load_file = fopen(path, "rb");

	if (load_file) {

		int size = 0;

		std::ifstream in(path, std::ifstream::ate | std::ifstream::binary);
		size = in.tellg();

		if (size > 0) {
			*buffer = new char[size];
			uint readed = (uint)fread(*buffer, sizeof(char), size, load_file);
			if (readed != size) {
				LOG("ERROR while reading file:\n\t%s", path);
				if (*buffer != nullptr)
					delete[] buffer;
			}
			else {
				ret = readed;
			}
		}
		if (fclose(load_file) != 0) {
			LOG("ERROR while closing file:\n\t%s", path);
		}
	}
	else {
		LOG("ERROR while opening file:\n\t%s\n\t%s", path, strerror(errno));
	}

	return ret;
}

bool FileSystem::SaveUnique(const char * path, const char * file_content, const char * name, const char * extension, int size, std::string & output_file, bool gen_uid)
{
	bool ret = false;

	uint uniqueID = 0;
	string file;

	if (gen_uid == true) {
		do {
			file = path;
			file += name;
			file += "_";
			file += std::to_string(uniqueID++);
			file += ".";
			file += extension;
		} while (exists(file));
	}
	else {
		file = path;
		file += name;
		file += ".";
		file += extension;
	}

	std::ofstream;
	FILE* new_file = fopen(file.c_str(), "wb");

	if (new_file)
	{
		fwrite(file_content, sizeof(char), size, new_file);
		ret = true;
	}
	else
	{
		LOG("ERROR saving unique file %s: ", name);
	}

	fclose(new_file);
	return ret;
}

std::string FileSystem::GetFileFromPath(const char * path)
{
	std::string ret;

	for (int i = 0; path[i] != '\0'; i++) {
		if (path[i] == '\\' || path[i] == '/') {
			ret.clear();
			continue;
		}
		ret += path[i];
	}

	return ret;
}
