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

void Panel::Enable() {
	Active = true;
}

void Panel::Disable() {
	Active = false;
}
bool Panel::IsActive()const  {

	return Active;
}

SDL_Scancode Panel::GetShortcut() {
	return Shortcut;
}
