#pragma once
#include "GameObject.h"
#include "Brick.h"

#define CROSS_BBOX_WIDTH  34
#define CROSS_BBOX_HEIGHT 32

class CCross : public CGameObject
{
public:
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};



