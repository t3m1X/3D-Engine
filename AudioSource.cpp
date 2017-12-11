#include "AudioSource.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleCamera3D.h"

AudioSource::AudioSource(GameObject * own) : Component(own)
{
	id = RandomNumber();
	App->audio->RegisterGO(id);
	Setname("AudioSource");
	SetType(AUDIO_SOURCE);

}

AudioSource::AudioSource()
{
}

AudioSource::~AudioSource()
{
	App->audio->UnRegisterGO(id);
}

void AudioSource::Update()
{
	float3 front = App->camera->GetCurrentCamera()->GetFront();
	float3 up = App->camera->GetCurrentCamera()->GetUp();
	float3 pos = App->camera->GetCurrentCamera()->GetPos();

	AkListenerPosition ak_pos;
	ak_pos.Set(pos.x, pos.z, pos.y, front.x, front.z, front.y, up.x, up.z, up.y);

	AK::SoundEngine::SetPosition(id, ak_pos);

}

void AudioSource::PlayMusic(double audio_id)
{
	AK::SoundEngine::PostEvent(audio_id, id, AK_EnableGetMusicPlayPosition);
}

void AudioSource::PlayMusic(const char * name)
{
	AK::SoundEngine::PostEvent(name, id, AK_EnableGetMusicPlayPosition);
}

void AudioSource::PlayEvent(uint id)
{
	AK::SoundEngine::PostEvent(id, this->id);
}

void AudioSource::PlayEvent(const char * event_name)
{
	AK::SoundEngine::PostEvent(event_name, (AkGameObjectID)this->id);
}

void AudioSource::StopEvent(uint id)
{
}

double AudioSource::GetID() const
{
	return id;
}
