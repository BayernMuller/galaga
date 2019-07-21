#pragma once
#include "Ship.h"
#include "MathVector.h"

class CEnemyShip : public CShip
{
public:	
	CEnemyShip(CPoint &pt, ENEMY Type);
	~CEnemyShip();	

	CBullet& CreateBullet();
	void Move(CPoint &pt);	// pt가 함선의 중심이 됨.
	ENEMY GetType() { return m_EnemyType; }
	inline CPoint& GetPos()  { return m_pt; }	
	inline void SetMoveShip() { m_bIsMoveShip = true; }
	inline bool IsThisMoveShip() { return m_bIsMoveShip; }

protected:
	CPoint m_pt;
	ENEMY m_EnemyType;
	bool m_bIsMoveShip;


private:	
};

