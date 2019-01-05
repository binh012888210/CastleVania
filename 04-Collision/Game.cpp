#include "Game.h"
#include "debug.h"

CGame * CGame::__instance = NULL;

/*
	Initialize DirectX, create a Direct3D device for rendering within the window, initial Sprite library for 
	rendering 2D images
	- hInst: Application instance handle
	- hWnd: Application window handle
*/
void CGame::Init(HWND hWnd)
{
	stage = 0;
	screenMode = 0;
	ObjectIDInStage = 0;
	gameChangeMode = false;
	gameChangeStage = false;
	pauseGame = false;
	LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);

	this->hWnd = hWnd;									

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;

	RECT r
		;
	GetClientRect(hWnd, &r);	// retrieve Window width & height 

	d3dpp.BackBufferHeight = r.bottom + 1;
	d3dpp.BackBufferWidth = r.right + 1;

	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddv);

	if (d3ddv == NULL)
	{
		OutputDebugString(L"[ERROR] CreateDevice failed\n");
		return;
	}

	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	// Initialize sprite helper from Direct3DX helper library
	D3DXCreateSprite(d3ddv, &spriteHandler);

	OutputDebugString(L"[INFO] InitGame done;\n");
}

/*
	Utility function to wrap LPD3DXSPRITE::Draw 
*/
void CGame::Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha)
{
	D3DXVECTOR3 p(x, y, 0);
	cam->setPositionInCam(p.x, p.y);
	RECT r;
	r.left = left;
	r.top = top;
	r.right = right;
	r.bottom = bottom;
	spriteHandler->Draw(texture, &r, NULL, &p, D3DCOLOR_ARGB(alpha, 255, 255, 255));
}

void CGame::ReadMatrixFromFile(char * filename, const string & splitchar, vector<vector<float>> &ImportMatrix)
{
	string      line;
	string      strnum;
	ifstream	in(filename);



	while (getline(in, line)) //tách từng dòng ra từ file in
	{
		ImportMatrix.push_back(vector<float>()); // thêm 1 vector int vào cuối

		for (string::const_iterator i = line.begin(); i != line.end(); ++i) //iterator dùng để so sánh
		{
			// nếu i không phải kí tự cách, thì bỏ nó vào strnum
			if (splitchar.find(*i) == -1)
			{
				strnum += *i;
				if (i + 1 != line.end()) //Khác end thì vẫn tiếp tục(1-0-3-tab-1-0-4)
					continue;
			}

			// nếu strnum vẫn rỗng thì tiếp tục thêm vào (trường hợp 2 dấu cách liên tiếp)
			//if (strnum.empty())
			//	continue;

			// nếu strnum không rỗng thì chuyển kí tự thành số
			float       number;
			istringstream(strnum) >> number;
			ImportMatrix.back().push_back(number); //thêm số này vào cuối vector

			strnum.clear();
		}
	}
}

void CGame::LoadSpriteFromMatrix(vector<vector<float>> ImportMatrix, CTextures *textures, CSprites *sprites)
{
	textures = CTextures::GetInstance();
	sprites = CSprites::GetInstance();
	for (int i = 0; i < ImportMatrix.size(); i++)
	{
		LPDIRECT3DTEXTURE9 textureForInputSprite = textures->Get(ImportMatrix[i][0]);
		sprites->Add(ImportMatrix[i][1], ImportMatrix[i][2], ImportMatrix[i][3], ImportMatrix[i][4], ImportMatrix[i][5], textureForInputSprite);
	}
}

void CGame::LoadAnimationFromMatrix(vector<vector<float>> ImportMatrix, CAnimations *animations)
{
	LPANIMATION ani;
	animations = CAnimations::GetInstance();
	for (int i = 0; i < ImportMatrix.size(); i++)
	{
		ani = new CAnimation(ImportMatrix[i][1]);
		for (int j = 2; j < ImportMatrix[i].size(); j++)
		{
			if (ImportMatrix[i][j] == 0)
				break;
			ani->Add(ImportMatrix[i][j]);
		}
		animations->Add(ImportMatrix[i][0], ani);
	}
}

int CGame::IsKeyDown(int KeyCode)
{
	return (keyStates[KeyCode] & 0x80) > 0;
}

void CGame::InitKeyboard(LPKEYEVENTHANDLER handler)
{
	HRESULT
		hr = DirectInput8Create
		(
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			DIRECTINPUT_VERSION,
			IID_IDirectInput8, (VOID**)&di, NULL
		);

	if (hr != DI_OK)
	{
		DebugOut(L"[ERROR] DirectInput8Create failed!\n");
		return;
	}

	hr = di->CreateDevice(GUID_SysKeyboard, &didv, NULL);

	// TO-DO: put in exception handling
	if (hr != DI_OK) 
	{
		DebugOut(L"[ERROR] CreateDevice failed!\n");	
		return;
	}

	// Set the data format to "keyboard format" - a predefined data format 
	//
	// A data format specifies which controls on a device we
	// are interested in, and how they should be reported.
	//
	// This tells DirectInput that we will be passing an array
	// of 256 bytes to IDirectInputDevice::GetDeviceState.

	hr = didv->SetDataFormat(&c_dfDIKeyboard);

	hr = didv->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);


	// IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
	//
	// DirectInput uses unbuffered I/O (buffer size = 0) by default.
	// If you want to read buffered data, you need to set a nonzero
	// buffer size.
	//
	// Set the buffer size to DINPUT_BUFFERSIZE (defined above) elements.
	//
	// The buffer size is a DWORD property associated with the device.
	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE; // Arbitary buffer size

	hr = didv->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

	hr = didv->Acquire();
	if (hr != DI_OK)
	{
		DebugOut(L"[ERROR] DINPUT8::Acquire failed!\n");
		return;
	}

	this->keyHandler = handler;

	DebugOut(L"[INFO] Keyboard has been initialized successfully\n");
}

void CGame::ProcessKeyboard()
{
	HRESULT hr;

	// Collect all key states first
	hr = didv->GetDeviceState(sizeof(keyStates), keyStates);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			HRESULT h = didv->Acquire();
			if (h == DI_OK)
			{
				DebugOut(L"[INFO] Keyboard re-acquired!\n");
			}
			else return;
		}
		else
		{
			//DebugOut(L"[ERROR] DINPUT::GetDeviceState failed. Error: %d\n", hr);
			return;
		}
	}

	keyHandler->KeyState((BYTE *)&keyStates);



	// Collect all buffered events
	DWORD dwElements = KEYBOARD_BUFFER_SIZE;
	hr = didv->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), keyEvents, &dwElements, 0);
	if (FAILED(hr))
	{
		//DebugOut(L"[ERROR] DINPUT::GetDeviceData failed. Error: %d\n", hr);
		return;
	}

	// Scan through all buffered events, check if the key is pressed or released
	for (DWORD i = 0; i < dwElements; i++)
	{
		int KeyCode = keyEvents[i].dwOfs;
		int KeyState = keyEvents[i].dwData;
		if ((KeyState & 0x80) > 0)
			keyHandler->OnKeyDown(KeyCode);
		else
			keyHandler->OnKeyUp(KeyCode);
	}
}

CGame::~CGame()
{
	if (spriteHandler != NULL) spriteHandler->Release();
	if (backBuffer != NULL) backBuffer->Release();
	if (d3ddv != NULL) d3ddv->Release();
	if (d3d != NULL) d3d->Release();
}

/*
	SweptAABB 
*/
void CGame::SweptAABB(						//ham xu ly nhung cai co the xay ra va cham
	float ml, float mt,	float mr, float mb,			
	float dx, float dy,			
	float sl, float st, float sr, float sb,
	float &t, float &nx, float &ny)
{

	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry; 
	float t_exit; 


	t = -1.0f;			// no collision
	nx = ny = 0;

	//
	// Broad-phase test 
	//

	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	if (br < sl || bl > sr || bb < st || bt > sb) return;//chua co va cham xay ra


	if (dx == 0 && dy == 0) return;		// moving object is not moving > obvious no collision

	if (dx > 0)
	{
		dx_entry = sl - mr; 
		dx_exit = sr - ml;  

	}
	else if (dx < 0)
	{
		dx_entry = sr - ml;
		dx_exit = sl- mr;
	}


	if (dy > 0)
	{
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}
	else if (dy < 0)
	{
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}

	if (dx == 0)
	{
		tx_entry = -99999999999;
		tx_exit = 99999999999;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}
	
	if (dy == 0)
	{
		ty_entry = -99999999999;
		ty_exit = 99999999999;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}
	

	if (  (tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);
	
	if (t_entry > t_exit) return; 

	t = t_entry; 

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else 
	{
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : ny = 1.0f;
	}

}
void CGame::SweptAABB2(						
	float s1l, float s1t, float s1r, float s1b,
	float nx,
	float s2l, float s2t, float s2r, float s2b,
	float &t,float &nx2)
{
	float dx1;//min
	float dx2;//max

	float dxInside;

	t = -1.0f;			// no collision

	if (s1b<s2t || s1t>s2b)
		return;

	if (nx > 0) 
	{
		dx1 = s1l - s2r;
		dx2 = s2l - s1r;
		if (dx1 > 0 || dx2 > 0)
			return;
		else
		{
			dxInside = s2l - s1l;
			if (dxInside < 0)//neu left cua roi nam trong object  
				dxInside = s2r - s1l;
			t = dxInside / (s1r - s1l);
			nx2 = -1;
		}
	}
	else if (nx<0)
	{
		dx1 = s2l - s1r;
		dx2 = s1l - s2r;
		if (dx1 > 0 || dx2 > 0)
			return;
		else
		{
			dxInside = s1r - s2r;
			if (dxInside <0)
				dxInside = s1r - s2l;
			t = dxInside / (s1r - s1l);
			nx2 = 1;
		}
	}
}
CGame *CGame::GetInstance()
{
	if (__instance == NULL)
	{
		__instance = new CGame();
		__instance->cam = new CCamera();//ghi chu
	}
	return __instance;
}
