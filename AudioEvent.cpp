#include "AudioEvent.h"

AudioEvent::AudioEvent()
{
}

AudioEvent::~AudioEvent()
{
}

void AudioEvent::UnLoad()
{
}

void AudioEvent::Load(JSON_File * file, SoundBank * p)
{
	this->id = file->GetNumber("Id");
	this->name = file->GetString("Name");
	this->parent = p;
}

