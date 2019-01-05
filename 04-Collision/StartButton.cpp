#include "StartButton.h"

void CStartButton::Render()
{
	int ani;
	if (GetState() == START_BUTTON_STATE_IDLE)
		ani = START_BUTTON_ANI_IDLE;
	else 
		ani = START_BUTTON_ANI_PRESS;
	animations[ani]->Render(x,y);
}
void CStartButton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetState() == START_BUTTON_STATE_PRESS)
	{
		if (timeLife < 0)
			ActivateObject();
		else
			timeLife -= dt;
	}
	
}
