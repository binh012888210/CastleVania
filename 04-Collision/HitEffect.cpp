#include "HitEffect.h"

void CHitEffect::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (isActive == true)
	{
		if (timeAnimation > 0)
		{
			timeAnimation -= dt;
		}
		else
			DeactivateObject();
	}
}

void CHitEffect::Render()
{
	if (isActive == true)
		animations[0]->Render(x, y);
}

void CHitEffect::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
	if (isActive == false)
	{
		t = 512;
		b = 512;
	}
}