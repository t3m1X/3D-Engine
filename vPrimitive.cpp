
#include "Globals.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "vPrimitive.h"
#include "glew\include\GL\glew.h"

// ------------------------------------------------------------
vPrimitive::vPrimitive() : transform(IdentityMatrix), color(White), wire(false), axis(false), type(PrimitiveTypes::Primitive_Point)
{}

// ------------------------------------------------------------
PrimitiveTypes vPrimitive::GetType() const
{
	return type;
}

void vPrimitive::SetWire(bool b)
{
	wire = b;
}

void vPrimitive::SetColor(float r, float g, float b, float a)
{
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
}

// ------------------------------------------------------------
void vPrimitive::Render() const
{
	glPushMatrix();
	glMultMatrixf(transform.M);

	if (axis == true)
	{
		// Draw Axis Grid
		glLineWidth(2.0f);

		glBegin(GL_LINES);

		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
		glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
		glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
		glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

		glEnd();

		glLineWidth(1.0f);
	}

	glColor3f(color.r, color.g, color.b);

	if (wire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	InnerRender();

	glPopMatrix();
}

// ------------------------------------------------------------
void vPrimitive::InnerRender() const
{
	glPointSize(5.0f);

	glBegin(GL_POINTS);

	glVertex3f(0.0f, 0.0f, 0.0f);

	glEnd();

	glPointSize(1.0f);
}

// ------------------------------------------------------------
void vPrimitive::SetPos(float x, float y, float z)
{
	transform.translate(x, y, z);
}

// ------------------------------------------------------------
void vPrimitive::SetRotation(float angle, const vec3 &u)
{
	transform.rotate(angle, u);
}

// ------------------------------------------------------------
void vPrimitive::Scale(float x, float y, float z)
{
	transform.scale(x, y, z);
}

// CUBE ============================================
Cube::Cube() : vPrimitive(), size(1.0f, 1.0f, 1.0f)
{
	type = PrimitiveTypes::Primitive_Cube;
	glGenBuffers(1, (GLuint*) &(vertices_arr));
	glBindBuffer(GL_ARRAY_BUFFER, vertices_arr);

	float sx = size.x * 0.5f;
	float sy = size.y * 0.5f;
	float sz = size.z * 0.5f;
	float vertices[8*3] = {
		-sx, sy, sz,
		sx, sy, sz,
		-sx, -sy, sz,
		sx, -sy, sz,
		-sx, sy, -sz,
		sx, sy, -sz,
		-sx, -sy, -sz,
		sx, -sy, -sz
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * 3, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) &(index_arr));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_arr);

	uint indices[6 * 6] = {
		2, 3, 0, 3, 1, 0,
		6, 7, 4, 7, 5, 4,
		0, 1, 4, 1, 5, 4,
		6, 7, 2, 7, 3, 2,
		6, 2, 4, 2, 0, 4,
		3, 7, 1, 7, 5, 1
	};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 36, indices, GL_STATIC_DRAW);
}

Cube::Cube(float sizeX, float sizeY, float sizeZ) : vPrimitive(), size(sizeX, sizeY, sizeZ)
{
	type = PrimitiveTypes::Primitive_Cube;
	glGenBuffers(1, (GLuint*) &(vertices_arr));
	glBindBuffer(GL_ARRAY_BUFFER, vertices_arr);

	float sx = size.x * 0.5f;
	float sy = size.y * 0.5f;
	float sz = size.z * 0.5f;
	float vertices[8 * 3] = {
		-sx, sy, sz,
		sx, sy, sz,
		-sx, -sy, sz,
		sx, -sy, sz,
		-sx, sy, -sz,
		sx, sy, -sz,
		-sx, -sy, -sz,
		sx, -sy, -sz
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * 3, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) &(index_arr));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_arr);

	uint indices[6 * 6] = {
		2, 3, 0, 3, 1, 0,
		6, 7, 4, 7, 5, 4,
		0, 1, 4, 1, 5, 4,
		6, 7, 2, 7, 3, 2,
		6, 2, 4, 2, 0, 4,
		3, 7, 1, 7, 5, 1
	};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 36, indices, GL_STATIC_DRAW);
}

void Cube::InnerRender() const
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_arr);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_arr);
	glDrawElements(GL_TRIANGLES, index_arr, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}

// SPHERE ============================================
Sphere::Sphere() : vPrimitive(), radius(1.0f)
{
	type = PrimitiveTypes::Primitive_Sphere;
}

Sphere::Sphere(float radius) : vPrimitive(), radius(radius)
{
	type = PrimitiveTypes::Primitive_Sphere;
}

void Sphere::InnerRender() const
{
	//glutSolidSphere(radius, 25, 25);
}