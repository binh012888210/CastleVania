#include "BrickDestroyEffect.h"

void CBrickDestroyEffect::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (isActive == true)
	{
		CGameObject::Update(dt);
		if (timeAnimation > 0)
		{
			float time = dt / 1000.0f;
			timeAnimation -= time;
		}
		else
			DeactivateObject();
		vy += BONUS_OBJECT_GRAVITY*dt;
		x += dx;
		y += dy;
	}
}

void CBrickDestroyEffect::Render()
{
	if (isActive == true)
		animations[0]->Render(x, y);
	//RenderBoundingBox();
}

void CBrickDestroyEffect::GetBoundingBox(float &l, float &t, float &r, float &b)
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