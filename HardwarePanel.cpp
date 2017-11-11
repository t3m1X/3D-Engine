#include "HardwarePanel.h"
#include "imgui.h"
#include "imgui_docking.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Hardware.h"

HardwarePanel::HardwarePanel(bool isActive) : Panel("HardwarePanel", SDL_SCANCODE_UNKNOWN, isActive)
{
}

HardwarePanel::~HardwarePanel()
{
}

void HardwarePanel::Start()
{
}

void HardwarePanel::Draw()
{
	if (!Active)
		return;

	if (ImGui::Begin("Hardware", &Active, ImGuiWindowFlags_AlwaysAutoResize)) {

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
		ImGui::Text("%.1f Mb", info.vram_mb_usage);

		ImGui::Text("VRAM Available:");
		ImGui::SameLine();
		ImGui::Text("%.1f Mb", info.vram_mb_available);

		ImGui::Text("VRAM Reserved:");
		ImGui::SameLine();
		ImGui::Text("%.1f Mb", info.vram_mb_reserved);

	}

	ImGui::End();
}
