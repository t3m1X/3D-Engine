#include "Console.h"
#include "Application.h"

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
{
	Clear();
}

void Console::Clear()
{
	Buf.clear();
}
void Console::AddLog(const char* entry)
{
	if (App->con != nullptr && entry != nullptr) {
		Buf.append(entry);
		ScrollToBottom = true;
	}
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
