#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <fstream>
#include <sstream>
#include <iostream>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "Camera.h"
#include "Textures.h"
#include "Sprites.h"

#define KEYBOARD_BUFFER_SIZE 1024
/*
Abstract class to define keyboard event handlers
*/
class CKeyEventHandler
{
public:
	virtual void KeyState(BYTE *state) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
};

typedef CKeyEventHandler * LPKEYEVENTHANDLER;

class CGame
{
	static CGame * __instance;
	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;		
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	LPKEYEVENTHANDLER keyHandler;



public:
	int screenMode;
	int stage;
	bool gameChangeMode;
	bool gameChangeStage;
	bool pauseGame;
	CCamera *cam;

	int ObjectIDInStage;
	vector<int> ObjectIDInCamera;

	void InitKeyboard(LPKEYEVENTHANDLER handler);
	void Init(HWND hWnd);
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255);
	void ReadMatrixFromFile(char* filename, const string& splitchar,vector<vector<float>> &ImportMatrix);
	void LoadSpriteFromMatrix(vector<vector<float>> ImportMatrix, CTextures *texture, CSprites *sprites);
	void LoadAnimationFromMatrix(vector<vector<float>> ImportMatrix, CAnimations *animations);

	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();

	void SetScreenMode(int screenMode) { this->screenMode = screenMode; };
	void NextScreenMode() { screenMode++; };
	void NextStage() { stage++; };

	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st, 
		float sr, 
		float sb,
		float &t, 
		float &nx, 
		float &ny);
	static void SweptAABB2(						
		float s1l, float s1t, float s1r, float s1b,
		float nx,
		float s2l, float s2t, float s2r, float s2b,
		float &t, float &nx2);

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	static CGame * GetInstance();

	~CGame();
};


