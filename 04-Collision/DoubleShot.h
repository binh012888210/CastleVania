#pragma once
#include "GameObject.h"

#define DOUBLE_SHOT_BBOX_WIDTH  28
#define DOUBLE_SHOT_BBOX_HEIGHT 28

class CDoubleShot : public CGameObject
{
public:
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};