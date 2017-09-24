#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "Panel.h"
#include "imgui.h"


class Console : public Panel {

	public:

		Console();
		virtual ~Console();
		void Draw(Application* App);
		void Clear();
		void AddLog(const char* log);

	private:

		ImGuiTextBuffer Buf;
		bool ScrollToBottom;



};



#endif
