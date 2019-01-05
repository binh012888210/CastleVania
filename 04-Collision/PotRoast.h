#pragma once

#include "GameObject.h"
#include "Brick.h"


#define POR_ROAST_BBOX_WIDTH  33
#define POP_ROAST_BBOX_HEIGHT 27
class CPotRoast : public CGameObject
{
public:
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};


