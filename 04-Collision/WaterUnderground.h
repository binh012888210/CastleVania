#pragma once
#include "GameObject.h"
#include "WaterBlast.h"

#define WATER_BBOX_WIDTH  64
#define WATER_BBOX_HEIGHT 1

#define WATER_STATE_NORMAL 100
#define WATER_STATE_BLAST 200

#define BRICK_DESTROYABLE_ANI_NORMAL 0


class CWaterUnderground : public CGameObject
{
public:

	CWaterBlast *blastWater;
	void InitWaterBlastEffect(vector<LPGAMEOBJECT> *coObjects);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};