#include "ConfigPanel.h"
#include "imgui.h"
#include "Application.h"
#include "imgui_docking.h"
#include "mmgr\mmgr.h"
#include "Application.h"
#include "ModuleWindow.h"


ConfigPanel::ConfigPanel() : Panel("Configuration", SDL_SCANCODE_4),
fps_log(FPS_LOG_SIZE), ms_log(FPS_LOG_SIZE)
{
	strcpy(name_buffer, App->GetName());
	strcpy(organization_buffer, App->GetOrg());
	strcpy(version_buffer, App->GetVersion());

}
ConfigPanel::~ConfigPanel() {}

void ConfigPanel::Draw() {

	if (!Active)
		return;

	if (ImGui::Begin("Hardware", &Active, ImGuiWindowFlags_AlwaysAutoResize)) {
		DrawApp();
		for (list<Module*>::iterator it = App->list_modules.begin(); it != App->list_modules.end(); ++it)
			(*it)->ImGuiDraw();
	}
	ImGui::End();
}

void ConfigPanel::DrawApp(){


	if (ImGui::CollapsingHeader("Application"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		int max_fps = App->GetFramerateLimit();
		if (ImGui::SliderInt("Max FPS", &max_fps, 0, 120))
			App->SetFramerateLimit(max_fps);

		ImGui::Text("Limit Framerate:");
		ImGui::SameLine();
		ImGui::Text("%i", App->GetFramerateLimit());

		std::vector<float> mem_container;
		sMStats stats;

		stats = m_getMemoryStatistics();

		mem_container.push_back((float)stats.totalReportedMemory);

		if (mem_container.size() > 500) {
			mem_container.erase(mem_container.begin(), mem_container.begin() + 1);
		}

		/*ImGui::Text(App->GetName());
		ImGui::Text(App->GetOrg());
		ImGui::Text(App->GetVersion());*/

		if (ImGui::InputText("App Name", name_buffer, 254))
		{
			App->SetName(name_buffer);
			App->SaveConfig(nullptr);
		}

		if (ImGui::InputText("Organization", organization_buffer, 254))
		{
			App->SetOrg(organization_buffer);
			App->SaveConfig(nullptr);
		}
		if (ImGui::InputText("Version", version_buffer, 254))
		{
			App->SetVersion(version_buffer);
			App->SaveConfig(nullptr);
		}
		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
		ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
		sprintf_s(title, 25, "Milliseconds %0.1f", ms_log[ms_log.size() - 1]);
		ImGui::PlotHistogram("##milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));

		if (!mem_container.empty())
		{
			char title[25];
			sprintf_s(title, 25, "%.1f", mem_container[mem_container.size() - 1]);
			ImGui::PlotHistogram("Memory", &mem_container[0], mem_container.size(), 0, title, 0.0f, 30000.0f, ImVec2(0, 100));
		}
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



