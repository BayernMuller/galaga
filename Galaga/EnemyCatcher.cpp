#include "stdafx.h"
#include "EnemyCatcher.h"
#include "Resource.h"

CEnemyCatcher::CEnemyCatcher(CPoint &pt) : CEnemyShip(pt, Catcher), m_bOneLife(false)
{
	CreateShip(IDB_ENEMYCATCHER, CRect(pt.x - 25, pt.y - 25, pt.x + 25, pt.y + 25));
}

void CEnemyCatcher::OnFirstHit()
{
	m_nBmpID = IDB_ENEMY1;
	m_bOneLife = true;
}

CEnemyCatcher::~CEnemyCatcher()
{
}


