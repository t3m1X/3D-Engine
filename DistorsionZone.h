#pragma once
#ifndef _DISTORSIONZONE_H_
#define _DISTORSIONZONE_H_

#define DEFAULT_RADIUS 10

#include "Component.h"
#include "Geomath.h"

class DistorsionZone : public Component {

public:

	DistorsionZone(GameObject* own);
	DistorsionZone();
	void Update();
	void DebugDraw();

private:

	Sphere zone;
	float radius;
};





#endif
