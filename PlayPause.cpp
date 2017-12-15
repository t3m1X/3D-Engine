

#include "Application.h"
#include "ModuleImGui.h"
#include "TimeManager.h"
#include "PlayPause.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "AudioSource.h"

void PlayPause::Draw(Application* App)
{

	if (App->tm->GetGameState() == IN_EDITOR) {
		App->tm->SetPaused(true);
	}
	else {
		App->tm->SetPaused(false);
	}
		ImGui::Begin("GameTimeManager", &Active, ImGuiWindowFlags_AlwaysAutoResize| ImGuiWindowFlags_NoCollapse| ImGuiWindowFlags_NoTitleBar| ImGuiWindowFlags_NoMove| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
		
		bool playbutton = (App->tm->GetGameState() == IN_PLAY) ? true : false;
		if (ImGui::Checkbox("PLAY", &playbutton)) {
			// Testing
			AudioSource* source = (AudioSource*)App->scene_intro->camera_obj->FindComponentbyType(AUDIO_SOURCE);
			source->SendEvent("Play_Blend");
			AudioSource* s = (AudioSource*)App->scene_intro->static_obj->FindComponentbyType(AUDIO_SOURCE);
			s->SendEvent("Play_Short_Music");
			AudioSource* sound = (AudioSource*)App->scene_intro->non_static_obj->FindComponentbyType(AUDIO_SOURCE);
			sound->SendEvent("Play_spell4");
		};
		if (App->tm->GetGameState() != IN_PLAY && playbutton == true)
			App->tm->SetGameState(IN_PLAY);
		if (App->tm->GetGameState() == IN_PLAY && playbutton == false)
			App->tm->SetGameState(IN_EDITOR);
		ImGui::SameLine();
		if (ImGui::Checkbox("PAUSE", &App->tm->Paused)) {
			App->tm->SetGameState(IN_EDITOR);
			App->audio->StopAllEvents();
		};
		ImGui::SameLine();
		if (ImGui::SmallButton("Next Frame")) {
			App->tm->NextFrame();
		}
		ImGui::Separator();
		float tm = App->tm->GetTimeMultiplier();
		if (ImGui::SliderFloat("timeMultiplier", &tm, 0, TIMEMULTIPLIER_LIMIT, "%.0f", TIMEMULTIPLIER_STEP)) {
			App->tm->SetTimeMultiplier((int)tm);
		}
		ImGui::Text("Clock:");
		char realclock[10];
		char gameclock[10];

		strcpy_s(realclock, std::to_string(App->tm->GetRealTimeSc()).c_str());
		strcpy_s(gameclock, std::to_string(App->tm->GetGameTimeSc()).c_str());

		int realclocki = App->tm->GetRealTimeSc();
		int gameclocki = App->tm->GetGameTimeSc();

		ImGui::Text("Real:");
		ImGui::SameLine();
		ImGui::Text(realclock);
		ImGui::SameLine();
		ImGui::Text("Game:");
		ImGui::SameLine();
		ImGui::Text(gameclock);
		ImGui::SameLine();
		ImGui::Checkbox("Hide", &Active);
		ImGui::End();
	}

