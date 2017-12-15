#include "TimeManager.h"
#include "ModuleInput.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleSceneIntro.h"

TimeManager::TimeManager(bool start_enabled)
{
	SetName("Time Manager");
}

TimeManager::~TimeManager()
{
}

bool TimeManager::Init()
{
	RealTimer.Start();
	return true;
}

bool TimeManager::Start()
{
	SetGameState(IN_EDITOR);
	return true;
}

update_status TimeManager::PreUpdate(float dt)
{
	dTime = dt;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && gameState == IN_PLAY) {
		SetGameState(IN_EDITOR);
	}

	switch (gameState) {
	case IN_EDITOR:
		break;
	case IN_PLAY:
		break;
	default:
		LOG_OUT("Unknown Game State --- Returning to [Editor] state");
		gameState = IN_EDITOR;
		break;
	}

	if (Paused)
		GameTimer.Pause();
	else
		GameTimer.UnPause();

	return UPDATE_CONTINUE;
}

update_status TimeManager::Update(float dt)
{
	if (gameState == IN_PLAY) {	// GameState: Play
		for (int i = 0; i < (Paused ? 0 : timeMultiplier); i++)
			UpdateGame(dt);
	}
	return UPDATE_CONTINUE;
}

update_status TimeManager::PostUpdate(float dt)
{
	/*if (tmp_gamestate != gameState) { // if Game State just Changed

		switch (gameState) {
		case IN_EDITOR:			// To Editor
			GameTimer.Stop();
		//	App->fs->FileDelete(App->scene_intro->LoadScene("__tmp__playsavescene"));
			break;
		case IN_PLAY:			// To Play
			GameTimer.Start();
			//App->scene_intro->Serialize("__tmp__playsavescene");
			break;
		default:						
			break;
		}

		tmp_gamestate = gameState;
	}
	*/
	return UPDATE_CONTINUE;
}

bool TimeManager::CleanUp()
{
	return true;
}

update_status TimeManager::UpdateGame(float dt)
{
	//LOG_OUT("GAME UPDATE at speed x %.0f", timeMultiplier);

	return UPDATE_CONTINUE;
}

update_status TimeManager::NextFrame()
{
	
	return UpdateGame(dTime);
}

float TimeManager::SetTimeMultiplier(float tM)
{
	if (tM > TIMEMULTIPLIER_LIMIT)
		tM = TIMEMULTIPLIER_LIMIT;
	if (tM < 0)
		tM = 0;

	timeMultiplier = tM;

	GameTimer.SetMultiplier(timeMultiplier);

	return timeMultiplier;
}

void TimeManager::SetGameState(uint gs)
{
	if (gs < UNKNOWN)
		gameState = gs;
	else
		LOG_OUT("ERROR Setting GameState: [Unknown GameState]");
}

int TimeManager::GetRealTime()
{
	return (int)RealTimer.Read();
}

int TimeManager::GetGameTime()
{
	return (int)GameTimer.Read();
}

int TimeManager::GetRealTimeSc()
{
	return (int)RealTimer.ReadSc();
}

int TimeManager::GetGameTimeSc()
{
	return (int)GameTimer.ReadSc();
}
