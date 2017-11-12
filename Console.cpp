#include "Console.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleImGui.h"
#include "imgui.h"
#include "imgui_docking.h"

Console::Console(){

	Name = "Console";
	Shortcut = SDL_SCANCODE_0;
	Active = true;
}

Console::~Console()
{
	Clear();
}

void Console::Clear()
{
	Buf.clear();
}
void Console::AddLog(const char* entry)
{
	if (App->imgui->GetConsole() != nullptr && entry != nullptr) {
		Buf.append(entry);
		ScrollToBottom = true;
	}
}
void Console::Draw()
{
	if (App->input->GetKey(Shortcut) == SDL_KEYDOWN)
		Active = !Active;

	if (!Active)
		return;

	igBeginDock("Console", &Active, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	// Scrollable
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

	// Scrollable text

	ImVec4 col = { 255, 255, 255, 255 };
	ImGui::PushStyleColor(ImGuiCol_Text, col);
	ImGui::TextUnformatted(Buf.begin());
	ImGui::PopStyleColor();

	//// Scroll to bottom
	//if (scroll_bottom)
	//{
	//	ImGui::SetScrollHere();
	//	scroll_bottom = false;
	//}

	ImGui::PopStyleVar();
	ImGui::EndChild();

	ImGui::Separator();

	igEndDock();
}
