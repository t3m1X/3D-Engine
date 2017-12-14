#include "DistorsionZone.h"
#include "Transform.h"

DistorsionZone::DistorsionZone(GameObject * own) : Component(own)
{
	Setname("Distorsion Zone");
	SetType(DIST_ZONE);
	zone.SetNegativeInfinity();

	Transform* trans = (Transform*)own->FindComponentbyType(TRANSFORM);
	zone.pos = trans->GetPosition();
	zone.r = DEFAULT_RADIUS;
}

DistorsionZone::DistorsionZone()
{
}

void DistorsionZone::Update()
{

	Transform* trans = (Transform*)GetOwner()->FindComponentbyType(TRANSFORM);
	zone.pos = trans->GetPosition();

	//check collision with sound objects and modify rtpc value if so
	//add aabb to the SoundObject to check the collision easily
}

void DistorsionZone::DebugDraw()
{

}

