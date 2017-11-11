#include "Panel.h"

Panel::Panel(const char* name, SDL_Scancode shortcut, bool active){
	Name = name;
	Shortcut = shortcut;
	Active = active;
}
Panel::Panel() {
}

Panel::~Panel() {

};

void Panel::SetActive(bool active) {
	Active = active;
}

void Panel::ToggleActive()
{
	Active = !Active;
}

bool Panel::IsActive()const  {

	return Active;
}

SDL_Scancode Panel::GetShortcut() {
	return Shortcut;
}
