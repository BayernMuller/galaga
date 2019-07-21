#include "stdafx.h"
#include "Bullet.h"


CBullet::CBullet(int nX, int nY, UINT radius, COLORREF color) : m_nRadius(radius), m_BulletColor(color)
{
	m_BulletSize.SetRect(nX - radius, nY - radius, nX + radius, nY + radius);
}


void CBullet::MoveBullet(DIRECTION dir, int nRange)
{
	int nMove = nRange;
	if (dir == UP)			
		nMove *= -1;

	m_BulletSize.SetRect(m_BulletSize.left, m_BulletSize.top + nMove, m_BulletSize.right, m_BulletSize.bottom + nMove);		
}

CBullet::~CBullet()
{
}
