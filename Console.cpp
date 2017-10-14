#include "Console.h"

Console::Console(){

	Name = "Console";
	Shortcut = SDL_SCANCODE_0;
	Active = true;
	w = 400;
	h= 105;
	x= 50;
	y= 50;

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
void Console::Draw(Application* App)
{
	ImGui::Begin("Console", &Active);
	ImGui::TextUnformatted(Buf.begin());
	if (ScrollToBottom)
		ImGui::SetScrollHere(1.0f);
	ScrollToBottom = false;
	ImGui::End();
}
