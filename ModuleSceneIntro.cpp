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
	if (!objects.empty()) {
		for (list<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it) {
			delete (*it);
		}
		objects.clear();
	}

	return true;
}

void ModuleSceneIntro::AddBody(Primitive * body)
{
	bodies.push_back(body);
}

void ModuleSceneIntro::AddObject(GameObject * obj)
{
	objects.push_back(obj);
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

	for (list<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it) {
		(*it)->Draw();
	}

}

bool ModuleSceneIntro::Empty()
{
	return objects.empty();
}

void ModuleSceneIntro::Clear()
{
	if (!objects.empty()) {
		for (list<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it) {
			delete (*it);
		}
		objects.clear();
	}
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	bPlane p(0, 1, 0, 0);
	p.axis = true;
	
	for (list<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it) {
		(*it)->Update();
	}

	p.Render();

	
	
	return UPDATE_CONTINUE;
}