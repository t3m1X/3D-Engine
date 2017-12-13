#include "AudioSource.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleCamera3D.h"

AudioSource::AudioSource(GameObject* own) : Component(own)
{
	this->id = RandomNumber();
	//App->audio->RegisterGO(this->id);
	Setname("AudioSource");
	SetType(AUDIO_SOURCE);
	n = "";


}


AudioSource::~AudioSource()
{
	//App->audio->UnRegisterGO(id);
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
	AK::SoundEngine::PostEvent(event_name, id);
	LOG_OUT("Event played: %s", event_name);
}

void AudioSource::StopEvent(uint id)
{
}

AkGameObjectID AudioSource::GetID() const
{
	return id;
}

void AudioSource::SetPosition(float x, float y, float z, float x_front, float y_front, float z_front, float x_top, float y_top, float z_top)
{
	position.X = -x;
	position.Y = y;
	position.Z = -z;

	orient_front.X = x_front;
	orient_front.Y = y_front;
	orient_front.Z = z_front;
	orient_top.X = x_top;
	orient_top.Y = y_top;
	orient_top.Z = z_top;

	float length_front = sqrt(pow(orient_front.X, 2) + pow(orient_front.Y, 2) + pow(orient_front.Z, 2));
	float length_top = sqrt(pow(orient_top.X, 2) + pow(orient_top.Y, 2) + pow(orient_top.Z, 2));

	//Normalize vectors
	orient_front.X = orient_front.X / length_front;
	orient_front.Y = orient_front.Y / length_front;
	orient_front.Z = orient_front.Z / length_front;
	orient_top.X = orient_top.X / length_top;
	orient_top.Y = orient_top.Y / length_top;
	orient_top.Z = orient_top.Z / length_top;

	//Check if the are orthogonals
	float dot_prod = orient_top.X*orient_front.X + orient_top.Y*orient_front.Y + orient_top.Z*orient_front.Z;

	if (dot_prod >= 0.0001)
		LOG_OUT("Vectors are not orthogonal!");

	AkSoundPosition sound_pos;
	sound_pos.Set(position, orient_front, orient_top);
	AKRESULT res = AK::SoundEngine::SetPosition((AkGameObjectID)id, sound_pos);
	if (res != AK_Success)
		LOG_OUT("Something went wrong. Check the res variable for more info");
}
