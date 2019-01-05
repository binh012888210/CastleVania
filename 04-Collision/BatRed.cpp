#include "BatRed.h"


void CBatRed::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (isActive == true && GetState() == BAT_RED_FLY_STATE_FLY)
	{
		CGameObject::Update(dt);

		if (vy > BAT_RED_FLY_SPEED_Y_MAX && isGoDown == true)
		{
			isGoDown = false;
		}
		if (vy < -BAT_RED_FLY_SPEED_Y_MAX && isGoDown == false)
		{
			isGoDown = true;
		}

		if (isGoDown == true)
			vy += BAT_RED_FLY_SPEED_Y;
		else
			vy -= BAT_RED_FLY_SPEED_Y;
	
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		CalcPotentialCollisions(coObjects, coEvents);

		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		if (coEventsResult.size() == 0)
		{
			x += dx;
			y += dy;
		}
		else
		{
			vx = -vx;
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CBatRed::Render()
{
	int ani;
	if (isActive == true)
	{
		if (GetState() == BAT_RED_FLY_STATE_FLY)
		{
			if (vx > 0)
			{
				nx = 1;
				ani = BAT_RED_FLY_ANI_RIGHT;
			}
			else
			{
				nx = -1;
				ani = BAT_RED_FLY_ANI_LEFT;
			}
			animations[ani]->Render(x, y);
		}
		else if (GetState() == BAT_RED_FLY_STATE_DIE)
		{
			ani = BAT_RED_FLY_ANI_DIE;
			if (isDestroying == false)
			{
				animations[ani]->NextFrame(currentFrameRendering, currentFramesSize);
				animations[ani]->Render2(x, y);
				isDestroying = true;
			}
			else if (currentFrameRendering < currentFramesSize)
			{
				animations[ani]->NextFrame(currentFrameRendering, currentFramesSize);
				animations[ani]->Render2(x, y);
				if (currentFrameRendering + 1 == currentFramesSize)
				{
					DeactivateObject();
					isDestroying == false;
				}
			}
		}
		/*RenderBoundingBox();*/
	}
}

void CBatRed::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BAT_RED_FLY_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	case BAT_RED_FLY_STATE_FLY:
		vx = -BAT_RED_FLY_SPEED_X;
		vy = BAT_RED_FLY_SPEED_Y;
		break;
	}

}

void CBatRed::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + BAT_RED_FLY_BBOX_WIDTH;
	bottom = y + BAT_RED_FLY_BBOX_HEIGHT;
	if (GetState() == BAT_RED_FLY_STATE_DIE)
	{
		top = 512;
		bottom = 512;
	}
}