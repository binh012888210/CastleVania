#pragma once
#include "Brick.h"
#include "GameObject.h"

#define DAGGER_BBOX_WIDTH  34
#define DAGGER_BBOX_HEIGHT 18

class CDagger : public CGameObject
{
public:
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};

