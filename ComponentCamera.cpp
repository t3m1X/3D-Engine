#include "ComponentCamera.h"
#include "Transform.h"

ComponentCamera::ComponentCamera(GameObject * own) : Component(own)
{
	Setname("Camera");
	camera = new Camera();

	
}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::Start()
{
}

void ComponentCamera::Update()
{
	
	Transform* trans = (Transform*)GetOwner()->FindComponentbyType(TRANSFORM);
	camera->SetPosition(trans->GetPosition());
	camera->SetZDir(trans->GetGlobalTransform().WorldZ());
	camera->SetYDir(trans->GetGlobalTransform().WorldY());

/*
	float3 corners[8];
	camera->GetCorners(corners);
	App->renderer3D->GetDebugDraw()->DrawFrustum(corners);*/

}

void ComponentCamera::CleanUp()
{
	delete camera;
}

Camera * ComponentCamera::GetCamera() const
{
	return camera;
}
