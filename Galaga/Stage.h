#pragma once

class CGalagaMap;

class CStage
{

public:
	CStage(CGalagaMap* pMap);
	~CStage();	
	void SetStage();

	inline void InitStage()
	{
		m_nStage = 1;
		SetStage();
	}

	inline void IncreaseStage()
	{
		m_nStage++;
		SetStage();
	}

	inline UINT GetStage() { return m_nStage; }
	inline UINT GetMaxMoveEnemy() { return m_nMaxMoveEnemy; }
	inline UINT GetMaxEnemyBullet() { return m_nMaxEnemyBullet; }

private:
	CGalagaMap* m_pMap;
	UINT m_nStage;
	UINT m_nMaxMoveEnemy;
	UINT m_nMaxEnemyBullet;

	
};

