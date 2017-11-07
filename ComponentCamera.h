#ifndef __COMPONENTCAMERA__H__
#define __COMPONENTCAMERA__H__


#include "Component.h"

class Camera3D;

class ComponentCamera : public Component {

public:

	ComponentCamera(GameObject* own);
	~ComponentCamera();

	void Start();
	void Update();
	void CleanUp();
	Camera3D* GetCamera()const;
	bool IsInside(AABB& bounding_box);

private:
	Camera3D* camera = nullptr;
};



#endif
