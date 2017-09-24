#ifndef __PANEL_H__
#define __PANEL_H__

#include "Globals.h"
#include <string>
#include "SDL/include/SDL_scancode.h"
#include "Module.h"


class Panel {
	public:
		Panel(const char* name, SDL_Scancode shortcut = SDL_SCANCODE_UNKNOWN, bool active = false);
		Panel();
		virtual ~Panel();
		void Enable();
		void Disable();
		bool IsActive() const;
		SDL_Scancode GetShortcut();
		virtual void Draw(Application* App) = 0;
	public:
		int x, y, w, h;
	protected:
		const char* Name;
		SDL_Scancode Shortcut;
		bool Active;
};

#endif
