#include "BrickDestroyable.h"

void CBrickDestroyable::AddBonusItem(LPGAMEOBJECT bonusObject)
{
	this->bonusObject = bonusObject;
}

void CBrickDestroyable::InitBrickDestroyEffect(vector<LPGAMEOBJECT> *coObjects)
{
	for (int i = 0; i < 4; i++)
	{
		brickDestroyEffect = new CBrickDestroyEffect();
		brickDestroyEffect->AddAnimation(903);
		if (i == 0)
		{
			brickDestroyEffect->x = x - 8;
			brickDestroyEffect->y = y - 8;
			brickDestroyEffect->vx = 0.15;
		}
		else if (i == 1)
		{
			brickDestroyEffect->x = x + 16;
			brickDestroyEffect->y = y - 8;
			brickDestroyEffect->vx = 0.15;
		}
		else if (i == 2)
		{
			brickDestroyEffect->x = x - 8;
			brickDestroyEffect->y = y + 8;
			brickDestroyEffect->vx = -0.2;
		}
		else if (i == 3)
		{
			brickDestroyEffect->x = x + 16;
			brickDestroyEffect->y = y + 8;
			brickDestroyEffect->vx = 0.1;
		}
		brickDestroyEffect->ActivateObject();
		coObjects->push_back(brickDestroyEffect);
		bonusObject->SetPosition(x + 4, y);
		bonusObject->ActivateObject();
		bonusObject->nx = -1;
		coObjects->push_back(bonusObject);
	}
}

void CBrickDestroyable::Update(DWORD dt , vector<LPGAMEOBJECT> *coObjects)
{
	if (added == false && state == BRICK_DESTROYABLE_STATE_INACTIVATE && isActive == false)
	{
		InitBrickDestroyEffect(coObjects);
		added = true;
	}
}

void CBrickDestroyable::Render()
{
	int ani;
	if(isActive==true)
	{
		if (state == BRICK_DESTROYABLE_STATE_ACTIVATE)
		{
			ani = BRICK_DESTROYABLE_ANI_ACTIVATE;
			animations[ani]->Render(x, y);
		}
		else if (state == BRICK_DESTROYABLE_STATE_INACTIVATE)
		{
			DeactivateObject();
		}
	}
	//RenderBoundingBox();
}

void CBrickDestroyable::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y + 2;
	r = x + BRICK_DESTROYABLE_BBOX_WIDTH;
	b = y + BRICK_DESTROYABLE_BBOX_HEIGHT;
	if (isActive == false)
	{
		t = 512;
		b = 512;
	}
}








