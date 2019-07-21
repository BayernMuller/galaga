#pragma once

#include <vector>
#include "Bullet.h"

#define SETTER public
#define GETTER public

using namespace std;

class CShip
{
public:
	CShip();
	~CShip();
	
	void CreateShip(const UINT nBmpID, const CRect& rect);
GETTER:
	inline int GetShipRight() { return m_ShipSize.right; }
	inline int GetShipLeft() { return m_ShipSize.left; }
	inline int GetShipTop() { return m_ShipSize.top; }
	inline int GetShipBottom() { return m_ShipSize.bottom; }
	inline CPoint GetShipCenter() { return m_ShipSize.CenterPoint(); }
	inline CRect* GetShip() { return &m_ShipSize; }	
	inline UINT GetBitmapID() { return m_nBmpID; }

protected:
	UINT m_nBmpID;
	CRect m_ShipSize;
	

private:
	
};

