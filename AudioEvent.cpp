#include "AudioEvent.h"
#include "ModuleImGui.h"

AudioEvent::AudioEvent()
{
}

AudioEvent::~AudioEvent()
{
}

void AudioEvent::UnLoad()
{
}

void AudioEvent::Load(JSON_File * file, SoundBank * p, int id)
{
	bool succes = file->MoveToInsideArray("IncludedEvents", id);
	if (succes) {
		LOG_OUT("Can be readed");
	}
	this->id = file->GetNumber("Id");
	this->name = file->GetString("Name");
	this->parent = p;
}

void AudioEvent::UIDraw(Wwise::SoundObject* obj)
{
	if (ImGui::CollapsingHeader(name.c_str())) {
		if (ImGui::Button("Play")) {
			//play event
			obj->PlayEvent(name.c_str());
		}
	}
}

