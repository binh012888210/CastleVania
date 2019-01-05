#include "CheckPoint.h"

void CCheckPoint::Render()
{
	animations[0]->Render(x, y);
	//RenderBoundingBox();
}

void CCheckPoint::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + CHECK_POINT_BBOX_WIDTH;
	b = y + CHECK_POINT_BBOX_HEIGHT;
}