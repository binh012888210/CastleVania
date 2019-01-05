#include "FishBullet.h"

void CFishBullet::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (isActive == true)
	{
		CGameObject::Update(dt);
		if (nx > 0)
			vx = FISHMEN_BULLET_SPEED_X;
		else
			vx = -FISHMEN_BULLET_SPEED_X;
		x += dx;
	}
}

void CFishBullet::Render()
{
	if (isActive==true)
	{
		int ani;
		if (nx > 0)
			ani = FISH_BULLET_RIGHT;
		else
			ani = FISH_BULLET_LEFT;
		animations[ani]->Render(x, y);
	}
}

void CFishBullet::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + FISH_BULLET_BBOX_WIDTH;
	b = y + FISH_BULLET_BBOX_HEIGHT;
	if (isActive == false)
	{
		t = 960;
		b = 960;
	}

	
}

