#pragma once

#include "GameObject.h"

#define BRICK_EFFECT_ANI 0

#define BRICK_BBOX_WIDTH  0
#define BRICK_BBOX_HEIGHT 0

#define HIT_TIME_ANIMATION 100

class CHitEffect :public CGameObject
{
public:

	int timeAnimation;
	void ResetTimeAnimation() { timeAnimation = HIT_TIME_ANIMATION; };
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};


