#include "AudioSource.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "Transform.h"
#include "ModuleImGui.h"

AudioSource::AudioSource(GameObject* own) : Component(own)
{
	//this->id = RandomNumber();
	//App->audio->RegisterGO(this->id);
	Setname(own->GetName());
	SetType(AUDIO_SOURCE);
	Transform* trans = (Transform*)own->FindComponentbyType(TRANSFORM);

	obj = App->audio->CreateSoundObject(own->GetName(), trans->GetPosition());

}


AudioSource::~AudioSource()
{
	//App->audio->UnRegisterGO(id);
}

void AudioSource::Update()
{
	Transform* trans = (Transform*)GetOwner()->FindComponentbyType(TRANSFORM);


	if (trans)
	{


		float3 pos = trans->GetPosition();
		Quat rot = trans->GetRotation();

		float3 up = rot.Transform(float3(0, 1, 0));
		float3 front = rot.Transform(float3(0, 0, 1));

		up.Normalize();
		front.Normalize();

		obj->SetPosition(-pos.x, pos.y, pos.z, -front.x, front.y, front.z, -up.x, up.y, up.z);
	}
}

void AudioSource::PlayMusic(double audio_id)
{
	AK::SoundEngine::PostEvent(audio_id, obj->GetID(), AK_EnableGetMusicPlayPosition);
}

void AudioSource::PlayMusic(const char * name)
{
	AK::SoundEngine::PostEvent(name, obj->GetID(), AK_EnableGetMusicPlayPosition);
}

void AudioSource::PlayEvent(uint id)
{
	AK::SoundEngine::PostEvent(id, obj->GetID());
}

void AudioSource::PlayEvent(const char * event_name)
{
	if (obj == nullptr) {
		LOG_OUT("Nullptr");
	}
	obj->PlayEvent(event_name);
	LOG_OUT("Event played: %s", event_name);
}

void AudioSource::StopEvent(uint id)
{
}

AkGameObjectID AudioSource::GetID() const
{
	return obj->GetID();
}

void AudioSource::UI_draw()
{
	if(events.empty())
	GetEvents();

	if(ImGui::CollapsingHeader("Audio Source")) {
		
			for (int i = 0; i < events.size(); i++) {
				events[i]->UIDraw(obj);
			
		}
	}
}

void AudioSource::GetEvents()
{
	if (soundbank != nullptr) {
		for (int i = 0; i < soundbank->events.size(); i++) {

			events.push_back(soundbank->events[i]);

		}
	}
}

