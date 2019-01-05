#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

class CBrickUnderground : public CGameObject
{
public:
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};
