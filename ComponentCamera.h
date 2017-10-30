#ifndef __COMPONENTCAMERA__H__
#define __COMPONENTCAMERA__H__

#include "Camera.h"
#include "Component.h"

class ComponentCamera : public Component {

public:

	ComponentCamera(GameObject* own);
	~ComponentCamera();

	void Start();
	void Update();
	void CleanUp();
	Camera* GetCamera()const;

private:
	Camera* camera = nullptr;
};



#endif
