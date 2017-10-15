#include "ConfigPanel.h"
#include "Application.h"
#include "mmgr\mmgr.h"


ConfigPanel::ConfigPanel(Application* App) : Panel("Configuration", SDL_SCANCODE_4),
fps_log(FPS_LOG_SIZE), ms_log(FPS_LOG_SIZE)
{
	w = 325;
	h = 417;
	x = 100;
	y = 50;

	strcpy(name_buffer, App->GetName());
	strcpy(organization_buffer, App->GetOrg());
	strcpy(version_buffer, App->GetVersion());

}
ConfigPanel::~ConfigPanel() {}

void ConfigPanel::Draw(Application* App) {

	ImGui::Begin("Configuration", &Active);
	DrawApp(App);
	DrawHardware();
	for (list<Module*>::iterator it = App->list_modules.begin(); it != App->list_modules.end(); ++it) {
		it._Ptr->_Myval->ImGuiDraw();
	}
	ImGui::End();
}

void ConfigPanel::DrawApp(Application* App) {

	
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


	if (ImGui::CollapsingHeader("Application"))
	{

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
void ConfigPanel::DrawHardware() {

	if (ImGui::CollapsingHeader("Hardware")) {

		Hardware hw;
		Hardware::hardware_info info;
		info = hw.GetInfo();
		ImGui::Text("SDL Version:", info.sdl_version);
		ImGui::SameLine();
		ImGui::Text("%u", info.sdl_version);
		ImGui::Separator();

		ImGui::Text("CPUs:", "%u (Cache: %ukb)", info.cpu_count, info.l1_cachekb);
		ImGui::SameLine();
		ImGui::Text("%u", info.cpu_count, info.l1_cachekb);

		ImGui::Text("System RAM:");
		ImGui::SameLine();
		ImGui::Text("%.1fGb", info.ram_gb);

		ImGui::Separator();
		ImGui::Text("GPU:");
		ImGui::SameLine();
		ImGui::Text("vendor %u device %u", info.gpu_vendor, info.gpu_device);

		ImGui::Text("Brand:");
		ImGui::SameLine();
		ImGui::Text(info.gpu_brand);

		ImGui::Text("VRAM Budget:");
		ImGui::SameLine();
		ImGui::Text("%.1f Mb", info.vram_mb_budget);

		ImGui::Text("VRAM Usage:");
		ImGui::SameLine();
		ImGui::Text( "%.1f Mb", info.vram_mb_usage);

		ImGui::Text("VRAM Available:");
		ImGui::SameLine();
		ImGui::Text("%.1f Mb", info.vram_mb_available);

		ImGui::Text("VRAM Reserved:");
		ImGui::SameLine();
		ImGui::Text("%.1f Mb", info.vram_mb_reserved);

	}
}



