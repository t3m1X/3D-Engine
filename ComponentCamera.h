#ifndef __COMPONENTCAMERA__H__
#define __COMPONENTCAMERA__H__


#include "Component.h"
#include "ModuleCamera3D.h"


class ComponentCamera : public Component {

public:

	ComponentCamera(GameObject* own);
	~ComponentCamera();

	void Start();
	void Update();
	void CleanUp();
	Camera3D* GetCamera()const;
	bool IsInside(AABB& bounding_box);
	void UI_draw();
	bool GetCulling() const;

private:
	Camera3D* camera = nullptr;
	bool culling = false;
};



#endif
