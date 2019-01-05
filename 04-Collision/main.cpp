/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102
	
	SAMPLE 04 - COLLISION

	This sample illustrates how to:

		1/ Implement SweptAABB algorithm between moving objects
		2/ Implement a simple (yet effective) collision frame work

	Key functions: 
		CGame::SweptAABB
		CGameObject::SweptAABBEx
		CGameObject::CalcPotentialCollisions
		CGameObject::FilterCollision

		CGameObject::GetBoundingBox
		
================================================================ */

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "DefineNumber.h"

//-objectForCinematic
#include "CastleCinematic.h"
#include "StartButton.h"
#include "BatBlue.h"
#include "Cloud.h"

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "TileMaps.h"
#include "Textures.h"
#include "Screen.h"
#include "Door.h"

#include "Intro.h"
#include "StartScreen.h"

#include "Simon.h"
#include "Weapon.h"
#include "SubWeapon.h"
#include "HitEffect.h"

#include "Candle.h"
#include "Cross.h"
#include "Dagger.h"
#include "Heart.h"
#include "HolyFire.h"
#include "Whip.h"
#include "ThrowingAxe.h"
#include "MoneyBag.h"
#include "HolyWater.h"
#include "InvisibilityPotion.h"
#include "PotRoast.h"
#include "DoubleShot.h"
#include "StopWatch.h"
#include "CrystalBall.h"
#include "Boomearang.h"

#include "BatRed.h"
#include "Ghoul.h"
#include "Fishmen.h"
#include "Panther.h"
#include "BossBat.h"

#include "Brick.h"
#include "BrickStair.h"
#include "CheckPoint.h"
#include "BrickWater.h"
#include "BrickDestroyable.h"
#include "BrickUnderground.h"
#include "BrickDestroyEffect.h"
#include "WaterUnderground.h"

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"Castle Vania"

//-objectForCinematic
CCastleCinematic *castleCinematic;
CStartButton *startButton;
CBatBlue *batBlue;
CCloud *cloud;

CGame *game;
CScreen *screen;
CSprites *sprites;
CTileMaps *tileMap;
CTextures *textures;
CAnimations * animations;
CIntro *introGame;
CStartScreen *startScreen;
CDoor *door;

CSimon *simon;
CWeapon *weapon;
CSubWeapon *subWeapon;
CHitEffect *hitEffect;

CCross *cross;
CDagger *dagger;
CDoubleShot *doubleShot;
CHeart *heart;
CHolyWater *holyWater;
CInvisibilityPotion *invisibilityPotion;
CMoneyBag *moneyBag;
CPotRoast *potRoast;
CThrowingAxe *throwingAxe;
CStopWatch *stopWatch;
CWhip *whip;
CBoomearang *boomearang;

CBatRed *batRed;
CBossBat *bossBat;
CCandle *candle;
CCrystalBall *crystalball;
CFishBullet *fishBullet;
CFishmen *fishmen;
CGhoul *ghoul;
CHolyFire *holyFire;
CPanther *panther;
CBrick *brick;
CBrickStair *brickStair;
CBrickWater *brickWater;
CCheckPoint *checkPoint;
CBrickDestroyable *brickDestroyable;
CBrickUnderground *brickUnderground;
CBrickDestroyEffect *brickDestroyEffect;
CWaterUnderground *waterUnderground;

vector<LPGAMEOBJECT> objects;
vector<LPGAMEOBJECT> coObjects;

//+++++++++++++++++++Tile Map++++++++++++++++++++++
//Tao hai mang
//Mot mang nam trong game.cpp de chua
//Mot mang nam trong ham main de nap
CGameObjects *gameObjects;
vector<int> ObjectIDInCamera2;

class CSampleKeyHander: public CKeyEventHandler
{
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander * keyHandler; 

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	if (game->screenMode == GAMEMODE_START || game->screenMode == GAMEMODE_INTRO)
	{
		if (KeyCode == DIK_RETURN) // enter
			startButton->SetState(START_BUTTON_STATE_PRESS);
	}
	else if(game->pauseGame == true)
	{
		if (KeyCode == DIK_RETURN) // enter
			game->pauseGame = false;
	}
	else
	{
		switch (KeyCode)
		{
		case DIK_RETURN: // reset
			game->pauseGame = true;
			break;
		case DIK_S: // jump
			if (simon->GetState() == SIMON_STATE_JUMP || simon->GetState() == SIMON_STATE_CROUCH
				|| simon->GetState() == SIMON_STATE_ATTACK || simon->isOnStair == true
				|| simon->GetState() == SIMON_STATE_WAS_HIT || simon->GetState() == SIMON_STATE_DIE)
				break;
			else
			{
				simon->SetState(SIMON_STATE_JUMP);
				break;
			}
		//case DIK_A: // reset
		//	simon->SetState(SIMON_STATE_IDLE);
		//	simon->SetPosition(5500, 50.0f);//3900.600
		//	simon->isOnStair = false;
		//	simon->nx = 1;
		//	simon->SetSpeed(0, 0);
		//	break;
		case DIK_C: // reset
			subWeapon->ChangeSubWeapon();
			break;
		case DIK_A:
			if ((simon->isOnStair == true && simon->GetState() != SIMON_STATE_IDLE_ON_THE_STAIR)
				|| simon->GetState() == SIMON_STATE_WAS_HIT || simon->GetState() == SIMON_STATE_DIE)
				break;
			else
			{
				simon->SetState(SIMON_STATE_ATTACK);
				break;
			}
		case DIK_Q:
			game->gameChangeStage=true;
			break;
		}
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_UP:
		simon->DeactivateSubWeapon();
		break;
	}
}

void CSampleKeyHander::KeyState(BYTE *states)
{
	// disable control key when Mario die 
	if (simon->GetState() == SIMON_STATE_DIE || simon->GetState() == SIMON_STATE_ATTACK
		|| simon->GetState() == SIMON_STATE_TAKE_WEAPON || simon->GetState() == SIMON_STATE_JUMP
		|| simon->GetState() == SIMON_STATE_WAS_HIT || simon->changeStage == true|| game->pauseGame == true) return;
	if (game->IsKeyDown(DIK_UP))
	{
		if (simon->isTouchingStair == true)
		{
			if (simon->firstBrickStairNy > 0 )
			{
				if (simon->firstBrickStairNx < 0)
					simon->SetState(SIMON_STATE_CLIMBING_RIGHT_ON_THE_STAIR);
				else
					simon->SetState(SIMON_STATE_CLIMBING_LEFT_ON_THE_STAIR);
			}
			else if (simon->getBrickStairNy < 0)
			{
				if (simon->getBrickStairNx > 0)
					simon->SetState(SIMON_STATE_CLIMBING_RIGHT_ON_THE_STAIR);
				else
					simon->SetState(SIMON_STATE_CLIMBING_LEFT_ON_THE_STAIR);
			}
			else
				simon->ActivateSubWeapon();
		}
		else
			simon->ActivateSubWeapon();
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		if (simon->isTouchingStair == true)
		{
			if (simon->firstBrickStairNy < 0)
			{
				if (simon->firstBrickStairNx < 0)
					simon->SetState(SIMON_STATE_CLIMBING_RIGHT_ON_THE_STAIR);
				else
					simon->SetState(SIMON_STATE_CLIMBING_LEFT_ON_THE_STAIR);
			}
			else if (simon->getBrickStairNy > 0)
			{
				if (simon->getBrickStairNx > 0)
					simon->SetState(SIMON_STATE_CLIMBING_RIGHT_ON_THE_STAIR);
				else
					simon->SetState(SIMON_STATE_CLIMBING_LEFT_ON_THE_STAIR);
			}
			else
				simon->SetState(SIMON_STATE_CROUCH);
		}
		else
			simon->SetState(SIMON_STATE_CROUCH);
	}
	else if (game->IsKeyDown(DIK_RIGHT))
	{
		if (simon->isOnStair == true)
			simon->SetState(SIMON_STATE_CLIMBING_RIGHT_ON_THE_STAIR);
		else
			simon->SetState(SIMON_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (simon->isOnStair == true)
			simon->SetState(SIMON_STATE_CLIMBING_LEFT_ON_THE_STAIR);
		else
			simon->SetState(SIMON_STATE_WALKING_LEFT);
	}
	else if (simon->GetState() == SIMON_STATE_WALKING_RIGHT || simon->GetState() == SIMON_STATE_WALKING_LEFT || 
		simon->GetState() == SIMON_STATE_CROUCH )
		simon->SetState(SIMON_STATE_IDLE);
	else if (simon->GetState() == SIMON_STATE_CLIMBING_RIGHT_ON_THE_STAIR||
		simon->GetState() == SIMON_STATE_CLIMBING_LEFT_ON_THE_STAIR )
		simon->SetState(SIMON_STATE_IDLE_ON_THE_STAIR);
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/* Load animation and Init Simon */
void InitGame()
{
	//Tile-map-matrix	
	tileMap = new CTileMaps();

	tileMap->LoadTiledMap(L"textures\\Maps\\map1_tiled.png");
	tileMap->ReadMatrixFromFile("textures\\Maps\\map1_matrix.txt", "\t");

	gameObjects = CGameObjects::GetInstance();

	//+++++++++++++++++++++++++++++++++++++++++Load texture+++++++++++++++++++++++++++++++++++++++++++++

	textures = CTextures::GetInstance();
	textures->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(100, 100, 100));

	textures->Add(ID_TEX_CASTLE_CINEMATIC, L"textures\\Cinematics\\castle_cinematic.bmp", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_OBJECT_CINEMATIC, L"textures\\Cinematics\\object_cinematic.bmp", D3DCOLOR_XRGB(255, 0, 255));

	
	textures->Add(ID_TEX_SIMON_RIGHT, L"textures\\simon_right.bmp", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_SIMON_LEFT, L"textures\\simon_left.bmp", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_SIMON_POTION_RIGHT, L"textures\\simon_potion_right.bmp", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_SIMON_POTION_LEFT, L"textures\\simon_potion_left.bmp", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_DOOR, L"textures\\door1.bmp", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_WEAPON_RIGHT, L"textures\\whip_right.bmp", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_WEAPON_LEFT, L"textures\\whip_left.bmp", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_MISC, L"textures\\destructible.bmp", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_EFFECT, L"textures\\effect.bmp", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_BONUS_OBJECT, L"textures\\bonus.bmp", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_SUBWEAPON_RIGHT, L"textures\\subweapon_right.bmp", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_SUBWEAPON_LEFT, L"textures\\subweapon_left.bmp", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_CREEP_RIGHT, L"textures\\creep_right.bmp", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_CREEP_LEFT, L"textures\\creep_left.bmp", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_VAMBAT, L"textures\\VamBat.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_BLACKBOARD, L"textures\\blackboard.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_BACKGROUND_START_SCREEN, L"textures\\Cinematics\\start_screen.bmp", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_BACKGROUND_INTRO_1, L"textures\\Cinematics\\intro1.bmp", D3DCOLOR_XRGB(255, 0, 255));

	//++++++++++++++++++++++++++++++++++++++++++Load sprite+++++++++++++++++++++++++++++++++++++++++++++

	sprites = CSprites::GetInstance();
	vector<vector<float>> LoadSpriteMatrix;
	game->ReadMatrixFromFile("textures\\Maps\\LoadSprite.txt", "\t", LoadSpriteMatrix);
	game->LoadSpriteFromMatrix(LoadSpriteMatrix, textures, sprites);

	//+++++++++++++++++++++++++++++++++++++++Load animations++++++++++++++++++++++++++++++++++++++++++

	animations = CAnimations::GetInstance();
	vector<vector<float>> LoadAnimationMatrix;
	game->ReadMatrixFromFile("textures\\Maps\\LoadAnimation.txt", "\t", LoadAnimationMatrix);
	game->LoadAnimationFromMatrix(LoadAnimationMatrix, animations);


	//+++++++++++++++++++++++++++++Add animation to object and Init object++++++++++++++++++++++++++++

	//boss bat
	bossBat = new CBossBat();

	//hit effect
	hitEffect = new CHitEffect();
	hitEffect->AddAnimation(900);

	//weapon
	weapon = new CWeapon();	
	subWeapon = new CSubWeapon();
	simon = new CSimon();

	simon->AddWeapon(weapon);
	simon->AddSubWeapon(subWeapon);
	simon->AddHitEffect(hitEffect);

	vector<vector<float>> LoadSimon;
	LoadSimon.clear();
	game->ReadMatrixFromFile("textures\\Maps\\LoadSimon.txt", "\t", LoadSimon);
	for (int i = 0; i < LoadSimon.size(); i++)
	{
		if (LoadSimon[i][0] == 1)
		{
			for (int j = 1; j < LoadSimon[i].size(); j++)
			{
				weapon->AddAnimation(LoadSimon[i][j]);
				if (LoadSimon[i][j] == 0)
					break;
			}
		}
		else if (LoadSimon[i][0] == 2)
		{
			for (int j = 1; j < LoadSimon[i].size(); j++)
			{
				subWeapon->AddAnimation(LoadSimon[i][j]);
				if (LoadSimon[i][j] == 0)
					break;
			}
		}
		else if (LoadSimon[i][0] == 3)
		{
			for (int j = 1; j < LoadSimon[i].size(); j++)
			{
				simon->AddAnimation(LoadSimon[i][j]);
				if (LoadSimon[i][j] == 0)
					break;
			}
		}
		else if (LoadSimon[i][0] == 4)
		{
			simon->InitSimonLife(LoadSimon[i][1]);
			simon->SetTimeLife(LoadSimon[i][2]);
			simon->subWeapon->SetState(LoadSimon[i][3]);
			simon->weaponLvl = LoadSimon[i][4];
		}
	}
}

/* Load start screen and object  in start screen */
void LoadStartScreen()
{
	vector<LPGAMEOBJECT> objectsForStartScreen;
	startButton = new CStartButton();
	startButton->AddAnimation(112);
	startButton->AddAnimation(113);
	startButton->SetPosition(144, 256);
	startButton->SetState(START_BUTTON_STATE_IDLE);
	objectsForStartScreen.push_back(startButton);
	castleCinematic = new CCastleCinematic();
	castleCinematic->AddAnimation(114);
	castleCinematic->AddAnimation(115);
	castleCinematic->SetPosition(368, 192);
	castleCinematic->SetState(CASTLE_STATE_1);
	objectsForStartScreen.push_back(castleCinematic);
	startScreen->LoadStartScreen(&objectsForStartScreen);
}

/* Load intro 1 */
void LoadIntro1()
{
	vector<LPGAMEOBJECT> objectsForIntro;
	batBlue = new CBatBlue();
	batBlue->AddAnimation(111);
	batBlue->SetPosition(200, 130);
	objectsForIntro.push_back(batBlue);
	cloud = new CCloud();
	cloud->AddAnimation(110);
	cloud->SetPosition(400, 130);
	objectsForIntro.push_back(cloud);
	simon->SetPosition(450, 360);
	introGame->LoadIntro(simon, &objectsForIntro, 2000);
	introGame->AddScreen(screen);
}

/* Load intro and object  in start screen */
void LoadIntro()
{
	switch (game->stage)
	{
		// intro 1
	case 0:
		LoadIntro1();
		break;
		// intro 2
	case 4:
		LoadIntro1();
		break;
		// intro 2
	default:
		break;
	}
}

/* Loader machine */
void LoaderMachine(char * filename, const string & splitchar)
{
	game->ObjectIDInStage = 0;
	objects.clear();//Objects tong cua stage hien tai
	gameObjects->ClearAll();//Ham duoc goi trong update de lay object theo ID
	game->ObjectIDInCamera.clear();//su dung trong ham upsate

	vector<vector<float>> LoadObject;
	LoadObject.clear();
	game->ReadMatrixFromFile(filename, splitchar, LoadObject);
	for (int i = 0; i < LoadObject.size(); i++)
	{
		if (LoadObject[i][0] == 0)
		{
			door = new CDoor();
			door->AddAnimation(LoadObject[i][1]);
			door->AddAnimation(LoadObject[i][2]);
			door->AddAnimation(LoadObject[i][3]);
			door->AddAnimation(LoadObject[i][4]);
			door->SetPosition(LoadObject[i][5], LoadObject[i][6]);
			if (LoadObject[i][7] == 1)
				door->ActivateObject();
			else
				door->DeactivateObject();
			door->SetState(LoadObject[i][8]);
			door->ObjectID = game->ObjectIDInStage;
			objects.push_back(door);
		}
		else if (LoadObject[i][0] == 12)
		{
			batRed = new CBatRed();
			batRed->AddAnimation(LoadObject[i][1]);
			batRed->AddAnimation(LoadObject[i][2]);
			batRed->AddAnimation(LoadObject[i][3]);
			batRed->SetPosition(LoadObject[i][4], LoadObject[i][5]);
			if (LoadObject[i][6] == 1)
				batRed->ActivateObject();
			else
				batRed->DeactivateObject();
			batRed->SetState(LoadObject[i][7]);
			batRed->ObjectID = game->ObjectIDInStage;
			objects.push_back(batRed);
		}
		else if (LoadObject[i][0] == 13)
		{
			bossBat = new CBossBat();
			bossBat->AddAnimation(LoadObject[i][1]);
			bossBat->AddAnimation(LoadObject[i][2]);
			bossBat->AddAnimation(LoadObject[i][3]);
			bossBat->SetPosition(LoadObject[i][4], LoadObject[i][5]);
			if (LoadObject[i][6] == 1)
				bossBat->ActivateObject();
			else
				bossBat->DeactivateObject();
			bossBat->SetState(LoadObject[i][7]);
			bossBat->lifePoint = LoadObject[i][8];
			bossBat->AddFishBullet(fishBullet);
			bossBat->ObjectID = game->ObjectIDInStage;
			objects.push_back(bossBat);
		}
		else if (LoadObject[i][0] == 14)
		{
			candle = new CCandle();
			candle->AddAnimation(LoadObject[i][1]);
			candle->AddAnimation(LoadObject[i][2]);
			candle->SetPosition(LoadObject[i][3], LoadObject[i][4]);
			if (LoadObject[i][5] == 1)
				candle->ActivateObject();
			else
				candle->DeactivateObject();
			candle->SetState(LoadObject[i][6]);
			if (LoadObject[i][7] == 1)
			{
				cross = new CCross();
				cross->AddAnimation(LoadObject[i][8]);
				candle->AddBonusItem(cross);
			}
			else if (LoadObject[i][7] == 2)
			{
				dagger = new CDagger();
				dagger->AddAnimation(LoadObject[i][8]);
				candle->AddBonusItem(dagger);
			}
			else if (LoadObject[i][7] == 3)
			{
				doubleShot = new CDoubleShot();
				doubleShot->AddAnimation(LoadObject[i][8]);
				candle->AddBonusItem(doubleShot);
			}
			else if (LoadObject[i][7] == 4)
			{
				heart = new CHeart();
				heart->AddAnimation(LoadObject[i][8]);
				candle->AddBonusItem(heart);
			}
			else if (LoadObject[i][7] == 5)
			{
				holyWater = new CHolyWater();
				holyWater->AddAnimation(LoadObject[i][8]);
				candle->AddBonusItem(holyWater);
			}
			else if (LoadObject[i][7] == 6)
			{
				invisibilityPotion = new CInvisibilityPotion();
				invisibilityPotion->AddAnimation(LoadObject[i][8]);
				candle->AddBonusItem(invisibilityPotion);
			}
			else if (LoadObject[i][7] == 7)
			{
				moneyBag = new CMoneyBag();
				moneyBag->moneyBagValue = LoadObject[i][8];
				moneyBag->AddAnimation(LoadObject[i][9]);
				candle->AddBonusItem(moneyBag);
			}
			else if (LoadObject[i][7] == 8)
			{
				potRoast = new CPotRoast();
				potRoast->AddAnimation(LoadObject[i][8]);
				candle->AddBonusItem(potRoast);
			}
			else if (LoadObject[i][7] == 9)
			{
				throwingAxe = new CThrowingAxe();
				throwingAxe->AddAnimation(LoadObject[i][8]);
				candle->AddBonusItem(throwingAxe);
			}
			else if (LoadObject[i][7] == 10)
			{
				stopWatch = new CStopWatch();
				stopWatch->AddAnimation(LoadObject[i][8]);
				candle->AddBonusItem(stopWatch);
			}
			else if (LoadObject[i][7] == 11)
			{
				whip = new CWhip();
				whip->AddAnimation(LoadObject[i][8]);
				candle->AddBonusItem(whip);
			}
			else if (LoadObject[i][7] == 29)
			{
				boomearang = new CBoomearang();
				boomearang->AddAnimation(LoadObject[i][8]);
				candle->AddBonusItem(boomearang);
			}
			candle->ObjectID = game->ObjectIDInStage;
			objects.push_back(candle);
		}
		else if (LoadObject[i][0] == 15)
		{
			crystalball = new CCrystalBall();
			crystalball->AddAnimation(LoadObject[i][1]);
			crystalball->SetPosition(LoadObject[i][2], LoadObject[i][3]);
			if (LoadObject[i][4] == 1)
				crystalball->ActivateObject();
			else
				crystalball->DeactivateObject();
			crystalball->ObjectID = game->ObjectIDInStage;
			objects.push_back(crystalball);
		}
		else if (LoadObject[i][0] == 16)
		{
			fishBullet = new CFishBullet();
			fishBullet->AddAnimation(LoadObject[i][1]);
			fishBullet->AddAnimation(LoadObject[i][2]);
			fishBullet->SetPosition(LoadObject[i][3], LoadObject[i][4]);
			fishBullet->ObjectID = game->ObjectIDInStage;
			objects.push_back(fishBullet);
		}
		else if (LoadObject[i][0] == 17)
		{
			fishmen = new CFishmen();
			fishmen->AddAnimation(LoadObject[i][1]);
			fishmen->AddAnimation(LoadObject[i][2]);
			fishmen->AddAnimation(LoadObject[i][3]);
			fishmen->AddAnimation(LoadObject[i][4]);
			fishmen->AddAnimation(LoadObject[i][5]);
			fishmen->AddAnimation(LoadObject[i][6]);
			fishmen->AddAnimation(LoadObject[i][7]);
			fishmen->SetPosition(LoadObject[i][8], LoadObject[i][9]);
			fishmen->AddFishBullet(fishBullet);
			if (LoadObject[i][10] == 1)
				fishmen->ActivateObject();
			else
				fishmen->DeactivateObject();
			fishmen->SetState(LoadObject[i][11]);
			if (LoadObject[i][12] == 1)
				fishmen->checKGroundCollision = true;
			else
				fishmen->checKGroundCollision = false;
			fishmen->ObjectID = game->ObjectIDInStage;
			objects.push_back(fishmen);
		}
		else if (LoadObject[i][0] == 18)
		{
			ghoul = new CGhoul();
			ghoul->AddAnimation(LoadObject[i][1]);
			ghoul->AddAnimation(LoadObject[i][2]);
			ghoul->AddAnimation(LoadObject[i][3]);
			ghoul->SetPosition(LoadObject[i][4], LoadObject[i][5]);
			if (LoadObject[i][6] == 1)
				ghoul->ActivateObject();
			else
				ghoul->DeactivateObject();
			ghoul->SetState(LoadObject[i][7]);
			ghoul->SetPoint(LoadObject[i][8]);
			if (LoadObject[i][9] == 4)
			{
				heart = new CHeart();
				heart->AddAnimation(LoadObject[i][10]);
				ghoul->AddBonusItem(heart);
			}
			else if (LoadObject[i][9] == 11)
			{
				whip = new CWhip();
				whip->AddAnimation(LoadObject[i][10]);
				ghoul->AddBonusItem(whip);
			}
			else if (LoadObject[i][9] == 2)
			{
				dagger = new CDagger();
				dagger->AddAnimation(LoadObject[i][10]);
				ghoul->AddBonusItem(dagger);
			}
			else if (LoadObject[i][9] == 1)
			{
				cross = new CCross();
				cross->AddAnimation(LoadObject[i][10]);
				ghoul->AddBonusItem(cross);
			}
			else if (LoadObject[i][9] == 5)
			{
				holyWater = new CHolyWater();
				holyWater->AddAnimation(LoadObject[i][10]);
				ghoul->AddBonusItem(holyWater);
			}
			else if (LoadObject[i][9] == 7)
			{
				moneyBag = new CMoneyBag();
				moneyBag->moneyBagValue = LoadObject[i][10];
				moneyBag->AddAnimation(LoadObject[i][11]);
				ghoul->AddBonusItem(moneyBag);
			}
			ghoul->ObjectID = game->ObjectIDInStage;
			objects.push_back(ghoul);
		}
		else if (LoadObject[i][0] == 19)
		{
			holyFire = new CHolyFire();
			holyFire->AddAnimation(LoadObject[i][1]);
			holyFire->AddAnimation(LoadObject[i][2]);
			holyFire->SetPosition(LoadObject[i][3], LoadObject[i][4]);
			if (LoadObject[i][5] == 1)
				holyFire->ActivateObject();
			else
				holyFire->DeactivateObject();
			holyFire->SetState(LoadObject[i][6]);
			if (LoadObject[i][7] == 4)
			{
				heart = new CHeart();
				heart->AddAnimation(LoadObject[i][8]);
				holyFire->AddBonusItem(heart);
			}
			else if (LoadObject[i][7] == 11)
			{
				whip = new CWhip();
				whip->AddAnimation(LoadObject[i][8]);
				holyFire->AddBonusItem(whip);
			}
			else if (LoadObject[i][7] == 2)
			{
				dagger = new CDagger();
				dagger->AddAnimation(LoadObject[i][8]);
				holyFire->AddBonusItem(dagger);
			}
			else if (LoadObject[i][7] == 1)
			{
				cross = new CCross();
				cross->AddAnimation(LoadObject[i][8]);
				holyFire->AddBonusItem(cross);
			}
			else if (LoadObject[i][7] == 5)
			{
				holyWater = new CHolyWater();
				holyWater->AddAnimation(LoadObject[i][8]);
				holyFire->AddBonusItem(holyWater);
			}
			else if (LoadObject[i][7] == 7)
			{
				moneyBag = new CMoneyBag();
				moneyBag->moneyBagValue = LoadObject[i][8];
				moneyBag->AddAnimation(LoadObject[i][9]);
				holyFire->AddBonusItem(moneyBag);
			}
			holyFire->ObjectID = game->ObjectIDInStage;
			objects.push_back(holyFire);
		}
		else if (LoadObject[i][0] == 20)
		{
			panther = new CPanther();
			panther->nx = -1;
			panther->AddAnimation(LoadObject[i][1]);
			panther->AddAnimation(LoadObject[i][2]);
			panther->AddAnimation(LoadObject[i][3]);
			panther->AddAnimation(LoadObject[i][4]);
			panther->AddAnimation(LoadObject[i][5]);
			panther->AddAnimation(LoadObject[i][6]);
			panther->AddAnimation(LoadObject[i][7]);
			panther->SetPosition(LoadObject[i][8], LoadObject[i][9]);
			if (LoadObject[i][10] == 1)
				panther->ActivateObject();
			else
				panther->DeactivateObject();
			panther->SetState(LoadObject[i][11]);
			panther->SetPoint(LoadObject[i][12]);
			if (LoadObject[i][13] == 4)
			{
				heart = new CHeart();
				heart->AddAnimation(LoadObject[i][14]);
				panther->AddBonusItem(heart);
			}
			else if (LoadObject[i][13] == 11)
			{
				whip = new CWhip();
				whip->AddAnimation(LoadObject[i][14]);
				panther->AddBonusItem(whip);
			}
			else if (LoadObject[i][13] == 2)
			{
				dagger = new CDagger();
				dagger->AddAnimation(LoadObject[i][14]);
				panther->AddBonusItem(dagger);
			}
			else if (LoadObject[i][13] == 1)
			{
				cross = new CCross();
				cross->AddAnimation(LoadObject[i][14]);
				panther->AddBonusItem(cross);
			}
			else if (LoadObject[i][13] == 5)
			{
				holyWater = new CHolyWater();
				holyWater->AddAnimation(LoadObject[i][14]);
				panther->AddBonusItem(holyWater);
			}
			else if (LoadObject[i][13] == 7)
			{
				moneyBag = new CMoneyBag();
				moneyBag->moneyBagValue = LoadObject[i][14];
				moneyBag->AddAnimation(LoadObject[i][15]);
				panther->AddBonusItem(moneyBag);
			}
			panther->ObjectID = game->ObjectIDInStage;
			objects.push_back(panther);
		}
		else if (LoadObject[i][0] == 21)
		{
			brick = new CBrick();
			brick->AddAnimation(LoadObject[i][1]);
			brick->SetPosition(LoadObject[i][2], LoadObject[i][3]);
			brick->ObjectID = game->ObjectIDInStage;
			objects.push_back(brick);
		}
		else if (LoadObject[i][0] == 22)
		{
			brickStair = new CBrickStair();
			brickStair->AddAnimation(LoadObject[i][1]);
			brickStair->SetPosition(LoadObject[i][2], LoadObject[i][3]);
			brickStair->nx = LoadObject[i][4];
			brickStair->ny2 = LoadObject[i][5];
			brickStair->ObjectID = game->ObjectIDInStage;
			objects.push_back(brickStair);
		}
		else if (LoadObject[i][0] == 23)
		{
			brickWater = new CBrickWater();
			brickWater->AddAnimation(LoadObject[i][1]);
			brickWater->SetPosition(LoadObject[i][2], LoadObject[i][3]);
			brickWater->ObjectID = game->ObjectIDInStage;
			objects.push_back(brickWater);
		}
		else if (LoadObject[i][0] == 24)
		{
			checkPoint = new CCheckPoint();
			checkPoint->AddAnimation(LoadObject[i][1]);
			checkPoint->SetPosition(LoadObject[i][2], LoadObject[i][3]);
			checkPoint->ObjectID = game->ObjectIDInStage;
			objects.push_back(checkPoint);
		}
		else if (LoadObject[i][0] == 25)
		{
			brickDestroyable = new CBrickDestroyable();
			brickDestroyable->AddAnimation(LoadObject[i][1]);
			brickDestroyable->SetPosition(LoadObject[i][2], LoadObject[i][3]);
			if (LoadObject[i][4] == 1)
				brickDestroyable->ActivateObject();
			else
				brickDestroyable->DeactivateObject();
			brickDestroyable->SetState(LoadObject[i][5]);

			if (LoadObject[i][6] == 4)
			{
				heart = new CHeart();
				heart->AddAnimation(LoadObject[i][7]);
				brickDestroyable->AddBonusItem(heart);
			}
			else if (LoadObject[i][6] == 3)
			{
				doubleShot = new CDoubleShot();
				doubleShot->AddAnimation(LoadObject[i][7]);
				brickDestroyable->AddBonusItem(doubleShot);
			}
			else if (LoadObject[i][6] == 8)
			{
				potRoast = new CPotRoast();
				potRoast->AddAnimation(LoadObject[i][7]);
				brickDestroyable->AddBonusItem(potRoast);
			}
			brickDestroyable->ObjectID = game->ObjectIDInStage;
			objects.push_back(brickDestroyable);
		}
		else if (LoadObject[i][0] == 26)
		{
			brickUnderground = new CBrickUnderground();
			brickUnderground->AddAnimation(LoadObject[i][1]);
			brickUnderground->SetPosition(LoadObject[i][2], LoadObject[i][3]);
			brickUnderground->nx = LoadObject[i][4];
			brickUnderground->ny2 = LoadObject[i][5];
			brickUnderground->ObjectID = game->ObjectIDInStage;
			objects.push_back(brickUnderground);
		}
		else if (LoadObject[i][0] == 28)
		{
			waterUnderground = new CWaterUnderground();
			waterUnderground->AddAnimation(LoadObject[i][1]);
			waterUnderground->SetPosition(LoadObject[i][2], LoadObject[i][3]);
			waterUnderground->ObjectID = game->ObjectIDInStage;
			objects.push_back(waterUnderground);
		}
		game->ObjectIDInStage++;
	}

	game->ObjectIDInStage--;

	for (int i = 0; i < objects.size(); i++)
	{
		gameObjects->Add(objects[i]->ObjectID, objects[i]);
	}
	tileMap->LoadGridView(&objects);
}

/* Load object in stage 0  */
void LoadObjectStage0()
{
	simon->nx = 1;
	simon->SetState(SIMON_STATE_IDLE);
	simon->SetPosition(50, 320);

	LoaderMachine("textures\\Maps\\LoadObjectStage0.txt", "\t");

}

/* Load object in stage 1  */
void LoadObjectStage1()
{
	simon->nx = 1;
	simon->SetState(SIMON_STATE_IDLE);
	simon->SetPosition(80, 320);

	LoaderMachine("textures\\Maps\\LoadObjectStage1.txt", "\t");
}

/* Load object in stage 2  */
void LoadObjectStage2()
{
	simon->nx = 1;
	simon->SetPosition(3162, 163.5);
	simon->isUnderground = false;
	simon->isOnStair = false;
	simon->SetState(SIMON_STATE_IDLE);

	LoaderMachine("textures\\Maps\\LoadObjectStage2.txt", "\t");
}

/* Load object in stage 3  */
void LoadObjectStage3()
{
	simon->nx = 1;
	simon->SetPosition(4185, 163.5);
	simon->isUnderground = false;
	simon->isOnStair = false;
	simon->SetState(SIMON_STATE_IDLE);

	LoaderMachine("textures\\Maps\\LoadObjectStage3.txt", "\t");
}

/* Load stage manager  */
void LoadStage()
{
	switch (game->stage)
	{
	case 0:
		LoadObjectStage0();
		break;
	case 1:
		LoadObjectStage1();
		break;
	case 2:
		LoadObjectStage2();
		break;
	case 3:
		LoadObjectStage3();
		break;
	default:
		break;
	}
}

/* Update startscreen */ //empty function
void UpdateStartScreen(DWORD dt)
{
	startScreen->UpdateStartScreen(dt);
}

/* Update intro */
void UpdateIntro(DWORD dt)
{
	introGame->UpdateIntro(dt);
}

/* Update a frame */
void UpdateFrame(DWORD dt)
{
	int i = objects.size();
	simon->SetStage(game->stage);
	if (simon->changeStage == true)
	{
		screen->UpdateBlackboard(simon, game->stage);
		if (game->stage == 0)
		{
			simon->SetState(SIMON_STATE_WALKING_RIGHT);
			simon->Update(dt, NULL);
		}
		else 
		{
			if (simon->changeStageWalking == true)
			{
				if (simon->timeChangeStageWalking < 0)
				{
					simon->changeStageWalking = false;
					simon->SetState(SIMON_STATE_IDLE);
					door->SetState(DOOR_STATE_CLOSE);
				}
				else
				{
					simon->SetState(SIMON_STATE_WALKING_RIGHT);
				}
			}
			if (door->GetState() == DOOR_STATE_OPEN || door->GetState() == DOOR_STATE_CLOSE)
			{
				door->Update(dt, NULL);
				if (door->GetState() == DOOR_STATE_IDLE && door->doorOpened == true)
					simon->changeStageWalking = true;
			}
			if (door->GetState() == DOOR_STATE_IDLE && simon->changeStageWalking == false)
			{
				game->GetInstance()->cam->AutoUpdatePosition(dt, game->stage);
				
				if (game->stage == 1)
				{
					if (game->GetInstance()->cam->CameraX >= (CAMERA_X_LEFT_STAGE_2 + CAMERA_X_RIGHT_STAGE_1) / 2
						&& simon->x < CAMERA_X_LEFT_STAGE_2)
					{
						door->ActivateObject();
						door->SetState(DOOR_STATE_OPEN);
						door->timeLife = DOOR_TIME_ANIMATION;
					}
				}
				else if (game->stage == 2)
				{
					if (game->GetInstance()->cam->CameraX >= (CAMERA_X_LEFT_STAGE_3 + CAMERA_X_RIGHT_STAGE_2) / 2
						&& simon->x < CAMERA_X_LEFT_STAGE_3)
					{
						door->ActivateObject();
						door->SetState(DOOR_STATE_OPEN);
						door->timeLife = DOOR_TIME_ANIMATION;
					}
				}

			}			
			simon->Update(dt, NULL);
		}
		if (simon->timeChangeStage < 0)
		{
			game->gameChangeStage = true;
			simon->changeStage = false;
		}
	}
	else
	{
		if (game->pauseGame == false)
		{
			//Xu ly camera
			if (simon->isUnderground == true)
				game->GetInstance()->cam->GoDownCameraY();
			else
				game->GetInstance()->cam->ResetCameraY();
			if (simon->isAtBoss == false)
				game->GetInstance()->cam->UpdatePosition(simon, game->stage);

			int i, j;
			//TileMap-Chuc nang nap
			ObjectIDInCamera2.clear();
			tileMap->GetCoObjectsFromGridView(game->GetInstance()->cam->CameraX, &ObjectIDInCamera2);
			if (game->ObjectIDInCamera.size() == 0)
			{
				for (i = 0; i < ObjectIDInCamera2.size(); i++)
				{
					game->ObjectIDInCamera.push_back(ObjectIDInCamera2[i]);
				}
			}
			else
			{
				for (i = 0; i < ObjectIDInCamera2.size(); i++)
				{
					for (j = 0; j < game->ObjectIDInCamera.size(); j++)
					{
						if (game->ObjectIDInCamera[j] == ObjectIDInCamera2[i])
							break;
					}
					if (j == game->ObjectIDInCamera.size())//Do chay toi cuoi mang
						game->ObjectIDInCamera.push_back(ObjectIDInCamera2[i]);
				}
			}

			//TileMap-Chuc nang xoa
			for (i = 0; i < game->ObjectIDInCamera.size(); i++)
			{
				if (gameObjects->Get(game->ObjectIDInCamera[i])->GetObjectFromGameObjects()->x < game->GetInstance()->cam->CameraX - 100
					|| gameObjects->Get(game->ObjectIDInCamera[i])->GetObjectFromGameObjects()->x > game->GetInstance()->cam->CameraX + 820)
				{
					/*gameObjects->Get(game->ObjectIDInCamera[i])->GetObjectFromGameObjects()->DeactivateObject();*/
					game->ObjectIDInCamera.erase(game->ObjectIDInCamera.begin() + i);
				}
			}


			//update Simon position in boss
			if (bossBat->isActive == true)
				bossBat->UpdateSimonPosition(simon);
			if (bossBat->GetState() == BOSSBAT_STATE_DIE)
				if (simon->killBoss == false)
					crystalball->ActivateObject();

			subWeapon->Update(dt, &coObjects);//update subweapon
			int scoreFromSubWeapon = 0;
			subWeapon->GetScore(scoreFromSubWeapon);
			simon->AddScoreFromSubWeapon(scoreFromSubWeapon);

			//Update chinh cua cac object trong man hinh
			coObjects.clear();
			int numberOfObjectInCamera = game->ObjectIDInCamera.size();
			for (i = 0; i < game->ObjectIDInCamera.size(); i++)//nap vao mang coOject
			{
				coObjects.push_back(gameObjects->Get(game->ObjectIDInCamera[i])->GetObjectFromGameObjects());
			}

			simon->Update(dt, &coObjects);//update Simon
			if (simon->isAtBoss == true)
				screen->UpdateBlackboard(simon, game->stage, bossBat->lifePoint);
			else
				screen->UpdateBlackboard(simon, game->stage);

			for (i = 0; i < coObjects.size(); i++)//Update tung object
			{
				if (simon->useStopWatch == true)
					coObjects[i]->isfreeze = true;
				else
					coObjects[i]->isfreeze = false;
				coObjects[i]->Update(dt, &coObjects);
			}
			if (simon->killAllTheEnemy == true)
			{
				for (i = 0; i < game->ObjectIDInCamera.size(); i++)
				{
					gameObjects->Get(game->ObjectIDInCamera[i])->GetObjectFromGameObjects()->DeactivateObject();
				}
			}
			if (coObjects.size() - numberOfObjectInCamera > 0)//kiem tra xem co object nao duoc them vao khong
			{
				int newObjectAdded = coObjects.size() - numberOfObjectInCamera;
				for (int j = 0; j < newObjectAdded; j++)
				{
					game->ObjectIDInStage +=(j+1) ;
					coObjects[numberOfObjectInCamera + j]->ObjectID = game->ObjectIDInStage;
					tileMap->AddObjectToGridView(coObjects[numberOfObjectInCamera + j]);
					gameObjects->Add(coObjects[numberOfObjectInCamera + j]->ObjectID, coObjects[numberOfObjectInCamera + j]);
				}
			}
			
		}
		
	}
	tileMap->GetIDTopLeftTile(game->GetInstance()->cam->CameraX);
}

/* dt: time period between beginning of last frame and beginning of this frame */
void Update(DWORD dt)
{
	switch (game->screenMode)
	{
		// start screen
	case GAMEMODE_START:
		UpdateStartScreen(dt);
		break;
		// intro
	case GAMEMODE_INTRO:
		UpdateIntro(dt);
		break;
		// game running
	case GAMEMODE_GAMERUN:
		UpdateFrame(dt);
		break;
	default:
		break;
	}
}

/* Render startscreen */ //empty function
void RenderStartScreen()
{
	sprites = CSprites::GetInstance();
	startScreen->RenderStartScreen();
	if (startScreen->finishedStartScreen == true)
	{
		game->gameChangeMode = true;
		for (int id = 1000; id < 1017; id++)
		{
			sprites->Remove(id);
		}
	}
}

/* Render intro */
void RenderIntro()
{
	introGame->RenderIntro();

	if (introGame->finishedIntro == true)
	{
		game->gameChangeMode = true;
		sprites->Remove(1000);
		for (int id = 2001; id < 2006; id++)
		{
			sprites->Remove(id);
		}
	}
}

/* Render a frame */
void RenderFrame()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);
		if (((simon->timeLife / 100) % 2 == 0) && (simon->killAllTheEnemy == true))
		{
			d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR2);
		}
		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
		
		tileMap->RenderTiledMap(game->stage, simon->isUnderground);
		
		subWeapon->Render();

		if (game->stage > 0)//do man dau tien khong co door
			door->Render();
		if (game->stage != 2)
		{
			for (int i = 0; i < coObjects.size(); i++)//object trong man hinh thi moi them vao mang xet va cham va update
			{
				coObjects[i]->Render();
			}
			simon->Render();
		}
		else
		{
			simon->Render();
			for (int i = 0; i < coObjects.size(); i++)//object trong man hinh thi moi them vao mang xet va cham va update
			{
				coObjects[i]->Render();
			}
		}

		screen->RenderBlackboard(game->GetInstance()->cam->CameraX, game->GetInstance()->cam->CameraY);
		spriteHandler->End();
		d3ddv->EndScene();

	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

void Render()
{
	switch (game->screenMode)
	{
		// start screen
	case GAMEMODE_START:
		RenderStartScreen();
		break;
		// intro
	case GAMEMODE_INTRO:
		RenderIntro();
		break;
		// game running
	case GAMEMODE_GAMERUN:
		RenderFrame();
		break;
	default:
		break;
	}
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd) 
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;
			game->ProcessKeyboard();
			
			Update(dt);
			Render();
			if (simon->resetStage == true)
			{
				Sleep(750);
				simon->resetStage = false;
				if (game->stage == 1)
					LoadObjectStage1();
				else if (game->stage == 2)
					LoadObjectStage2();
				else if (game->stage == 3)
					LoadObjectStage3();
			}
			if (game->gameChangeMode == true)
			{
				if (game->screenMode == 0)
				{
					LoadIntro();
				}
				if (game->screenMode == 1)
				{
					LoadStage();
				}
				game->NextScreenMode();
				game->gameChangeMode = false;
				done = 1;
			}
			if (game->gameChangeStage == true)
			{
				game->NextStage();
				LoadStage();
				game->gameChangeStage = false;
				done = 1;
			}
		}
		else
			Sleep(tickPerFrame - dt);	
	}
	
	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);
	
	screen = CScreen::GetInstance();

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);

	startScreen = CStartScreen::GetInstance();
	introGame = CIntro::GetInstance();
	
	InitGame();
	LoadStartScreen();
	
	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
	while (1)
	{
		Run();
	}
	return 0;
}