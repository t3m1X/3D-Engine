#ifndef __OCTREE__H__
#define __OCTREE__H__

#include "GameObject.h"

class OctreeNode {
public:

	OctreeNode(AABB& box, OctreeNode* p = nullptr);
	virtual ~OctreeNode();
	void Subdivide();
	void DebugDrawNode();
	void DebugDraw();
	bool IsALeaf();
	void AddGO(GameObject* go);
	void EraseInNode(GameObject* go);
	bool IsFull();
	void ClearNode();

	void CollectIntersections(std::list<GameObject*>& intersections_list, GameObject* go);

public:
	OctreeNode* children[8];
	AABB box;
private:

	int capacity = 1;
	std::list<GameObject*> objects;
	OctreeNode* parent;
	bool leaf = false;
	float3 box_size;
	

	

};

class Octree {
public:

	Octree();
	~Octree();
	void Create(float3 max_point,float3 min_point);
	void DebugDraw();
	void Divide();
	void CollectIntersections(std::list<GameObject*>& intersections_list, GameObject* go);
	void InsertGO(GameObject* go);
	void EraseGO(GameObject* go);

private:
	int depth=0; //max num of subdivisions
	OctreeNode* root;
	bool need_update;
};






#endif