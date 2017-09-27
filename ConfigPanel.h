#ifndef __CONFIGPANEL_H__
#define __CONFIGPANEL_H__

#include "Panel.h"
#include "imgui.h"
#include <vector>
#include "Hardware.h"
#include "ModuleWindow.h"
#include "Application.h"

#define FPS_LOG_SIZE 100

class ConfigPanel : public Panel {

	public: 
		ConfigPanel(Application* App);
		virtual ~ConfigPanel();
		void Draw(Application* App);
		void DrawApp(Application* App);
		void DrawHardware();
		void FPS(float fps, float ms);
		//void DrawWin(Application* App);

	private : 
		ImGuiTextBuffer input_buf;
		bool need_scroll = false;
		std::vector<float> fps_log;
		std::vector<float> ms_log;
		char name_buffer[255];
		char organization_buffer[255];
		char version_buffer[255];
};


#endif
