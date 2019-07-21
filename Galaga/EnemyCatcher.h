#pragma once
#include "EnemyShip.h"

class CEnemyCatcher : public CEnemyShip
{
public:
	CEnemyCatcher(CPoint &pt);
	~CEnemyCatcher();

	void OnFirstHit();
	inline bool IsOneLife() { return m_bOneLife; }

private:
	bool m_bOneLife;
};

