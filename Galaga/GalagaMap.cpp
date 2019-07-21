#include "stdafx.h"
#include "GalagaMap.h"
#include <vector>
#include "Resource.h"
#include "EnemyBee.h"
#include "EnemyButterfly.h"
#include "EnemyCatcher.h"

using namespace std;

// Thread Func
UINT MoveEnemyShotThread(LPVOID p);
UINT EnemyWander(LPVOID p); // 적군 왔다갔다 하는거

int rand(int low, int high)
{
	return rand() % (high - low + 1) + low;
}

CGalagaMap::CGalagaMap(CWnd *pParent) : m_bFlag(false), m_nScore(0), m_Stage(this)
{
	m_pWanderThread = m_pMoveThread = nullptr;
	m_pMyShip = nullptr;
	m_pParent = pParent;
	InitMap();
	StartThread();
}

CGalagaMap::~CGalagaMap()
{
	delete m_pMyShip;
	for (int i = 0; i < (int)m_Enemy.size(); i++)
	{
		delete m_Enemy[i];
	}
}

void CGalagaMap::KeyDown(const UINT nChar)
{
	const short VK_ESC = 27;
	CRect rect;
	GetPlayAreaRect(&rect);

	switch (nChar)
	{
	case VK_LEFT: // MOVING
		if (m_pMyShip->GetShipLeft() > rect.left)
			m_pMyShip->MoveLeft(5);
		break;
	case VK_RIGHT:
		if (m_pMyShip->GetShipRight() < rect.right)
			m_pMyShip->MoveRight(5);
		break;

	case VK_SPACE: // SHOT
		if (m_pMyShip->GetBullets()->size() < 2) // Max Bullet = 2	
			m_pMyShip->ShotBullet();
		break;

	case VK_ESC:   // PAUSE			
		break;	
	}
}

void CGalagaMap::InitMap()
{
	CRect rect;
	GetPlayAreaRect(&rect);
	int CenterX = rect.CenterPoint().x;
	int Y = rect.Height();
	// Create MyShip
	m_pMyShip = new CMyShip(CPoint(CenterX, Y - 100));
	CreateEnemy();
	m_nScore = 0;
	CreateMoveEnemy(4);
	// 처음 생성할때 넣는 인자가 최대 움직이는 적군수.
}

void CGalagaMap::DrawGalaga(CDC* pDC)
{
	CDC MemDC;
	CRect rect;
	CRect PlayRect;
	m_pParent->GetClientRect(&rect);
	MemDC.CreateCompatibleDC(pDC);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap *pOldBitmap = (CBitmap*)MemDC.SelectObject(&bitmap);

	MemDC.PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), BLACKNESS);

	/// Draw
	DrawBackground(&MemDC);
	DrawShip(&MemDC);
	DrawBullet(&MemDC);
	GetPlayAreaRect(&PlayRect);
	MemDC.SelectStockObject(BLACK_BRUSH);
	MemDC.Rectangle(PlayRect.right + 1, PlayRect.top, rect.right, rect.bottom);
	DrawInfo(&MemDC);
	///

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(pOldBitmap);
	bitmap.DeleteObject();
}

void CGalagaMap::DrawShip(CDC* pDC)
{
	for (int i = 0; i < (int)m_Enemy.size() + 1; i++)
	{
		CShip* pShip = nullptr;
		if (!i)
			pShip = m_pMyShip;
		else
			pShip = m_Enemy[i - 1];

		CDC MemDC;
		MemDC.CreateCompatibleDC(pDC);

		CRect rect;
		m_pParent->GetClientRect(&rect);
		MemDC.PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), BLACKNESS);

		CBitmap bitmap;
		bitmap.LoadBitmap(pShip->GetBitmapID());

		BITMAP bmpinfo;
		bitmap.GetBitmap(&bmpinfo);

		CBitmap *pOldBitmap = (CBitmap*)MemDC.SelectObject(&bitmap);
		pDC->TransparentBlt(
			pShip->GetShipLeft(),
			pShip->GetShipTop(),
			pShip->GetShip()->Width(),
			pShip->GetShip()->Height(),
			&MemDC,
			0,
			0,
			bmpinfo.bmWidth,
			bmpinfo.bmHeight,
			RGB(0, 0, 0));

		MemDC.SelectObject(pOldBitmap);
		bitmap.DeleteObject();
	}
}

void CGalagaMap::DrawBullet(CDC *pDC)
{
	for (int i = 0; i < (int)m_pMyShip->GetBullets()->size(); i++)
	{		
		CBrush brush, *pOldBrush;
		brush.CreateSolidBrush(m_pMyShip->GetBullets()->at(i).GetBulletColor());
		pOldBrush = pDC->SelectObject(&brush);
		pDC->Rectangle(m_pMyShip->GetBullets()->at(i).GetBulletRect());
		pDC->SelectObject(pOldBrush);
		brush.DeleteObject();		
	}
	for (int i = 0; i < (int)m_EnemyBullet.size(); i++)
	{
		CBrush brush, *pOldBrush;
		brush.CreateSolidBrush(m_EnemyBullet[i].GetBulletColor());
		pOldBrush = pDC->SelectObject(&brush);
		pDC->Rectangle(m_EnemyBullet[i].GetBulletRect());
		pDC->SelectObject(pOldBrush);
		brush.DeleteObject();
	}
}

void CGalagaMap::DrawBackground(CDC *pDC)
{
	CRect rect;
	m_pParent->GetClientRect(&rect);
	pDC->SelectStockObject(BLACK_BRUSH);
	pDC->Rectangle(&rect);

	GetPlayAreaRect(&rect);
	CPen pen(PS_SOLID, 1, RGB(128, 128, 128));
	CPen *pOldPen;
	pOldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(rect.right, 0);
	pDC->LineTo(rect.right, rect.bottom);
	pDC->SelectObject(pOldPen);
	pDC->SelectStockObject(BLACK_PEN);
}

void CGalagaMap::DrawInfo(CDC * pDC)
{
	CRect rect;
	CRect PlayRect;
	m_pParent->GetClientRect(&rect);
	GetPlayAreaRect(&PlayRect);
	CRect Info(PlayRect.right, PlayRect.top, rect.right, rect.bottom);

	CString str, str2, str3, str4;
	str.Format(_T("Enemy : %d"), m_Enemy.size());
	str2.Format(_T("Score : %d"), m_nScore);
	str3.Format(_T("Stage : %d"), m_Stage.GetStage());
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->TextOut(Info.left + 3, 0, str);
	pDC->TextOut(Info.left + 3, 30, str3);
	pDC->TextOut(Info.left + 3, 775, _T("Life :"));
	pDC->TextOut(Info.left + 3, 15, str2);
	for (int i = 0; i < m_pMyShip->GetLife() - 1; i++)
	{
		CDC MemDC;
		MemDC.CreateCompatibleDC(pDC);
		CBitmap bitmap;
		bitmap.LoadBitmap(m_pMyShip->GetBitmapID());
		BITMAP bmpinfo;
		bitmap.GetBitmap(&bmpinfo);
		CBitmap *pOldBitmap = (CBitmap*)MemDC.SelectObject(&bitmap);
		pDC->TransparentBlt(Info.CenterPoint().x + (i * 55) - 100, 800, m_pMyShip->GetShip()->Width(), m_pMyShip->GetShip()->Height(), &MemDC, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));
		MemDC.SelectObject(pOldBitmap);
		bitmap.DeleteObject();
	}

}

void CGalagaMap::RemoveDeadMoveShip(CEnemyShip *ship)
{	
	auto itr = m_MoveEnemy.begin();
	for (; itr != m_MoveEnemy.end();)
	{
		if (itr->pShip == ship)
		{
			m_MoveEnemy.erase(itr);
			CreateMoveEnemy();
			return;
		}
		else
		{
			itr++;
		}
	}
}

void CGalagaMap::CreateMoveEnemy(const int num)
{
	int nSize = 0;
	for (int i = 0; i < (int)m_Enemy.size(); i++)
	{
		if (m_Enemy[i]->GetType() != Catcher && !m_Enemy[i]->IsThisMoveShip())
		{
			nSize++;
		}
	}

	if (!nSize)
		return;
		
	int nCnt = 0;
	while (nCnt != num)
	{
		int nShip = rand(0, m_Enemy.size() - 1);
		if (m_Enemy[nShip]->GetType() != Catcher && !m_Enemy[nShip]->IsThisMoveShip())
		{
			MOVEENEMY ME;
			ME.pShip = m_Enemy[nShip];
			ME.pShip->SetMoveShip();
			m_MoveEnemy.push_back(ME);
			nCnt++;
		}
	}
}

void CGalagaMap::CheckMyShipBulletHitOut()
{
	vector<CBullet>::iterator BullItr;
	vector<CBullet> *pVec = m_pMyShip->GetBullets();
	vector<CEnemyShip*>::iterator ShipItr;

	for (BullItr = pVec->begin(); BullItr != pVec->end();) // Bullet Check
	{
		bool bOut = BullItr->GetBulletRect()->bottom < 0;
		BOOL bHit = FALSE;
		for (ShipItr = m_Enemy.begin(); ShipItr != m_Enemy.end();) // Enemy Check
		{
			bHit = (*ShipItr)->GetShip()->PtInRect(BullItr->GetBulletRect()->CenterPoint());
			if (bHit) // 맞았을때
			{
				if ((*ShipItr)->GetType() == Catcher) // 캐처일때
				{					
					if (!((CEnemyCatcher*)(*ShipItr))->IsOneLife()) // 목숨 두개 남았으면 
					{
						((CEnemyCatcher*)(*ShipItr))->OnFirstHit(); // 하나 줄이고 비트맵 변경
						break;
					}
				}
				OnScore((*ShipItr)->GetType());
				if ((*ShipItr)->IsThisMoveShip()) // 움직이는 적이면
				{
					RemoveDeadMoveShip((*ShipItr)); // 움직이는 적 벡터에서 삭제후 움직이는 적 새로생성
				}
				delete (*ShipItr); // 메모리 해제
				m_Enemy.erase(ShipItr); // 벡터에서 삭제

				if (!m_Enemy.size()) // 다 잡았을떄
				{
					m_Stage.IncreaseStage();
					SuspendThread();
					CString str;
					str.Format(_T("Get Ready Stage %d!"), m_Stage.GetStage());
					AfxMessageBox(str);
					ResumeThread();
					CreateEnemy();
					CreateMoveEnemy(m_Stage.GetMaxMoveEnemy());
					GoBackThePlace();
				}
				break;
			}
			else
			{
				ShipItr++;
			}
		}

		if (bOut || bHit) // 맞았거나 장외이면
		{
			BullItr = pVec->erase(BullItr); // 총알 삭제	
		}
		else // 아니면
		{
			BullItr++; // 다음 반복자로
		}
	}
}


void CGalagaMap::CheckMyShipDie()
{
	for (int i = 0; i < (int)m_MoveEnemy.size(); i++)
	{
		if (m_pMyShip->GetShip()->PtInRect(m_MoveEnemy[i].pShip->GetShipCenter()))
		{
			m_MoveEnemy[i].pShip->Move(CPoint(400, -100));
			OnHitMyShip();
			return;
		}
	}
}

void CGalagaMap::CheckEnemyBulletHitOut()
{
	CRect rect;
	m_pParent->GetClientRect(&rect);
	auto BullItr = m_EnemyBullet.begin();
	for (; BullItr != m_EnemyBullet.end();) // Bullet Check
	{
		bool bOut = BullItr->GetBulletRect()->bottom > rect.bottom;
		BOOL bHit = m_pMyShip->GetShip()->PtInRect(BullItr->GetBulletRect()->CenterPoint());

		if (bHit)
		{
			OnHitMyShip();
			m_EnemyBullet.erase(BullItr);
			return;
		}

		if (bOut || bHit) // 맞았거나 장외이면
		{
			BullItr = m_EnemyBullet.erase(BullItr); // 총알 삭제	
		}
		else // 아니면
		{
			BullItr++; // 다음 반복자로
		}
	}
}

void CGalagaMap::CreateEnemy()
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			CPoint pt(j * 59 + 50, i * 55 + 50);
			CEnemyShip *pEnemy = nullptr;
			switch (i)
			{
			case 0:
				if (3 <= j && j <= 6)
					pEnemy = new CEnemyCatcher(pt);
				break;
			case 1: case 2:
				if (1 <= j && j <= 8)
					pEnemy = new CEnemyButterfly(pt);
				break;
			case 3: case 4:
				pEnemy = new CEnemyBee(pt);
				break;
			}
			if (pEnemy != nullptr)
			{
				m_Enemy.push_back(pEnemy);
			}
		}
	}
}

void CGalagaMap::OnScore(ENEMY type)
{
	switch (type)
	{
	case Catcher:
		m_nScore += 100;
		break;
	case Butterfly:
		m_nScore += 80;
		break;
	case Bee:
		m_nScore += 50;
		break;
	}
}

void CGalagaMap::GetPlayAreaRect(CRect* rect)
{
	CRect Area;
	m_pParent->GetClientRect(&Area);
	Area.SetRect(0, 0, Area.Width() - 250, Area.Height());
	*rect = Area;
}

void CGalagaMap::StartThread()
{
	m_bFlag = true;
	m_pMoveThread = AfxBeginThread(MoveEnemyShotThread, this, THREAD_PRIORITY_TIME_CRITICAL, 0, CREATE_SUSPENDED);
	m_pWanderThread = AfxBeginThread(EnemyWander, this, THREAD_PRIORITY_TIME_CRITICAL, 0, CREATE_SUSPENDED);

	m_pWanderThread->m_bAutoDelete = m_pMoveThread->m_bAutoDelete = FALSE;
	m_pMoveThread->ResumeThread();
	m_pWanderThread->ResumeThread();
}

void CGalagaMap::StopThread()
{
	m_bFlag = false; 
	::WaitForSingleObject(m_pMoveThread->m_hThread, INFINITE);
	::WaitForSingleObject(m_pWanderThread->m_hThread, INFINITE);

	delete m_pMoveThread;
	delete m_pWanderThread;
	m_pWanderThread = m_pMoveThread = nullptr;
}

void CGalagaMap::SuspendThread()
{
	m_pMoveThread->SuspendThread();
	m_pWanderThread->SuspendThread();	
}

void CGalagaMap::ResumeThread()
{
	m_pMoveThread->ResumeThread();
	m_pWanderThread->ResumeThread();
}

void CGalagaMap::ShotEnemyBullet(MOVEENEMY* p)
{
	int nY = p->pShip->GetShipCenter().y;
	if (!rand(0, 100) && (300 < nY && nY < 450) && m_EnemyBullet.size() < m_Stage.GetMaxEnemyBullet())
	{
		int nE = p->pShip->GetShipCenter().x;
		int nM = m_pMyShip->GetShipCenter().x;
		int nDistance = (nE - nM < 0) ? (nE - nM) * -1 : nE - nM;

		if (100 > nDistance)
		{
			CBullet bul(p->pShip->CreateBullet());
			m_EnemyBullet.push_back(bul);
			// Shot Bullet
		}
	}
}

void CGalagaMap::GoBackThePlace()
{
	for (int i = 0; i < (int)m_Enemy.size(); i++)
	{
		m_Enemy[i]->Move(m_Enemy[i]->GetPos());
	}
}

void CGalagaMap::OnHitMyShip()
{
	m_pMyShip->OnHit();
	if (!m_pMyShip->GetLife())
	{
		CString str;
		str.Format(_T("Game Over\nScore : %d"), m_nScore);
		AfxMessageBox(str);
		AfxMessageBox(_T("Ready!"));

		m_EnemyBullet.clear();
		m_Enemy.clear();
		m_MoveEnemy.clear();
		m_pMyShip->GetBullets()->clear();
		delete m_pMyShip;
		m_Stage.InitStage();
		InitMap();
		StopThread();
		StartThread();		
		m_pParent->PostMessage(UM_RESTART, 0, 0);		
	}
}

/*
UINT CGalagaMap::DestroyEffectThread(void* p)
{
	DESTROYSHIP *pDS = (DESTROYSHIP*)p;

	CWnd *pWnd = pDS->pWnd;
	CRect Rect = pDS->Rect;

	CClientDC dc(pWnd);
	CDC MemDC;

	MemDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_EXPLOSION);

	BITMAP bmpinfo;
	bitmap.GetBitmap(&bmpinfo);

	CBitmap *pOldBitmap = (CBitmap*)MemDC.SelectObject(&bitmap);

	int W = bmpinfo.bmWidth / 6;
	int H = bmpinfo.bmHeight / 7;

	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			Sleep(20);
			dc.StretchBlt(Rect.left, Rect.top, Rect.Width(), Rect.Height(), &MemDC, j * W, i * H, W, H, SRCCOPY);
		}
	}
	MemDC.SelectObject(pOldBitmap);
	bitmap.DeleteObject();
	return 0;
}
*/


UINT MoveEnemyShotThread(LPVOID p)
{
	CGalagaMap* pMap = (CGalagaMap*)p;
	UINT nCnt = 0;
	while (pMap->m_bFlag)
	{
		for (int i = 0; i < (int)pMap->GetMoveEnemy()->size(); i++)
		{		
			MOVEENEMY *m = &pMap->GetMoveEnemy()->at(i);
			m->ShipVec.x = m->pShip->GetShipCenter().x;
			m->ShipVec.y = m->pShip->GetShipCenter().y;
		
			m->Vec.x = pMap->GetMyShip()->GetShipCenter().x - m->ShipVec.x;
			m->Vec.y = pMap->GetMyShip()->GetShipCenter().y - m->ShipVec.y + 150;

			m->Vec.Normalize();
			m->Vec *= 0.1;

			m->Acc = m->Vec;
			m->Vel += m->Vec;
			m->Vel.Limit(3);
			m->ShipVec += m->Vel;
			
			if (m->pShip->GetType() == Butterfly)
				pMap->ShotEnemyBullet(m);

			if (m->pShip->GetShipTop() > 900) // 밑으로 내려갔으면
				m->pShip->Move(CPoint(400, -100)); // 위로 텔레포트
			else									// 아니면
				m->pShip->Move(CPoint((int)m->ShipVec.x, (int)m->ShipVec.y));	// 정상적으로 이동	
		}
		Sleep(10);
		nCnt++;		
	}
	return 0;
}

UINT EnemyWander(LPVOID p)
{
	CGalagaMap* pMap = (CGalagaMap*)p;
	while (pMap->m_bFlag)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < (int)pMap->GetEnemy()->size(); j++)
			{
				if (!pMap->GetEnemy()->at(j)->IsThisMoveShip())
				{					
					int nRange = (i == 0 || i == 3) ? -20 : 20;
					CPoint &pt = pMap->GetEnemy()->at(j)->GetShipCenter();
					pMap->GetEnemy()->at(j)->Move(CPoint(pt.x + nRange, pt.y));
				}
			}
			Sleep(1500);
		}
	}
	
	return 0;
}

