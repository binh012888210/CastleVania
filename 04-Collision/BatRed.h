#pragma once
#include "GameObject.h"
#include "Brick.h"

#define BAT_RED_FLY_SPEED_X 0.08f
#define BAT_RED_FLY_SPEED_Y 0.01f
#define BAT_RED_FLY_SPEED_Y_MAX 0.1f

#define BAT_RED_FLY_STATE_FLY 100
#define BAT_RED_FLY_STATE_DIE 200

#define BAT_RED_FLY_ANI_RIGHT 0
#define BAT_RED_FLY_ANI_LEFT 1
#define BAT_RED_FLY_ANI_DIE 2

#define BAT_RED_FLY_BBOX_WIDTH 30
#define BAT_RED_FLY_BBOX_HEIGHT 30

class CBatRed : public CGameObject
{
public:
	bool isGoDown = false;

	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);

	void SetState(int state);
	bool isDestroying = false;
	int currentFrameRendering = 0;
	int currentFramesSize = 0;
};



