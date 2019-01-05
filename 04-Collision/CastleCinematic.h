#pragma once
#include "GameObject.h"

#define CASTLE_STATE_1 100
#define CASTLE_STATE_2 200

#define CASTLE_ANI_1 0
#define CASTLE_ANI_2 1

#define TIME_LIFE_OF_CASTLE 1500

class CCastleCinematic : public CGameObject
{
	int timeLife = TIME_LIFE_OF_CASTLE;
public:
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
};