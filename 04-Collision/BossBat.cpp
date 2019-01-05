#include "BossBat.h"


void CBossBat::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (isActive == true)
	{
		CGameObject::Update(dt);
		x += dx;
		y += dy;
		BossBatTempX = x - 5140;
		if (BossBatTempX < 0 || BossBatTempX>400)//dam vao tuong
		{
			if (BossBatTempX < 0)
				BossBatTempX += 2;
			if (BossBatTempX > 400)
				BossBatTempX -= 2;
			vx = -vx;
			/*vy = -vy;*/
		}
		if (y > 390 || y < 96)
		{
			if (y > 390)
				y = y - 2;
			if (y > 96)
				y = y + 2;
			vy = -vy;
		}
		if (GetState() == BOSSBAT_STATE_WAS_HIT)
		{
			if (timeWasHit > 0)
				timeWasHit -= dt;
			else
			{
				SetState(BOSSBAT_STATE_FLY_SLOW_1);
				timeWasHit = BOSSBAT_TIME_WAS_HIT;
			}
		}
		if (GetState() == BOSSBAT_STATE_FLY_ATTACK)
		{
			/*if (p->GetState() == SIMON_STATE_JUMP)
			vy = -vy;*/
			if (timeAttack > 0)
				timeAttack -= dt;
			else
			{
				SetState(BOSSBAT_STATE_FLY_SLOW_1);
				timeAttack = BOSSBAT_TIME_ATTACK;
			}
		}
		if (GetState() == BOSSBAT_STATE_IDLE)
		{
			if (timeIdle > 0)
				timeIdle -= dt;
			else
			{
				SetState(BOSSBAT_STATE_FLY_ATTACK);
				timeIdle = BOSSBAT_TIME_IDLE;
			}
		}
		if (GetState() == BOSSBAT_STATE_FLY_SLOW_1)
		{
			if (SimonY - y > 128)
			{
				SetState(BOSSBAT_STATE_FLY_SLOW_2);
			}
		}
		if (GetState() == BOSSBAT_STATE_FLY_SLOW_2)
		{
			if (SimonX > 314)
			{
				SetState(BOSSBAT_STATE_FLY_SLOW_3);
			}
			else
			{
				if (SimonY - y < 64)
				{
					SetState(BOSSBAT_STATE_IDLE);
				}
			}
		}
		if (GetState() == BOSSBAT_STATE_FLY_SLOW_3)
		{
			if (SimonY < 230 || SimonY > 294)
			{
				if (BossBatTempX < 64 || BossBatTempX > 272)
					SetState(BOSSBAT_STATE_IDLE);
			}
			else
			{
				if (BossBatTempX < 64 || BossBatTempX > 192)
					SetState(BOSSBAT_STATE_IDLE);
			}
		}
			
	}
}

void CBossBat::SetState(int state)
{
	BossBatTempX = x - 5140;// reset position to 0
	CGameObject::SetState(state);
	switch (state)
	{
	case BOSSBAT_STATE_FLY_ATTACK:
		if (SimonX < 314)
		{
			if (abs(SimonX - BossBatTempX) <= 16)
			{
				vy = BOSSBAT_FLYING_SPEED_Y * 2;
				if (SimonX - BossBatTempX > 0)
					vx = BOSSBAT_FLYING_SPEED_X*0.4f;
				else
					vx = -BOSSBAT_FLYING_SPEED_X*0.4f;
			}
			else if (abs(SimonX - BossBatTempX) <= 32)
			{
				vy = BOSSBAT_FLYING_SPEED_Y * 2;
				if (SimonX - BossBatTempX > 0)
					vx = BOSSBAT_FLYING_SPEED_X*0.75f;
				else
					vx = -BOSSBAT_FLYING_SPEED_X*0.75f;
			}
			else if (abs(SimonX - BossBatTempX) > 32 && abs(SimonX - BossBatTempX) <= 64)
			{
				vy = BOSSBAT_FLYING_SPEED_Y * 2;
				if (SimonX - BossBatTempX > 0)
					vx = BOSSBAT_FLYING_SPEED_X * 1.25f;
				else
					vx = -BOSSBAT_FLYING_SPEED_X * 1.25f;
			}
			else if (abs(SimonX - BossBatTempX) > 64)
			{
				vy = BOSSBAT_FLYING_SPEED_Y*2;
				if (SimonX - BossBatTempX > 0)
					vx = BOSSBAT_FLYING_SPEED_X * 2.5f;
				else
					vx = -BOSSBAT_FLYING_SPEED_X * 2.5f;
			}
		}
		else
		{
			if (SimonY < 230)//floor 2
			{
				if (abs(SimonX - BossBatTempX) < 160)
				{
					vy = BOSSBAT_FLYING_SPEED_Y * 2;
					if (abs(SimonX - BossBatTempX) <= 80)
					{
						if (SimonX - BossBatTempX > 0)
							vx = BOSSBAT_FLYING_SPEED_X;
						else
							vx = -BOSSBAT_FLYING_SPEED_X;
					}
					else
					{
						if (SimonX - BossBatTempX > 0)
							vx = BOSSBAT_FLYING_SPEED_X * 2.0f;
						else
							vx = -BOSSBAT_FLYING_SPEED_X * 2.0f;
					}
				}
				else if (abs(SimonX - BossBatTempX) < 250)
				{
					if (SimonX - BossBatTempX > 0)
						vx = BOSSBAT_FLYING_SPEED_X * 4.0f;
					else
						vx = -BOSSBAT_FLYING_SPEED_X;//do goc lag
					vy = BOSSBAT_FLYING_SPEED_Y*1.5;
				}
				else
				{
					if (SimonY - y < 100)
					{
						fishBullet->ActivateObject();
						fishBullet->SetPosition(x, y);
						fishBullet->ActivateObject();
						fishBullet->nx = 1;
					}
					else 
					{
						vx = BOSSBAT_FLYING_SPEED_X * 4.0f;
						vy = BOSSBAT_FLYING_SPEED_Y*1.5;
					}
				}
			}
			if (SimonY < 295 && SimonY>230)//floor 1
			{
				if (abs(SimonX - BossBatTempX) < 144)
				{
					vy = BOSSBAT_FLYING_SPEED_Y * 2;
					if (abs(SimonX - BossBatTempX) < 80)
					{
						if (SimonX - BossBatTempX > 0)
							vx = BOSSBAT_FLYING_SPEED_X;
						else
							vx = -BOSSBAT_FLYING_SPEED_X;
					}
					else
					{
						if (SimonX - BossBatTempX > 0)
							vx = BOSSBAT_FLYING_SPEED_X * 2.0f;
						else
							vx = -BOSSBAT_FLYING_SPEED_X * 2.0f;
					}
				}
				else if (abs(SimonX - BossBatTempX) < 400)
				{
					if (SimonX - BossBatTempX > 0)
						vx = BOSSBAT_FLYING_SPEED_X * 4.0f;
					else
						vx = -BOSSBAT_FLYING_SPEED_X;//do goc lag
					vy = BOSSBAT_FLYING_SPEED_Y*0.75f;
					
				}
				else
				{
					if (SimonY - y < 63.5)
					{
						fishBullet->ActivateObject();
						fishBullet->SetPosition(x, y);
						fishBullet->ActivateObject();
						fishBullet->nx = 1;
					}
					else
					{
						vx = BOSSBAT_FLYING_SPEED_X * 4.0f;
						vy = BOSSBAT_FLYING_SPEED_Y*0.75f;
					}
				}
			}
			if (SimonY > 295)//duoi cau thang
			{
				if (abs(SimonX - BossBatTempX) <= 160)
				{
					vy = BOSSBAT_FLYING_SPEED_Y * 2;
					if (abs(SimonX - BossBatTempX) <= 80)
					{
						if (SimonX - BossBatTempX > 0)
							vx = BOSSBAT_FLYING_SPEED_X;
						else
							vx = -BOSSBAT_FLYING_SPEED_X;
					}
					else
					{
						if (SimonX - BossBatTempX > 0)
							vx = BOSSBAT_FLYING_SPEED_X * 2;
						else
							vx = -BOSSBAT_FLYING_SPEED_X * 2;
					}
				}
				else
				{
					vy = BOSSBAT_FLYING_SPEED_Y*2;
					if (SimonX - BossBatTempX > 0)
						vx = BOSSBAT_FLYING_SPEED_X * 4;
					else
						vx = -BOSSBAT_FLYING_SPEED_X;//do goc lag
				}
			}
		}
		break;
	case BOSSBAT_STATE_FLY_SLOW_1://di ra sau danh
		if (SimonX - BossBatTempX > 0)
			vx = -BOSSBAT_FLYING_SPEED_X*1.5;
		else
			vx = BOSSBAT_FLYING_SPEED_X*1.5;

		if (SimonX < 314)
		{
				vy = -BOSSBAT_FLYING_SPEED_Y*1.5 ;
		}
		else
		{
			if (SimonY <= 230)//floor 2
			{
				vy = -BOSSBAT_FLYING_SPEED_Y*0.5;
			}
			if (SimonY <= 294 && SimonY>230)//floor 1
			{
				vy = -BOSSBAT_FLYING_SPEED_Y;
			}
			if (SimonY > 294)//duoi cau thang
			{
				vy = -BOSSBAT_FLYING_SPEED_Y*2;
			}
		}
		break;
	case BOSSBAT_STATE_FLY_SLOW_2://sau fly slow 1
		vy = BOSSBAT_FLYING_SPEED_Y/2 ;
		break;
	case BOSSBAT_STATE_FLY_SLOW_3://sau fly slow 1
		vy = -BOSSBAT_FLYING_SPEED_Y/2;
		break;
	case BOSSBAT_STATE_IDLE://sau fly slow 2
		vy = 0;
		vx = 0;
		break;
	case BOSSBAT_STATE_WAS_HIT:
		vy = 0;
		vx = 0;
		break;
	case BOSSBAT_STATE_DIE:
		vy = 0;
		vx = 0;
		break;
	}
}
void CBossBat::Render()
{
	int ani;
	if (isActive == false && BOSSBAT_ANI_IDLE)
	{
		ani = BOSSBAT_ANI_IDLE;
		animations[ani]->Render(x, y);
	}
	else if(isActive == true)
	{
		if (state != BOSSBAT_STATE_DIE)
		{
			ani = BOSSBAT_ANI_ACTIVATE;
			animations[ani]->Render(x, y);
		}
		else 
		{
			ani = BOSSBAT_ANI_DIE;
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
	}
}

void CBossBat::UpdateSimonPosition(LPGAMEOBJECT  simon)
{
	 SimonX = simon->x - 5140;
	 SimonY = simon->y; ;
}
void CBossBat::AddFishBullet(CFishBullet *fishBullet)
{
	this->fishBullet = fishBullet;
}
void CBossBat::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + 8 + BOSSBAT_BBOX_WIDTH;
	bottom = y + BOSSBAT_BBOX_HEIGHT;
	if (isActive == false)
	{
		bottom = 512;
	}
	if (GetState() == BOSSBAT_STATE_DIE)
	{
		top = 512;
		bottom = 512;
	}

}