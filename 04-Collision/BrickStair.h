#pragma once
#include "GameObject.h"

#define BRICK_STAIR_BBOX_WIDTH  16
#define BRICK_STAIR_BBOX_HEIGHT 16

class CBrickStair :public CGameObject
{
public:
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};