#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"


#include "glew\include\GL\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl\GL.h>
#include <gl\GLU.h>

#define MAX_LIGHTS 8
#define STD_AMBIENT_LIGHTING 0.6f
#define STD_MATERIAL_AMBIENT 1.0f

struct Mesh;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init(JSON_File* conf);
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	void UI_attributes();
	void Custom_attributes();
	void OnResize(int width, int height);
	void ImGuiDraw();
	void Load(JSON_File*c);
	void Save(JSON_File*c);
	void Draw(Mesh*m);
	GLuint LoadTexBuffer(GLuint * texture, uint size, int width, int height, uint wrap_s, uint wrap_t, uint mag, uint min);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	bool enable_depth_test;
	bool enable_cull_face;
	bool enable_lightning;
	bool enable_color_material;
	bool enable_texture_2D;
	bool enable_wireframe;
	bool smooth;
	bool enable_fog;
	GLfloat fog_density;
	float light_model_ambient;
	float material_ambient;
};