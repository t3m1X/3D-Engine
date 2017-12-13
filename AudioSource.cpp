#include "AudioSource.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleCamera3D.h"
#include "Transform.h"

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
		obj->SetPosition(pos.x, pos.y, pos.z);
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
	AK::SoundEngine::PostEvent(event_name, obj->GetID());
	LOG_OUT("Event played: %s", event_name);
}

void AudioSource::StopEvent(uint id)
{
}

AkGameObjectID AudioSource::GetID() const
{
	return obj->GetID();
}

