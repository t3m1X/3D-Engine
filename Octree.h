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

	OctreeNode* children[8];

private:

	
	std::vector<GameObject*> objects;
	AABB box;
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


private:
	int depth=0; //max num of subdivisions
	OctreeNode* root;
};






#endif