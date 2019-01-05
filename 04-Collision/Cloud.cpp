#include "Cloud.h"

void CCloud::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vx = CLOUD_FLY_SPEED_X;
	CGameObject::Update(dt);
	x += dx;
	y += dy;
}

void CCloud::Render()
{
	int ani = CLOUD_ANI_FLY;
	animations[ani]->Render(x, y);
}

