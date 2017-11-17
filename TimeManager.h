#pragma once
#ifndef __MODULE_GAME_H__
#define __MODULE_GAME_H__

#include "Module.h"
#include "Timer.h"

#define TIMEMULTIPLIER_LIMIT 3.0f
#define TIMEMULTIPLIER_STEP	1.0f

enum GameState {
	IN_EDITOR = 0,
	IN_PLAY,
	UNKNOWN
};

class TimeManager : public Module {
public:
	TimeManager(bool start_enabled = true);
	~TimeManager();
public:	

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();


public:
	update_status UpdateGame(float dt);
	update_status NextFrame();
public: 
	float SetTimeMultiplier(float tM);
	float GetTimeMultiplier() const {
		return timeMultiplier;
	}
	void SetGameState(uint gs);
	int GetGameState() const {
		return gameState;
	}
	void SetPaused(bool flag) {
		Paused = flag;
	}
	bool GetPaused() const {
		return Paused;
	}

	int GetRealTime();
	int GetGameTime();
	int GetRealTimeSc();
	int GetGameTimeSc();

private:
	uint gameState = IN_EDITOR;
	uint tmp_gamestate = IN_EDITOR;
	float timeMultiplier = 1.0f;
	float dTime = 0;
	Timer RealTimer;
	Timer GameTimer;
public:
	bool Paused = false;
};

#endif 
