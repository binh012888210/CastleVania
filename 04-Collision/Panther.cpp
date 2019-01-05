#include "Panther.h"

void CPanther::AddBonusItem(LPGAMEOBJECT bonusObject)
{
	this->bonusObject = bonusObject;
}

void CPanther::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (isActive == true && state != PANTHER_STATE_DIE)
	{
		CGameObject::Update(dt);
		x += dx;

		if(state!=PANTHER_STATE_IDLE)
			vy += BONUS_OBJECT_GRAVITY*dt;
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		CalcPotentialCollisions(coObjects, coEvents);

		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		y += min_ty*dy + ny*0.4f;

		if (ny != 0) vy = 0;
		
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBrick *>(e->obj))
			{
				if (GetState() != PANTHER_STATE_IDLE)
				{
					CBrick *brick = dynamic_cast<CBrick *>(e->obj);
					if (e->ny != 0)
					{
						if (GetState() == PANTHER_STATE_JUMP)
						{
							this->nx = -this->nx;//chu y
							SetState(PANTHER_STATE_RUN);
						}
					}
				}
			}
		}
		if (coEventsResult.size() == 0 && vy > 0.15f && jumped == false)//define vy to jump
		{
			SetState(PANTHER_STATE_JUMP);
			jumped = true;
		}
		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
	if (Added == false && state == PANTHER_STATE_DIE && isActive == false)
	{
		bonusObject->SetPosition(x + 3, y + PANTHER_BBOX_HEIGHT / 4);
		bonusObject->ActivateObject();
		bonusObject->nx = -1;
		coObjects->push_back(bonusObject);
		Added = true;
	}
}

void CPanther::Render()
{
	int ani;
	if (isActive == true)
	{
		if (state == PANTHER_STATE_IDLE)
		{
			if (nx>0)
				ani = PANTHER_ANI_IDLE_RIGHT;
			else
				ani = PANTHER_ANI_IDLE_LEFT;
			animations[ani]->Render(x, y);
		}
		else if (state == PANTHER_STATE_RUN)
		{
			if (nx>0)
				ani = PANTHER_ANI_RUN_RIGHT;
			else
				ani = PANTHER_ANI_RUN_LEFT;
			animations[ani]->Render(x, y);
		}
		else if (state == PANTHER_STATE_JUMP)
		{
			if (nx>0)
				ani = PANTHER_ANI_JUMP_RIGHT;
			else
				ani = PANTHER_ANI_JUMP_LEFT;
			animations[ani]->Render(x, y);
		}
		else if (state == PANTHER_STATE_DIE)
		{
			ani = PANTHER_ANI_DIE;
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
	/*	RenderBoundingBox();*/
	}
}

void CPanther::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PANTHER_STATE_IDLE:
		vx = 0;
		break;
	case PANTHER_STATE_DIE:
		vx = 0;
		break;
	case PANTHER_STATE_RUN:
		if (nx > 0)
			vx = PANTHER_RUN_SPEED;
		else
			vx = -PANTHER_RUN_SPEED;
		break;
	case PANTHER_STATE_JUMP:
		if (nx > 0)
			vx = PANTHER_JUMP_SPEED_X;
		else
			vx = -PANTHER_JUMP_SPEED_X;
		vy = -PANTHER_JUMP_SPEED_Y;
		break;
	}
}

void CPanther::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + PANTHER_BBOX_WIDTH;
	bottom = y + PANTHER_BBOX_HEIGHT;
	if (state == PANTHER_STATE_IDLE)
	{
		left = x - 96;
		bottom = 512;
	}
	if (state == PANTHER_STATE_DIE)
	{
		top = 512;
		bottom = 512;
	}
}