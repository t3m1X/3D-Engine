#ifndef __MESH__H__
#define __MESH__H__

#include "Globals.h"
#include "Component.h"

class Mesh : public Component {

public:

	Mesh(GameObject* own);
	void Render(uint id)const;
	void SetVertices(uint id, float* vert, uint num);
	void SetIndices(uint id, uint* ind, uint num);
	void SetUV(uint id, float* uvs, uint num);
	void GenerateVertexBuffer();
	void GenerateIndicesBuffer();
	void GenerateUVBuffer();
	void CleanUp();
	void SetWire(bool w);
	void UI_draw();
	~Mesh();

public:
	uint id_vertices = 0; // id in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;
	uint id_indices = 0; // id in VRAM
	uint num_vertices = 0;
	uint num_faces = 0;
	float* vertices = nullptr;
	float* UVs = nullptr;
	uint id_uv = 0;
	uint num_uv = 0;
	bool wire = false;
};



#endif
