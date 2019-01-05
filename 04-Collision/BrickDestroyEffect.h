#pragma once
#include "GameObject.h"

#define BRICK_EFFECT_ANI 0

#define BRICK_EFFECT_FLY_SPEED_X 0.3f
#define BRICK_EFFECT_FLY_SPEED_Y 0.15f

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

class CBrickDestroyEffect : public CGameObject
{
	float timeAnimation = 0.19f;
public:
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};


