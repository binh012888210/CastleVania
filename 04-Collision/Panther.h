#pragma once
#include "GameObject.h"
#include "BrickStair.h"
#include "Brick.h"
#define PANTHER_RUN_SPEED 0.3f
#define PANTHER_JUMP_SPEED_X 0.3f
#define PANTHER_JUMP_SPEED_Y 0.15f

#define PANTHER_STATE_IDLE 50
#define PANTHER_STATE_RUN 100
#define PANTHER_STATE_JUMP 200
#define PANTHER_STATE_DIE 300

#define PANTHER_ANI_IDLE_RIGHT 0
#define PANTHER_ANI_IDLE_LEFT 1
#define PANTHER_ANI_RUN_RIGHT 2
#define PANTHER_ANI_RUN_LEFT 3
#define PANTHER_ANI_JUMP_RIGHT 4
#define PANTHER_ANI_JUMP_LEFT 5
#define PANTHER_ANI_DIE 6

#define PANTHER_BBOX_WIDTH 64
#define PANTHER_BBOX_HEIGHT 32

class CPanther: public CGameObject
{
	

public:

	bool Added = false;
	void SetState(int state);
	bool isDestroying = false;
	bool jumped = false;

	int currentFrameRendering=0;
	int currentFramesSize=0;

	LPGAMEOBJECT bonusObject;
	void AddBonusItem(LPGAMEOBJECT bonusObject);

	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);


};