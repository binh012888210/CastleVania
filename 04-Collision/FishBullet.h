#pragma once
#include "GameObject.h"

#define FISHMEN_BULLET_SPEED_X	0.15f

#define FISH_BULLET_RIGHT 0
#define FISH_BULLET_LEFT 1

#define FISH_BULLET_BBOX_WIDTH 14
#define FISH_BULLET_BBOX_HEIGHT 14

class CFishBullet :public CGameObject
{
public:
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};

