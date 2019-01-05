#include "WaterBlast.h"


void CWaterBlast::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (isActive == true)
	{
		CGameObject::Update(dt);
		if (timeAnimation > 0)
		{
			timeAnimation -= dt;
		}
		else
			DeactivateObject();
		vy += BONUS_OBJECT_GRAVITY*dt;
		x += dx;
		y += dy;
	}
}

void CWaterBlast::Render()
{
	if (isActive == true)
		animations[0]->Render(x, y);
	//RenderBoundingBox();
}

void CWaterBlast::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
	if (isActive == false)
	{
		t = 960;
		b = 960;
	}
}