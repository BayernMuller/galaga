#pragma once


class CBullet
{
public:
	CBullet(int nX, int nY, UINT radius, COLORREF color);
	~CBullet();	
	
	void MoveBullet(DIRECTION dir, int nRange = 15);
	inline CRect* GetBulletRect() { return &m_BulletSize; }
	inline COLORREF GetBulletColor() { return m_BulletColor; }
		
protected:	
	 UINT m_nRadius;
	 COLORREF m_BulletColor;
	 CRect m_BulletSize;
};

