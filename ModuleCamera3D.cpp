#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "imgui.h"
#include "ModuleRenderer3D.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{

	debug = true;
	speed = 10.0f;
	Sensitivity = 0.25f;
	SetName("Camera");
	editor_camera = new Camera3D();
	curr_camera = editor_camera;


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
	delete editor_camera;

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{

	//vec3 newPos(0,0,0);
	/*if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN && !App->scene_intro->objects.empty()) {
		ModuleCamera3D::Move(vec3(0, App->scene_intro->objects.back()->boundingbox.r.y + 5, App->scene_intro->objects.back()->boundingbox.r.z - 5) - App->camera->Position);
		ModuleCamera3D::LookAt(vec3(0, 0, 0));
	}*/
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT) {

		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = 8.0f * dt;

		//WASD MOVEMENT + R AND F TO MOVE UP AND DOWN

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) editor_camera->MoveForward(speed*dt);
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) editor_camera->MoveBackwards(speed*dt);


		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) editor_camera->MoveLeft(speed*dt);
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) editor_camera->MoveRight(speed*dt);




	}
	//MOUSE WHEEL

	if (App->input->GetMouseWheel() == 1)
	{
		editor_camera->MoveForward(speed*dt);
	}
	else if (App->input->GetMouseWheel() == -1)
	{
		editor_camera->MoveBackwards(speed*dt);
	}
	/*Position += newPos;
	Reference += newPos;*/

	// Mouse motion ----------------

	if((App->input->GetMouseButton(SDL_BUTTON_LEFT)==KEY_REPEAT)&&(App->input->GetKey(SDL_SCANCODE_LALT)==KEY_REPEAT))
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		//Reference = (0, 0, 0);
		editor_camera->Rotate(-App->input->GetMouseXMotion()*Sensitivity*0.01f, -App->input->GetMouseYMotion()*Sensitivity*0.01f);
		Position -= Reference;
		/*
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

		Position = Reference + Z * length(Position);*/
	}

	// Recalculate matrix -------------
	//CalculateViewMatrix();
	if (debug) {
		DrawDebug();
	}
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
	return editor_camera->GetViewMatrix().Transposed().ptr();
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
		
		ImGui::SliderFloat("Camera speed", &speed, 0, 120);
		ImGui::Text("Speed:");
		ImGui::SameLine();
		ImGui::Text("%f", speed);

		ImGui::SliderFloat("Sensitivity", &Sensitivity, 0, 1);
		ImGui::Text("Sensitivity:");
		ImGui::SameLine();
		ImGui::Text("%f", Sensitivity);
	}
}

void ModuleCamera3D::SetCurrentCamera(Camera3D * cam)
{
	curr_camera = cam;
}

Camera3D * ModuleCamera3D::GetCurrentCamera() const
{
	return curr_camera;
}

Camera3D * ModuleCamera3D::GetEditorCamera() const
{
	return editor_camera;
}

void ModuleCamera3D::DrawDebug()
{
	float3 corners[8];
	curr_camera->GetCorners(corners);
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
		colors[i] = float3(60, 1, 1);
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