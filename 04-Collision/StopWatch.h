#pragma once

#include "GameObject.h"
#include "Brick.h"


#define STOP_WATCH_BBOX_WIDTH  34
#define STOP_WATCH_BBOX_HEIGHT 34

class CStopWatch : public CGameObject
{
public:
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};

