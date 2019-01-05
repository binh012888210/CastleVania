#include "CastleCinematic.h"

void CCastleCinematic::Render()
{
	int ani = CASTLE_ANI_1;
	if (state == CASTLE_STATE_2)
		ani = CASTLE_ANI_2;
	animations[ani]->Render(x, y);
}

void CCastleCinematic::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (timeLife > 0)
		timeLife -= dt;
	else
		state = CASTLE_STATE_2;
}
