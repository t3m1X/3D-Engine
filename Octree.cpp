#include "Octree.h"
#include "ModuleRenderer3D.h"


OctreeNode::OctreeNode(AABB& box, OctreeNode* p)
{
	if (p != nullptr) {
		this->parent = p;
	}

	for (uint i = 0; i < 8; i++) {
		children[i] = nullptr;
	}

	
	this->box = box;
	box_size = this->box.Size();

}

OctreeNode::~OctreeNode()
{
	for (uint i = 0; i < 8; i++) {
		if(children[i]!=nullptr)
		delete children[i];
	}
}

void OctreeNode::Subdivide()
{
	if (children[0]==nullptr) {
		AABB new_box;
		float3 new_lenght = box.HalfSize();
		float3 center = this->box.CenterPoint();

		int child_index = 0;

		for (int x = 0; x < 2; x++)
		{
			for (int y = 0; y < 2; y++)
			{
				for (int z = 0; z < 2; z++)
				{
					float3 min_point(box.minPoint.x + z * new_lenght.x, box.minPoint.y + y * new_lenght.y, box.minPoint.z + x * new_lenght.z);
					float3 max_point(min_point.x + new_lenght.x, min_point.y + new_lenght.y, min_point.z + new_lenght.z);

					new_box.minPoint = min_point;
					new_box.maxPoint = max_point;

					children[child_index] = new OctreeNode(new_box,this);
					children[child_index]->box_size = new_lenght;
					LOG_OUT("New node created with size %f", children[child_index]->box_size.x);
					child_index++;
				}
			}
		}

		this->leaf = true;
		LOG_OUT("Node subdivided");
	}
	else {
		for (int i = 0; i < 8; i++) {
			children[i]->Subdivide();
		}
	}
}

void OctreeNode::DebugDrawNode()
{
	float3 corners[8];
	this->box.GetCornerPoints(corners);
	const int s = 24;

	float3* lines = new float3[s];
	float3* colors = new float3[s];

	lines[0] = float3(corners[0].x, corners[0].y, corners[0].z);
	lines[1] = float3(corners[2].x, corners[2].y, corners[2].z);

	lines[2] = float3(corners[2].x, corners[2].y, corners[2].z);
	lines[3] = float3(corners[6].x, corners[6].y, corners[6].z);

	lines[4] = float3(corners[4].x, corners[4].y, corners[4].z);
	lines[5] = float3(corners[6].x, corners[6].y, corners[6].z);

	lines[6] = float3(corners[4].x, corners[4].y, corners[4].z);
	lines[7] = float3(corners[0].x, corners[0].y, corners[0].z);

	//

	lines[8] = float3(corners[1].x, corners[1].y, corners[1].z);
	lines[9] = float3(corners[3].x, corners[3].y, corners[3].z);

	lines[10] = float3(corners[3].x, corners[3].y, corners[3].z);
	lines[11] = float3(corners[7].x, corners[7].y, corners[7].z);

	lines[12] = float3(corners[5].x, corners[5].y, corners[5].z);
	lines[13] = float3(corners[7].x, corners[7].y, corners[7].z);

	lines[14] = float3(corners[5].x, corners[5].y, corners[5].z);
	lines[15] = float3(corners[1].x, corners[1].y, corners[1].z);

	//

	lines[16] = float3(corners[0].x, corners[0].y, corners[0].z);
	lines[17] = float3(corners[1].x, corners[1].y, corners[1].z);

	lines[18] = float3(corners[2].x, corners[2].y, corners[2].z);
	lines[19] = float3(corners[3].x, corners[3].y, corners[3].z);

	lines[20] = float3(corners[4].x, corners[4].y, corners[4].z);
	lines[21] = float3(corners[5].x, corners[5].y, corners[5].z);

	lines[22] = float3(corners[6].x, corners[6].y, corners[6].z);
	lines[23] = float3(corners[7].x, corners[7].y, corners[7].z);

	for (int i = 0; i < s; i++)
	{
		colors[i] = float3(60, 60, 60);
	}

	//	DrawLinesList(lines, s, 5, colors);

	glLineWidth((float)5);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, (float*)lines->ptr());

	if (colors != nullptr)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, (float*)colors->ptr());
	}

	glDrawArrays(GL_LINES, 0, s);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glLineWidth(1);

	delete[] lines;
	delete[] colors;
}

void OctreeNode::DebugDraw()
{

	DebugDrawNode();

	if (this->children[0] != nullptr)
	{
		for (int i = 0; i < 8; i++)
		{
			children[i]->DebugDraw();
		}
	}

	
}

bool OctreeNode::IsALeaf()
{
	return leaf;
}

void OctreeNode::AddGO(GameObject * go)
{
	if (children[0] == nullptr)
	{
		if (!IsFull())
		{
			objects.push_back(go);
		}
		else
		{
			Subdivide();
		}
	}

	if (children[0] != nullptr)
	{
		for (int i = 0; i < 8; i++)
		{
			if (children[i]->box.Intersects(go->boundingbox))
			{
				children[i]->AddGO(go);
				break;
			}
		}
	}
}

bool OctreeNode::IsFull()
{
	return objects.size() == capacity;
}

void OctreeNode::CollectIntersections(std::list<GameObject*>& intersections_list, GameObject * go)
{
	if (children[0] != nullptr)
	{
		for (int i = 0; i < 8; i++)
		{
			if (children[i]->box.Intersects(go->boundingbox))
			{
				children[i]->CollectIntersections(intersections_list, go);
			}
		}
	}

	for (uint i = 0; i < objects.size(); i++)
	{
		if ((objects[i]->boundingbox.Intersects(go->boundingbox)))
		{
			intersections_list.push_back(objects[i]);
		}
	}
}

Octree::Octree()
{
	depth = 0;
}

Octree::~Octree()
{
	if (root != nullptr) {
		delete root;
	}
}

void Octree::Create(float3 max_point, float3 min_point)
{
	AABB box(min_point, max_point);
	root = new OctreeNode(box);
}

void Octree::DebugDraw()
{
	if(root!=nullptr)
	root->DebugDraw();
}

void Octree::Divide()
{
	
		if (!root->IsALeaf()) {
			root->Subdivide();
		}
		else {
			for (int i = 0; i < 8; i++) {
				root->children[i]->Subdivide();
			}
		
		
	}
}

void Octree::CollectIntersections(std::list<GameObject*>& intersections_list, GameObject * go)
{
	if (root != nullptr)
	{
		if (go->boundingbox.Intersects(root->box))
		{
			root->CollectIntersections(intersections_list, go);
		}
	}
}

void Octree::InsertGO(GameObject * go)
{
	if (root != nullptr) {
		root->AddGO(go);
	}
}
