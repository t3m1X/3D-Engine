#include "ConfigPanel.h"
#include "Application.h"

ConfigPanel::ConfigPanel() : Panel("Configuration", SDL_SCANCODE_4),
fps_log(FPS_LOG_SIZE), ms_log(FPS_LOG_SIZE)
{
	w = 325;
	h = 417;
	x = 100;
	y = 50;
}
ConfigPanel::~ConfigPanel() {}

void ConfigPanel::Draw() {

	ImGui::Begin("Configuration", &Active);
	DrawApp();
	ImGui::End();
}

void ConfigPanel::DrawApp() {

	
	/*int max_fps = App->GetFramerateLimit();
	if (ImGui::SliderInt("Max FPS", &max_fps, 0, 120))
		App->SetFramerateLimit(max_fps);

	ImGui::Text("Limit Framerate:");
	ImGui::SameLine();
	ImGui::Text("%i", App->GetFramerateLimit());*/

	if (ImGui::CollapsingHeader("Application"))
	{
		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
		ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
		sprintf_s(title, 25, "Milliseconds %0.1f", ms_log[ms_log.size() - 1]);
		ImGui::PlotHistogram("##milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
	}
}

void ConfigPanel::FPS(float fps, float ms) {
	 
	static uint count = 0;

	if (count == FPS_LOG_SIZE)
	{
		for (uint i = 0; i < FPS_LOG_SIZE - 1; ++i)
		{
			fps_log[i] = fps_log[i + 1];
			ms_log[i] = ms_log[i + 1];
		}
	}
	else
		++count;

	fps_log[count - 1] = fps;
	ms_log[count - 1] = ms;
}