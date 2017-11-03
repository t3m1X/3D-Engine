#include "Transform.h"
#include "ModuleImGui.h"

Transform::Transform(float3 s, Quat rot, float3 pos , GameObject* own) : Component(own)
{
	local_transform.SetIdentity();
	scale = s;
	rotation = rot;
	position = pos;
	Euler_rotation = rot.ToEulerXYZ();
	global_transform.SetIdentity();

	this->SetType(TRANSFORM);
	
}

Transform::Transform(float4x4 trans, GameObject * own) : Component(own)
{
	float3x3 rot;
	local_transform.Set(trans);
	local_transform.Decompose(position, rot, scale);
	Euler_rotation = rot.ToEulerXYZ();
	rotation.FromEulerXYZ(Euler_rotation.x, Euler_rotation.y, Euler_rotation.z);
}

Transform::~Transform()
{
}

void Transform::SetPosition(float3 p)
{
	position = p;

	RecalculateLocalTransform();
}

void Transform::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;


	RecalculateLocalTransform();
}

void Transform::SetScale(float3 s)
{
	scale = s;
	RecalculateLocalTransform();
}

void Transform::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
	RecalculateLocalTransform();
}

void Transform::SetRotation(Quat rot)
{
	rotation = rot;
	RecalculateLocalTransform();
}

void Transform::UI_draw()
{
	if (ImGui::CollapsingHeader("Transform")) {
		float pos[3];
		float rot[3];
		float sca[3];

		pos[0] = position.x;
		pos[1] = position.y;
		pos[2] = position.z;

		rot[0] = RadToDeg(rotation.ToEulerXYZ().x);
		rot[1] = RadToDeg(rotation.ToEulerXYZ().y);
		rot[2] = RadToDeg(rotation.ToEulerXYZ().z);

		sca[0] = scale.x;
		sca[1] = scale.y;
		sca[2] = scale.z;

		if (ImGui::DragFloat3("Position:", pos, 0.1f)) {
			position.x = pos[0];
			position.y = pos[1];
			position.z = pos[2];
		}
		if (ImGui::DragFloat3("Rotation:", rot, 0.1f)) {
			rotation = rotation.FromEulerXYZ(DegToRad(rot[0]), DegToRad(rot[1]), DegToRad(rot[2]));
		}

		if (ImGui::DragFloat3("Scale:", sca, 0.1f)) {
			scale.x = sca[0];
			scale.y = sca[1];
			scale.z = sca[2];
		}
	}
	
}

const float4x4 Transform::GetLocalTransform() const
{
	return float4x4::FromTRS(position, rotation, scale);
}
void Transform::SetLocalTransform(const float4x4 & transform)
{
	local_transform = transform;
}

const float4x4 Transform::GetGlobalTransform() const
{
	return global_transform;
}

void Transform::RecalculateLocalTransform()
{
	local_transform = float4x4::FromTRS(position, rotation, scale);
}

float3 Transform::GetPosition() const
{
	return position;
}
