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
class Component;
enum COMPONENT_TYPE;

class GameObject
{
public:

	GameObject(std::string name, GameObject* _parent = nullptr);
	GameObject(std::string name, uint id, GameObject* _parent = nullptr);
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
	//void AddEmptyComponent(COMPONENT_TYPE t);
	void AddComponent(Component* c);
	void AddChild(GameObject* child);
	//void RemoveComponent(Component* c);
	bool HasComponent(COMPONENT_TYPE type);
	Component* FindComponentbyType(COMPONENT_TYPE type);
	vector<Component*> GetComponents() const;
	void UIDraw();
	void DrawComponents();
	void RecalculateAABB();
	void RecalculateAABB(float4x4 transformation);
	vector<GameObject*> GetChild() const;
	GameObject* GetParent() const;
	void DrawBox();
	void OnGuizmo();
	
		
public:
	
	AABB boundingbox;
	std::vector<GameObject*> children;
private:

	std::string name;
	bool		enabled = true;
	bool		selected = false;
	bool		bbinit = false;
	std::vector<Component*> components;
	
	GameObject* parent;	

};

#endif
