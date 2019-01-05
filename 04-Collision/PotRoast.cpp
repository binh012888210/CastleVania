#include "PotRoast.h"

void CPotRoast::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{

}

void CPotRoast::Render()
{
	if (isActive == true)
	{
		animations[0]->Render(x, y);
	}
}

void CPotRoast::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + POR_ROAST_BBOX_WIDTH;
	b = y + POP_ROAST_BBOX_HEIGHT;
	if (isActive == false)
	{
		t = 512;
		b = 512;
	}
}
