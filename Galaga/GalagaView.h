
// GalagaView.h : CGalagaView 클래스의 인터페이스
//

#pragma once


class CGalagaMap;

class CGalagaView : public CView
{
protected: // serialization에서만 만들어집니다.
	CGalagaView();
	DECLARE_DYNCREATE(CGalagaView)

// 특성입니다.
public:
	CGalagaDoc* GetDocument() const;
	CGalagaMap *m_pMap;

	void StartThread();
	void StopThread();
	void SuspendThread();
	void ResumeThread();

	void Restart();

private:	

	bool m_bFlag;
	bool m_bSuspend;
	CWinThread *m_pMyBullet;
	CWinThread *m_pMoveThread;
	CWinThread *m_pDrawThread;

	// Thread Func
	
	static UINT BulletThread(LPVOID p);
	static UINT DrawThread(LPVOID p);
	static UINT KeyDownThread(LPVOID p);

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CGalagaView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();

	LRESULT OnRestart(WPARAM wparam, LPARAM lparam);
};

#ifndef _DEBUG  // GalagaView.cpp의 디버그 버전
inline CGalagaDoc* CGalagaView::GetDocument() const
   { return reinterpret_cast<CGalagaDoc*>(m_pDocument); }
#endif

