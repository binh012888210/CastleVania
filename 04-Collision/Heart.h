#pragma once
#include "GameObject.h"
#include "Brick.h"
#include "BrickStair.h"

#define HEART_BBOX_WIDTH  26
#define HEART_BBOX_HEIGHT 20


class CHeart : public CGameObject
{
public:
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};

