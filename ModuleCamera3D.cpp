#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "imgui.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
	speed = 3.0f;
	Sensitivity = 0.25f;
	SetName("Camera");
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{

	vec3 newPos(0,0,0);
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN && !App->scene_intro->objects.empty()) {
		ModuleCamera3D::Move(vec3(0, App->scene_intro->objects.back()->boundingbox.r.y * 2, App->scene_intro->objects.back()->boundingbox.r.z * 2) - App->camera->Position);
		ModuleCamera3D::LookAt(vec3(0, 0, 0));
	}
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT) {

		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = 8.0f * dt;

		//WASD MOVEMENT + R AND F TO MOVE UP AND DOWN

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed*dt;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed*dt;


		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed*dt;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed*dt;

	}
	//MOUSE WHEEL

	if (App->input->GetMouseWheel() == 1)
	{
		newPos -= Z * speed*dt;
	}
	else if (App->input->GetMouseWheel() == -1)
	{
		newPos += Z * speed*dt;
	}
	Position += newPos;
	Reference += newPos;

	// Mouse motion ----------------

	if((App->input->GetMouseButton(SDL_BUTTON_LEFT)==KEY_REPEAT)&&(App->input->GetKey(SDL_SCANCODE_LALT)==KEY_REPEAT))
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		Reference = (0, 0, 0);

		Position -= Reference;

		if(dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if(dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if(Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		Position = Reference + Z * length(Position);
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}


void ModuleCamera3D::Focus(const vec3 target, const float dist)
{
	Reference = target;
	Position = Reference + Z * dist;
}

void ModuleCamera3D::Orbit(const vec3 & orbit_center, const float & motion_x, const float & motion_y)
{
	Reference = orbit_center;

	int x = -motion_x;
	int y = -motion_y;

	Position -= Reference;

	if (x != 0)
	{
		float dx = (float)x * Sensitivity;

		X = rotate(X, dx, vec3(0.0f, 1.0f, 0.0f));
		Y = rotate(Y, dx, vec3(0.0f, 1.0f, 0.0f));
		Z = rotate(Z, dx, vec3(0.0f, 1.0f, 0.0f));
	}

	if (y != 0)
	{
		float dy = (float)y * Sensitivity;
		
		Y = rotate(Y, dy, X);
		Z = rotate(Z, dy, X);
	}

	Position = Reference + Z * length(Position);
}

void ModuleCamera3D::Rotate(const float & motion_x, const float & motion_y)
{

	Reference = Position;

	int x = -motion_x;
	int y = -motion_y;

	Position -= Reference;

	if (x != 0)
	{
		float dx = (float)x * Sensitivity;

		X = rotate(X, dx, vec3(0.0f, 1.0f, 0.0f));
		Y = rotate(Y, dx, vec3(0.0f, 1.0f, 0.0f));
		Z = rotate(Z, dx, vec3(0.0f, 1.0f, 0.0f));
	}

	if (y != 0)
	{
		float dy = (float)y * Sensitivity;

		Y = rotate(Y, dy, X);
		Z = rotate(Z, dy, X);
	}

	Position = Reference + Z * length(Position);

}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

void ModuleCamera3D::ImGuiDraw() {

	if (ImGui::CollapsingHeader(this->GetName())) {

	}
}

/*void ModuleCamera3D::FocusMesh(Mesh* m)
{
	uint	y_test = 0;
	float	highest_vertex_y = 0.0f, highest_vertex_x = 0.0f, highest_vertex_z = 0.0f;

	for (uint i = 0; i < m->num_vertices; ++i) {
		switch (++y_test) {
		case 1:
			if (highest_vertex_x < m->vertices[i])
				highest_vertex_x = m->vertices[i];
			break;
		case 2:
			if (highest_vertex_y < m->vertices[i])
				highest_vertex_y = m->vertices[i];
			break;
		case 3:
			if (highest_vertex_z < m->vertices[i])
				highest_vertex_z = m->vertices[i];
			y_test = 0;
			break;
		}
	}

	vec3 first_v(m->vertices[0], m->vertices[1], m->vertices[2]);


	Position.y = Reference.y = highest_vertex_y * CAMERA_PROPORTION;
	Position.z = Reference.z = highest_vertex_z * CAMERA_PROPORTION;
	Position.x = Reference.x = highest_vertex_x * CAMERA_PROPORTION;

	vec3 closest_vertex(first_v), furthest_vertex;

	float dist_closest = dot(Position, vec3(m->vertices[0], m->vertices[1], m->vertices[2]));
	float dist_furthest = 0.0f;

	for (uint i = 0; i < m->num_vertices; i += 3)
	{
		if (dot(Position, vec3(m->vertices[i], m->vertices[i + 1], m->vertices[i + 2])) < dist_closest)
		{
			closest_vertex = vec3(m->vertices[i], m->vertices[i + 1], m->vertices[i + 2]);
			dist_closest = dot(Position, closest_vertex);
		}
		else if (dot(Position, vec3(m->vertices[i], m->vertices[i + 1], m->vertices[i + 2])) > dist_furthest)
		{
			furthest_vertex = vec3(m->vertices[i], m->vertices[i + 1], m->vertices[i + 2]);
			dist_furthest = dot(Position, closest_vertex);
		}
	}

	vec3 midpoint = (closest_vertex + furthest_vertex) / 2;

	LookAt(midpoint);
}
*/