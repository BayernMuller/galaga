
// GalagaView.cpp : CGalagaView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Galaga.h"
#endif

#include "GalagaDoc.h"
#include "GalagaView.h"

#include "GalagaMap.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGalagaView
IMPLEMENT_DYNCREATE(CGalagaView, CView)

BEGIN_MESSAGE_MAP(CGalagaView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_MESSAGE(UM_RESTART, OnRestart)
END_MESSAGE_MAP()

// CGalagaView 생성/소멸

CGalagaView::CGalagaView() : m_bSuspend(false)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_pMyBullet = m_pMoveThread = nullptr;	
	m_bFlag = false;
}

CGalagaView::~CGalagaView()
{	
	delete m_pMap;
}

BOOL CGalagaView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	cs.style &= ~(WS_BORDER);		// 테두리 없애기
	return CView::PreCreateWindow(cs);
}

// CGalagaView 그리기
void CGalagaView::OnDraw(CDC* pDC)
{
	CGalagaDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	/* GDI+ 더블버퍼링
	CRect rect;
	GetClientRect(&rect);

	Bitmap bmp(rect.Width(), rect.Height());

	Graphics memDC(&bmp);

	// 그리기
	

	Graphics g(pDC->m_hDC);
	g.DrawImage(&bmp, 0, 0);
	*/
	m_pMap->DrawGalaga(pDC);

}

// CGalagaView 인쇄

BOOL CGalagaView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CGalagaView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CGalagaView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CGalagaView 진단

#ifdef _DEBUG
void CGalagaView::AssertValid() const
{
	CView::AssertValid();
}

void CGalagaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGalagaDoc* CGalagaView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGalagaDoc)));
	return (CGalagaDoc*)m_pDocument;
}
#endif //_DEBUG


void CGalagaView::StartThread()
{
	m_bFlag = true;
	m_pMyBullet = AfxBeginThread(BulletThread, this, THREAD_PRIORITY_TIME_CRITICAL, 0, CREATE_SUSPENDED);
	m_pDrawThread = AfxBeginThread(DrawThread, this, THREAD_PRIORITY_TIME_CRITICAL, 0, CREATE_SUSPENDED);
	m_pMoveThread = AfxBeginThread(KeyDownThread, this, THREAD_PRIORITY_TIME_CRITICAL, 0, CREATE_SUSPENDED);

	m_pMoveThread->m_bAutoDelete = m_pDrawThread->m_bAutoDelete = m_pMyBullet->m_bAutoDelete = FALSE;
	m_pMyBullet->ResumeThread();
	m_pDrawThread->ResumeThread();
	m_pMoveThread->ResumeThread();
}

void CGalagaView::StopThread()
{
	m_bFlag = false;

	::WaitForSingleObject(m_pMyBullet->m_hThread, INFINITE);
	::WaitForSingleObject(m_pDrawThread->m_hThread, INFINITE);
	::WaitForSingleObject(m_pMoveThread->m_hThread, INFINITE);
	delete m_pDrawThread;
	delete m_pMyBullet;	
	delete m_pMoveThread;
	m_pMoveThread = m_pDrawThread = m_pMyBullet = nullptr;
}

void CGalagaView::SuspendThread()
{
	if (m_bSuspend)
		return;
	m_pMyBullet->SuspendThread();
	m_pDrawThread->SuspendThread();
	m_pMoveThread->SuspendThread();

	m_pMap->SuspendThread();
	m_bSuspend = true;
}

void CGalagaView::ResumeThread()
{
	if (!m_bSuspend)
		return;
	m_pMyBullet->ResumeThread();
	m_pDrawThread->ResumeThread();
	m_pMoveThread->ResumeThread();

	m_pMap->ResumeThread();
	m_bSuspend = false;
}



// CGalagaView 메시지 처리기


void CGalagaView::OnInitialUpdate()
{
	AfxMessageBox(_T("Ready!"));
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.		
	m_pMap = new CGalagaMap(this);
	StartThread();
}

void CGalagaView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{	
	const int VK_ESC = 27;
	switch (nChar)
	{
	case VK_ESC:
		if (m_bSuspend)
			ResumeThread();
		else
			SuspendThread();
		break;
	case VK_LEFT: case VK_RIGHT:
		break;
	default:
		if (!m_bSuspend)
			m_pMap->KeyDown(nChar);
	}
	//Invalidate(FALSE);
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


UINT CGalagaView::BulletThread(LPVOID p)
{
	CGalagaView *pView = (CGalagaView*)p;
	CMyShip *pShip = pView->m_pMap->GetMyShip();
	while (pView->m_bFlag)
	{
		for (int j = 0; j < (int)pShip->GetBullets()->size(); j++)
		{
			pShip->GetBullets()->at(j).MoveBullet(UP, 8); // Move And Draw
		}
		
		for (int i = 0; i < (int)pView->m_pMap->GetEnemyBullet()->size(); i++)
		{
			pView->m_pMap->GetEnemyBullet()->at(i).MoveBullet(DOWN, 4);
		}

		pView->m_pMap->CheckMyShipBulletHitOut();  // Check Hit Or Out
		pView->m_pMap->CheckEnemyBulletHitOut();
		pView->m_pMap->CheckMyShipDie();
		Sleep(10);
	}
	int a = 0;
	return 0;
}

UINT CGalagaView::DrawThread(LPVOID p)
{
	CGalagaView* pView = (CGalagaView*)p;

	while (pView->m_bFlag)
	{					
		pView->Invalidate(FALSE);
		Sleep(10);
	}
	return 0;
}

UINT CGalagaView::KeyDownThread(LPVOID p)
{
	CGalagaView* pView = (CGalagaView*)p;
	while (pView->m_bFlag)
	{
		BOOL right = GetKeyState(VK_RIGHT);
		BOOL left = GetKeyState(VK_LEFT);

		if ((left & 0x8000) == 0x8000)
			pView->m_pMap->KeyDown(VK_LEFT);
		else if ((right & 0x8000) == 0x8000)
			pView->m_pMap->KeyDown(VK_RIGHT);

		Sleep(20);
	}
	return 0;
}



BOOL CGalagaView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void CGalagaView::OnDestroy()
{	
	m_pMap->StopThread();
	StopThread();
	CView::OnDestroy();
}


LRESULT CGalagaView::OnRestart(WPARAM wparam, LPARAM lparam)
{
	StopThread();
	StartThread();
	return 0;
}