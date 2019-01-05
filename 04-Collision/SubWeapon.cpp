#include "SubWeapon.h"

void CSubWeapon::InitTimeLife()
{
	switch (state)
	{
	case SUBWEAPON_STATE_1://dagger
		timeLife = SUBWEAPON1_TIMELIFE;
		vy = 0;
		if (nx > 0)
			vx = SUBWEAPON_FLY_SPEED_X;
		else
			vx = -SUBWEAPON_FLY_SPEED_X;
		break;
	case SUBWEAPON_STATE_2://axe
		timeLife = SUBWEAPON2_TIMELIFE;
		vy = -SUBWEAPON_THROW_UP_SPEED_Y;
		if (nx > 0)
			vx = SUBWEAPON_FLY_SPEED_X / 2;
		else
			vx = -SUBWEAPON_FLY_SPEED_X / 2;
		break;
	case SUBWEAPON_STATE_3://boomerang
		timeLife = SUBWEAPON3_TIMELIFE;
		vy = 0;
		if (nx > 0)
			vx = SUBWEAPON_FLY_SPEED_X/2;
		else
			vx = -SUBWEAPON_FLY_SPEED_X/2;
		break;
	case SUBWEAPON_STATE_4://holywater
		timeLife = SUBWEAPON4_TIMELIFE;
		vy = -SUBWEAPON_THROW_UP_SPEED_Y/5;
		if (nx > 0)
			vx = SUBWEAPON_FLY_SPEED_X / 3;
		else
			vx = -SUBWEAPON_FLY_SPEED_X / 3;
		break;
	}
}

void CSubWeapon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (timeLife > 0)
	{
		timeLife -= dt;
		timeLife2 += dt;//bien nay do boomerang su dung do dem tu 0 thi boomerang se qua dau dung tai 1 vi tri
		if (GetState() == SUBWEAPON_STATE_3)
		{
			if (timeLife2 >= (SUBWEAPON3_TIMELIFE / 2)-8 && timeLife2 < (SUBWEAPON3_TIMELIFE / 2)+8 )
				vx = -vx;
		}
	}
	else
	{
		DeactivateObject();
		timeLife2 = 0;
		timeLife = 0;
		subWeaponExplose = false;
	}
	if (isActive == true)
	{
		CGameObject::Update(dt);
		if (GetState() == SUBWEAPON_STATE_4|| GetState() == SUBWEAPON_STATE_2)
			if (subWeaponExplose == false)
				vy += BONUS_OBJECT_GRAVITY*dt;
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
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (dynamic_cast<CBrick *>(e->obj))
				{
					CBrick *brick = dynamic_cast<CBrick *>(e->obj);
					if (GetState() == SUBWEAPON_STATE_4)
					{
						if (e->nx != 0)
							vx = 0;
						if (e->ny != 0)
						{
							subWeaponExplose = true;
							vx = 0;
							vy = 0;
						}
					}
					else
					{
						x += dx;
						y += dy;
					}
				}
				else if (dynamic_cast<CBrickStair *>(e->obj))
				{
					CBrick *brick = dynamic_cast<CBrick *>(e->obj);
					if (GetState() == SUBWEAPON_STATE_4)
					{
						subWeaponExplose = true;
						vx = 0;
					}
					else
					{
						x += dx;
						y += dy;
					}
				}
				else if (dynamic_cast<CHolyFire *>(e->obj))
				{
					CHolyFire *holyFire = dynamic_cast<CHolyFire *>(e->obj);
					if (e->nx != 0 || e->ny != 0)
					{
						holyFire->SetState(HOLYFIRE_STATE_INACTIVATE);
					}

				}
				else if (dynamic_cast<CCandle *>(e->obj))
				{
					CCandle *candle = dynamic_cast<CCandle *>(e->obj);
					if (e->nx != 0 || e->ny != 0)
					{
						candle->SetState(HOLYFIRE_STATE_INACTIVATE);
					}
				}
				else if (dynamic_cast<CBrickDestroyable *>(e->obj))
				{
					CBrickDestroyable *brickDestroyable = dynamic_cast<CBrickDestroyable *>(e->obj);
					if (e->nx != 0 || e->ny != 0)
					{
						brickDestroyable->SetState(BRICK_DESTROYABLE_STATE_INACTIVATE);
					}
				}
				else if (dynamic_cast<CGhoul *>(e->obj))
				{
					CGhoul *ghoul = dynamic_cast<CGhoul *>(e->obj);
					if (e->nx != 0 || e->ny != 0)
					{
						ghoul->SetState(GHOUL_STATE_DIE);
						score += ghoul->GetPoint();
					}
				}
				else if (dynamic_cast<CPanther *>(e->obj))
				{
					CPanther *panther = dynamic_cast<CPanther *>(e->obj);
					if (e->nx != 0 || e->ny != 0)
					{
						if (panther->GetState() != PANTHER_STATE_IDLE)
						{
							panther->SetState(PANTHER_STATE_DIE);
							score += panther->GetPoint();
						}
						x += dx;
						y += dy;
					}
				}
				else if (dynamic_cast<CFishmen *>(e->obj))
				{
					CFishmen *fishmen = dynamic_cast<CFishmen *>(e->obj);
					if (e->nx != 0 || e->ny != 0)
					{
						if (fishmen->GetState() != FISHMEN_STATE_IDLE)
						{
							fishmen->SetState(FISHMEN_STATE_DIE);
							score += fishmen->GetPoint();
						}
						x += dx;
						y += dy;
					}
				}
				else if (dynamic_cast<CBatRed *>(e->obj))
				{
					CBatRed *batRed = dynamic_cast<CBatRed *>(e->obj);
					if (e->nx != 0 || e->ny != 0)
					{
						batRed->SetState(BAT_RED_FLY_STATE_DIE);
						score += batRed->GetPoint();
					}
				}
				else if (dynamic_cast<CBossBat *>(e->obj))
				{
					CBossBat *bossBat = dynamic_cast<CBossBat *>(e->obj);
					if (e->nx != 0|| e->ny != 0)
					{
						if (bossBat->GetState() != BOSSBAT_STATE_WAS_HIT && bossBat->GetState() != BOSSBAT_STATE_DIE)
						{
							/*bossBat->lifePoint -= shotMutiplier;*/
							if (bossBat->lifePoint > 0)
							{
								bossBat->lifePoint -= shotMutiplier;
								bossBat->SetState(BOSSBAT_STATE_WAS_HIT);
							}
							else
								bossBat->SetState(BOSSBAT_STATE_DIE);
						}
						else
						{
							x += dx;
							y += dy;
						}
					}
				}
				else
				{
					x += 0.5*dx;
					y += 0.5*dy;
				}
			}
		}
		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CSubWeapon::Render()
{
	if (isActive == true)
	{
		int ani;
		if (state == SUBWEAPON_STATE_1)
		{
			if (nx > 0)
				ani = SUBWEAPON1_ANI_RIGHT;
			else
				ani = SUBWEAPON1_ANI_LEFT;
		}
		else if (state == SUBWEAPON_STATE_2)
		{
			if (nx > 0)
				ani = SUBWEAPON2_ANI_RIGHT;
			else
				ani = SUBWEAPON2_ANI_LEFT;
		}
		else if (state == SUBWEAPON_STATE_3)
		{
			if (nx > 0)
				ani = SUBWEAPON3_ANI_RIGHT;
			else
				ani = SUBWEAPON3_ANI_LEFT;
		}
		else if (state == SUBWEAPON_STATE_4)
		{
			if (nx > 0)
				ani = SUBWEAPON4_ANI_RIGHT;
			else
				ani = SUBWEAPON4_ANI_LEFT;
			if (subWeaponExplose == true)
				ani = SUBWEAPON4_ANI_EXPLOSE;
		}
		animations[ani]->Render(x, y);
	}
}

void CSubWeapon::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	if (state == SUBWEAPON_STATE_1)
	{
		r = x + SUBWEAPON1_BBOX_WIDTH;
		b = y + SUBWEAPON1_BBOX_HEIGHT;
	}
	else if (state == SUBWEAPON_STATE_2)
	{
		r = x + SUBWEAPON2_BBOX_WIDTH;
		b = y + SUBWEAPON2_BBOX_HEIGHT;
	}
	else if (state == SUBWEAPON_STATE_3)
	{
		r = x + SUBWEAPON3_BBOX_WIDTH;
		b = y + SUBWEAPON3_BBOX_HEIGHT;
	}
	else if (state == SUBWEAPON_STATE_4)
	{
		r = x + SUBWEAPON4_BBOX_WIDTH;
		b = y + SUBWEAPON4_BBOX_HEIGHT;
	}
	if (isActive == false)
	{
		t = 512;
		b = 512;
	}
	
}
