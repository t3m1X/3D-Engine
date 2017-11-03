#ifndef __CAMERA_H__
#define __CAMERA__H__

#include "glmath.h"
#include "Geomath.h"

class Camera3D {

public:

	Camera3D();
	~Camera3D();

	void SetPosition(const float3& pos);
	const float3 GetPosition();

	void SetZDir(const float3& front);
	void SetYDir(const float3& front);

	void GetCorners(float3* corners);

	void SetNearPlaneDistance(const float& set);
	void SetFarPlaneDistance(const float& set);
	void SetFOV(const float& set);
	void SetAspectRatio(const float& set);

	const float GetNearPlaneFistance() const;
	const float GetFarPlaneDistance() const;
	const float GetVerticalFOV() const;
	const float GetHorizontalFOV() const;
	const float4x4 GetViewMatrix() const;
	const float4x4 GetProjectionMatrix() const;
	const float* GetOpenGLViewMatrix() const;
	const float* GetOpenGLProjectionMatrix() const;

	void MoveForward(const float& speed);
	void MoveBackwards(const float& speed);
	void MoveRight(const float& speed);
	void MoveLeft(const float& speed);
	void MoveUp(const float& speed);
	void MoveDown(const float& speed);

	void Orbit(const float3& rotate_center, const float& motion_x, const float& motion_y);
	void Rotate(const float& motion_x, const float& motion_y);
	void Focus(const float3& focus_center, const float& distance);

	void Look(const float3& look_pos);
	bool IsInside(AABB& bounding);

private:

	Frustum frustum;
	float aspect_ratio = 0.0f;

};


#endif
