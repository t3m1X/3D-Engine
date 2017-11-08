#include "Transform.h"
#include "ModuleImGui.h"
#include "ImGui/ImGuizmo/ImGuizmo.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"

Transform::Transform(GameObject* own): Component(own)
{
	SetType(TRANSFORM);


	position = float3(0.f, 0.f, 0.f);
	rotation = Quat(0.f, 0.f, 0.f, 0.f);
	scale = float3(1.f, 1.f, 1.f);
	local_transform.SetIdentity();
	global_transform.SetIdentity();
}

Transform::Transform(float3 s, Quat rot, float3 pos , GameObject* own) : Component(own)
{
	this->SetType(TRANSFORM);
	rotation = Quat::identity;

	scale = s;
	rotation.Set(rot.x,rot.y,rot.z,rot.w);
	position = pos;
	Euler_rotation = rot.ToEulerXYZ();

	global_transform.SetIdentity();
	local_transform.SetIdentity();
	RecalculateTransform();

	
	
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

	RecalculateTransform();
}

void Transform::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;


	RecalculateTransform();
}

void Transform::SetScale(float3 s)
{
	scale = s;
	RecalculateTransform();
}

void Transform::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
	RecalculateTransform();
}

void Transform::SetRotation(Quat rot)
{
	rotation = rot;
	RecalculateTransform();
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

		if (ImGui::DragFloat3("Position:", pos, 0.1f) && !position.Equals(pos[0], pos[1], pos[2])) {
			position.x = pos[0];
			position.y = pos[1];
			position.z = pos[2];
			RecalculateTransform();
		}

		if (ImGui::DragFloat3("Rotation:", rot, 0.1f)) {
			Quat temp;
			temp = temp.FromEulerXYZ(DegToRad(rot[0]), DegToRad(rot[1]), DegToRad(rot[2]));
			if (!(temp.x == rotation.x && temp.y == rotation.y && temp.z == rotation.z && temp.w == rotation.w))
			{
				rotation = temp;
				RecalculateTransform();
			}
		}

		if (ImGui::DragFloat3("Scale:", sca, 0.1f) && !scale.Equals(sca[0], sca[1], sca[2])) {
			scale.x = sca[0];
			scale.y = sca[1];
			scale.z = sca[2];
			RecalculateTransform();
		}
	}
	
}

float4x4 Transform::GetLocalTransform() const
{
	return local_transform;
}
void Transform::SetLocalTransform(const float4x4 & transform)
{
	local_transform.Set(transform);
}

void Transform::SetGlobalTransform(const float4x4 & transform)
{
	global_transform.Set(transform);
}

float4x4 Transform::GetGlobalTransform() const
{
	return global_transform;
}

void Transform::RecalculateTransform()
{
	local_transform.Set(float4x4::FromTRS(position, rotation, scale));
	float4x4 old_global(global_transform);

	if (owner->GetParent() != nullptr) {
		Transform* parent = (Transform*)owner->GetParent()->FindComponentbyType(TRANSFORM);
		if (parent != nullptr)
			global_transform.Set(parent->GetGlobalTransform().Mul(local_transform));
		else
			global_transform.Set(local_transform);
	}
	else
		global_transform.Set(local_transform);

	//if (old_global.IsInvertible())
	//	owner->RecalculateAABB(old_global.Inverted().Mul(global_transform)); //Calculating the inversed sometimes causes the program to crash
	//else
		owner->RecalculateAABB();

	vector<GameObject*> childs = owner->GetChild();
	Transform* child_trans;
	for (int i = 0; i < childs.size(); ++i)
	{
		child_trans = (Transform*)childs[i]->FindComponentbyType(TRANSFORM);
		if (child_trans != nullptr)
			child_trans->RecalculateTransform();
	}
}

float3 Transform::GetPosition() const
{
	return position;
}

void Transform::OnGuizmo()
{
	ImGuizmo::Enable(true);

	float4x4 projMatrix = App->camera->GetEditorCamera()->GetFrustum().ViewProjMatrix().Transposed();
	float4x4 trans = global_transform.Transposed();

	ImGuiIO& io = ImGui::GetIO();

	int w, h;

	w = App->window->GetWidth();
	h = App->window->GetHeight();
	ImGuizmo::SetRect(0, 0, (float)w, float(h));

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		ImGuizmo::Manipulate(App->camera->GetViewMatrix().ptr(), projMatrix.ptr(), ImGuizmo::TRANSLATE, ImGuizmo::LOCAL, trans.ptr());
	}
	else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		ImGuizmo::Manipulate(App->camera->GetViewMatrix().ptr(), projMatrix.ptr(), ImGuizmo::ROTATE, ImGuizmo::LOCAL, trans.ptr());
	}
	else if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		ImGuizmo::Manipulate(App->camera->GetViewMatrix().ptr(), projMatrix.ptr(), ImGuizmo::SCALE, ImGuizmo::LOCAL, trans.ptr());
	}

	if (ImGuizmo::IsUsing())
	{
		
		ImGuizmo::DecomposeMatrixToComponents(trans.ptr(), (float*)position.ptr(), (float*)Euler_rotation.ptr(), (float*)scale.ptr());
		global_transform.Transpose();
		ImGuizmo::RecomposeMatrixFromComponents((float*)position.ptr(), (float*)Euler_rotation.ptr(), (float*)scale.ptr(), global_transform.ptr());
		global_transform.Transpose();
	}
//	LOG("On Guizmo!!!!");






}

void Transform::Edit()
{
	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
	if (ImGui::IsKeyPressed(90))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	if (ImGui::IsKeyPressed(69))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	if (ImGui::IsKeyPressed(82)) // r Key
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	float matrixTranslation[3], matrixRotation[3], matrixScale[3];

	ImGui::InputFloat3("Tr", matrixTranslation, 3);
	ImGui::InputFloat3("Rt", matrixRotation, 3);
	ImGui::InputFloat3("Sc", matrixScale, 3);


	if (mCurrentGizmoOperation != ImGuizmo::SCALE)
	{
		if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
			mCurrentGizmoMode = ImGuizmo::LOCAL;
		ImGui::SameLine();
		if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
			mCurrentGizmoMode = ImGuizmo::WORLD;
	}
	static bool useSnap(false);
	if (ImGui::IsKeyPressed(83))
		useSnap = !useSnap;
	ImGui::Checkbox("", &useSnap);
	ImGui::SameLine();

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
}
