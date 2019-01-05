#pragma once
#include "GameObject.h"
#include "FishBullet.h"
#include "Sprites.h"
#include "WaterUnderground.h"

#define FISHMEN_BBOX_WIDTH  32
#define FISHMEN_BBOX_HEIGHT 64

#define FISHMEN_JUMP_SPEED_Y 0.8
#define FISHMEN_GRAVITY 0.002
#define FISHMEN_WALKING_SPEED_X 0.05

#define FISHMEN_STATE_IDLE 100
#define FISHMEN_STATE_JUMP 200
#define FISHMEN_STATE_WALKING 300
#define FISHMEN_STATE_ATTACK 400
#define FISHMEN_STATE_DIE 500

#define FISHMEN_ANI_ATTACK_RIGHT 0
#define FISHMEN_ANI_ATTACK_LEFT 1
#define FISHMEN_ANI_JUMP_RIGHT 2
#define FISHMEN_ANI_JUMP_LEFT 3
#define FISHMEN_ANI_WALKING_RIGHT 4
#define FISHMEN_ANI_WALKING_LEFT 5
#define FISHMEN_ANI_IDE 6

#define FISHMEN_TIME_WALKING 2000
#define FISHMEN_TIME_ATTACK 500

class CFishmen : public CGameObject
{
	
public:
	CFishBullet *fishBullet;
	float timeWalking;
	float timeAttack;

	int currentFrameRendering=0;
	int currentFramesSize=0;

	bool useBullet = false;
	bool isDestroying = false;
	bool checKGroundCollision = false;

	void AddFishBullet(CFishBullet *fishBullet);
	void SetState(int state);
	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
};
