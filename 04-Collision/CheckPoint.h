#pragma once
#include "GameObject.h"
#define CHECK_POINT_BBOX_WIDTH  16
#define CHECK_POINT_BBOX_HEIGHT 16
class CCheckPoint : public CGameObject
{
public:
    void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};
