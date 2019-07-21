#include "stdafx.h"
#include "EnemyShip.h"
#include "resource.h"


CEnemyShip::CEnemyShip(CPoint &pt, ENEMY Type) : m_pt(pt), m_EnemyType(Type), m_bIsMoveShip(false)
{
	
}

CBullet& CEnemyShip::CreateBullet()
{
	return CBullet(GetShipCenter().x, GetShipBottom(), 3, RGB(255,0,0));
}

void CEnemyShip::Move(CPoint &pt)
{
	int W = m_ShipSize.Width() / 2;
	int H = m_ShipSize.Height() / 2;
	m_ShipSize.SetRect(pt.x - W, pt.y - H, pt.x + W, pt.y + H);
}

CEnemyShip::~CEnemyShip()
{
	
}

