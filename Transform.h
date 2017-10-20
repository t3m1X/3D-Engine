#ifndef __TRANSFORM__H__
#define __TRANSFORM__H__

#include "Component.h"
#include "glmath.h"
#include "MathGeoLib\include\MathGeoLib.h"

class Transform : public Component {

	public:

		Transform(float3 s, Quat rot, float3 pos, GameObject* own);
		Transform(float4x4 trans, GameObject* own);
		virtual ~Transform();

		void SetPosition(float3 p);
		void SetPosition(float x, float y, float z);

		void SetScale(float3 s);
		void SetScale(float x, float y, float z);

		void SetRotation(Quat rot);
		void UI_draw();

		const float4x4 GetLocalTransform() const;
		void SetLocalTransform(const float4x4& transform);
		const float4x4 GetGlobalTransform() const;
		void RecalculateLocalTransform();

	private:
		float4x4 local_transform;
		float3 position;
		float3 scale;
		Quat rotation;
		float3 Euler_rotation;
		float4x4 global_transform;



};





#endif
