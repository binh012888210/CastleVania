#pragma once
#include "GameObject.h"
#include "DefineNumber.h"

class CCamera : public CGameObject
{
public:
	static CCamera *instance;
	int CameraX;
	int CameraY;
	int CameraXLeft;
	int CameraXRight;
public:
	CCamera();
	~CCamera();
	static CCamera* GetInstance();
	void setPositionInCam(float &wx, float &wy);
	void UpdatePosition(LPGAMEOBJECT p,int stage);
	void AutoUpdatePosition(DWORD dt, int stage);
	void GoDownCameraY() { CameraY = CAMERA_Y_STAGE_2; };
	void ResetCameraY() { CameraY = 0; };
};

