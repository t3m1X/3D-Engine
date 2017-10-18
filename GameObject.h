#ifndef __GameObject_H__
#define __GameObject_H__

#include <iostream>
#include <vector>
#include "ModuleTextures.h"
#include "glmath.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "Component.h"

struct Texture;
struct Mesh;

class GameObject
{
public:
	GameObject(int _id, std::list< Mesh*>& m);
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
	void AddComponent(COMPONENT_TYPE t);
	void RemoveComponent(Component* c);
	/////////////////////
	void SetTexture(Texture* tex);
	void SetPosition(float3 pos);
	void SetScale(float3 scale);
	void SetRotation(float3 rot);
	void SetTransform(const float4x4& matrix);
	/////////////////////

	const int GetId() const;
	const bool GetSelected() const;

	/////////////////
	const float3 GetPosition()const;
	const float3 GetRotation()const;
	const float3 GetScale()const;



public:
	
	OBB boundingbox;

	//////////////////
	Texture*    texture = nullptr;
	uint		tris = 0;
	uint        vertices = 0;
	std::list<Mesh*>mesh;
	//////////////////////
private:

	std::string name;
	bool		enabled = true;
	bool		selected = false;
	std::vector<Component*> components;
	std::vector<GameObject*> children;
	GameObject* parent;






	///////////////////////
	float4x4    transform;
	float3        position;
	float3        scale;
	float3        rotation;
	uint		tex = 0;
	////////////////////////
};

#endif
