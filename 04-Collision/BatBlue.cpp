#include "BatBlue.h"

void CBatBlue::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (ny2 != 0)
	{
		vy = BATBLUE_FLY_SPEED_Y;
		vx = BATBLUE_FLY_SPEED_X1;
	}
	else
		vx = -BATBLUE_FLY_SPEED_X2;
	CGameObject::Update(dt);
	x += dx;
	y += dy;
}

void CBatBlue::Render()
{
	animations[0]->Render(x, y);
}