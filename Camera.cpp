#include "Camera.h"
#include "Globals.h"
Camera::Camera()
{
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3(0, 0, -1);
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;
	frustum.verticalFov = 0;
	frustum.horizontalFov = 0;
	frustum.farPlaneDistance = 1.0f;
	frustum.nearPlaneDistance = 0;
	aspect_ratio = 0;

	SetNearPlaneDistance(0.1f);
	SetFarPlaneDistance(500.0f);
	SetAspectRatio(1.3f);
	SetFOV(60);
}

Camera::~Camera()
{
}

void Camera::SetPosition(const float3 & pos)
{
	frustum.pos = pos;
}

const float3 Camera::GetPosition()
{
	return frustum.pos;
}

void Camera::SetZDir(const float3 & front)
{
	frustum.front = front.Normalized();
}

void Camera::SetYDir(const float3 & front)
{
	frustum.up = front.Normalized();
}

void Camera::GetCorners(float3 * corners)
{
	frustum.GetCornerPoints(corners);
}

void Camera::SetNearPlaneDistance(const float & set)
{
	if (set > 0 && set < frustum.farPlaneDistance)
		frustum.nearPlaneDistance = set;
}

void Camera::SetFarPlaneDistance(const float & set)
{
	if (set > 0 && set > frustum.nearPlaneDistance)
		frustum.farPlaneDistance = set;
}

void Camera::SetFOV(const float & set)
{
	if (set > 0)
		frustum.verticalFov = DEGTORAD * set;

	if (aspect_ratio > 0)
		frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect_ratio); 
}

void Camera::SetAspectRatio(const float & set)
{
	aspect_ratio = set;

	if (frustum.verticalFov > 0)
		frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect_ratio);
}

const float Camera::GetNearPlaneFistance() const
{
	return frustum.nearPlaneDistance;
}

const float Camera::GetFarPlaneDistance() const
{
	return frustum.farPlaneDistance;
}

const float Camera::GetVerticalFOV() const
{
	return frustum.verticalFov * RADTODEG;
}

const float Camera::GetHorizontalFOV() const
{
	return frustum.horizontalFov;
}

const float4x4 Camera::GetViewMatrix() const
{
	return frustum.ViewMatrix();
}

const float4x4 Camera::GetProjectionMatrix() const
{
	return frustum.ProjectionMatrix();
}

const float * Camera::GetOpenGLViewMatrix() const
{
	static float4x4 view = frustum.ViewMatrix();
	view.Transpose();
	return view.ptr();
}

const float * Camera::GetOpenGLProjectionMatrix() const
{
	return frustum.ProjectionMatrix().Transposed().ptr();
}

void Camera::MoveForward(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement += frustum.front * speed;
	frustum.Translate(movement);
}

void Camera::MoveBackwards(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement -= frustum.front * speed;
	frustum.Translate(movement);
}

void Camera::MoveRight(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement += frustum.WorldRight() * speed;
	frustum.Translate(movement);
}

void Camera::MoveLeft(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement -= frustum.WorldRight() * speed;
	frustum.Translate(movement);
}

void Camera::MoveUp(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement += float3::unitY * speed;
	frustum.Translate(movement);
}

void Camera::MoveDown(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement -= float3::unitY * speed;
	frustum.Translate(movement);
}

void Camera::Orbit(const float3 & rotate_center, const float & motion_x, const float & motion_y)
{
	float3 distance = frustum.pos - rotate_center;

	Quat X(frustum.WorldRight(), motion_y);
	Quat Y(frustum.up, motion_x);

	distance = X.Transform(distance);
	distance = Y.Transform(distance);

	frustum.pos = distance + rotate_center;
}

void Camera::Rotate(const float & motion_x, const float & motion_y)
{
	Quat rotation_x = Quat::RotateY(motion_x);
	frustum.front = rotation_x.Mul(frustum.front).Normalized();
	frustum.up = rotation_x.Mul(frustum.up).Normalized();

	Quat rotation_y = Quat::RotateAxisAngle(frustum.WorldRight(), motion_y);
	frustum.front = rotation_y.Mul(frustum.front).Normalized();
	frustum.up = rotation_y.Mul(frustum.up).Normalized();
}

void Camera::Look(const float3 & look_pos)
{
	float3 dir = look_pos - frustum.pos;

	float3x3 direction_matrix = float3x3::LookAt(frustum.front, dir.Normalized(), frustum.up, float3::unitY);

	frustum.front = direction_matrix.MulDir(frustum.front).Normalized();
	frustum.up = direction_matrix.MulDir(frustum.up).Normalized();
}

void Camera::Focus(const float3 & focus_center, const float & distance)
{
	float3 dir = frustum.pos - focus_center;
	frustum.pos = dir.Normalized() * distance;

	Look(focus_center);
}
