#include "Transform.h"
#include "ModuleImGui.h"

Transform::Transform(float3 s, Quat rot, float3 pos , GameObject* own) : Component(own)
{
	transform.SetIdentity();
	scale = s;
	rot = rotation;
	position = pos;
	Euler_rotation = rot.ToEulerXYZ();

	this->SetType(TRANSFORM);
	
}

Transform::Transform(float4x4 trans, GameObject * own) : Component(own)
{
	float3x3 rot;
	transform.Set(trans);
	transform.Decompose(position, rot, scale);
	Euler_rotation = rot.ToEulerXYZ();
	rotation.FromEulerXYZ(Euler_rotation.x, Euler_rotation.y, Euler_rotation.z);
}

Transform::~Transform()
{
}

void Transform::SetPosition(float3 p)
{
	position = p;
}

void Transform::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void Transform::SetScale(float3 s)
{
	scale = s;
}

void Transform::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
}

void Transform::SetRotation(Quat rot)
{
	rotation = rot;
}

void Transform::UI_draw()
{
	if (ImGui::CollapsingHeader("Transform")) {
		ImGui::Text("Position");
		ImGui::InputFloat("x", &position.x);
		ImGui::InputFloat("y", &position.y);
		ImGui::InputFloat("z", &position.z);
		ImGui::Separator();
		ImGui::Text("Rotation:");
		ImGui::InputFloat("x", &Euler_rotation.x);
		ImGui::InputFloat("y", &Euler_rotation.y);
		ImGui::InputFloat("z", &Euler_rotation.z);
		ImGui::Separator();
		ImGui::Text("Scale");
		ImGui::InputFloat("x", &scale.x);
		ImGui::InputFloat("y", &scale.y);
		ImGui::InputFloat("z", &scale.z);
	}
	
}
