#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"

#include "Wwise/IO/Win32/AkFilePackageLowLevelIOBlocking.h"
#include "GameObject.h"
#include "ModuleInput.h"
#include ".\mmgr\mmgr.h"
#include "Wwise.h"
#include <corecrt_wstring.h>
#include "ModuleCamera3D.h"
#include "Wwise.h"
#include "ModuleJson.h"
#include "SoundBank.h"
#include "ModuleImGui.h"
#include "Listener.h"

//CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

#define BANK_BASE_PATH "SoundBanks/"


ModuleAudio::ModuleAudio(bool start_enabled) : Module(start_enabled)
{
	SetName("Audio Engine");
}

ModuleAudio::~ModuleAudio()
{

}

bool ModuleAudio::Init(JSON_File * config)
{

	LOG_OUT("Loading Wwished library");

	//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	//std::wstring base_path = converter.from_bytes("SoundBanks");

	bool ret = Wwise::InitWwise("English(US)");

	//Wwise::LoadBank("SoundBanks/Test.bnk");

	LoadSoundBank("Blend.bnk");

	
	return true;
}

bool ModuleAudio::Start()
{

	return true;
}

update_status ModuleAudio::PreUpdate(float dt)
{
	
	SetRTPvalue("Volume", volume);

	return UPDATE_CONTINUE;
		
}


update_status ModuleAudio::PostUpdate(float dt)
{

	AK::SoundEngine::RenderAudio();
	
	return UPDATE_CONTINUE;
}

bool ModuleAudio::CleanUp()
{
	LOG_OUT("Unloading Wwished library");
	delete camera_listener;
	if (soundbank != nullptr) {
		delete soundbank;
	}

	for (list<Wwise::SoundObject*>::iterator it = sound_obj.begin(); it != sound_obj.end(); ++it) {

		delete (*it);
	}
	return Wwise::CloseWwise();

	return true;
}

SoundBank* ModuleAudio::LoadSoundBank(string path)
{
	SoundBank* new_bank = new SoundBank();
	string bank_path = BANK_BASE_PATH + path;
	Wwise::LoadBank(bank_path.c_str());

	std::string json_file = bank_path.substr(0, bank_path.find_last_of('.')) + ".json"; // Changing .bnk with .json
	GetBankInfo(json_file,new_bank);
	soundbanks.push_back(new_bank);
	soundbank = new_bank;
	return new_bank;
}

unsigned int ModuleAudio::GetBankInfo(string path, SoundBank* &bank)
{

	unsigned int ret = 0;
	JSON_File * bank_file = App->json->LoadJSON(path.c_str());
	bank_file->RootObject();

	if (bank_file == nullptr) {
		LOG_OUT("Error reading bank json file");
	}
	else {
		bank_file->ChangeObject("SoundBanksInfo");
		int n_banks = bank_file->ArraySize("SoundBanks");
		for (int i = 0; i < n_banks; i++) {
			bank_file->RootObject();
			bank_file->ChangeObject("SoundBanksInfo");
			bank_file->MoveToInsideArray("SoundBanks", i);
			ret = bank->id = bank_file->GetNumber("Id");
			bank->name = bank_file->GetString("ShortName");
			bank->path = bank_file->GetString("Path");

			//bank_file->RootObject();
			int n_events = bank_file->ArraySize("IncludedEvents");
			for (int j = 0; j < n_events; j++) {
				
				//create new event and load it
				AudioEvent* new_event = new AudioEvent();
				bank_file->RootObject();
				bank_file->ChangeObject("SoundBanksInfo");
				bank_file->MoveToInsideArray("SoundBanks", i);
				new_event->Load(bank_file, bank, j);
				bank->events.push_back(new_event);
				
			}
		}

	}
	return ret;
}


Wwise::SoundObject * ModuleAudio::CreateSoundObject(const char * name, float3 position)
{
	Wwise::SoundObject* ret = Wwise::CreateSoundObj(last_go_id++, name, position.x, position.y, position.z);
	sound_obj.push_back(ret);

	return ret;
}

Wwise::SoundObject * ModuleAudio::CreateListener(const char * name, math::float3 position)
{
	Wwise::SoundObject* ret;

	if (!listener_created) {

		float3 cam_up = App->camera->GetCurrentCamera()->GetUp();
		float3 cam_front = App->camera->GetCurrentCamera()->GetFront();
		float3 cam_pos = App->camera->GetCurrentCamera()->GetPos();

		ret = Wwise::CreateSoundObj(0, "Listener", cam_pos.x, cam_pos.y, cam_pos.z, true);
		ret->SetPosition(cam_pos.x, cam_pos.y, cam_pos.z, cam_front.x, cam_front.y, cam_front.z, cam_up.x, cam_up.y, cam_up.z);
		
		sound_obj.push_back(ret);
		listener_created = true;
		
	}
	else {
		LOG_OUT("It exist a listener already!");
		ret = nullptr;
	}

	return ret;
}

void ModuleAudio::SetRTPvalue(const char * rtpc, float value)
{
	AK::SoundEngine::SetRTPCValue(rtpc, value);
}

void ModuleAudio::StopAllEvents()
{
	for (int i = 0; i < soundbank->events.size(); i++) {
		AK::SoundEngine::ExecuteActionOnEvent(soundbank->events[i]->name.c_str(), AK::SoundEngine::AkActionOnEventType::AkActionOnEventType_Pause);
	}
}

void ModuleAudio::ImGuiDraw()
{
	if (ImGui::CollapsingHeader("Audio Engine")) {
		ImGui::SliderInt("Volume", &volume, 0, 100);
	}
}










































