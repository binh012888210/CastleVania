#pragma once
#include "GameObject.h"
#define CLOUD_FLY_SPEED_X -0.08f

#define CLOUD_ANI_FLY 0
class CCloud : public CGameObject
{
public:
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
};