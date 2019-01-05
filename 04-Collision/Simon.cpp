#include <algorithm>
#include "debug.h"

#include "Simon.h"

void CSimon::AddWeapon(CWeapon *weapon)
{
	this->weapon = weapon;
}

void CSimon::AddSubWeapon(CSubWeapon *subWeapon)
{
	this->subWeapon = subWeapon;
}

void CSimon::AddHitEffect(CHitEffect * hitEffect)
{
	this->hitEffect = hitEffect;
}

void CSimon::InitSimon()
{
	timeLife = 0;
	score = 0;
	lifePoint = 16;
	heartPoint = 50;
	untouchable = 0;
	shotMutiplier = 1;

	isAttacking = false;
	isCrouching = false;
	isJumping = false;
	attackWhenjump = false;
	isOnStair = false;
	isClimbingOnStair = false;
	isTakingWeapon = false;
	isInvisibility = false;
	isUsePotion = false;
	wasHitting = false;
	wasKilling = false;

	killBoss = false;
	isAtBoss = false;
	atBrickAtBoss = false;
	isUnderground = false;
	isTouchingStair = false;
	
	useStopWatch = false;
	changeStage = false;
	changeStageWalking = false;
	resetStage = false;
	killAllTheEnemy = false;
	exitGame = false;
}

void CSimon::ResetSimon()
{
	InitSimon();
	life--;
	resetStage = true;
	timeLife = saveTimeLife;
}

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (timeLife == 0 && killBoss == false)
	{
		SetState(SIMON_STATE_DIE);
	}
	else
	{
		if (isAtBoss == true && atBrickAtBoss == false)//chan update man hinh va tao tuong chan simon lai
		{
			atBrickAtBoss = true;
			for (int i = 0; i < 31; i++)
			{
				CBrick *brick = new CBrick();
				brick->AddAnimation(601);
				if (i < 31)
					brick->SetPosition(5126, 0 + i*16.0f);
				coObjects->push_back(brick);
			}
		}
		CGameObject::Update(dt);

		//timer
		if (coObjects != NULL)
			timeLife -= dt;

		if (killAllTheEnemy == true)
		{
			if (timeCrossActivate > 0)
			{
				timeCrossActivate -= dt;
			}
			else
				DeActivateCross();
		}

		if (useStopWatch == true)
		{
			if (timeStopWatchActivate > 0)
			{
				timeStopWatchActivate -= dt;
			}
			else
				DeActivateStopWatch();
		}

		if (killBoss == true)
		{
			if (heartPoint > 0)
			{
				heartPoint--;
				score += 100;
			}
			else
				exitGame = true;
		}

		if (killBoss == true)
		{
			if (exitGame == true)
			{
				if (timeLife > 0)
				{
					timeLife -= 1000;
					score += 10;
				}
			}
		}
		
		if(useStopWatch == true)//dirty way for not update timeLife
			timeLife += dt;
		if (changeStage == true)
			timeChangeStage -= dt;
		if (changeStageWalking == true)
			timeChangeStageWalking -= dt;
		if (isInvisibility == true)
		{
			if (timeInvisibility > 0)
			{
				timeInvisibility -= dt;
				if (timeInvisibility < SIMON_TIME_INVISIBILITY - SIMON_TIME_POTION_EFFECTIVE && timeInvisibility > SIMON_TIME_INVISIBILITY - 2*SIMON_TIME_POTION_EFFECTIVE)
					isUsePotion = true;
				else
					isUsePotion = false;
			}
			else

				isInvisibility = false;
		}
		//Update weapon and subweapon
		hitEffect->Update(dt, coObjects);
		if (weaponLvl == 1)
			weapon->SetState(WEAPON_STATE_1);
		else if (weaponLvl == 2)
			weapon->SetState(WEAPON_STATE_2);
		else
			weapon->SetState(WEAPON_STATE_3);
		subWeapon->shotMutiplier = shotMutiplier;
		
		//Update Simon
		if (coObjects == NULL)
		{
			x += dx;
		}
		else
		{
			
			if (isOnStair == false)
			{
				// Simple fall down
				vy += SIMON_GRAVITY*dt;
				firstBrickStairNy = 0;
				firstBrickStairNx = 0;
			}

			vector<LPCOLLISIONEVENT> coEvents;
			vector<LPCOLLISIONEVENT> coEventsResult;

			coEvents.clear();//clear
			coEventsResult.clear();

		
			CalcPotentialCollisions(coObjects, coEvents,true);//tinh toan va cham
			
															  // reset untouchable timer if untouchable time has passed
			if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME) //khoang thoi gian hoi sinh
			{
				untouchable_start = 0;
				untouchable = 0;
			}

			if (coEvents.size() == 0)// No collision occured, proceed normally
			{
				x += dx;
				y += dy;
			}
			else
			{
				float min_tx, min_ty, nx = 0, ny ;
				bool isTouchingBrick = false;
				FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
				if (isOnStair == true)
				{
					x += dx;
					y += dy;
				}
				//Collision logic with object
				for (UINT i = 0; i < coEventsResult.size(); i++)
				{
					LPCOLLISIONEVENT e = coEventsResult[i];
					if (!dynamic_cast<CBrick *>(e->obj))
					{
						isTouchingBrick = true;
					}
					if (dynamic_cast<CBrick *>(e->obj))
					{
						if (isTouchingBrick == false)
						{
							x += min_tx*dx + nx*0.4f;
							y += min_ty*dy + ny*0.4f;
							if (nx != 0) vx = 0;
							if (ny != 0) vy = 0;
							isTouchingBrick = true;
						}

					}
					if (dynamic_cast<CBrickDestroyable *>(e->obj))
					{
							x += min_tx*dx + nx*0.4f;
							y += min_ty*dy + ny*0.4f;
							if (nx != 0) vx = 0;
							if (ny != 0) vy = 0;
							isTouchingBrick = true;
					}
					else if (dynamic_cast<CPanther *>(e->obj))
					{
						CPanther *panther = dynamic_cast<CPanther *>(e->obj);
						if (e->nx != 0 || e->ny != 0)
						{
							if (GetState() != SIMON_STATE_DIE)
							{
								if (untouchable == 0)
								{
									if (panther->GetState() != PANTHER_STATE_IDLE)
									{
										lifePoint -= 2;
										if (lifePoint > 0)
										{
											StartUntouchable();
											if (isOnStair == false)
											{
												this->nx = -nx;//nhay nguoc ve huong va cham
												SetState(SIMON_STATE_WAS_HIT);

											}
										}
										else
											SetState(SIMON_STATE_DIE);
									}
									else
									{
										panther->SetState(PANTHER_STATE_RUN);
										panther->nx = -this->nx;
									}
								}
							}
						}
					}
					else if (dynamic_cast<CGhoul *>(e->obj))
					{
						CGhoul *ghoul = dynamic_cast<CGhoul *>(e->obj);
						if (e->nx != 0 || e->ny != 0)
						{
							if (GetState() != SIMON_STATE_DIE)
							{
								if (untouchable == 0)
								{
									lifePoint -= 2;
									if (lifePoint > 0)
									{
										StartUntouchable();
										if (isOnStair == false)
										{
											this->nx = -nx;
											SetState(SIMON_STATE_WAS_HIT);
										}
									}
									else
										SetState(SIMON_STATE_DIE);
								}
							}
						}
					}
					else if (dynamic_cast<CBatRed *>(e->obj))
					{
						CBatRed *batRed = dynamic_cast<CBatRed *>(e->obj);
						if (e->nx != 0 || e->ny != 0)
						{
							if (GetState() != SIMON_STATE_DIE)
							{
								if (untouchable == 0)
								{
									lifePoint -= 2;
									if (lifePoint > 0)
									{
										batRed->SetState(BAT_RED_FLY_STATE_DIE);
										StartUntouchable();
										if (isOnStair == false)
											SetState(SIMON_STATE_WAS_HIT);
									}
									else
										SetState(SIMON_STATE_DIE);
								}
							}
						}
					}
					else if (dynamic_cast<CFishmen *>(e->obj))
					{
						CFishmen *fishmen = dynamic_cast<CFishmen *>(e->obj);
						if (e->nx != 0 || e->ny != 0)
						{
							if (GetState() != SIMON_STATE_DIE)
							{
								if (untouchable == 0)
								{
									if (fishmen->GetState() != FISHMEN_STATE_IDLE)
									{
										lifePoint -= 2;
										if (lifePoint > 0)
										{
											StartUntouchable();
											if (isOnStair == false)
												SetState(SIMON_STATE_WAS_HIT);
										}
										else
											SetState(SIMON_STATE_DIE);
									}
									else
									{
										fishmen->SetState(FISHMEN_STATE_JUMP);
										fishmen->nx = -this->nx;
									}
								}
							}
						}
					}
					else if (dynamic_cast<CBossBat *>(e->obj))
					{
						CBossBat *bossBat = dynamic_cast<CBossBat *>(e->obj);
						if (e->nx != 0 || e->ny != 0)
						{
							if (untouchable == 0)
							{
								if (bossBat->isActive == false)
								{
									bossBat->ActivateObject();
									isAtBoss = true;
								}
								else
								{
									if (GetState() != SIMON_STATE_DIE)
									{
										lifePoint -= 2;
										if (lifePoint > 0)
										{
											StartUntouchable();
											if (isOnStair == false)
											{
												this->nx = -nx;
												SetState(SIMON_STATE_WAS_HIT);
											}
										}
										else
											SetState(SIMON_STATE_DIE);
									}
								}
							}

						}
					}
					else if (dynamic_cast<CFishBullet *>(e->obj))
					{
						CFishBullet *fishBullet = dynamic_cast<CFishBullet *>(e->obj);
						if (e->nx != 0 || e->ny != 0)
						{
							if (GetState() != SIMON_STATE_DIE)
							{
								if (untouchable == 0)
								{
									lifePoint -= 2;
									if (lifePoint > 0)
									{
										StartUntouchable();
										if (isOnStair == false)
											SetState(SIMON_STATE_WAS_HIT);
									}
									else
										SetState(SIMON_STATE_DIE);
								}
							}
						}
					}					
					else if (dynamic_cast<CInvisibilityPotion *>(e->obj))
					{
						CInvisibilityPotion *invisibilityPotion = dynamic_cast<CInvisibilityPotion *>(e->obj);
						if (e->nx != 0 || e->ny != 0)
						{
							timeInvisibility = SIMON_TIME_INVISIBILITY;
							invisibilityPotion->DeactivateObject();
							isInvisibility = true;
						}
					}
					else if (dynamic_cast<CThrowingAxe *>(e->obj))
					{
						CThrowingAxe *throwingAxe = dynamic_cast<CThrowingAxe *>(e->obj);
						if (e->nx != 0 || e->ny != 0)
						{
							throwingAxe->DeactivateObject();
							subWeapon->SetState(SUBWEAPON_STATE_2);
						}
					}
					else if (dynamic_cast<CCrystalBall *>(e->obj))
					{
						CCrystalBall *crystalBall = dynamic_cast<CCrystalBall *>(e->obj);
						if (e->nx != 0 || e->ny != 0)
						{
							if (e->ny != 0)
								y -= min_ty*dy + e->ny*0.4f;
							crystalBall->DeactivateObject();
							killBoss = true;
						}
					}
					else if (dynamic_cast<CDoubleShot *>(e->obj))
					{
						CDoubleShot *doubleShot = dynamic_cast<CDoubleShot *>(e->obj);
						if (e->nx != 0 || e->ny != 0)
						{
							doubleShot->DeactivateObject();
							shotMutiplier = 2;
						}
					}
					else if (dynamic_cast<CHolyWater *>(e->obj))
					{
						CHolyWater *holyWater = dynamic_cast<CHolyWater *>(e->obj);
						if (e->nx != 0 || e->ny != 0)
						{
							if (e->ny != 0)
								y -= min_ty*dy + e->ny*0.3f;
							holyWater->DeactivateObject();
							subWeapon->SetState(SUBWEAPON_STATE_4);
						}
					}
					else if (dynamic_cast<CStopWatch *>(e->obj))
					{
						CStopWatch *stopWatch = dynamic_cast<CStopWatch *>(e->obj);
						if (e->nx != 0 || e->ny != 0)
						{
							if (e->ny != 0)
								y -= min_ty*dy + e->ny*0.3f;
							stopWatch->DeactivateObject();
							subWeapon->SetState(SUBWEAPON_STATE_5);
						}
					}
					else if (dynamic_cast<CMoneyBag *>(e->obj))
					{
						CMoneyBag *moneyBag = dynamic_cast<CMoneyBag *>(e->obj);
						if (e->nx != 0 || e->ny != 0)
						{
							if (e->ny != 0)
								y -= min_ty*dy + e->ny*0.3f;
							moneyBag->DeactivateObject();
							score += moneyBag->moneyBagValue;
						}
					}
					else if (dynamic_cast<CPotRoast *>(e->obj))
					{
						CPotRoast *potRoast = dynamic_cast<CPotRoast *>(e->obj);
						if (e->nx != 0 || e->ny != 0)
						{
							potRoast->DeactivateObject();
							lifePoint += 6;
						}
					}
					else if (dynamic_cast<CDagger *>(e->obj))
					{
						CDagger *dagger = dynamic_cast<CDagger *>(e->obj);
						if (e->nx != 0 || e->ny != 0)
						{
							if (e->ny != 0)
								y -= min_ty*dy + e->ny*0.3f;
							dagger->DeactivateObject();
							subWeapon->SetState(SUBWEAPON_STATE_1);
						}
					}
					else if (dynamic_cast<CHeart *>(e->obj))
					{
						CHeart *heart = dynamic_cast<CHeart *>(e->obj);
						if (e->nx != 0 || e->ny != 0)
						{
							if (e->ny != 0)
								y -= min_ty*dy + e->ny*0.3f;
							heart->DeactivateObject();
							heartPoint++;
						}
					}
					else if (dynamic_cast<CWhip *>(e->obj))
					{
						CWhip *whip = dynamic_cast<CWhip *>(e->obj);
						if (e->nx != 0 || e->ny != 0)
						{
							if (e->ny != 0)
								y -= min_ty*dy + e->ny*0.3f;
							whip->DeactivateObject();
							weaponLvl++;
							if (weaponLvl > 3)
								weaponLvl = 3;
							SetState(SIMON_STATE_TAKE_WEAPON);//luom weapom
						}
					}
					else if (dynamic_cast<CCross *>(e->obj))
					{
						CCross *cross = dynamic_cast<CCross *>(e->obj);
						if (e->nx != 0 || e->ny != 0)
						{
							if (e->ny != 0)
								y -= min_ty*dy + e->ny*0.3f;
							cross->DeactivateObject();
							ActivateCross();
						}
					}
					else if (dynamic_cast<CCandle *>(e->obj))
					{
						CCandle *candle = dynamic_cast<CCandle *>(e->obj);
						if (e->nx != 0)
						{
							x += dx;
						}
						else if (e->ny != 0)
						{
							y += dy;
						}
					}
					else if (dynamic_cast<CHolyFire *>(e->obj))
					{
						CHolyFire *holyFire = dynamic_cast<CHolyFire *>(e->obj);
						if (e->nx != 0)
						{
							x += dx;
						}
						if (e->ny != 0)
						{
							y += dy;
						}
					}
					else if (dynamic_cast<CBrickStair *>(e->obj))
					{
						CBrickStair *brickStair = dynamic_cast<CBrickStair *>(e->obj);
						if (e->nx != 0 || e->ny != 0)
						{
							isTouchingStair = true;
							getBrickStairNy = brickStair->ny2;
							getBrickStairNx = brickStair->nx;
							if ( brickStair->ny2 != ny2 && isOnStair == true)
							{
								isOnStair = false;
								if (brickStair->ny2 > 0)
								{
									if (brickStair->nx > 0)
									{
										x = brickStair->x + SIMON_BBOX_WIDTH - 16;
										y = brickStair->y - SIMON_BBOX_HEIGHT;
									}
									else
									{
										x = brickStair->x - SIMON_BBOX_WIDTH;
										y = brickStair->y - SIMON_BBOX_HEIGHT;
									}
								}
								else
								{
									if (brickStair->nx > 0)
									{
										x = brickStair->x;
										y = brickStair->y - SIMON_BBOX_HEIGHT;
									}
									else
									{
										x = brickStair->x;
										y = brickStair->y - SIMON_BBOX_HEIGHT;
									}

								}
								vx = 0;
								isTouchingStair = false;
							}
							else if (state == SIMON_STATE_CLIMBING_RIGHT_ON_THE_STAIR || state == SIMON_STATE_CLIMBING_LEFT_ON_THE_STAIR)
							{
								if (isOnStair == false)
								{
									if (brickStair->ny2 > 0)
									{
										if (brickStair->nx > 0)
											x = brickStair->x + SIMON_BBOX_WIDTH-17;
										else
											x = brickStair->x - SIMON_BBOX_WIDTH+2;
									}
									else
									{
										if (brickStair->nx > 0)
											x = brickStair->x ;
										else
											x = brickStair->x -16;
									}
									ny2 = brickStair->ny2;
									firstBrickStairNy = brickStair->ny2;
									firstBrickStairNx = brickStair->nx;
									isOnStair = true;
								}
							}
							else
								x += dx;
						}
					}
					else if (dynamic_cast<CCheckPoint *>(e->obj))
					{
						CCheckPoint *checkPoint = dynamic_cast<CCheckPoint *>(e->obj);
						if (e->nx != 0 || e->ny != 0)
						{
							changeStage = true;
							if (gameStage == 0)
								timeChangeStage = SIMON_TIME_CHANGE_STAGE_0;
							else
							{
								SetState(SIMON_STATE_IDLE);
								timeChangeStage = SIMON_TIME_CHANGE_STAGE;
								timeChangeStageWalking = SIMON_TIME_CHANGE_STAGE_WALKING;
							}
						}
					}
					else if (dynamic_cast<CBrickWater *>(e->obj))
					{
						CBrickWater *brickWater = dynamic_cast<CBrickWater *>(e->obj);
						if (e->nx != 0 || e->ny != 0)
						{
							y = brickWater->y + 128;
							SetState(SIMON_STATE_DIE);
							vx = 0;

						}
					}
					else if (dynamic_cast<CWaterUnderground *>(e->obj))
					{
						CWaterUnderground *waterUnderGround = dynamic_cast<CWaterUnderground *>(e->obj);
						if (e->ny != 0)
						{
							waterUnderGround->SetState(WATER_STATE_BLAST);
						}
						y += dy;
					}
					else if (dynamic_cast<CBrickUnderground *>(e->obj))
					{
						CBrickUnderground *brickUnderground = dynamic_cast <CBrickUnderground *>(e->obj);
						if (e->nx != 0 || e->ny != 0)
						{
							if (brickUnderground->ny2>0)
								isUnderground = true;
							else
								isUnderground = false;
							if (brickUnderground->ny2 > 0)
							{
								if (brickUnderground->nx > 0)
								{
									x = brickUnderground->x;
									y = brickUnderground->y + SIMON_BBOX_HEIGHT + (SIMON_BBOX_HEIGHT / 2);
								}
								else
								{
									x = brickUnderground->x;
									y = brickUnderground->y - SIMON_BBOX_HEIGHT - (SIMON_BBOX_HEIGHT / 2);
								}
							}
							else
							{
								if (brickUnderground->nx > 0)
								{
									x = brickUnderground->x;
									y = brickUnderground->y - SIMON_BBOX_HEIGHT;
								}
								else
								{
									x = brickUnderground->x;
									y = brickUnderground->y - SIMON_BBOX_HEIGHT - SIMON_BBOX_HEIGHT - (SIMON_BBOX_HEIGHT / 2);
								}


							}
						}
					}
				}
			}
			// clean up collision events
			for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

			// xet va cham cua roi
			if (isAttacking == true && currentFrameRendering == 2)
			{
				coEvents.clear();//clear
				coEventsResult.clear();
				if (nx > 0)
				{
					weapon->x = x + 43;
					weapon->y = y + 12;
				}
				else
				{
					tempX = x - 12;
					weapon->x = tempX - 46;
					weapon->y = tempY + 12;
					if (weaponLvl == 3)
					{
						weapon->x -= 32;
					}
				}
				weapon->nx = this->nx;
				weapon->CalcPotentialCollisions2(coObjects, coEvents);
				if (coEvents.size() != 0)
				{
					float min_tx, min_ty, nx = 0, ny;
					weapon->FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
					
					float l, t, r, b;
					weapon->GetBoundingBox(l, t, r, b);
					if (this->nx > 0)
						hitEffect->SetPosition(weapon->x + min_tx*(r - l), weapon->y);
					else
						hitEffect->SetPosition(weapon->x + (1 - min_tx)*(r - l), weapon->y);
					
					for (UINT i = 0; i < coEventsResult.size(); i++)
					{
						LPCOLLISIONEVENT e = coEventsResult[i];
						if (dynamic_cast<CHolyFire *>(e->obj))
						{
							CHolyFire *holyFire = dynamic_cast<CHolyFire *>(e->obj);
							if (e->nx != 0)
							{
								holyFire->SetState(HOLYFIRE_STATE_INACTIVATE);
								hitEffect->ActivateObject();
								hitEffect->ResetTimeAnimation();
							}

						}
						else if (dynamic_cast<CBrickDestroyable *>(e->obj))
						{
							CBrickDestroyable *brickDestroyable = dynamic_cast<CBrickDestroyable *>(e->obj);
							if (e->nx != 0)
							{
								brickDestroyable->SetState(BRICK_DESTROYABLE_STATE_INACTIVATE);
								hitEffect->ActivateObject();
								hitEffect->ResetTimeAnimation();
							}
						}
						else if (dynamic_cast<CCandle *>(e->obj))
						{
							CCandle *candle = dynamic_cast<CCandle *>(e->obj);
						
							if (e->nx != 0)
							{
								candle->SetState(CANDLE_STATE_INACTIVATE);
								hitEffect->ActivateObject();
								hitEffect->ResetTimeAnimation();
							}
						}
						else if (dynamic_cast<CGhoul *>(e->obj))
						{
							CGhoul *ghoul = dynamic_cast<CGhoul *>(e->obj);
							if (e->nx != 0 || e->ny != 0)
							{
								ghoul->SetState(GHOUL_STATE_DIE);
								score += ghoul->GetPoint();
								hitEffect->ActivateObject();
								hitEffect->ResetTimeAnimation();
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
									hitEffect->ActivateObject();
									hitEffect->ResetTimeAnimation();
								}
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
									hitEffect->ActivateObject();
									hitEffect->ResetTimeAnimation();
								}
							}
						}
						else if (dynamic_cast<CFishBullet *>(e->obj))
						{
							CFishBullet *fishBullet = dynamic_cast<CFishBullet *>(e->obj);
							if (e->nx != 0 || e->ny != 0)
							{
								fishBullet->DeactivateObject();
								hitEffect->ActivateObject();
								hitEffect->ResetTimeAnimation();
							}
						}
						else if (dynamic_cast<CBatRed *>(e->obj))
						{
							CBatRed *batRed = dynamic_cast<CBatRed *>(e->obj);
							if (e->nx != 0 || e->ny != 0)
							{
								batRed->SetState(BAT_RED_FLY_STATE_DIE);
								score += batRed->GetPoint();
								hitEffect->ActivateObject();
								hitEffect->ResetTimeAnimation();
							}
						}
						else if (dynamic_cast<CBossBat *>(e->obj))
						{
							CBossBat *bossBat = dynamic_cast<CBossBat *>(e->obj);
							if (e->nx != 0 )
							{
								if (bossBat->GetState() != BOSSBAT_STATE_WAS_HIT)
								{
									hitEffect->ActivateObject();
									hitEffect->ResetTimeAnimation();
									if (bossBat->lifePoint > 0)
									{
										bossBat->lifePoint -= shotMutiplier;
										bossBat->SetState(BOSSBAT_STATE_WAS_HIT);
									}
									else
										bossBat->SetState(BOSSBAT_STATE_DIE);
								}
							}
						}
					}
				}
				for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
			}
		}
	}
}

void CSimon::Render()
{
	hitEffect->Render();
	tempX = x;
	tempY = y;
	alpha = 255;
	if (untouchable == 1 || isInvisibility == true)
		alpha = 128;
	if (currentFrameRendering  < currentFramesSize && currentFramesSize != 0)
	{
		if (isJumping == true && GetState() == SIMON_STATE_ATTACK )
		{
			attackWhenjump = true;
			isJumping = false;
			animations[ani]->ResetCurrentFrame();
			currentFrameRendering = 0;
			currentFramesSize = 0;
			RenderWhenAttack();
		}
		if (isAttacking == true)
		{
			RenderWhenAttack();
		}
		if (isJumping == true)
		{
			RenderWhenJump();
		}
		if (isClimbingOnStair == true)
		{
			RenderWhenClimbingOnStair();
		}
		if (isTakingWeapon == true)
		{
			RenderWhenTakeWeapon();
		}
		if (wasHitting == true)
		{
			RenderWhenWasHit();
		}
		if (wasKilling == true)
		{
			RenderWhenDie();
		}
		if (currentFrameRendering + 1 == currentFramesSize)
		{
			animations[ani]->ResetCurrentFrame();//tai vi currentFrame = -1 thi no ko bi giat
			currentFrameRendering = 0;
			currentFramesSize = 0;
			isAttacking = false;
			isJumping = false;
			attackWhenjump = false;
			isClimbingOnStair = false;
			isTakingWeapon = false;
			wasHitting = false;
			if (wasKilling == true)
			{
				if(lifePoint==0)
					timeLife = saveTimeLife;
				ResetSimon();
			}
			wasKilling = false;
			if (isOnStair == true)
				SetState(SIMON_STATE_IDLE_ON_THE_STAIR);
			else if (isCrouching == true)
				SetState(SIMON_STATE_CROUCH);//sau khi danh xong khong dung day
			else
				SetState(SIMON_STATE_IDLE);
		}
	}
	else if (GetState() == SIMON_STATE_ATTACK)
	{
		if (isAttacking == false)
		{
			RenderWhenAttack();
		}
	}
	else if (GetState() == SIMON_STATE_JUMP)
	{
		if (isJumping == false )
		{
			RenderWhenJump();
		}
	}
	else if (GetState() == SIMON_STATE_CLIMBING_LEFT_ON_THE_STAIR)
	{
		if (isClimbingOnStair == false)
		{
			RenderWhenClimbingOnStair();
		}
	}
	else if (GetState() == SIMON_STATE_CLIMBING_RIGHT_ON_THE_STAIR)
	{
		if (isClimbingOnStair == false)
		{
			RenderWhenClimbingOnStair();
		}
	}
	else if (GetState() == SIMON_STATE_TAKE_WEAPON)
	{
		if (isTakingWeapon == false)
		{
			RenderWhenTakeWeapon();
		}
	}
	else if (GetState() == SIMON_STATE_WAS_HIT)
	{

		if (wasHitting == false)
		{
			RenderWhenWasHit();
		}
	}
	else if (GetState() == SIMON_STATE_DIE)
	{
		if ( wasKilling == false)
		{
			RenderWhenDie();
		}
	}
	else
	{
		if (vx == 0)
		{
			if (GetState() == SIMON_STATE_CROUCH)
			{
				isCrouching = true;
				tempY += 14;
				if (nx > 0) ani = SIMON_ANI_CROUCH_RIGHT;
				else ani = SIMON_ANI_CROUCH_LEFT;
			}
			else if (GetState() == SIMON_STATE_IDLE_BEHIDE)
				ani = SIMON_ANI_IDLE_BEHIDE;
			else
			{
				if (isOnStair == true)
				{
					if (nx > 0)
					{
						if (ny2 < 0)
							ani = SIMON_ANI_IDLE_ON_STAIR_CLIMB_UP_RIGHT;
						if (ny2 > 0)
							ani = SIMON_ANI_IDLE_ON_STAIR_CLIMB_DOWN_RIGHT;
					}
					else if (nx < 0)
					{
						if (ny2 < 0)
							ani = SIMON_ANI_IDLE_ON_STAIR_CLIMB_UP_LEFT;
						if (ny2 > 0)
							ani = SIMON_ANI_IDLE_ON_STAIR_CLIMB_DOWN_LEFT;
					}
				}
				else
				{
					isCrouching = false;
					if (nx > 0) ani = SIMON_ANI_IDLE_RIGHT;
					else ani = SIMON_ANI_IDLE_LEFT;
				}
			}
		}
		else
		{
			if (vx > 0) ani = SIMON_ANI_WALKING_RIGHT;
			else ani = SIMON_ANI_WALKING_LEFT;		
		}
		if (isUsePotion == true)
			aniPotion = ani + 33;
		else
			aniPotion = ani;
		animations[aniPotion]->Render(x, tempY, alpha);
		/*RenderBoundingBox();*///Cap nhat lai bouding box
	}
}

void CSimon::RenderWhenAttack()
{
	isAttacking = true;
	int aniWeaponX;
	int aniWeaponY;
	if (nx > 0)
	{
		if (weaponLvl == 1)
			aniWeapon = 0;
		else if (weaponLvl == 2)
			aniWeapon = 2;
		if (isOnStair == true)
		{
			if (ny2 < 0)
				ani = SIMON_ANI_ATTACK_CLIMB_UP_RIGHT;
			if (ny2 > 0)
				ani = SIMON_ANI_ATTACK_CLIMB_DOWN_RIGHT;
		}
		else if (isCrouching == true)
		{
			ani = SIMON_ANI_ATTACK_CROUCH_RIGHT;
			tempY += 14;
		}
		else if (attackWhenjump == true)
			ani = SIMON_ANI_ATTACK_STAND_RIGHT_LONG;
		else
			ani = SIMON_ANI_ATTACK_STAND_RIGHT;
		
		if (isUsePotion == true)
			aniPotion = ani + 33;
		else
			aniPotion = ani;
		animations[aniPotion]->NextFrame(currentFrameRendering, currentFramesSize);

		if (currentFrameRendering == 0)
		{
			tempX = x - 16;
			aniWeaponX = tempX - 16;
			aniWeaponY = tempY + 14;
			if (weaponLvl == 3)
				aniWeapon = 4;
		}
		if (currentFrameRendering == 1)
		{
			aniWeaponX = tempX - 31;
			aniWeaponY = tempY + 6;
			if (weaponLvl == 3)
				aniWeapon = 5;
		}
		if (currentFrameRendering == 2)
		{
			aniWeaponX = tempX + 43;
			aniWeaponY = tempY + 13;
			if (weaponLvl == 3)
			{
				aniWeapon = 6;
			}
		}
	}
	else
	{
		if (weaponLvl == 1)
			aniWeapon = 1;
		else if (weaponLvl == 2)
			aniWeapon = 3;
		if (isOnStair == true)
		{
			if (ny2 > 0)
				ani = SIMON_ANI_ATTACK_CLIMB_DOWN_LEFT;
			if (ny2 < 0)
				ani = SIMON_ANI_ATTACK_CLIMB_UP_LEFT;
		}
		else if (isCrouching == true)
		{
			ani = SIMON_ANI_ATTACK_CROUCH_LEFT;
			tempY += 14;
		}
		else
		{
			ani = SIMON_ANI_ATTACK_STAND_LEFT;
			if (attackWhenjump == true)
			{
				ani = SIMON_ANI_ATTACK_STAND_LEFT_LONG;
			}
		}
		if (isUsePotion == true)
			aniPotion = ani + 33;
		else
			aniPotion = ani;
		animations[aniPotion]->NextFrame(currentFrameRendering, currentFramesSize);
		if (currentFrameRendering == 0)
		{
			aniWeaponX = tempX + 47;
			aniWeaponY = tempY + 14;
			if (weaponLvl == 3)
				aniWeapon = 7;
		}
		if (currentFrameRendering == 1)
		{
			aniWeaponX = tempX + 32;
			aniWeaponY = tempY + 7;
			if (weaponLvl == 3)
				aniWeapon = 8;

		}
		if (currentFrameRendering == 2)
		{
			tempX = x - 12;//do do tay nen bi lui lai
			aniWeaponX = tempX - 46;
			aniWeaponY = tempY + 13;
			if (weaponLvl == 3)
			{
				aniWeapon = 9;
				aniWeaponX -= 32;
			}
		}
	}
	if (isUsePotion == true)
		aniPotion = ani + 33;
	else
		aniPotion = ani;
	animations[aniPotion]->Render2(tempX, tempY, alpha);
	if (subWeaponIsActivate == true && subWeapon->GetState() != SUBWEAPON_STATE_0)
	{
		if (subWeapon->GetState() == SUBWEAPON_STATE_5 && useStopWatch == false)
		{
			heartPoint -= 5;
			ActivateStopWatch();
			SetState(SIMON_STATE_IDLE);
		}
		else if (heartPoint > 0 && subWeapon->isActive == false && currentFrameRendering == 2)
		{
			heartPoint--;
			subWeapon->nx = nx;
			subWeapon->SetPosition(x, aniWeaponY - 5);
			subWeapon->InitTimeLife();
			if (subWeapon->GetState() == SUBWEAPON_STATE_3 && attackWhenjump == true)
				subWeapon->timeLife = SUBWEAPON3_TIMELIFE_2;
			subWeapon->ActivateObject();
		}
	}
	else 
	{
		if (currentFrameRendering <= 2)
		{
			if (weaponLvl == 1 || weaponLvl == 2)
			{
				weapon->animations[aniWeapon]->SetCurrentFrame(currentFrameRendering);
				weapon->animations[aniWeapon]->Render2(aniWeaponX, aniWeaponY);
			}
			if (weaponLvl == 3)
			{
				aniWeaponY += 5;
				weapon->animations[aniWeapon]->Render(aniWeaponX, aniWeaponY);
			}
			weapon->RenderBoundingBox();
		}
	}
}

void CSimon::RenderWhenJump()
{
	isJumping = true;
	if (nx > 0)
	{
		ani = SIMON_ANI_JUMP_RIGHT;
	}
	else
	{
		ani = SIMON_ANI_JUMP_LEFT;
	}
	if (isUsePotion == true)
		aniPotion = ani + 33;
	else
		aniPotion = ani;
	animations[aniPotion]->NextFrame(currentFrameRendering, currentFramesSize);
	animations[aniPotion]->Render2(x, y, alpha);
}

void CSimon::RenderWhenClimbingOnStair()
{
	isClimbingOnStair = true;
	if (state == SIMON_STATE_CLIMBING_RIGHT_ON_THE_STAIR)
	{
		if (ny2 < 0)
			ani = SIMON_ANI_CLIMB_UP_RIGHT;
		if (ny2 > 0)
			ani = SIMON_ANI_CLIMB_DOWN_RIGHT;
	}
	else if (state == SIMON_STATE_CLIMBING_LEFT_ON_THE_STAIR)
	{
		if (ny2 < 0)
			ani = SIMON_ANI_CLIMB_UP_LEFT;
		if (ny2 > 0)
			ani = SIMON_ANI_CLIMB_DOWN_LEFT;
	}
	if (isUsePotion == true)
		aniPotion = ani + 33;
	else
		aniPotion = ani;
	animations[aniPotion]->NextFrame(currentFrameRendering, currentFramesSize);
	animations[aniPotion]->Render2(x, y, alpha);
}

void CSimon::RenderWhenTakeWeapon()
{
	isTakingWeapon = true;
	if (nx > 0)
		ani = SIMON_ANI_ATTACK_TAKE_WEAPON_RIGHT;
	else
		ani = SIMON_ANI_ATTACK_TAKE_WEAPON_LEFT;
	if (isUsePotion == true)
		aniPotion = ani + 33;
	else
		aniPotion = ani;
	animations[aniPotion]->NextFrame(currentFrameRendering, currentFramesSize);
	animations[aniPotion]->Render2(x, y, alpha);
}

void CSimon::RenderWhenWasHit()
{
	tempY = y;
	wasHitting = true;
	if (nx > 0)
		ani = SIMON_ANI_WAS_HIT_RIGHT;
	else
		ani = SIMON_ANI_WAS_HIT_LEFT;
	if (isUsePotion == true)
		aniPotion = ani + 33;
	else
		aniPotion = ani;
	animations[aniPotion]->NextFrame(currentFrameRendering, currentFramesSize);
	if (currentFrameRendering == 2)
		tempY = y + 16;
	animations[aniPotion]->Render2(x, tempY, alpha);
}

void CSimon::RenderWhenDie()
{
	tempY = y;
	wasKilling = true;
	if (nx > 0)
		ani = SIMON_ANI_DIE_RIGHT;
	else
		ani = SIMON_ANI_DIE_LEFT;
	animations[ani]->NextFrame(currentFrameRendering, currentFramesSize);
	if (isUsePotion == true)
		aniPotion = ani + 33;
	else
		aniPotion = ani;
	if (currentFrameRendering >= 2)
		tempY = y + 32;
	animations[aniPotion]->Render2(x, tempY, alpha);
}

void CSimon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_TAKE_WEAPON:
		vx = 0;
		break;
	case SIMON_STATE_IDLE_ON_THE_STAIR:
		vx = 0;
		vy = 0;
		break;
	case SIMON_STATE_CLIMBING_RIGHT_ON_THE_STAIR:
		nx = 1;
		if (isOnStair == true)
		{
			if (firstBrickStairNy < 0)
			{
				if (firstBrickStairNx > 0)
				{
					vy = -SIMON_WALKING_SPEED;
					ny2 = -1;
				}
				else
				{
					vy = SIMON_WALKING_SPEED;
					ny2 = 1;
				}
			}
			else if (firstBrickStairNy > 0)
			{
				if (firstBrickStairNx > 0)
				{
					vy = SIMON_WALKING_SPEED;
					ny2 = 1;
				}
				else
				{
					vy = -SIMON_WALKING_SPEED;
					ny2 = -1;
				}
			}
			vx = SIMON_WALKING_SPEED;
		}
		break;
	case SIMON_STATE_CLIMBING_LEFT_ON_THE_STAIR:
		nx = -1;
		if (isOnStair == true)
		{
			if (firstBrickStairNy < 0)
			{
				if (firstBrickStairNx > 0)
				{
					vy = SIMON_WALKING_SPEED;
					ny2 = 1;
				}
				else
				{
					vy = -SIMON_WALKING_SPEED;
					ny2 = -1;
				}
			}
			else if (firstBrickStairNy > 0)
			{
				if (firstBrickStairNx > 0)
				{
					vy = -SIMON_WALKING_SPEED;
					ny2 = -1;
				}
				else
				{
					vy = SIMON_WALKING_SPEED;
					ny2 = 1;
				}
			}
			vx = -SIMON_WALKING_SPEED;
		}
		break;
	case SIMON_STATE_WALKING_RIGHT:
		isTouchingStair = false;
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		isTouchingStair = false;
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_JUMP:
		vy = -SIMON_JUMP_SPEED_Y;
		break;
	case SIMON_STATE_IDLE:
		vx = 0;
		break;
	case SIMON_STATE_IDLE_BEHIDE:
		vx = 0;
		break;
	case SIMON_STATE_CROUCH:
		vx = 0;
		break;
	case SIMON_STATE_DIE:
		vy = -SIMON_DIE_DEFLECT_SPEED;
		if (nx > 0)
			vx = -SIMON_WALKING_SPEED;
		else
			vx = SIMON_WALKING_SPEED;
		break;
	case SIMON_STATE_WAS_HIT:
		vy = -SIMON_DIE_DEFLECT_SPEED;
		if (nx > 0)
			vx = -SIMON_WALKING_SPEED;
		else
			vx = SIMON_WALKING_SPEED;
		break;
	case SIMON_STATE_ATTACK:
		vx = 0;
		break;
	}
}

void CSimon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	top = y;
	left = x;
	right = x + SIMON_BBOX_WIDTH;
	bottom = y + SIMON_BBOX_HEIGHT;
	if (GetState() == SIMON_STATE_CROUCH)
	{
		top = y + 14;
		bottom = top + SIMON_BBOX_HEIGHT_STATE_CROUCH;
	}
}

