#include "ThrowingAxe.h"

void CThrowingAxe::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (isActive == true)
	{
		CGameObject::Update(dt);
		if (isTouchGround == false)
			vy += BONUS_OBJECT_GRAVITY*dt;
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		CalcPotentialCollisions(coObjects, coEvents);

		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		if (coEventsResult.size() == 0)
		{
			y += dy;
		}
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBrick *>(e->obj))
			{
				CBrick *brick = dynamic_cast<CBrick *>(e->obj);
				if (e->ny != 0)
				{
					y += min_ty*dy + ny*0.4f;
					if (ny != 0) vy = 0;
					isTouchGround = true;
				}
			}
			else
				y += 0.1*dy;
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CThrowingAxe::Render()
{
	if (isActive == true)
	{
		animations[0]->Render(x, y);
	}
}

void CThrowingAxe::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + THROWING_AXE_BBOX_WIDTH;
	b = y + THROWING_AXE_BBOX_HEIGHT;
	if (isActive == false)
	{
		t = 512;
		b = 512;
	}
}
