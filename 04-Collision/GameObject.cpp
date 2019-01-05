#include <d3dx9.h>
#include <algorithm>


#include "debug.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;	
	ny2 = 0;
}

void CGameObject::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	this->dt = dt;
	if (isfreeze == false)
	{
		dx = vx*dt;
		dy = vy*dt;
	}
	else
	{
		dx = 0;
		dy = 0;
	}
}

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CGameObject::SweptAABBEx(LPGAMEOBJECT coO,bool &objectMove,bool isSimon)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);
	GetBoundingBox(ml, mt, mr, mb);

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	coO->GetSpeed(svx, svy);

	float sdx = svx*dt;
	float sdy = svy*dt;

	float dx = this->dx - sdx;//hai vector doi nghich nhau
	float dy = this->dy - sdy;

	if (dx == 0 && dy == 0)
		objectMove = false;
	else 
		objectMove = true;

	if (sdy != 0 && isSimon == true)
	{
		ny = 0;
		nx = 0;
		CGame::SweptAABB2(
			ml, mt, mr, mb,
			this->nx,
			sl, st, sr, sb,
			t, nx);
		
	}
	else
	{
		CGame::SweptAABB(
			ml, mt, mr, mb,
			dx, dy,
			sl, st, sr, sb,
			t, nx, ny
		);
	}

	CCollisionEvent * e = new CCollisionEvent(t, nx, ny, coO);//tre ve mang chua cac doi tuong du doan se xay ra va cham
	return e;
}

LPCOLLISIONEVENT CGameObject::SweptAABBEx2(LPGAMEOBJECT coO)//danh cho 2 object dung yen 
{
	float s1l, s1t, s1r, s1b;		// static object 1 bbox
	float s2l, s2t, s2r, s2b;		// static object 1 bbox
	float t, ny, nx3;

	ny = 0;
	nx3 = 0;

	GetBoundingBox(s1l, s1t, s1r, s1b);//roi
	coO->GetBoundingBox(s2l, s2t, s2r, s2b);
	
	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	coO->GetSpeed(svx, svy);

	float sdx = svx*dt;
	float sdy = svy*dt;

	float dx = this->dx - sdx;//hai vector doi nghich nhau
	float dy = this->dy - sdy;
	
	CGame::SweptAABB2(
		s1l, s1t, s1r, s1b,
		this->nx,
		s2l, s2t, s2r, s2b,
		t,nx3);

	CCollisionEvent * e = new CCollisionEvent(t, nx3, ny, coO);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects 
	
	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void CGameObject::CalcPotentialCollisions( vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents,bool isSimon)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		bool objectMove;
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i), objectMove, isSimon);
		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);// sap xep lai theo TG
}

void CGameObject::CalcPotentialCollisions2(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		isWeapon = true;
		bool objectMove;
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i), objectMove);
		if (objectMove == false)
			e = SweptAABBEx2(coObjects->at(i));
		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);// sap xep lai theo TG
}

void CGameObject::FilterCollision(vector<LPCOLLISIONEVENT> &coEvents,vector<LPCOLLISIONEVENT> &coEventsResult,
	float &min_tx, float &min_ty, 
	float &nx, float &ny)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();//clear
	vector<LPCOLLISIONEVENT> coEventsResultMultiObject;
	coEventsResultMultiObject.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];
		
		if (isWeapon == true)
		{
			if (c->nx != 0)
			{
				coEventsResultMultiObject.push_back(coEvents[i]);
				if(c->t < min_tx)
					min_tx = c->t;
			}
		}
		else
		{
			if (c->t < min_tx && c->nx != 0)
			{
				min_tx = c->t; nx = c->nx; min_ix = i;
			}
		}
		if (c->t < min_ty  && c->ny != 0) 
		{
			min_ty = c->t; ny = c->ny; min_iy = i;			
		}
		
	}
	if (min_ix>=0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy>=0) coEventsResult.push_back(coEvents[min_iy]);
	if (isWeapon == true)
	{
		coEventsResult.clear();
		for (int i = 0; i < coEventsResultMultiObject.size(); i++)
		{
			coEventsResult.push_back(coEventsResultMultiObject[i]);
			}
		if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
		isWeapon = false;
	}
	coEventsResultMultiObject.clear();
}


void CGameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l,t,r,b; 

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	CGame::GetInstance()->Draw(x, y, bbox, rect.left, rect.top, rect.right, rect.bottom, 32);//quan tr?ng
}

void CGameObject::AddAnimation(int aniId)
{
	LPANIMATION ani = CAnimations::GetInstance()->Get(aniId);
	animations.push_back(ani);
}

CGameObject::~CGameObject()
{
}
//++++++++++++++++++++++++++++++++++++++++++++++++++TileMap++++++++++++++++++++++++++++++++++++++

CGameObjects * CGameObjects::__instance = NULL;

CGameObjects *CGameObjects::GetInstance()
{
	if (__instance == NULL) __instance = new CGameObjects();
	return __instance;
}

CGameObject1::CGameObject1(int id, LPGAMEOBJECT Object)
{
	this->id = id;
	this->Object = Object;
}

void CGameObjects::Add(int id, LPGAMEOBJECT Object)
{
	LPGAMEOBJECT1 s = new CGameObject1(id, Object);
	gameObjects[id] = s;
}

void CGameObjects::Remove(int id)
{
	gameObjects.erase(id);
}

void CGameObjects::ClearAll()
{
	gameObjects.clear();
}

LPGAMEOBJECT1 CGameObjects::Get(int id)
{
	return gameObjects[id];
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
