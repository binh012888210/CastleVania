#pragma once
#include "GameObject.h"

#define BATBLUE_FLY_SPEED_X1 0.1f
#define BATBLUE_FLY_SPEED_X2 0.05f
#define BATBLUE_FLY_SPEED_Y -0.1f

class CBatBlue : public CGameObject
{
public:
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
};
