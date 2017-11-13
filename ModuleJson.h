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
	//Secure method of GetString, if string with name "name" does not exist will return fallback
	const char* GetString(const char* name, const char* fallback);

	void SetBool(const char* name, bool b);
	bool GetBool(const char* name);
	//Secure method of GetBool, if bool with name "name" does not exist will return fallback
	bool GetBool(const char* name, bool fallback); 

	void SetNumber(const char* name, double num);
	double GetNumber(const char* name);
	//Secure method of GetNumber, if number with name "name" does not exist will return fallback
	double GetNumber(const char* name, double fallback);

	void AddObject(const char* name);
	void RemoveObject(const char* name);
	//Makes current object the object with name "name"
	bool ChangeObject(const char* name);
	//Makes current object the object inside array with index
	bool ChangeObjectFromArray(const char* array, unsigned int index);
	//Returns to root object of the file
	void RootObject();

	void AddArray(const char* name);
	void ClearArray(const char* name);
	//Will return -1 if array does not exist
	int ArraySize(const char* array);
	void ArrayAddString(const char* array, const char* str);
	void ArrayAddBool(const char* array, bool b);
	void ArrayAddNumber(const char* array, double num);
	void ArrayAddObject(const char* array);

	//Will return fallback if array does not exist
	const char* ArrayGetString(const char* array, unsigned int index, const char* fallback = "Invalid");
	//Will return fallback if array does not exist or bool not in index
	bool ArrayGetBool(const char* array, unsigned int index, bool fallback = false);
	//Will return fallback if array does not exist
	double ArrayGetNumber(const char* array, unsigned int index, double fallback = 0);
	

	const char* GetPath();
	void Save();
	void CleanUp();


private:
	JSON_Value*  val = nullptr;
	JSON_Object* obj = nullptr;
	JSON_Object* root = nullptr;
	const char*  path;
};

class ModuleJSON : public Module
{
public:
	ModuleJSON(bool start_enabled = true);
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
