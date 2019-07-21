#include "stdafx.h"
#include "Stage.h"


CStage::CStage(CGalagaMap* pMap) : m_nStage(1),  m_nMaxMoveEnemy(0)
{
	m_pMap = pMap;
}

CStage::~CStage()
{
}

void CStage::SetStage()
{
	m_nMaxMoveEnemy = 3 + m_nStage;
	m_nMaxEnemyBullet = 3 + m_nStage;
}
