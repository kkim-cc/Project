
// GASA3View.h : interface of the CGASA3View class
//

#pragma once
#include "ShootModel.h"
#include "ShootControl.h"

class CGASA3View : public CView
{
protected: // create from serialization only
	CGASA3View();
	DECLARE_DYNCREATE(CGASA3View)

// Attributes
private:
	ShootModel *m_pModel;
	ShootControl *m_pControl;
	BOOL m_bInitialize;
	ULONG_PTR m_gdiplusToken;
public:
	CGASA3Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CGASA3View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};

#ifndef _DEBUG  // debug version in GASA3View.cpp
inline CGASA3Doc* CGASA3View::GetDocument() const
   { return reinterpret_cast<CGASA3Doc*>(m_pDocument); }
#endif

