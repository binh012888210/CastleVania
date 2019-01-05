#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  32
#define BRICK_BBOX_HEIGHT 16

class CBrickWater : public CGameObject
{
public:
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};

