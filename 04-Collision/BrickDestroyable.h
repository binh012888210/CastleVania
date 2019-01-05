#pragma once
#include "GameObject.h"
#include "BrickDestroyEffect.h"

#define BRICK_DESTROYABLE_BBOX_WIDTH  32
#define BRICK_DESTROYABLE_BBOX_HEIGHT 20

#define BRICK_DESTROYABLE_STATE_ACTIVATE 100
#define BRICK_DESTROYABLE_STATE_INACTIVATE 200

#define BRICK_DESTROYABLE_ANI_ACTIVATE 0


class CBrickDestroyable : public CGameObject
{
	bool added=false;
public:
	LPGAMEOBJECT bonusObject;
	void AddBonusItem(LPGAMEOBJECT bonusObject);
	CBrickDestroyEffect *brickDestroyEffect;
	void InitBrickDestroyEffect(vector<LPGAMEOBJECT> *coObjects);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};
