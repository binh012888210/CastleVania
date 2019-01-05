#pragma once
#include "GameObject.h"
#include "Brick.h"

#define GHOUL_WALKING_SPEED 0.1f;

#define GHOUL_STATE_WALKING 100
#define GHOUL_STATE_DIE 200

#define GHOUL_ANI_WALKING_RIGHT 0
#define GHOUL_ANI_WALKING_LEFT 1
#define GHOUL_ANI_DIE 2

#define GHOUL_BBOX_WIDTH 20
#define GHOUL_BBOX_HEIGHT 64

class CGhoul : public CGameObject
{
	
public:
	bool Added = false;
	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);

	void SetState(int state);
	bool isDestroying=false;
	int currentFrameRendering=0;
	int currentFramesSize=0;
	LPGAMEOBJECT bonusObject;
	void AddBonusItem(LPGAMEOBJECT bonusObject);
};