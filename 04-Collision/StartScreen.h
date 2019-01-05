#pragma once

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "DefineNumber.h"

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "StartButton.h"
#include "CastleCinematic.h"

class CStartScreen
{
public:
	static CStartScreen *instance;

	bool finishedStartScreen = false;

	CGame *game;
	CSprites *sprites;
	CSprite *currentSprite;
	CCastleCinematic *castleCinematic;
	CStartButton *startButton;

	vector<LPGAMEOBJECT> objectsForStartScreen;
public:
	static CStartScreen* GetInstance();
	void LoadStartScreen(vector<LPGAMEOBJECT> *objectsForStartScreen1);
	void UpdateStartScreen(DWORD dt);
	void RenderStartScreen();
};
