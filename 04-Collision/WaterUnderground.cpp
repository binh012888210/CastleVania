#include "WaterUnderground.h"

void CWaterUnderground::InitWaterBlastEffect(vector<LPGAMEOBJECT> *coObjects)
{
	for (int i = 0; i < 3; i++)
	{
		blastWater = new CWaterBlast();
		blastWater->AddAnimation(904);
		blastWater->vy = -WATER_BLAST_UP_SPEED;
		blastWater->x = x + WATER_BBOX_WIDTH / 2;
		blastWater->y = y;
		if (i == 0)
		{
			blastWater->vx = 0.5*WATER_BLAST_SPEED_X;
		}
		else if (i == 1)
		{
			blastWater->vx = WATER_BLAST_SPEED_X;
		}
		else if (i == 2)
		{
			blastWater->vx = -WATER_BLAST_SPEED_X;
		}
		blastWater->ActivateObject();
		coObjects->push_back(blastWater);
	}
}

void CWaterUnderground::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (GetState() == WATER_STATE_BLAST)
	{
		InitWaterBlastEffect(coObjects);
		SetState(WATER_STATE_NORMAL);
	}
}

void CWaterUnderground::Render()
{
	int ani;
	animations[0]->Render(x, y);
	//RenderBoundingBox();
}

void CWaterUnderground::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + WATER_BBOX_WIDTH;
	b = y + WATER_BBOX_HEIGHT;
}








