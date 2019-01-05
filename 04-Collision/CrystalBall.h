#pragma once

#include "GameObject.h"
#include "Brick.h"
#include "BrickStair.h"

#define CRYSTAL_BALL_BBOX_WIDTH  28
#define CRYSTAL_BALL_BBOX_HEIGHT 33

class CCrystalBall :public CGameObject
{
public:
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};





