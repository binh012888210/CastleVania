#pragma once
#include "GameObject.h"
#include "Brick.h"

#define BOOMERANG_BBOX_WIDTH  29
#define BOOMERANG_BBOX_HEIGHT 29

class CBoomearang : public CGameObject
{
public:
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};