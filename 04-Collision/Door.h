#pragma once
#include "GameObject.h"

#define DOOR_STATE_IDLE 100
#define DOOR_STATE_OPEN 200
#define DOOR_STATE_CLOSE 300


#define DOOR_ANI_OPEN 0
#define DOOR_ANI_CLOSE 1
#define DOOR_ANI_OPENED 2
#define DOOR_ANI_CLOSED 3

#define DOOR_TIME_ANIMATION 600;

class CDoor : public CGameObject
{
public:
	bool doorOpened=false;
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
};

