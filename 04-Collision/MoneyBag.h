#pragma once
#include "GameObject.h"
#include "Brick.h"
#include "BrickStair.h"

#define MONEY_BAG_BBOX_WIDTH  30
#define MONEY_BAG_BBOX_HEIGHT 30

#define MOEY_BAG_ANI_RED 0
#define MOEY_BAG_ANI_PURPLE 0
#define MOEY_BAG_ANI_WHITE 0//Giong nhau de quyet dinh nam mau nao luc khoi tap

class CMoneyBag : public CGameObject
{
public:
	int moneyBagValue = 0;
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};



