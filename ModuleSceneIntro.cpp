#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"


ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{
}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	if (!bodies.empty()) {
		for (list<Primitive*>::iterator it = bodies.begin(); it != bodies.end(); ++it) {
			delete (*it);
		}
		bodies.clear();
	}
	return true;
}

void ModuleSceneIntro::AddBody(Primitive * body)
{
	bodies.push_back(body);
}

void ModuleSceneIntro::AddSphere(float x, float y, float z, float radius)
{
	bSphere* sphere = new bSphere(radius);
	sphere->SetPos(x, y, z);
	AddBody(sphere);
}

void ModuleSceneIntro::AddCube(float x, float y, float z, float posx, float posy, float posz)
{
	bCube* cube = new bCube(x, y, z);
	cube->SetPos(posx, posy, posz);
	AddBody(cube);
}

void ModuleSceneIntro::AddCylinder(float radius, float height, float posx, float posy, float posz)
{
	bCylinder* cylinder = new bCylinder(radius, height);
	cylinder->SetPos(posx, posy, posz);
	AddBody(cylinder);
}

void ModuleSceneIntro::AddPlane(float x, float y, float z, float d, float posx, float posy, float posz)
{
	bPlane* plane = new bPlane(x, y, z, d);
	plane->SetPos(posx, posy, posz);
	AddBody(plane);
}

void ModuleSceneIntro::Wireframe(bool b)
{
	for (list<Primitive*>::iterator it = bodies.begin(); it != bodies.end(); ++it) {
		(*it)->SetWire(b);
	}
}

void ModuleSceneIntro::Draw()
{
	for (list<Primitive*>::iterator it = bodies.begin(); it != bodies.end(); ++it) {
		(*it)->Render();
	}
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	bPlane p(0, 1, 0, 0);
	p.axis = true;
	/*glLineWidth(2.0f);
	glBegin(GL_LINES);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 10.f, 0.f);
	glEnd();
	glLineWidth(1.0f);*/
	


	//uint my_id = 0;
	//glGenBuffers(1, (GLuint*) &(my_id));
	//glBindBuffer(GL_ARRAY_BUFFER, my_id);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float)*36 * 3, vertices, GL_STATIC_DRAW);

	//glEnableClientState(GL_VERTEX_ARRAY);
	//glBindBuffer(GL_ARRAY_BUFFER, my_id);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);
	//// ... draw other buffers
	//glDrawArrays(GL_TRIANGLES, 0, 36 * 3);
	//glDisableClientState(GL_VERTEX_ARRAY);




	//glBegin(GL_TRIANGLES);
	//glVertex3f(0.f, 0.f, 0.f);
	//glVertex3f(1.f, 0.f, 0.f);
	//glVertex3f(0.f, 1.f, 0.f);
	//glVertex3f(1.f, 1.f, 0.f);
	//glVertex3f(0.f, 1.f, 0.f);
	//glVertex3f(1.f, 0.f, 0.f);

	//glVertex3f(0.f, 0.f, -1.f);
	//glVertex3f(0.f, 0.f, 0.f);
	//glVertex3f(0.f, 1.f, -1.f);
	//glVertex3f(0.f, 1.f, 0.f);
	//glVertex3f(0.f, 1.f, -1.f);
	//glVertex3f(0.f, 0.f, 0.f);
	//
	//glVertex3f(1.f, 0.f, 0.f);
	//glVertex3f(1.f, 0.f, -1.f);
	//glVertex3f(1.f, 1.f, -1.f);
	//glVertex3f(1.f, 1.f, 0.f);
	//glVertex3f(1.f, 0.f, 0.f);
	//glVertex3f(1.f, 1.f, -1.f);
	//
	//glVertex3f(0.f, 0.f, -1.f);
	//glVertex3f(0.f, 1.f, -1.f);
	//glVertex3f(1.f, 0.f, -1.f);
	//glVertex3f(1.f, 1.f, -1.f);
	//glVertex3f(1.f, 0.f, -1.f);
	//glVertex3f(0.f, 1.f, -1.f);
	//
	//glVertex3f(0.f, 1.f, 0.f);
	//glVertex3f(1.f, 1.f, 0.f);
	//glVertex3f(0.f, 1.f, -1.f);
	//glVertex3f(1.f, 1.f, -1.f);
	//glVertex3f(0.f, 1.f, -1.f);
	//glVertex3f(1.f, 1.f, 0.f);
	//
	//glVertex3f(0.f, 0.f, 0.f);
	//glVertex3f(0.f, 0.f, -1.f);
	//glVertex3f(1.f, 0.f, 0.f);
	//glVertex3f(1.f, 0.f, -1.f);
	//glVertex3f(1.f, 0.f, 0.f);
	//glVertex3f(0.f, 0.f, -1.f);




	//




	//glEnd();
	p.Render();

	
	
	return UPDATE_CONTINUE;
}