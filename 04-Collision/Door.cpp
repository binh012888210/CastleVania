#include "Door.h"

void CDoor::Render()
{
	int ani;
	if (isActive == true)
	{
		
		if (GetState() == DOOR_STATE_OPEN)
			ani = DOOR_ANI_OPEN;
		else if (GetState() == DOOR_STATE_CLOSE)
			ani = DOOR_ANI_CLOSE;
		else
		{
			if (doorOpened == true)
				ani = DOOR_ANI_OPENED;
			if (doorOpened == false)
				ani = DOOR_ANI_CLOSED;
		}
	}
	else if (GetState() == DOOR_STATE_IDLE && doorOpened == false)
		{
			ani = DOOR_ANI_CLOSED;
		}
	animations[ani]->Render(x, y);
}

void CDoor::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isActive == true)
	{
		if (timeLife > 0)
			timeLife -= dt;
		else
		{
			if (doorOpened == true)
			{
				doorOpened = false;
				SetState(DOOR_STATE_IDLE);
			}
			else
			{
				doorOpened = true;
				SetState(DOOR_STATE_IDLE);
			}
			timeLife = DOOR_TIME_ANIMATION;
		}
	}
}
