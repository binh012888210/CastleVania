#include "Weapon.h"


void CWeapon::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	if (state == WEAPON_STATE_3)
		r = x + WEAPON_BBOX_WIDTH_STATE_3;
	else
		r = x + WEAPON_BBOX_WIDTH;
	b = y + WEAPON_BBOX_HEIGHT;
}