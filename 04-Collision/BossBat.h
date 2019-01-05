#pragma once
#include "GameObject.h"
#include "FishBullet.h"

#define BOSSBAT_FLYING_SPEED_X 0.1f
#define BOSSBAT_FLYING_SPEED_Y 0.1f

#define BOSSBAT_ANI_IDLE 0
#define BOSSBAT_ANI_ACTIVATE 1
#define BOSSBAT_ANI_DIE 2

#define BOSSBAT_STATE_FLY_SLOW_1 100
#define BOSSBAT_STATE_FLY_SLOW_2 200
#define BOSSBAT_STATE_FLY_SLOW_3 300
#define BOSSBAT_STATE_IDLE 400
#define BOSSBAT_STATE_FLY_ATTACK 500
#define BOSSBAT_STATE_DIE 600
#define BOSSBAT_STATE_WAS_HIT 700

#define BOSSBAT_TIME_ATTACK 1500
#define BOSSBAT_TIME_IDLE 500
#define BOSSBAT_TIME_WAS_HIT 120

#define BOSSBAT_BBOX_WIDTH 80
#define BOSSBAT_BBOX_HEIGHT 46

class CBossBat : public CGameObject
{
public:

	float SimonX;
	float SimonY;

	int lifePoint;
	int timeAttack = BOSSBAT_TIME_ATTACK;
	int timeIdle = BOSSBAT_TIME_IDLE;
	int timeWasHit = BOSSBAT_TIME_WAS_HIT;

	float BossBatTempX;

	CFishBullet *fishBullet;//muon vien dan cua fishmen
	bool useBullet = false;
	void AddFishBullet(CFishBullet *fishBullet);
	
	void UpdateSimonPosition(LPGAMEOBJECT simon);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Render();

	void SetState(int state);
	bool isDestroying = false;

	int currentFrameRendering = 0;
	int currentFramesSize = 0;

	
};