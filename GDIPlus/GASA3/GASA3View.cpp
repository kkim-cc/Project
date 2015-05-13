
// GASA3View.cpp : implementation of the CGASA3View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GASA3.h"
#endif

#include "GASA3Doc.h"
#include "GASA3View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGASA3View
IMPLEMENT_DYNCREATE(CGASA3View, CView)

BEGIN_MESSAGE_MAP(CGASA3View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

// CGASA3View construction/destruction

CGASA3View::CGASA3View()
{
	// TODO: add construction code here
	m_bInitialize = FALSE;
	m_pModel = NULL;
	m_pControl = NULL;
}

CGASA3View::~CGASA3View()
{
}

BOOL CGASA3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGASA3View drawing
void CGASA3View::OnDraw(CDC* /*pDC*/)
{
	CGASA3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CGASA3View printing
BOOL CGASA3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGASA3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGASA3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGASA3View diagnostics
#ifdef _DEBUG
void CGASA3View::AssertValid() const
{
	CView::AssertValid();
}

void CGASA3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGASA3Doc* CGASA3View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGASA3Doc)));
	return (CGASA3Doc*)m_pDocument;
}
#endif //_DEBUG


// CGASA3View message handlers
void CGASA3View::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// check the state of windows.
	if (IsIconic())
		return;

	// check initialized
	if (m_bInitialize == FALSE)
		return;

	// Create a backbuffer bmp buffer to draw to in memory.
	HWND hwnd = GetSafeHwnd();
	HDC hdc = dc.GetSafeHdc();
	RECT rcClient;
	::GetClientRect(hwnd, &rcClient);

	int left = rcClient.left;
	int top = rcClient.top;
	int width = rcClient.right - rcClient.left;
	int height = rcClient.bottom - rcClient.top;

	// Create a memory DC for double buffering
	HDC hdcMem = ::CreateCompatibleDC(hdc);
	const int nMemDC = ::SaveDC(hdcMem);

	HBITMAP hBitmap = ::CreateCompatibleBitmap(hdc, width, height);
	::SelectObject(hdcMem, hBitmap);

	Graphics graphics(hdcMem);
	SolidBrush white(Color(255, 255, 255));
	graphics.FillRectangle(&white, left, top, width, height);

	// TODO: Add your message handler code here
	m_pModel->Paint(hdcMem, rcClient);

	// Swap buffers ie. push memory backbuffer to the screen frontbuffer
	RECT rcClip;
	::GetClipBox(hdc, &rcClip);
	left = rcClip.left;
	top = rcClip.top;
	width = rcClip.right - rcClip.left;
	height = rcClip.bottom - rcClip.top;
	::BitBlt(hdc, left, top, width, height, hdcMem, left, top, SRCCOPY);

	::RestoreDC(hdcMem, nMemDC);
	::DeleteObject(hBitmap);
}


BOOL CGASA3View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return FALSE;
}

// Initialize GDI+ Startup
// Start Timer
// Create Class and Load Image
void CGASA3View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	// In Your Load Method
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	m_pModel = new ShootModel();
	m_pModel->Load(L"image/Background.bmp", 
				   L"image/Midground.bmp", 
				   L"image/Foreground.bmp", 
				   L"image/pic",
				   L"image/box");

	m_pControl = new ShootControl(m_pModel);

	// set timer
	SetTimer(1, 50, NULL);

	m_bInitialize = TRUE;
}

// Destroy Class
// Stop Timer
// Shuttdown GDI+ 
void CGASA3View::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	if (m_pModel != NULL)
	{
		m_pModel->FreeShootModel();
		delete m_pModel;
		m_pModel = NULL;
	}
	if (m_pControl != NULL)
	{
		delete m_pControl;
		m_pControl = NULL;
	}

	KillTimer(1);

	GdiplusShutdown(m_gdiplusToken);
}


void CGASA3View::OnTimer(UINT_PTR nIDEvent)
{
	Invalidate();

	CView::OnTimer(nIDEvent);
}


void CGASA3View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (m_pControl->ProcessShoot(point))
		m_pControl->SetHit();

	CView::OnLButtonDown(nFlags, point);
}


BOOL CGASA3View::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (IsIconic())
	{
		return CView::OnSetCursor(pWnd, nHitTest, message);
	}
	else
	{
		HCURSOR hCursor = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR));
		SetCursor(hCursor);
		return TRUE;
	}
}
