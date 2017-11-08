#include "Camera.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

Camera3D::Camera3D()
{
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);

	frustum.SetPos(float3::zero);
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	SetNearPlaneDistance(0.1f);
	SetFarPlaneDistance(300.0f);
	SetFOV(DEGTORAD * 60.0f);
	SetAspectRatio(1.3f);
	//SetFOV(30);
}

Camera3D::~Camera3D()
{
}

void Camera3D::SetPosition(const float3 & pos)
{
	frustum.SetPos(pos);
}

const float3 Camera3D::GetPosition()
{
	return frustum.Pos();
}

void Camera3D::SetZDir(const float3 & front)
{
	frustum.SetFront(front.Normalized());
}

void Camera3D::SetYDir(const float3 & front)
{
	frustum.SetUp(front.Normalized());
}

void Camera3D::GetCorners(float3 * corners)
{
	frustum.GetCornerPoints(corners);
}

void Camera3D::SetNearPlaneDistance(const float & set)
{
	if (set > 0 && set < frustum.FarPlaneDistance())
		frustum.SetViewPlaneDistances(set, frustum.FarPlaneDistance());
}

void Camera3D::SetFarPlaneDistance(const float & set)
{
	if (set > 0 && set > frustum.NearPlaneDistance())
		frustum.SetViewPlaneDistances(frustum.NearPlaneDistance(), set);
}

void Camera3D::SetFOV(const float & set)
{
	if (set > 0)
	{
		vertical_fov = DEGTORAD * set;
		frustum.SetVerticalFovAndAspectRatio(vertical_fov, aspect_ratio);
		SetAspectRatio(aspect_ratio);
	}
}

void Camera3D::SetAspectRatio(const float & set)
{
	if (set > 0)
	{
		aspect_ratio = set;

		float HorizontalFov = 2.f * atanf(tanf(frustum.VerticalFov() * 0.5f) * aspect_ratio);
		frustum.SetHorizontalFovAndAspectRatio(HorizontalFov, set);
	}
}

const float Camera3D::GetNearPlaneFistance() const
{
	return frustum.NearPlaneDistance();
}

const float Camera3D::GetFarPlaneDistance() const
{
	return frustum.FarPlaneDistance();
}

const float Camera3D::GetVerticalFOV() const
{
	return frustum.VerticalFov() * RADTODEG;
}

const float Camera3D::GetHorizontalFOV() const
{
	return frustum.HorizontalFov() * RADTODEG;
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
	return frustum;
}

void Camera3D::MoveForward(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement += frustum.Front() * speed;
	frustum.Translate(movement);
}

void Camera3D::MoveBackwards(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement -= frustum.Front() * speed;
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
	float3 distance = frustum.Pos() - rotate_center;

	Quat X(frustum.WorldRight(), motion_y);
	Quat Y(frustum.Up(), motion_x);

	distance = X.Transform(distance);
	distance = Y.Transform(distance);

	frustum.SetPos(distance + rotate_center);
}

void Camera3D::Rotate(const float & motion_x, const float & motion_y)
{
	Quat rotation_x = Quat::RotateY(motion_x);
	frustum.SetFront(rotation_x.Mul(frustum.Front()).Normalized());
	frustum.SetUp(rotation_x.Mul(frustum.Up()).Normalized());

	Quat rotation_y = Quat::RotateAxisAngle(frustum.WorldRight(), motion_y);
	frustum.SetFront(rotation_y.Mul(frustum.Front()).Normalized());
	frustum.SetUp(rotation_y.Mul(frustum.Up()).Normalized());
}

void Camera3D::Look(const float3 & look_pos)
{
	float3 dir = look_pos - frustum.Pos();

	float3x3 direction_matrix = float3x3::LookAt(frustum.Front(), dir.Normalized(), frustum.Up(), float3::unitY);

	frustum.SetFront(direction_matrix.MulDir(frustum.Front()).Normalized());
	frustum.SetUp(direction_matrix.MulDir(frustum.Up()).Normalized());
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
	float3 dir = frustum.Pos() - focus_center;
	frustum.SetPos(dir.Normalized() * distance);

	Look(focus_center);
}
