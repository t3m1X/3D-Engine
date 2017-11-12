#include "Mesh.h"
#include "glew\include\GL\glew.h"
#include "ModuleImGui.h"
#include "Transform.h"

Mesh::Mesh(GameObject * own) : Component(own)
{
	this->SetType(MESH);
}

void Mesh::Render(uint id)const
{
	if (wire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);

	//Apply UV if exist
	if (num_uv != 0)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, id_uv);
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);
	}

	//glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, (GLuint)id);

	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);



	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//Unbind textures affter rendering
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Mesh::SetVertices(uint id, float * vert, uint num)
{
	id_vertices = id;
	num_vertices = num;

	vertices = new float[num * 3];
	memcpy(vertices, vert, sizeof(float) * num * 3);
}

void Mesh::SetIndices(uint id, uint* ind, uint num)
{
	id_indices = id;
	indices = ind;
	num_indices = num;
}

void Mesh::SetUV(uint id, float * uvs, uint num)
{
	uint id_uv = id;
	UVs = uvs;
	num_uv = num;
}

void Mesh::GenerateVertexBuffer()
{
	glGenBuffers(1, (GLuint*) &(id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::GenerateIndicesBuffer()
{
	glGenBuffers(1, (GLuint*) &(id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * num_indices, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::GenerateUVBuffer()
{
	glGenBuffers(1, (GLuint*)&(id_uv));
	glBindBuffer(GL_ARRAY_BUFFER, (GLuint)id_uv);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_uv * 3, UVs, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}



void Mesh::CleanUp()
{
	id_vertices = 0;
	num_indices = 0;

	id_indices = 0;
	num_vertices = 0;
}

void Mesh::SetWire(bool w)
{
	wire = w;
}

void Mesh::UI_draw()
{
	ImGui::Separator();
	ImGui::Text("Faces: %d", num_faces);
	ImGui::Text("Vertices: %d", num_vertices);
	ImGui::Text("UVs: %d", num_uv);
}

bool Mesh::TriCheck(LineSegment & picking, float & distance, float3 & hitPoint)
{
	bool ret = false;
	Transform* trans = nullptr;
	float distance2 = distance;
	float prevDistance = distance;

	trans = (Transform*)GetOwner()->FindComponentbyType(TRANSFORM);

	LineSegment newSegment = picking;
	newSegment.Transform(trans->GetGlobalTransform().Inverted());

	for (uint i = 0; i < num_indices; i += 3)
	{
		Triangle tri(float3(vertices[indices[i] * 3], vertices[indices[i] * 3 + 1], vertices[indices[i] * 3 + 2]), float3(vertices[indices[i + 1] * 3], vertices[indices[i + 1] * 3 + 1], vertices[indices[i + 1] * 3 + 2]), float3(vertices[indices[i + 2] * 3], vertices[indices[i + 2] * 3 + 1], vertices[indices[i + 2] * 3 + 2]));



		if (newSegment.Intersects(tri, &distance2, &hitPoint))
		{
			if (distance2 < prevDistance)
			{
				prevDistance = distance2;
				distance = distance2;
				ret = true;
			}


		}
	}

	return ret;

}

Mesh::~Mesh()
{
	glDeleteBuffers(1, (GLuint*)&this->id_vertices);
	glDeleteBuffers(1, (GLuint*)&this->id_indices);
	glDeleteBuffers(1, (GLuint*)&this->id_uv);
	LOG_OUT("Mesh destroyed");
}