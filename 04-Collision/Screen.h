#pragma once
#include "Simon.h"

#include <d3d9.h>
#include <d3dx9.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include "DefineNumber.h"
#include "Sprites.h"
#include "Textures.h"
#include "Game.h"


class CScreen 
{
	int score;
	int timeLife;
	int stage;
	int playerLifePoint;
	int enemyLifePoint;
	int heartPoint;
	int playerLife;
	int shotMultiplier;
	int subweapon;
public:
	static CScreen *instance;
	static CScreen *GetInstance();
	
	CSprites *sprites;
	CSprite *currentSprite;
	CGame *game;
	
	CScreen();

	void UpdateBlackboard(CSimon *p, int stage, int enemyLifePoint = 16);
	void RenderBlackboard(int CameraX,int CameraY);
	void GetHealthBarSprite(int number);
	void GetSubweaponSprite(int number);
	void GetNumberSprite(int number);
	void GetShotMultiplierSprite(int number);

	~CScreen();
};

