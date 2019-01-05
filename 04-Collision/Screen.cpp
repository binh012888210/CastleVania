#include "Screen.h"

CScreen*CScreen::instance = NULL;

CScreen::CScreen()
{
	score =0;
	timeLife=0;
	stage=0;
	playerLifePoint=0;
	enemyLifePoint=0;
	heartPoint=0;
	playerLife=0;
	shotMultiplier=0;
	subweapon = 0;
}

CScreen::~CScreen()
{
}

void CScreen::UpdateBlackboard(CSimon *p, int stage,int enemyLifePoint)
{
	score = p->score;
	/*score = p->x;*/
	this->timeLife = p->timeLife/1000;
	this->stage = stage;
	playerLifePoint = p->lifePoint;
	this->enemyLifePoint = enemyLifePoint;
	heartPoint = p->heartPoint;
	playerLife = p->life;
	shotMultiplier = p->shotMutiplier;
	subweapon = p->subWeapon->GetState();
}

void CScreen::RenderBlackboard(int CameraX, int CameraY)
{
	sprites = CSprites::GetInstance();
	currentSprite = sprites->Get(200);
	currentSprite->Draw(CameraX, CameraY);
	for (int i = 1; i <= 16; i++)//playerLifePoint
	{
		if (i <= playerLifePoint)
			GetHealthBarSprite(0);
		else
			GetHealthBarSprite(2);
		currentSprite->Draw(CameraX+106 + i*8.0, CameraY+38);
	}
	for (int i = 1; i <= 16; i++)//enemyLifePoint
	{
		if (i <= enemyLifePoint)
			GetHealthBarSprite(1);
		else
			GetHealthBarSprite(2);
		currentSprite->Draw(CameraX+106 + i*8.0, CameraY+55);
	}
	int number[6];
	number[0] = score / 100000;
	number[1] = (score - number[0] * 100000) / 10000;
	number[2] = (score - (number[0] * 100000) - (number[1] * 10000)) / 1000;
	number[3] = (score - (number[2] * 1000) - (number[0] * 100000) - (number[1] * 10000)) / 100;
	number[4] = (score - (number[3] * 100) - (number[2] * 1000) - (number[0] * 100000) - (number[1] * 10000)) / 10;
	number[5] = score - (number[4] * 10)- (number[3] * 100) - (number[2] * 1000) - (number[0] * 100000) - (number[1] * 10000);
	for (int i = 0; i < 6; i++)
	{
		GetNumberSprite(number[i]);
		currentSprite->Draw(CameraX+98 + i*16.0, CameraY+19);
	}
	if (timeLife >= 1000)
		number[0] = timeLife / 1000;
	else
		number[0] = 0;
	number[1] = (timeLife - (number[0] * 1000)) / 100;
	number[2] = (timeLife - (number[1] * 100)- (number[0] * 1000)) / 10;
	number[3] = timeLife - (number[2] * 10) - (number[1] * 100) - (number[0] * 1000);
	for (int i = 0; i < 4; i++)
	{
		GetNumberSprite(number[i]);
		currentSprite->Draw(CameraX+ 292 + i*16.0, CameraY+18);
	}
	number[0] = (stage / 10) ;
	number[1] = (stage - number[0] * 10);
	for (int i = 0; i < 2; i++)
	{
		GetNumberSprite(number[i]);
		currentSprite->Draw(CameraX+456 + i*16.0, CameraY+18);
	}
	number[0] = (heartPoint / 10);
	number[1] = (heartPoint - number[0] * 10);
	for (int i = 0; i < 2; i++)
	{
		GetNumberSprite(number[i]);
		currentSprite->Draw(CameraX+368 + i*16.0, CameraY+38);
	}
	number[0] = (playerLife / 10);
	number[1] = (playerLife - number[0] * 10);
	for (int i = 0; i < 2; i++)
	{
		GetNumberSprite(number[i]);
		currentSprite->Draw(CameraX+368 + i*16.0, CameraY+55);
	}
	if (subweapon != 0)
	{
		GetSubweaponSprite(subweapon);
		currentSprite->Draw(CameraX + 258, CameraY+44);
	}
	if (shotMultiplier != 1)
	{
		GetShotMultiplierSprite(shotMultiplier);
		currentSprite->Draw(CameraX + 416, CameraY+40);
	}
}

void CScreen::GetHealthBarSprite(int number)
{
	switch (number)
	{
	case 0:
		currentSprite = sprites->Get(201);
		break;
	case 1:
		currentSprite = sprites->Get(202);
		break;
	case 2:
		currentSprite = sprites->Get(203);
		break;
	}
}

void CScreen::GetSubweaponSprite(int number)
{
	switch (number)
	{
	case 1:
		currentSprite = sprites->Get(214);
		break;
	case 2:
		currentSprite = sprites->Get(215);
		break;
	case 3:
		currentSprite = sprites->Get(216);
		break;
	case 4:
		currentSprite = sprites->Get(217);
		break;
	case 5:
		currentSprite = sprites->Get(218);
		break;
	}
}

void CScreen::GetNumberSprite(int number)
{
	switch (number)
	{
	case 0:
		currentSprite = sprites->Get(204);
		break;
	case 1:
		currentSprite = sprites->Get(205);
		break;
	case 2:
		currentSprite = sprites->Get(206);
		break;
	case 3:
		currentSprite = sprites->Get(207);
		break;
	case 4:
		currentSprite = sprites->Get(208);
		break;
	case 5:
		currentSprite = sprites->Get(209);
		break;
	case 6:
		currentSprite = sprites->Get(210);
		break;
	case 7:
		currentSprite = sprites->Get(211);
		break;
	case 8:
		currentSprite = sprites->Get(212);
		break;
	case 9:
		currentSprite = sprites->Get(213);
		break;
	}
}

void CScreen::GetShotMultiplierSprite(int number)
{
	switch (number)
	{
	case 2:
		currentSprite = sprites->Get(219);
		break;
	case 3:
		currentSprite = sprites->Get(220);
		break;
	}
}

CScreen *CScreen::GetInstance()
{
	if (instance == NULL)
	{
		instance = new CScreen();
	}
	return instance;
}