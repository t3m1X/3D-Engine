#ifndef __CONFIGPANEL_H__
#define __CONFIGPANEL_H__

#include "Panel.h"

#define FPS_LOG_SIZE 100


class ConfigPanel : public Panel {

	public: 
		ConfigPanel();
		virtual ~ConfigPanel();
		void Draw();
		void DrawApp();
		void FPS(float fps, float ms);
		//void DrawWin(Application* App);

	private : 
		bool need_scroll = false;
		std::vector<float> fps_log;
		std::vector<float> ms_log;
		
		char name_buffer[255];
		char organization_buffer[255];
		char version_buffer[255];
};


#endif
