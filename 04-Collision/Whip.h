#pragma once

#include "GameObject.h"
#include "Brick.h"

#define WHIP_BBOX_WIDTH  34
#define WHIP_BBOX_HEIGHT 34

class CWhip : public CGameObject
{
public:
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};






