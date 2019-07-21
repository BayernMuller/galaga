#pragma once
#include "Ship.h"

class CMyShip : public CShip
{
public:
	CMyShip(CPoint &pt);
	~CMyShip();

	void ShotBullet();
	void MoveRight(int nRange);
	void MoveLeft(int nRange);
	void OnHit();
	inline vector<CBullet>* GetBullets() { return &m_Bullet; }
	inline int GetLife() { return m_nLife; }

private:
	int m_nLife;
	vector<CBullet> m_Bullet;
};

