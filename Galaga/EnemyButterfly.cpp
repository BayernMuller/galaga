#include "stdafx.h"
#include "EnemyButterfly.h"
#include "Resource.h"

CEnemyButterfly::CEnemyButterfly(CPoint &pt) : CEnemyShip(pt, Butterfly)
{
	CreateShip(IDB_ENEMY2, CRect(pt.x - 18, pt.y - 18, pt.x + 18, pt.y + 18));
}


CEnemyButterfly::~CEnemyButterfly()
{

}

