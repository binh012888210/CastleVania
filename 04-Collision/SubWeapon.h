#pragma once

#include "GameObject.h"

#include "Candle.h"
#include "HolyFire.h"

#include "Ghoul.h"
#include "BatRed.h"
#include "Panther.h"
#include "Fishmen.h"
#include "FishBullet.h"
#include "Bossbat.h"

#include "Brick.h"
#include "BrickDestroyable.h"

#define SUBWEAPON_THROW_UP_SPEED_Y	0.5f
#define SUBWEAPON_FLY_SPEED_X 0.5f

#define SUBWEAPON1_ANI_RIGHT	0
#define SUBWEAPON1_ANI_LEFT		1
#define SUBWEAPON2_ANI_RIGHT	2
#define SUBWEAPON2_ANI_LEFT		3
#define SUBWEAPON3_ANI_RIGHT	4
#define SUBWEAPON3_ANI_LEFT		5
#define SUBWEAPON4_ANI_RIGHT	6
#define SUBWEAPON4_ANI_LEFT		7
#define SUBWEAPON4_ANI_EXPLOSE	8


#define SUBWEAPON_STATE_0 0	    //state0=no weapon
#define SUBWEAPON_STATE_1 1		//state1=dagger
#define SUBWEAPON_STATE_2 2		//state2=axe
#define SUBWEAPON_STATE_3 3		//state3=boomerang
#define SUBWEAPON_STATE_4 4		//state1=holywater
#define SUBWEAPON_STATE_5 5		//state1=stopwatch

#define SUBWEAPON1_BBOX_WIDTH 36
#define SUBWEAPON1_BBOX_HEIGHT 36
#define SUBWEAPON2_BBOX_WIDTH 40
#define SUBWEAPON2_BBOX_HEIGHT 40
#define SUBWEAPON3_BBOX_WIDTH 40
#define SUBWEAPON3_BBOX_HEIGHT 40
#define SUBWEAPON4_BBOX_WIDTH 30
#define SUBWEAPON4_BBOX_HEIGHT 30

#define SUBWEAPON1_TIMELIFE 600
#define SUBWEAPON2_TIMELIFE 1000
#define SUBWEAPON3_TIMELIFE 1500
#define SUBWEAPON3_TIMELIFE_2 2500
#define SUBWEAPON4_TIMELIFE 1000

class CSubWeapon:public CGameObject
{
public:
	int score = 0;
	int shotMutiplier;
	bool subWeaponExplose = false;
	int timeLife2 = 0;

	void GetScore(int &score1) { score1 = score; score = 0; };
	void InitTimeLife();
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void ChangeSubWeapon() {  if (timeLife == 0) {state++; if (state > 5)state = 1; } };
};

