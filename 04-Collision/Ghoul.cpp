#include "Ghoul.h"

void CGhoul::AddBonusItem(LPGAMEOBJECT bonusObject)
{
	this->bonusObject = bonusObject;
}

void CGhoul::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (isActive == true)
	{
		CGameObject::Update(dt);
			x += dx;

		if (GetState() != GHOUL_STATE_DIE)
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
					vy = 0;
				}
			}
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
		
	if (Added == false && GetState() == GHOUL_STATE_DIE && isActive == false)
	{
		bonusObject->SetPosition(x + 3, y + GHOUL_BBOX_HEIGHT / 4);
		bonusObject->ActivateObject();
		bonusObject->nx = -1;
		coObjects->push_back(bonusObject);
		Added = true;
	}
}

void CGhoul::Render()
{
	int ani;
	if (isActive == true)
	{
		if (GetState() == GHOUL_STATE_WALKING)
		{
			if (vx > 0)
			{
				nx = 1;
				ani = GHOUL_ANI_WALKING_RIGHT;
			}
			else
			{
				nx = -1;
				ani = GHOUL_ANI_WALKING_LEFT;
			}
			animations[ani]->Render(x, y);
		}
		else if (GetState() == GHOUL_STATE_DIE)
		{
			ani = GHOUL_ANI_DIE;
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

void CGhoul::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GHOUL_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	case GHOUL_STATE_WALKING:
		vx = -GHOUL_WALKING_SPEED;
		break;
	}

}

void CGhoul::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + GHOUL_BBOX_WIDTH;
	bottom = y + GHOUL_BBOX_HEIGHT;
	if (GetState() == GHOUL_STATE_DIE)
	{
		top = 512;
		bottom = 512;
	}
}