#pragma once
#include "GameObject.h"

#define START_BUTTON_STATE_IDLE 100
#define START_BUTTON_STATE_PRESS 200

#define START_BUTTON_ANI_IDLE 0
#define START_BUTTON_ANI_PRESS 1

#define TIME_LIFE_AFTER_PRESS_START_BUTTON 1000

class CStartButton : public CGameObject
{
public:
	int timeLife = TIME_LIFE_AFTER_PRESS_START_BUTTON;
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
};



