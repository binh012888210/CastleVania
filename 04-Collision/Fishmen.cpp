#include "Fishmen.h"

void CFishmen::Render()
{
	int ani;
	if (isActive == true)
	{
		/*if (GetState() == FISHMEN_STATE_IDLE)
		{
			if (nx>0)
				ani = FISHMEN_ANI_JUMP_RIGHT;
			else
				ani = FISHMEN_ANI_JUMP_LEFT;
			animations[ani]->Render(x, y);
		}*/
		if (GetState() == FISHMEN_STATE_WALKING)
		{
			if (nx>0)
				ani = FISHMEN_ANI_WALKING_RIGHT;
			else
				ani = FISHMEN_ANI_WALKING_LEFT;
			animations[ani]->Render(x, y);
		}
		else if (GetState() == FISHMEN_STATE_JUMP)
		{
			if (nx>0)
				ani = FISHMEN_ANI_JUMP_RIGHT;
			else
				ani = FISHMEN_ANI_JUMP_LEFT;
			animations[ani]->Render(x, y);
		}
		else if (GetState() == FISHMEN_STATE_ATTACK)
		{
			if (nx>0)
				ani = FISHMEN_ANI_ATTACK_RIGHT;
			else
				ani = FISHMEN_ANI_ATTACK_LEFT;
			animations[ani]->Render(x, y+4);
		}
		else if (GetState() == FISHMEN_STATE_DIE)
		{
			ani = FISHMEN_ANI_IDE;
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

void CFishmen::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (isActive == true)
	{
		CGameObject::Update(dt);
		x += dx;

		if (GetState() == FISHMEN_STATE_ATTACK && useBullet == true)
		{
			useBullet = false;
			if (nx > 0)
				fishBullet->SetPosition(x + FISHMEN_BBOX_WIDTH, y + 12);
			else
				fishBullet->SetPosition(x , y + 10);
			fishBullet->ActivateObject();
			fishBullet->nx = nx;
		}
		
		if (GetState() == FISHMEN_STATE_JUMP)
			y += dy;
		
		if (GetState() != FISHMEN_STATE_IDLE && GetState() != FISHMEN_STATE_DIE)
			vy += BONUS_OBJECT_GRAVITY*dt;
		
		if (checKGroundCollision == true)
		{
			if (GetState() == FISHMEN_STATE_JUMP && vy > 0)
			{
				SetState(FISHMEN_STATE_WALKING);
				timeWalking = FISHMEN_TIME_WALKING;

			}
			if (GetState() == FISHMEN_STATE_WALKING || GetState() == FISHMEN_STATE_ATTACK)
			{
				if (GetState() == FISHMEN_STATE_WALKING)
				{
					if (timeWalking > 0)
						timeWalking -= dt;
					else
					{
						useBullet = true;
						SetState(FISHMEN_STATE_ATTACK);
						timeAttack = FISHMEN_TIME_ATTACK;
					}
				}
				else if (GetState() == FISHMEN_STATE_ATTACK)
				{

					if (timeAttack > 0)
						timeAttack -= dt;
					else
					{
						nx = -nx;
						SetState(FISHMEN_STATE_WALKING);
						if (dt % 3 == 1)
							timeWalking = FISHMEN_TIME_WALKING;
						else
							timeWalking = FISHMEN_TIME_WALKING*0.9;
					}
				}
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
					{
						if (dynamic_cast<CWaterUnderground *>(e->obj))
						{
							CWaterUnderground *waterUnderGround = dynamic_cast<CWaterUnderground *>(e->obj);
							if (e->ny != 0)
							{
								waterUnderGround->SetState(WATER_STATE_BLAST);
								DeactivateObject();
							}
						}
					}
				}
				// clean up collision events
				for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
			}
		}
		
		if (GetState() == FISHMEN_STATE_JUMP)
		{
			vector<LPCOLLISIONEVENT> coEvents;
			vector<LPCOLLISIONEVENT> coEventsResult;

			coEvents.clear();

			CalcPotentialCollisions(coObjects, coEvents);

			float min_tx, min_ty, nx = 0, ny;

			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);


			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				{
					if (dynamic_cast<CWaterUnderground *>(e->obj))
					{
						CWaterUnderground *waterUnderGround = dynamic_cast<CWaterUnderground *>(e->obj);
						if (e->ny != 0)
						{
							waterUnderGround->SetState(WATER_STATE_BLAST);
						}
					}
				}
			}
			// clean up collision events
			for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		}
	}
}
void CFishmen::AddFishBullet(CFishBullet *fishBullet)
{
	this->fishBullet = fishBullet;
}
void CFishmen::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FISHMEN_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case FISHMEN_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	case FISHMEN_STATE_WALKING:
		if (nx > 0)
			vx = FISHMEN_WALKING_SPEED_X;
		else
			vx = -FISHMEN_WALKING_SPEED_X;
		break;
	case FISHMEN_STATE_JUMP:
		vy = -FISHMEN_JUMP_SPEED_Y;
		break;
	case FISHMEN_STATE_ATTACK:
		vx = 0;
		break;
	}
}
void CFishmen::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + FISHMEN_BBOX_WIDTH;
	b = y + FISHMEN_BBOX_HEIGHT;
	if (GetState() == FISHMEN_STATE_IDLE)
	{
		l = x - 32;
		t = 600;
		r = x + 64;
	}
	if (GetState() == FISHMEN_STATE_DIE)
	{
		t = 960;
		b = 960;
	}

}