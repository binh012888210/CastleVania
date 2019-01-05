#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprites.h"


using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box
#define BONUS_OBJECT_GRAVITY 0.0012

class CGameObject; 
typedef CGameObject * LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;

class CGameObject1;
typedef CGameObject1 * LPGAMEOBJECT1;

class CGameObjects;
typedef CGameObjects * LPGAMEOBJECTS;

struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	CCollisionEvent(float t, float nx, float ny, LPGAMEOBJECT obj = NULL) { this->t = t; this->nx = nx; this->ny = ny; this->obj = obj; }

	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};


class CGameObject
{
public:

	float x; 
	float y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;

	int nx ; 
	int ny2;

	int state;

	int ObjectID;
	int timeLife;
	int point;

	bool isActive = false;
	bool isWeapon = false;//for sweptaabb2
	bool isfreeze = false;
	bool isTouchGround = false;

	DWORD dt; 

	vector<LPANIMATION> animations;
	

public: 
	void SetPoint(int point) { this->point = point; };
	int GetPoint() { return this->point; };
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }
	void ActivateObject() { isActive = true; }
	void DeactivateObject() { isActive = false; }

	int GetState() { return this->state; }

	void RenderBoundingBox();

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO, bool &objectMove, bool isSimon = false);
	LPCOLLISIONEVENT SweptAABBEx2(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents, bool isSimon = false);
	void CalcPotentialCollisions2(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents, 
		vector<LPCOLLISIONEVENT> &coEventsResult,
		float &min_tx, 
		float &min_ty, 
		float &nx, 
		float &ny);

	void AddAnimation(int aniId);

	CGameObject();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render() {};
	virtual void SetState(int state) { this->state = state; }


	~CGameObject();
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class CGameObject1
{
	
public:
	int id;				// Object ID in the object database
	LPGAMEOBJECT Object;
	CGameObject1(int id, LPGAMEOBJECT Object);
	LPGAMEOBJECT GetObjectFromGameObjects() { return Object; };
};

class CGameObjects
{
	static CGameObjects * __instance;
	unordered_map<int, LPGAMEOBJECT1> gameObjects;

public:
	void Add(int id, LPGAMEOBJECT Object);
	void Remove(int id);
	void ClearAll();
	LPGAMEOBJECT1 Get(int id);

	static CGameObjects * GetInstance();

};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++