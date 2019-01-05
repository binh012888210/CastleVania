#include "DoubleShot.h"

void CDoubleShot::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{

}

void CDoubleShot::Render()
{
	if (isActive == true)
	{
		animations[0]->Render(x, y);
	}
}

void CDoubleShot::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + DOUBLE_SHOT_BBOX_WIDTH;
	b = y + DOUBLE_SHOT_BBOX_HEIGHT;
	if (isActive == false)
	{
		t = 512;
		b = 512;
	}
}
