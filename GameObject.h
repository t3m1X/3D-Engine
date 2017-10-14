#ifndef __GameObject_H__
#define __GameObject_H__

#include <iostream>
#include <vector>
#include "ModuleTextures.h"
#include "glmath.h"
#include "MathGeoLib\include\MathGeoLib.h"

struct Texture;
struct Mesh;

class GameObject
{
public:
	GameObject(int _id, Mesh* m);
	virtual ~GameObject();

	void Update();
	void Draw();
	

	void Enable();
	void Disable();
	const bool GetEnabled() const;
	void SetEnabled(const bool& set);
	void CleanUp();

	const char* GetName() const;
	void SetName(const std::string& set);
	void SetSelected(const bool& set);
	void SetTexture(Texture* tex);

	const int GetId() const;
	const bool GetSelected() const;
	const vec3 GetPosition();
	const vec3 GetRotation();
	const vec3 GetScale();


public:
	
	Mesh* mesh;
	OBB boundingbox;
	Texture*    texture = nullptr;

private:

	std::string name;
	bool		enabled = true;
	int			id = 0;
	bool		selected = false;
	vec3        position;
	vec3        scale;
	vec3        rotation;
	uint		tex = 0;
};

#endif
