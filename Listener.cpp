#include "Listener.h"
#include "Camera.h"
#include "ModuleAudio.h"
#include "Application.h"
#include "Transform.h"

Listener::Listener(GameObject * own) : Component(own)
{
	Setname("Audio Listener");
	SetType(LISTENER);

	Transform* trans = (Transform*)own->FindComponentbyType(TRANSFORM);

	obj = App->audio->CreateListener(own->GetName(), trans->GetPosition());
}

Listener::Listener()
{

}

Listener::~Listener()
{

}

void Listener::Update()
{
	//Update Listener Positioin

	Transform* trans = (Transform*)GetOwner()->FindComponentbyType(TRANSFORM);


	if (trans!=nullptr)
	{


		float3 pos = trans->GetPosition();
		Quat rot = trans->GetRotation();

		float3 up = rot.Transform(float3(0, 1, 0));
		float3 front = rot.Transform(float3(0, 0, 1));

		up.Normalize();
		front.Normalize();

		obj->SetPosition(pos.x, pos.y, pos.z, front.x, front.y, -front.z, up.x, up.y, up.z);
	}


}

AkGameObjectID  Listener::GetId() const
{
	return obj->GetID();
}
