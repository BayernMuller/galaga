#include "stdafx.h"
#include "MyShip.h"
#include "resource.h"

CMyShip::CMyShip(CPoint &pt) : m_nLife(4)
{
	CreateShip(IDB_MYSHIP, CRect(pt.x - 25, pt.y - 25, pt.x + 25, pt.y + 25));
}

void CMyShip::ShotBullet()
{
	COLORREF clr = RGB(rand() % 255, rand() % 255, rand() % 255);
	CBullet bul(GetShipCenter().x, GetShipTop(), 3, clr);
	m_Bullet.push_back(bul);
}

void CMyShip::OnHit()
{
	m_nLife--;
}

void CMyShip::MoveRight(int nRange)
{
	m_ShipSize.SetRect(GetShipLeft() + nRange, GetShipTop(), GetShipRight() + nRange, GetShipBottom());
}

void CMyShip::MoveLeft(int nRange)
{
	m_ShipSize.SetRect(GetShipLeft() - nRange, GetShipTop(), GetShipRight() - nRange, GetShipBottom());
}

CMyShip::~CMyShip()
{
}
