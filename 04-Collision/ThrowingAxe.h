#pragma once
#include "GameObject.h"
#include "Brick.h"

#define THROWING_AXE_BBOX_WIDTH  30
#define THROWING_AXE_BBOX_HEIGHT 30
class CThrowingAxe : public CGameObject
{
public:
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};
