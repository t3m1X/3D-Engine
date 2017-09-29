#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
	SetName("Window");
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init(JSON_File* conf)
{
	
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = SCREEN_WIDTH * SCREEN_SIZE;
		int height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}
	Load(conf);
	SetFullscreen(fullscreen);
	SetBorderless(borderless);
	SetResizable(resizable);
	SetFullScreenDesktop(fullscreen_desktop);
	SetTitle(App->GetName());
	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}


uint ModuleWindow::GetHeight() const
{
	return screen_height;
}

void ModuleWindow::SetWidth(uint width)
{
	SDL_SetWindowSize(window, width, GetHeight());
}

void ModuleWindow::SetHeigth(uint height)
{
	SDL_SetWindowSize(window, GetWidth(), height);
}


bool ModuleWindow::IsFullscreen() const
{
	return fullscreen;
}

bool ModuleWindow::IsResizable() const
{
	return resizable;
}

bool ModuleWindow::IsBorderless() const
{
	return borderless;
}

bool ModuleWindow::IsFullscreenDesktop() const
{
	return fullscreen_desktop;
}

float ModuleWindow::GetBrightness() const
{
	return SDL_GetWindowBrightness(window);
}

void ModuleWindow::SetFullscreen(bool set)
{
	if (set != fullscreen)
	{
		fullscreen = set;
		if (fullscreen == true)
		{
			if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) != 0)
				LOG("Could not switch to fullscreen: %s\n", SDL_GetError());
			fullscreen_desktop = false;
			SDL_Log("this is a test");
		}
		else
		{
			if (SDL_SetWindowFullscreen(window, 0) != 0)
				LOG("Could not switch to windowed: %s\n", SDL_GetError());
		}
	}
}

void ModuleWindow::SetResizable(bool set)
{
	// cannot be changed while the program is running, but we can save the change
	resizable = set;
}

void ModuleWindow::SetBorderless(bool set)
{
	if (set != borderless && fullscreen == false && fullscreen_desktop == false)
	{
		borderless = set;
		SDL_SetWindowBordered(window, (SDL_bool)!borderless);
	}
}

void ModuleWindow::SetFullScreenDesktop(bool set)
{
	if (set != fullscreen_desktop)
	{
		fullscreen_desktop = set;
		if (fullscreen_desktop == true)
		{
			if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
				LOG("Could not switch to fullscreen desktop: %s\n", SDL_GetError());
			fullscreen = false;
		}
		else
		{
			if (SDL_SetWindowFullscreen(window, 0) != 0)
				LOG("Could not switch to windowed: %s\n", SDL_GetError());
		}
	}
}

void ModuleWindow::SetBrightness(float set)
{
	CAP(set);
	if (SDL_SetWindowBrightness(window, set) != 0)
		LOG("Could not change window brightness: %s\n", SDL_GetError());
}

void ModuleWindow::ImGuiDraw()
{
	if (ImGui::CollapsingHeader(this->GetName())) {
		uint w, h;
		w = App->window->GetWidth();
		h = App->window->GetHeight();

		if (ImGui::SliderInt("Width", (int*)&w, 0, 1280))
			App->window->SetWidth(w);

		if (ImGui::SliderInt("Height", (int*)&h, 0, 1024))
			App->window->SetHeigth(h);

		float brightness = App->window->GetBrightness();
		if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f)) {
			App->window->SetBrightness(brightness);
			App->SaveConfig(App->window);
		}

		bool fullscreen = App->window->IsFullscreen();
		bool resizable = App->window->IsResizable();
		bool borderless = App->window->IsBorderless();
		bool full_desktop = App->window->IsFullscreenDesktop();

		if (ImGui::Checkbox("Fullscreen", &fullscreen)) {
			App->window->SetFullscreen(fullscreen);
			App->SaveConfig(App->window);
		}

		ImGui::SameLine();
		if (ImGui::Checkbox("Resizable", &resizable)) {
			App->window->SetResizable(resizable);
			App->SaveConfig(App->window);
		}
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Restart to apply");

		if (ImGui::Checkbox("Borderless", &borderless)) {
			App->window->SetBorderless(borderless);
			App->SaveConfig(App->window);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Full Desktop", &full_desktop)) {
			App->window->SetFullScreenDesktop(full_desktop);
			App->SaveConfig(App->window);
		}

	}
}

void ModuleWindow::Load(JSON_File* config)
{
	if (config != nullptr) {
		int w = config->GetNumber("window.width");
		int h = config->GetNumber("window.height");
		SetWidth(w);
		SetHeigth(h);

		if (config->GetBool("window.fullscreen")) {
			SetFullscreen(true);
		}
		else {
			SetFullscreen(false);
		}

		if (config->GetBool("window.resizable")) {
			SetResizable(true);
		}
		else {
			SetResizable(false);
		}
		if (config->GetBool("window.borderless")) {
			SetBorderless(true);
		}
		else {
			SetBorderless(false);
		}
		if (config->GetBool("window.fulldesktop")) {
			SetFullScreenDesktop(true);
		}
		else {
			SetFullScreenDesktop(false);
		}
	}
}

void ModuleWindow::Save(JSON_File * c)
{
	if (c != nullptr) {
		c->SetNumber("window.width", GetWidth());
		c->SetNumber("window.height", GetHeight());
		c->SetBool("window.fullscreen", IsFullscreen());
		c->SetBool("window.resizable", IsResizable());
		c->SetBool("window.borderless", IsBorderless());
		c->SetBool("window.fulldesktop", IsFullscreenDesktop());

		c->Save();
	}

}

uint ModuleWindow::GetWidth() const
{
	return screen_width;
}

