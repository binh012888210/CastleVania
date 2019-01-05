#include "HolyFire.h"


void CHolyFire::AddBonusItem(LPGAMEOBJECT bonusObject)
{
	this->bonusObject = bonusObject;
}

void CHolyFire::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (Added == false && GetState() == HOLYFIRE_STATE_INACTIVATE && isActive == false)
	{
		bonusObject->SetPosition(x + 3, y + HOLYFIRE_BBOX_HEIGHT / 4);
		bonusObject->ActivateObject();
		bonusObject->nx = -1;
		coObjects->push_back(bonusObject);
		Added = true;
	}
}

void CHolyFire::Render()
{
	int ani;
	if (isActive == true)
	{
		if (GetState() == HOLYFIRE_STATE_ACTIVATE)
		{
			ani = HOLYFIRE_ANI_ACTIVATE;
			animations[ani]->Render(x, y);
		}
		else if (GetState() == HOLYFIRE_STATE_INACTIVATE)
		{
			ani = HOLYFIRE_ANI_INACTIVATE;
			if (isDestroying == false)
			{
				animations[ani]->NextFrame(currentFrameRendering, currentFramesSize);
				animations[ani]->Render2(x, y);
				isDestroying = true;
			}
			else if (currentFrameRendering < currentFramesSize)
			{
				animations[ani]->NextFrame(currentFrameRendering, currentFramesSize);
				animations[ani]->Render2(x, y);
				if (currentFrameRendering + 1 == currentFramesSize)
				{
					DeactivateObject();
					currentFrameRendering = 0;
					currentFramesSize = 0;
					isDestroying == false;
				}
			}
		}
		//RenderBoundingBox();
	}
}



void CHolyFire::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + HOLYFIRE_BBOX_WIDTH;
	b = y + HOLYFIRE_BBOX_HEIGHT;
	if (GetState() == HOLYFIRE_STATE_INACTIVATE)
	{
		t = 512;
		b = 512;
	}
}

