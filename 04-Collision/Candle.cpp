#include "Candle.h"

void CCandle::AddBonusItem(LPGAMEOBJECT bonusObject)
{
	this->bonusObject = bonusObject;
}

void CCandle::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (Added == false && GetState() == CANDLE_STATE_INACTIVATE && isActive == false)
	{
		bonusObject->SetPosition(x+4, y);
		bonusObject->ActivateObject();
		bonusObject->nx = -1;
		coObjects->push_back(bonusObject);
		Added = true;
	}
}

void CCandle::Render()
{
	int ani;
	if (isActive == true)
	{
		if (GetState() == CANDLE_STATE_ACTIVATE)
		{
			ani = CANDLE_ANI_ACTIVATE;
			animations[ani]->Render(x, y);
		}
		else if (GetState() == CANDLE_STATE_INACTIVATE)
		{
			ani = CANDLE_ANI_INACTIVATE;
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
					isDestroying == false;
				}
			}
		}
		//RenderBoundingBox();
	}
}



void CCandle::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + CANDLE_BBOX_WIDTH;
	b = y + CANDLE_BBOX_HEIGHT;
	if (GetState() == CANDLE_STATE_INACTIVATE)
	{
		t = 512;
		b = 512;
	}
}




