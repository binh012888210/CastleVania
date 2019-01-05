#pragma once

#include "GameObject.h"
#include "Brick.h"

#define INVI_POTION_BBOX_WIDTH  26
#define INVI_POTION_BBOX_HEIGHT 32

class CInvisibilityPotion : public CGameObject
{
public:
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};




