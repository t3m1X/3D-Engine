#include "Listener.h"
#include "Camera.h"
#include "ModuleAudio.h"
#include "Application.h"
#include "Transform.h"
#include "ModuleJson.h"

Listener::Listener(GameObject * own) : Component(own)
{
	Setname("Audio Listener");
	SetType(LISTENER);

	Transform* trans = (Transform*)own->FindComponentbyType(TRANSFORM);

	obj = App->audio->CreateListener(own->GetName(), trans->GetPosition());
	App->audio->default_listener = this;
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

		box.minPoint = trans->GetPosition() - float3(1, 1, 1);
		box.maxPoint = trans->GetPosition() + float3(1, 1, 1);


	}


}

AkGameObjectID  Listener::GetId() const
{
	return obj->GetID();
}

void Listener::Serialize(JSON_File * doc)
{
	/*if (doc == nullptr)
		return;

	doc->SetNumber("type", type);
	doc->SetNumber("ownerUID", (owner != nullptr) ? owner->GetUID() : -1);
	doc->SetString("name", name);*/
}
