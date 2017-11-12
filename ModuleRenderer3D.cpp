
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleLoader.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleImGui.h"
#include "ModuleSceneIntro.h"
#include "FBO.h"


#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
//#pragma comment (lib, "glew/lib/Release/x64/glew32s.lib")

ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module(start_enabled)
{
	SetName("Renderer");
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init(JSON_File* conf)
{
	LOG_OUT("Creating 3D Renderer context");
	bool ret = true;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	Load(conf);
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG_OUT("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	// Glew
	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		LOG_OUT("Glew library could not init %s\n", glewGetErrorString(err));
		ret = false;
	}

	//OpenGL
	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG_OUT("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize FBO
		fbo_texture = new FBO();
		fbo_texture->Create(App->window->GetWidth(), App->window->GetHeight());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG_OUT("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG_OUT("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG_OUT("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		LOG_OUT("Using Glew %s", glewGetString(GLEW_VERSION));
		LOG_OUT("Vendor: %s", glGetString(GL_VENDOR));
		LOG_OUT("Renderer: %s", glGetString(GL_RENDERER));
		LOG_OUT("OpenGL version supported %s", glGetString(GL_VERSION));
		LOG_OUT("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		GLfloat LightModelAmbient[] = {0.6f, 0.6f, 0.6f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		

		Custom_attributes();
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	fbo_texture->Bind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix().ptr());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();
	

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	//App->scene_intro->Draw();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	fbo_texture->Unbind();

	UI_attributes();
	App->imgui->Draw();
	Custom_attributes();
	
	SDL_GL_SwapWindow(App->window->window);
	
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG_OUT("Destroying 3D Renderer");
	
	fbo_texture->Unbind();
	delete fbo_texture;
	fbo_texture = nullptr;

	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer3D::UI_attributes()
{
	GLfloat LightModelAmbient[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

	GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
}

void ModuleRenderer3D::Custom_attributes()
{
	if (light_model_ambient != STD_AMBIENT_LIGHTING) {
		GLfloat LightModelAmbient[] = { light_model_ambient, light_model_ambient, light_model_ambient, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
	}
	if (material_ambient != STD_MATERIAL_AMBIENT) {
		GLfloat MaterialAmbient[] = { material_ambient, material_ambient, material_ambient, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);
	}

	enable_depth_test ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	enable_cull_face ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
	enable_lightning ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
	enable_color_material ? glEnable(GL_COLOR_MATERIAL) : glDisable(GL_COLOR_MATERIAL);
	enable_texture_2D ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);
	(smooth) ? glShadeModel(GL_SMOOTH) : glShadeModel(GL_FLAT);
	if (enable_fog)
	{
		glEnable(GL_FOG);
		glFogfv(GL_FOG_DENSITY, &fog_density);
	}
	else
	{
		glDisable(GL_FOG);
	}
	/*if (enable_wireframe) {
		App->scene_intro->Wireframe(enable_wireframe);
	}
	else {
		App->scene_intro->Wireframe(enable_wireframe);
	}
	*/
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

uint ModuleRenderer3D::GetFrameBuffer()
{
	return fbo_texture->GetTexture();
}

void ModuleRenderer3D::ImGuiDraw()
{
	if (ImGui::CollapsingHeader("Renderer")) {
		//Check Boxes
		if (ImGui::Checkbox("Depth Test", &enable_depth_test)) {
			enable_depth_test ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
			App->SaveConfig(App->renderer3D);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Face Culling", &enable_cull_face)) {
			enable_cull_face ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
			App->SaveConfig(App->renderer3D);
		}
		if (ImGui::Checkbox("Lighting", &enable_lightning)) {
			enable_lightning ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
			App->SaveConfig(App->renderer3D);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Material Color", &enable_color_material)) {
			enable_color_material ? glEnable(GL_COLOR_MATERIAL) : glDisable(GL_COLOR_MATERIAL);
			App->SaveConfig(App->renderer3D);
		}
		if (ImGui::Checkbox("2D Textures", &enable_texture_2D)) {
			enable_texture_2D ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);
			App->SaveConfig(App->renderer3D);
		}
		if (ImGui::Checkbox("Wireframe Mode", &enable_wireframe)) {
			if (enable_wireframe) {
				App->loader->SetWire(enable_wireframe);
			}
			else {
				App->loader->SetWire(enable_wireframe);
			}
			App->SaveConfig(App->renderer3D);
		}
			if (ImGui::Checkbox("Smooth", &smooth))
			{
				(smooth) ? glShadeModel(GL_SMOOTH) : glShadeModel(GL_FLAT);
				App->SaveConfig(App->renderer3D);
			}

			//Sliders
			if (ImGui::SliderFloat("Ambient Lighting", &light_model_ambient, 0, 1.0f)) {
				GLfloat LightModelAmbient[] = { light_model_ambient, light_model_ambient, light_model_ambient, 1.0f };
				glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
				App->SaveConfig(App->renderer3D);
			}
			if (ImGui::SliderFloat("Material Ambient", &material_ambient, 0, 1.0f)) {
				GLfloat MaterialAmbient[] = { material_ambient, material_ambient, material_ambient, 1.0f };
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);
			}

			if (ImGui::CollapsingHeader("Fog"))
			{
				if (ImGui::Checkbox("Active", &enable_fog))
				{
					if (enable_fog)
					{
						glEnable(GL_FOG);
						glFogfv(GL_FOG_DENSITY, &fog_density);
					}
					else
					{
						glDisable(GL_FOG);
					}
					App->SaveConfig(App->renderer3D);
				}
				if (enable_fog)
				{
					if (ImGui::SliderFloat("Density", &fog_density, 0.0f, 1.0f, "%.3f"))
					{
						glFogfv(GL_FOG_DENSITY, &fog_density);
					}
				}
			}

		}

	}
	

void ModuleRenderer3D::Load(JSON_File * c)
{
	enable_depth_test = c->GetBool("renderer.depth_test");
	enable_cull_face = c->GetBool("renderer.cull_face");
	enable_color_material = c->GetBool("renderer.color_material");
	enable_lightning = c->GetBool("renderer.lightning");
	enable_wireframe = c->GetBool("renderer.wireframe");
	smooth = c->GetBool("renderer.smooth");
	enable_fog = c->GetBool("renderer.fog");
	enable_texture_2D = c->GetBool("renderer.2D_texture");
	fog_density = c->GetNumber("renderer.fog_density");
	light_model_ambient = c->GetNumber("renderer.light_model_ambient");
	material_ambient = c->GetNumber("renderer.material_ambient");
}

void ModuleRenderer3D::Save(JSON_File* c)
{
	c->SetBool("renderer.depth_test", enable_depth_test);
	c->SetBool("renderer.cull_face", enable_cull_face);
	c->SetBool("renderer.color_material", enable_color_material);
	c->SetBool("renderer.lightning", enable_lightning);
	c->SetBool("renderer.wireframe", enable_wireframe);
	c->SetBool("renderer.smooth", smooth);
	c->SetBool("renderer.fog", enable_fog);
	c->SetBool("renderer.2D_texture", enable_texture_2D);

	c->Save();
}

/*void ModuleRenderer3D::Draw(Mesh * m)
{

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, m->id_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->id_indices);
	glDrawElements(GL_TRIANGLES, m->num_indices, GL_UNSIGNED_INT, NULL);
	glBindTexture(GL_TEXTURE_2D, App->loader->texture);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}*/
