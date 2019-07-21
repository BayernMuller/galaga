#include "stdafx.h"
#include "Ship.h"


CShip::CShip()
{
	
}

void CShip::CreateShip(const UINT nBmpID, const CRect& rect)
{
	m_nBmpID = nBmpID;
	m_ShipSize = rect;
	m_ShipSize.NormalizeRect();
}



CShip::~CShip()
{
}
