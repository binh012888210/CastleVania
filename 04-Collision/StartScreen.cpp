#include "StartScreen.h"

CStartScreen*CStartScreen::instance = NULL;

CStartScreen * CStartScreen::GetInstance()
{
	if (instance == NULL)
	{
		instance = new CStartScreen();
	}
	return instance;
}



void CStartScreen::LoadStartScreen(vector<LPGAMEOBJECT> *objectsForStartScreen1)
{
	for (int i = 0; i < objectsForStartScreen1->size(); i++)
		objectsForStartScreen.push_back(objectsForStartScreen1->at(i));
}

void CStartScreen::UpdateStartScreen(DWORD dt)
{
	if (objectsForStartScreen[0]->isActive == true)
		finishedStartScreen = true;
	for (int i = 0; i < objectsForStartScreen.size(); i++)
	{
		objectsForStartScreen.at(i)->Update(dt, NULL);
	}
}

void CStartScreen::RenderStartScreen()
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

		currentSprite = sprites->Get(1017);
		currentSprite->Draw(0, 0);
		for (int i = 0; i < objectsForStartScreen.size(); i++)
			objectsForStartScreen[i]->Render();
		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

