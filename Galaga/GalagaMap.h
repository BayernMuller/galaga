#pragma once

#include "MyShip.h"
#include "EnemyShip.h"
#include "Stage.h"

struct MOVEENEMY
{
	CEnemyShip* pShip;
	CMathVector Vec;
	CMathVector Vel;
	CMathVector Acc;
	CMathVector ShipVec;
};

class CGalagaView;
class CBullet;

class CGalagaMap
{
public:
	CGalagaMap(CWnd *pParent);
	~CGalagaMap();

	void StartThread();
	void StopThread();
	void SuspendThread();
	void ResumeThread();
	void DrawGalaga(CDC* pDC);
	void KeyDown(const UINT nChar);
	void CheckMyShipBulletHitOut();
	void CheckEnemyBulletHitOut();
	void CheckMyShipDie();
	void ShotEnemyBullet(MOVEENEMY* p);
	void GoBackThePlace();
	bool m_bFlag;	// Thread Control

	/// Getter:
	inline CMyShip* GetMyShip() { return m_pMyShip; }
	inline vector<CEnemyShip*>* GetEnemy() { return &m_Enemy; }
	inline vector<MOVEENEMY>* GetMoveEnemy() { return &m_MoveEnemy; }
	inline vector<CBullet>* GetEnemyBullet() { return &m_EnemyBullet; }
	void GetPlayAreaRect(CRect* rect);	
	/// /// ///
	
private:	
	CMyShip *m_pMyShip;
	CWnd* m_pParent;
	vector<CEnemyShip*> m_Enemy;
	vector<MOVEENEMY> m_MoveEnemy;
	vector<CBullet> m_EnemyBullet;
	CWinThread* m_pMoveThread;
	CWinThread* m_pWanderThread;
	CStage m_Stage;
	UINT m_nScore;
	
	void CreateEnemy();
	void OnScore(ENEMY type);
	void OnHitMyShip();
	void DrawBackground(CDC *pDC);	
	void DrawShip(CDC* pDC);
	void DrawBullet(CDC* pDC);
	void DrawInfo(CDC* pDC);
	void CreateMoveEnemy(const int num = 1);
	void RemoveDeadMoveShip(CEnemyShip* ship);	
	void InitMap();
	
};

