#pragma once

#include "GameObject.h"
#include "Brick.h"

#define HOLY_WATER_BBOX_WIDTH  32
#define HOLY_WATER_BBOX_HEIGHT 32
class CHolyWater : public CGameObject
{
public:
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};





