#include "Console.h"

Console::Console(){

	Name = "Console";
	Shortcut = SDL_SCANCODE_0;
	Active = false;
	w = 658;
	h= 105;
	x= 325;
	y= 919;

}

Console::~Console()
{}

void Console::Clear()
{
	Buf.clear();
}
void Console::AddLog(const char* entry)
{
	Buf.append(entry);
	ScrollToBottom = true;
}
void Console::Draw()
{
	ImGui::Begin("Console", &Active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing);
	ImGui::TextUnformatted(Buf.begin());
	if (ScrollToBottom)
		ImGui::SetScrollHere(1.0f);
	ScrollToBottom = false;
	ImGui::End();
}
