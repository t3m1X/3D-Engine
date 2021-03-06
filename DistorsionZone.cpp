#include "DistorsionZone.h"
#include "Transform.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleRenderer3D.h"


DistorsionZone::DistorsionZone(GameObject * own) : Component(own)
{
	Setname("Distorsion Zone");
	SetType(DIST_ZONE);
	zone.SetNegativeInfinity();
	
	zone.minPoint = own->GetBoundingBox().minPoint;
	zone.maxPoint = own->GetBoundingBox().maxPoint;
	
}

DistorsionZone::DistorsionZone()
{
}

void DistorsionZone::Update()
{
/*
	zone.minPoint = owner->GetBoundingBox().minPoint;
	zone.maxPoint = owner->GetBoundingBox().maxPoint;

	//check collision with sound objects and modify rtpc value if so
	//add aabb to the SoundObject to check the collision easily

	if (App->audio->default_listener != nullptr) {
		if (zone.Intersects(App->audio->default_listener->box)) {
			//set rtpc value
			AkReal32 nDryRatio = 0.5f;
			AkReal32 nHangarRatio = 0.25f;
			AkReal32 nTunnelRatio = 0.25f;

			AkAuxSendValue aEnvs[2];
			aEnvs[0].listenerID = App->audio->default_listener->GetID(); // Use the same set of listeners assigned via the SetListeners/SetDefaultListeners API.
			aEnvs[0].auxBusID = AK::SoundEngine::GetIDFromString("Reverb");
			aEnvs[0].fControlValue = nHangarRatio;
			aEnvs[1].listenerID = App->audio->default_listener->GetID();// Use the same set of listeners assigned via the SetListeners/SetDefaultListeners API.
			aEnvs[1].auxBusID = AK::SoundEngine::GetIDFromString("Reverb");
			aEnvs[1].fControlValue = nTunnelRatio;

			AK::SoundEngine::SetGameObjectOutputBusVolume(App->audio->def, nDryRatio);
			AK::SoundEngine::SetGameObjectAuxSendValues(GAME_OBJECT_ID_HUMAN, aEnvs, 2);

		}
	}
	*/
}

void DistorsionZone::DebugDraw()
{
	float3 corners[8];
	AABB box = zone;
	box.GetCornerPoints(corners);
	const int s = 24;

	float3* lines = new float3[s];
	float3* colors = new float3[s];

	lines[0] = float3(corners[0].x, corners[0].y, corners[0].z);
	lines[1] = float3(corners[2].x, corners[2].y, corners[2].z);

	lines[2] = float3(corners[2].x, corners[2].y, corners[2].z);
	lines[3] = float3(corners[6].x, corners[6].y, corners[6].z);

	lines[4] = float3(corners[4].x, corners[4].y, corners[4].z);
	lines[5] = float3(corners[6].x, corners[6].y, corners[6].z);

	lines[6] = float3(corners[4].x, corners[4].y, corners[4].z);
	lines[7] = float3(corners[0].x, corners[0].y, corners[0].z);

	//

	lines[8] = float3(corners[1].x, corners[1].y, corners[1].z);
	lines[9] = float3(corners[3].x, corners[3].y, corners[3].z);

	lines[10] = float3(corners[3].x, corners[3].y, corners[3].z);
	lines[11] = float3(corners[7].x, corners[7].y, corners[7].z);

	lines[12] = float3(corners[5].x, corners[5].y, corners[5].z);
	lines[13] = float3(corners[7].x, corners[7].y, corners[7].z);

	lines[14] = float3(corners[5].x, corners[5].y, corners[5].z);
	lines[15] = float3(corners[1].x, corners[1].y, corners[1].z);

	//

	lines[16] = float3(corners[0].x, corners[0].y, corners[0].z);
	lines[17] = float3(corners[1].x, corners[1].y, corners[1].z);

	lines[18] = float3(corners[2].x, corners[2].y, corners[2].z);
	lines[19] = float3(corners[3].x, corners[3].y, corners[3].z);

	lines[20] = float3(corners[4].x, corners[4].y, corners[4].z);
	lines[21] = float3(corners[5].x, corners[5].y, corners[5].z);

	lines[22] = float3(corners[6].x, corners[6].y, corners[6].z);
	lines[23] = float3(corners[7].x, corners[7].y, corners[7].z);

	for (int i = 0; i < s; i++)
	{
		colors[i] = float3(60, 60, 1);
	}

	//	DrawLinesList(lines, s, 5, colors);

	glLineWidth((float)5);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, (float*)lines->ptr());

	if (colors != nullptr)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, (float*)colors->ptr());
	}

	glDrawArrays(GL_LINES, 0, s);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glLineWidth(1);

	delete[] lines;
	delete[] colors;
}

