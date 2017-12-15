#include "Console.h"
#include "Application.h"
#include "TimeManager.h"

Console::Console(){

	Name = "Console";
	Shortcut = SDL_SCANCODE_0;
	Active = true;
	w = 850;
	h= 400;
	x= 50;
	y= 600;

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
	if (App->tm->GetGameState() != IN_PLAY) {
		ImGui::Begin("Console", &Active);
		ImGui::TextUnformatted(Buf.begin());
		if (ScrollToBottom)
			ImGui::SetScrollHere(1.0f);
		ScrollToBottom = false;
		ImGui::End();
	}
}
