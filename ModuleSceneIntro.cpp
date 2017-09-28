#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"


ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
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
	AddSphere(0, 0, 0, 0.5f);
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	for (list<Primitive*>::iterator it = bodies.begin(); it != bodies.end(); ++it) {
		delete (*it);
	}
	bodies.clear();
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
	p.Render();

	
	
	return UPDATE_CONTINUE;
}