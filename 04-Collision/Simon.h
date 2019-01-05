#pragma once
#include "DefineNumber.h"
#include "GameObject.h"
#include "Sprites.h"
#include "Weapon.h"
#include "SubWeapon.h"
#include "HitEffect.h"

#include "Candle.h"
#include "HolyFire.h"

#include "Cross.h"
#include "Heart.h"
#include "Whip.h"
#include "Dagger.h"
#include "ThrowingAxe.h"
#include "BrickStair.h"
#include "HolyWater.h"
#include "MoneyBag.h"
#include "PotRoast.h"
#include "InvisibilityPotion.h"
#include "DoubleShot.h"
#include "StopWatch.h"
#include "CrystalBall.h"

#include "Ghoul.h"
#include "BatRed.h"
#include "Panther.h"
#include "Fishmen.h"
#include "BossBat.h"
#include "FishBullet.h"

#include "Brick.h"
#include "CheckPoint.h"
#include "BrickWater.h"
#include "BrickUnderground.h"
#include "BrickDestroyable.h"
#include "WaterUnderground.h"

//0.13f
#define SIMON_JUMP_SPEED_Y		0.42f
#define SIMON_JUMP_DEFLECT_SPEED 0.3f
#define SIMON_GRAVITY			0.00115f
#define SIMON_DIE_DEFLECT_SPEED	 0.3f

#define SIMON_STATE_IDLE				0
#define SIMON_STATE_IDLE_BEHIDE			100
#define SIMON_STATE_WALKING_RIGHT		200
#define SIMON_STATE_WALKING_LEFT		300
#define SIMON_STATE_IDLE_ON_THE_STAIR	400
#define SIMON_STATE_CLIMBING_RIGHT_ON_THE_STAIR	500
#define SIMON_STATE_CLIMBING_LEFT_ON_THE_STAIR	600
#define SIMON_STATE_JUMP				700
#define SIMON_STATE_WAS_HIT				800
#define SIMON_STATE_DIE					900
#define SIMON_STATE_ATTACK				1000
#define SIMON_STATE_CROUCH				1100
#define SIMON_STATE_TAKE_WEAPON			1200

//Normal
#define SIMON_ANI_IDLE_RIGHT					0
#define SIMON_ANI_IDLE_LEFT						1	
#define SIMON_ANI_WALKING_RIGHT					2
#define SIMON_ANI_WALKING_LEFT					3
#define SIMON_ANI_IDLE_BEHIDE					4
#define SIMON_ANI_CROUCH_RIGHT					5
#define SIMON_ANI_CROUCH_LEFT					6
#define SIMON_ANI_JUMP_RIGHT					7
#define SIMON_ANI_JUMP_LEFT						8
#define SIMON_ANI_IDLE_ON_STAIR_CLIMB_DOWN_RIGHT	9
#define SIMON_ANI_IDLE_ON_STAIR_CLIMB_UP_RIGHT		10
#define SIMON_ANI_CLIMB_DOWN_RIGHT					11
#define SIMON_ANI_CLIMB_UP_RIGHT					12
#define SIMON_ANI_IDLE_ON_STAIR_CLIMB_DOWN_LEFT		13
#define SIMON_ANI_IDLE_ON_STAIR_CLIMB_UP_LEFT		14
#define SIMON_ANI_CLIMB_DOWN_LEFT					15
#define SIMON_ANI_CLIMB_UP_LEFT						16
#define SIMON_ANI_WAS_HIT_RIGHT					17
#define SIMON_ANI_WAS_HIT_LEFT					18
#define SIMON_ANI_DIE_RIGHT						19
#define SIMON_ANI_DIE_LEFT						20
#define SIMON_ANI_ATTACK_STAND_RIGHT			21
#define SIMON_ANI_ATTACK_STAND_LEFT				22
#define SIMON_ANI_ATTACK_STAND_RIGHT_LONG		23
#define SIMON_ANI_ATTACK_STAND_LEFT_LONG		24
#define SIMON_ANI_ATTACK_CROUCH_RIGHT			25
#define SIMON_ANI_ATTACK_CROUCH_LEFT			26
#define SIMON_ANI_ATTACK_CLIMB_DOWN_RIGHT		27
#define SIMON_ANI_ATTACK_CLIMB_DOWN_LEFT		28
#define SIMON_ANI_ATTACK_CLIMB_UP_RIGHT			29
#define SIMON_ANI_ATTACK_CLIMB_UP_LEFT			30
#define SIMON_ANI_ATTACK_TAKE_WEAPON_RIGHT		31
#define SIMON_ANI_ATTACK_TAKE_WEAPON_LEFT		32
//Potion
#define SIMON_ANI_POTION_IDLE_RIGHT						33
#define SIMON_ANI_IPOTION_DLE_LEFT						34	
#define SIMON_ANI_POTION_WALKING_RIGHT					35
#define SIMON_ANI_POTION_WALKING_LEFT					36
#define SIMON_ANI_POTION_IDLE_BEHIDE					37
#define SIMON_ANI_POTION_CROUCH_RIGHT					38
#define SIMON_ANI_POTION_CROUCH_LEFT					39
#define SIMON_ANI_POTION_JUMP_RIGHT						40
#define SIMON_ANI_POTION_JUMP_LEFT						41
#define SIMON_ANI_POTION_IDLE_ON_STAIR_CLIMB_DOWN_RIGHT	42
#define SIMON_ANI_POTION_IDLE_ON_STAIR_CLIMB_UP_RIGHT		43
#define SIMON_ANI_POTION_CLIMB_DOWN_RIGHT					44
#define SIMON_ANI_POTION_CLIMB_UP_RIGHT						45
#define SIMON_ANI_POTION_IDLE_ON_STAIR_CLIMB_DOWN_LEFT		46
#define SIMON_ANI_POTION_IDLE_ON_STAIR_CLIMB_UP_LEFT		47
#define SIMON_ANI_POTION_CLIMB_DOWN_LEFT					48
#define SIMON_ANI_POTION_CLIMB_UP_LEFT						49
#define SIMON_ANI_POTION_WAS_HIT_RIGHT					50
#define SIMON_ANI_POTION_WAS_HIT_LEFT					51
#define SIMON_ANI_POTION_DIE_RIGHT						52
#define SIMON_ANI_POTION_DIE_LEFT						53
#define SIMON_ANI_POTION_ATTACK_STAND_RIGHT				54
#define SIMON_ANI_POTION_ATTACK_STAND_LEFT				55
#define SIMON_ANI_POTION_ATTACK_STAND_RIGHT_LONG		56
#define SIMON_ANI_POTION_ATTACK_STAND_LEFT_LONG			57
#define SIMON_ANI_POTION_ATTACK_CROUCH_RIGHT			58
#define SIMON_ANI_POTION_ATTACK_CROUCH_LEFT				59
#define SIMON_ANI_POTION_ATTACK_CLIMB_DOWN_RIGHT		60
#define SIMON_ANI_POTION_ATTACK_CLIMB_DOWN_LEFT			61
#define SIMON_ANI_POTION_ATTACK_CLIMB_UP_RIGHT			62
#define SIMON_ANI_POTION_ATTACK_CLIMB_UP_LEFT			63
#define SIMON_ANI_POTION_ATTACK_TAKE_WEAPON_RIGHT		64
#define SIMON_ANI_POTION_ATTACK_TAKE_WEAPON_LEFT		65

#define SIMON_BBOX_WIDTH  32
#define SIMON_BBOX_HEIGHT 62
#define SIMON_BBOX_HEIGHT_STATE_CROUCH 48

#define SIMON_UNTOUCHABLE_TIME 1500
#define SIMON_TIME_INVISIBILITY 15000
#define SIMON_TIME_POTION_EFFECTIVE 5000
#define SIMON_TIME_CHANGE_STAGE_WALKING 1200
#define SIMON_TIME_CHANGE_STAGE_0 350
#define SIMON_TIME_CHANGE_STAGE 7300
#define SIMON_TIME_CROSS_ACTIVATE 450
#define SIMON_TIME_STOP_WATCH_ACTIVATE 10000


class CSimon : public CGameObject
{
public:

	CWeapon *weapon;
	CSubWeapon *subWeapon;
	CHitEffect *hitEffect;

	int currentFrameRendering;
	int currentFramesSize;

	int saveTimeLife;
	int timeLife;
	int score;
	int lifePoint;
	int life;
	int heartPoint;
	int untouchable;
	int weaponLvl;
	int shotMutiplier;
	DWORD untouchable_start;

	bool isAttacking;
	bool isCrouching;
	bool isJumping;
	bool attackWhenjump;
	bool isOnStair;
	bool isClimbingOnStair;
	bool isTakingWeapon;
	bool isInvisibility;
	bool isUsePotion;
	bool wasHitting;
	bool wasKilling;

	bool killBoss;
	bool isAtBoss;
	bool atBrickAtBoss;
	bool isUnderground;
	bool isTouchingStair;
	

	int firstBrickStairNy;
	int firstBrickStairNx;
	int getBrickStairNy;
	int getBrickStairNx;

	bool subWeaponIsActivate;

	int gameStage;
	int timeInvisibility;
	int timeChangeStage;
	int timeCrossActivate;
	int timeChangeStageWalking;
	int timeStopWatchActivate;

	bool useStopWatch;
	bool changeStage;
	bool changeStageWalking;
	bool resetStage;
	bool killAllTheEnemy;
	bool exitGame;

	float tempX;
	float tempY;

	int ani;
	int aniPotion;
	int aniWeapon;
	int alpha;

public: 
	CSimon() : CGameObject()
	{
		currentFramesSize = 0;
		currentFrameRendering = 0;
		InitSimon();
	}
	void InitSimonLife(int life) { this->life = life; };
	void InitSimon();
	void ResetSimon();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void RenderWhenAttack();
	void RenderWhenJump();
	void RenderWhenClimbingOnStair();
	void RenderWhenTakeWeapon();
	void RenderWhenWasHit();
	void RenderWhenDie();

	void SetTimeLife(int timeLife) { this->timeLife = timeLife; saveTimeLife = timeLife; };
	void SetStage(int stage) { this->gameStage = stage; };
	void AddWeapon(CWeapon *weapon);
	void AddSubWeapon(CSubWeapon *subWeapon);
	void AddHitEffect(CHitEffect *hitEffect);
	
	void AddScoreFromSubWeapon(int score1) { score += score1; };
	void ActivateSubWeapon() { subWeaponIsActivate = true; };
	void DeactivateSubWeapon() { subWeaponIsActivate = false; };

	void ActivateCross() { killAllTheEnemy = true; timeCrossActivate = SIMON_TIME_CROSS_ACTIVATE; };
	void DeActivateCross() { killAllTheEnemy = false; timeCrossActivate = 0; };

	void ActivateStopWatch() { useStopWatch = true; timeStopWatchActivate = SIMON_TIME_STOP_WATCH_ACTIVATE; };
	void DeActivateStopWatch() { useStopWatch = false; timeStopWatchActivate = 0; };

	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};