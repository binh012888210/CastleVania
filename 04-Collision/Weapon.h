#pragma once
#include "GameObject.h"
#include "Textures.h"

#define WEAPON1_ANI_RIGHT 1
#define WEAPON1_ANI_LEFT 2
#define WEAPON2_ANI_RIGHT 3
#define WEAPON2_ANI_LEFT 4
#define WEAPON3_ANI_RIGHT1 5
#define WEAPON3_ANI_RIGHT2 6
#define WEAPON3_ANI_RIGHT3 7
#define WEAPON3_ANI_LEFT1 8
#define WEAPON3_ANI_LEFT2 9
#define WEAPON3_ANI_LEFT3 10

#define WEAPON_STATE_1 100
#define WEAPON_STATE_2 200
#define WEAPON_STATE_3 300


#define WEAPON_BBOX_WIDTH 48
#define WEAPON_BBOX_HEIGHT 20
#define WEAPON_BBOX_WIDTH_STATE_3 78

class CWeapon :public CGameObject
{
public:
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};