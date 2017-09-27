#include "ModuleJSON.h"
#include "Globals.h"
#include <stdio.h>
#include "imgui.h"

ModuleJSON::ModuleJSON(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	SetName("JSON");
}

ModuleJSON::~ModuleJSON()
{
}

bool ModuleJSON::Awake()
{
	bool ret = true;

	LOG("Loading JSON Module");

	return ret;
}

JSON_File* ModuleJSON::LoadJSON(const char * path)
{
	JSON_File* file = nullptr;

	bool exists = false;

	for (list<JSON_File*>::iterator it = files.begin(); it != files.end(); it++)
	{
		if (strcmp(path, (*it)->GetPath()))
		{
			file = (*it);
			exists = true;
			break;
		}
	}

	if (!exists)
	{
		JSON_Value *user_data = json_parse_file(path);
		JSON_Object *root_object = json_value_get_object(user_data);

		if (user_data == nullptr)
		{
			LOG("Error loading %s", path);
		}
		else
		{
			LOG("Succes loading %s", path);

			JSON_File* new_doc = new JSON_File(user_data, root_object, path);
			files.push_back(new_doc);

			file = new_doc;
		}
	}

	return file;
}


JSON_File* ModuleJSON::CreateJSON(const char * path)
{
	JSON_File* ret = nullptr;

	bool exists = false;
	for (list<JSON_File*>::iterator it = files.begin(); it != files.end(); it++)
	{
		if (strcmp(path, (*it)->GetPath()))
		{
			exists = true;
			break;
		}
	}

	if (exists)
	{
		LOG("Error creating %s. There is already a file with this path/name", path);
	}
	else
	{
		JSON_Value* root_value = json_value_init_object();

		if (root_value == nullptr)
		{
			LOG("Error creating %s. Wrong path?", path);
		}
		else
		{

			JSON_Object* root_object = json_value_get_object(root_value);

			JSON_File* new_doc = new JSON_File(root_value, root_object, path);
			files.push_back(new_doc);

			ret = new_doc;
		}
	}

	return ret;
}

bool ModuleJSON::CleanUp()
{
	bool ret = true;

	LOG("Unloading JSON Module");

	for (list<JSON_File*>::iterator it = files.begin(); it != files.end();)
	{
		(*it)->CleanUp();
		delete (*it);

		it = files.erase(it);
	}

	return ret;
}

void ModuleJSON::ImGuiDraw()
{
	if (ImGui::CollapsingHeader(this->GetName())) {

	}
}

JSON_File::JSON_File(JSON_Value * _value, JSON_Object * _object, const char* _path)
{
	val = _value;
	obj = _object;
	this->path = _path;
}

JSON_File::~JSON_File()
{
}

void JSON_File::SetString(const char * name, const char * str)
{
	json_object_dotset_string(obj, name, str);
}

void JSON_File::SetBool(const char * name, bool b)
{
	json_object_dotset_boolean(obj, name, b);
}

void JSON_File::SetNumber(const char * name, double n)
{
	json_object_dotset_number(obj, name, n);
}

const char * JSON_File::GetString(const char * name)
{
	return json_object_dotget_string(obj, name);
}

bool JSON_File::GetBool(const char * name)
{
	return json_object_dotget_boolean(obj, name);
}

double JSON_File::GetNumber(const char * name)
{
	return json_object_dotget_number(obj, name);
}

const char * JSON_File::GetPath()
{
	return path;
}

void JSON_File::Save()
{
	json_serialize_to_file_pretty(val, path);
}

void JSON_File::CleanUp()
{
	json_value_free(val);
}


