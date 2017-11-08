#include "Camera.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

Camera3D::Camera3D()
{
	frustum.type = FrustumType::PerspectiveFrustum;

	frustum.pos = float3::zero;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;

	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 1000.0f;
	frustum.verticalFov = DEGTORAD * 60.0f;
	SetAspectRatio(1.3f);

	
	
	
	//SetFOV(30);
}

Camera3D::~Camera3D()
{
}

void Camera3D::SetPosition(const float3 & pos)
{
	frustum.pos = pos;
}

const float3 Camera3D::GetPosition()
{
	return frustum.pos;
}

void Camera3D::SetZDir(const float3 & front)
{
	frustum.front = front.Normalized();
}

void Camera3D::SetYDir(const float3 & front)
{
	frustum.up = front.Normalized();
}

void Camera3D::GetCorners(float3 * corners)
{
	frustum.GetCornerPoints(corners);
}

void Camera3D::SetNearPlaneDistance(const float & set)
{
	if (set > 0 && set < frustum.farPlaneDistance)
		frustum.nearPlaneDistance = set;
}

void Camera3D::SetFarPlaneDistance(const float & set)
{
	if (set > 0 && set > frustum.nearPlaneDistance)
		frustum.farPlaneDistance = set;
}

void Camera3D::SetFOV(const float & set)
{
	float aspect_ratio = frustum.AspectRatio();

	frustum.verticalFov = DEGTORAD * set;
	SetAspectRatio(aspect_ratio);
}

void Camera3D::SetAspectRatio(const float & set)
{
	aspect_ratio = set;

	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect_ratio);
}

const float Camera3D::GetNearPlaneFistance() const
{
	return frustum.nearPlaneDistance;
}

const float Camera3D::GetFarPlaneDistance() const
{
	return frustum.farPlaneDistance;
}

const float Camera3D::GetVerticalFOV() const
{
	return frustum.verticalFov * RADTODEG;
}

const float Camera3D::GetHorizontalFOV() const
{
	return frustum.horizontalFov;
}

const float4x4 Camera3D::GetViewMatrix() const
{
	return frustum.ViewMatrix();
}

const float4x4 Camera3D::GetProjectionMatrix() const
{
	return frustum.ProjectionMatrix();
}

const float * Camera3D::GetOpenGLViewMatrix() const
{
	static float4x4 view = frustum.ViewMatrix();
	view.Transpose();
	return view.ptr();
}

const float * Camera3D::GetOpenGLProjectionMatrix() const
{
	return frustum.ProjectionMatrix().Transposed().ptr();
}

const Frustum  Camera3D::GetFrustum() const
{
	return this->frustum;
}

void Camera3D::MoveForward(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement += frustum.front * speed;
	frustum.Translate(movement);
}

void Camera3D::MoveBackwards(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement -= frustum.front * speed;
	frustum.Translate(movement);
}

void Camera3D::MoveRight(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement += frustum.WorldRight() * speed;
	frustum.Translate(movement);
}

void Camera3D::MoveLeft(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement -= frustum.WorldRight() * speed;
	frustum.Translate(movement);
}

void Camera3D::MoveUp(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement += float3::unitY * speed;
	frustum.Translate(movement);
}

void Camera3D::MoveDown(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement -= float3::unitY * speed;
	frustum.Translate(movement);
}

void Camera3D::Orbit(const float3 & rotate_center, const float & motion_x, const float & motion_y)
{
	float3 distance = frustum.pos - rotate_center;

	Quat X(frustum.WorldRight(), motion_y);
	Quat Y(frustum.up, motion_x);

	distance = X.Transform(distance);
	distance = Y.Transform(distance);

	frustum.pos = distance + rotate_center;
}

void Camera3D::Rotate(const float & motion_x, const float & motion_y)
{
	Quat rotation_x = Quat::RotateY(motion_x);
	frustum.front = rotation_x.Mul(frustum.front).Normalized();
	frustum.up = rotation_x.Mul(frustum.up).Normalized();

	Quat rotation_y = Quat::RotateAxisAngle(frustum.WorldRight(), motion_y);
	frustum.front = rotation_y.Mul(frustum.front).Normalized();
	frustum.up = rotation_y.Mul(frustum.up).Normalized();
}

void Camera3D::Look(const float3 & look_pos)
{
	float3 dir = look_pos - frustum.pos;

	float3x3 direction_matrix = float3x3::LookAt(frustum.front, dir.Normalized(), frustum.up, float3::unitY);

	frustum.front = direction_matrix.MulDir(frustum.front).Normalized();
	frustum.up = direction_matrix.MulDir(frustum.up).Normalized();
}

bool Camera3D::IsInside(AABB & bounding)
{
	int vertex_num = bounding.NumVertices();
	for (int i = 0; i < 6; i++) //planes of frustum
	{
		int points_out = 0;
		for (int j = 0; j < vertex_num; j++) //vertex number of box
		{
			Plane plane = frustum.GetPlane(i);
			if (plane.IsOnPositiveSide(bounding.CornerPoint(j)))
			{
				points_out++;
			}
		}
		//if all the points are outside of a plane, the gameobject is not inside the frustum
		if (points_out == 8) return false;
	}

	return true;
}

void Camera3D::Focus(const float3 & focus_center, const float & distance)
{
	float3 dir = frustum.pos - focus_center;
	frustum.pos = dir.Normalized() * distance;

	Look(focus_center);
}
