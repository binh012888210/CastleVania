#include "Camera.h"

CCamera*CCamera::instance = NULL;

CCamera::CCamera()
{
	CameraY = 0;
}

CCamera::~CCamera()
{
}

CCamera * CCamera::GetInstance()
{
	if (instance == NULL)
	{
		instance = new CCamera();
	}
	return instance;
}

void CCamera::setPositionInCam(float &wx, float &wy)
{
	wx = wx - this->CameraX;
	wy = wy - this->CameraY;
}

void CCamera::UpdatePosition(LPGAMEOBJECT p,int stage)
{
	CameraXLeft = CAMERA_X_LEFT;
	if (stage == 0)
		CameraXRight = CAMERA_X_RIGHT_STAGE_0;
	else if (stage == 1)
		CameraXRight = CAMERA_X_RIGHT_STAGE_1;
	else if (stage == 2)
	{
		CameraXLeft = CAMERA_X_LEFT_STAGE_2;
		CameraXRight = CAMERA_X_RIGHT_STAGE_2;
	}
	else if (stage == 3)
	{
		CameraXLeft = CAMERA_X_LEFT_STAGE_3;
		CameraXRight = CAMERA_X_RIGHT_STAGE_3;
	}

	CameraX = (p->x-224);

	if (CameraX < CameraXLeft)
	{
		CameraX = CameraXLeft;
		return;	
	}
	if (CameraX > CameraXRight)
	{
		CameraX = CameraXRight;
		return;
	}
}

void CCamera::AutoUpdatePosition(DWORD dt, int stage)
{
	if (stage == 1)
	{
		if (CameraX < CAMERA_X_LEFT_STAGE_2)
			CameraX += SIMON_WALKING_SPEED*dt;
	}
	else if (stage == 2)
	{
		if (CameraX < CAMERA_X_LEFT_STAGE_3)
			CameraX += SIMON_WALKING_SPEED*dt;
	}
}
