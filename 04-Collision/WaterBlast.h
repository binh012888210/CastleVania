#pragma once
#include "GameObject.h"

#define WATER_BLAST_UP_SPEED 0.5
#define WATER_BLAST_SPEED_X 0.03

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

class CWaterBlast : public CGameObject
{

public:	
	float timeAnimation = 900.0f;
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};


