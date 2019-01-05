#pragma once
#include "GameObject.h"


#define HOLYFIRE_BBOX_WIDTH  32
#define HOLYFIRE_BBOX_HEIGHT 64

#define HOLYFIRE_STATE_ACTIVATE 100
#define HOLYFIRE_STATE_INACTIVATE 200

#define HOLYFIRE_ANI_ACTIVATE 0
#define HOLYFIRE_ANI_INACTIVATE 1

class CHolyFire : public CGameObject	
{
	bool Added = false;
public:
	
	bool isDestroying = false;
	int currentFrameRendering = 0;
	int currentFramesSize = 0;
	
	LPGAMEOBJECT bonusObject;
	void AddBonusItem(LPGAMEOBJECT bonusObject);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};
