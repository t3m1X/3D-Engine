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

		virtual void Start() {};
		virtual void Draw() {};
		virtual void CleanUp() {};

		virtual ~Panel();
		void SetActive(bool active);
		void ToggleActive();
		bool IsActive() const;
		SDL_Scancode GetShortcut();

	public:
		bool Active;

	protected:
		const char* Name;
		SDL_Scancode Shortcut;
};

#endif
