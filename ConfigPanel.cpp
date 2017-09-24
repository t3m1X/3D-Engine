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

void ConfigPanel::Draw(Application* App) {

	ImGui::Begin("Configuration", &Active);
	DrawApp();
	DrawHardware();
	DrawWin(App);
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

void ConfigPanel::DrawWin(Application* App) {

	if (ImGui::CollapsingHeader("Window")) {
		uint w, h;
		w = App->window->GetWidth();
		h = App->window->GetHeight();

		if (ImGui::SliderInt("Width", (int*)&w, 0, 1280))
			App->window->SetWidth(w);

		if (ImGui::SliderInt("Height", (int*)&h, 0, 1024))
			App->window->SetHeigth(h);

		float brightness = App->window->GetBrightness();
		if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f))
			App->window->SetBrightness(brightness);

		bool fullscreen = App->window->IsFullscreen();
		bool resizable = App->window->IsResizable();
		bool borderless = App->window->IsBorderless();
		bool full_desktop = App->window->IsFullscreenDesktop();

		if (ImGui::Checkbox("Fullscreen", &fullscreen))
			App->window->SetFullscreen(fullscreen);

		ImGui::SameLine();
		if (ImGui::Checkbox("Resizable", &resizable))
			App->window->SetResizable(resizable);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Restart to apply");

		if (ImGui::Checkbox("Borderless", &borderless))
			App->window->SetBorderless(borderless);

		ImGui::SameLine();
		if (ImGui::Checkbox("Full Desktop", &full_desktop))
			App->window->SetFullScreenDesktop(full_desktop);

	}
}

