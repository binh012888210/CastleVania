#include "Intro.h"

CIntro*CIntro::instance = NULL;

CIntro * CIntro::GetInstance()
{
	if (instance == NULL)
	{
		instance = new CIntro();
	}
	return instance;
}



void CIntro::LoadIntro(CSimon *simon, vector<LPGAMEOBJECT> *objectForIntro1,int timeLife)
{
	finishedIntro = false;
	this->timeLife = timeLife;
	this->simon = simon;

	for (int i = 0; i < objectForIntro1->size(); i++)
		objectsForIntro.push_back(objectForIntro1->at(i));
	this->simon->SetState(SIMON_STATE_WALKING_LEFT);
}
void CIntro::UpdateIntro(DWORD dt)
{
	screen->UpdateBlackboard(simon, 0);
	simon->SetState(SIMON_STATE_WALKING_LEFT);
	timeLife -= dt;
	if (timeLife < 0)
		finishedIntro = true;
	float y, x;
	simon->GetPosition(x, y);
	if (x >(SCREEN_WIDTH / 2))
	{
		simon->Update(dt, NULL);
	}
	else
	{
		simon->SetState(SIMON_STATE_IDLE_BEHIDE);
	}
	for (int i = 0; i < objectsForIntro.size(); i++)
	{
		objectsForIntro[i]->Update(dt,NULL);
	}
}

void CIntro::RenderIntro()
{
	sprites = CSprites::GetInstance();
	game = CGame::GetInstance();
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, D3DCOLOR_XRGB(0, 0, 0));

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
		
		currentSprite = sprites->Get(1018);
		currentSprite->Draw(0, 0);
		screen->RenderBlackboard(0, 0);
		simon->Render();
		for (int i = 0; i < objectsForIntro.size(); i++)
			objectsForIntro[i]->Render();
		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

