#pragma once
#include "GameObject.h"

#define CANDLE_BBOX_WIDTH  16
#define CANDLE_BBOX_HEIGHT 32

#define CANDLE_STATE_ACTIVATE 100
#define CANDLE_STATE_INACTIVATE 200

#define CANDLE_ANI_ACTIVATE 0
#define CANDLE_ANI_INACTIVATE 1

class CCandle : public CGameObject
{
	
public:


	bool isDestroying = false;
	int currentFrameRendering=0;
	int currentFramesSize=0;

	bool Added = false;
	LPGAMEOBJECT bonusObject;
	void AddBonusItem(LPGAMEOBJECT bonusObject);

	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};








