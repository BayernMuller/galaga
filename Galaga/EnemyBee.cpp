#include "stdafx.h"
#include "EnemyBee.h"
#include "Resource.h"

CEnemyBee::CEnemyBee(CPoint &pt) : CEnemyShip(pt, Bee)
{
	CreateShip(IDB_ENEMY3, CRect(pt.x - 18, pt.y - 18, pt.x + 18, pt.y + 18));
}


CEnemyBee::~CEnemyBee()
{
}
