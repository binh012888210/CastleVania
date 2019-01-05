#pragma once

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "DefineNumber.h"

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Screen.h"

#include "Simon.h"

class CIntro
{
public:
	static CIntro *instance;

	int timeLife;
	bool finishedIntro;

	CGame *game;
	CSprites *sprites;
	CSprite *currentSprite;
	CSimon *simon;
	CScreen *screen;
	vector<LPGAMEOBJECT> objectsForIntro;
public:
	static CIntro* GetInstance();
	void LoadIntro(CSimon *simon, vector<LPGAMEOBJECT> *objectsForIntro1,int timeLife);
	void UpdateIntro(DWORD dt);
	void RenderIntro();
	void AddScreen(CScreen *screen) { this->screen = screen; };
};

