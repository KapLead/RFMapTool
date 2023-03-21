// RFMapToolView.cpp : implementation of the CRFMapToolView class
//

#include "stdafx.h"
#include "RFMapTool.h"

#include "RFMapToolDoc.h"
#include "RFMapToolView.h"
#include "TabCollision.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CRFMapToolView

IMPLEMENT_DYNCREATE(CRFMapToolView, CFormView)

BEGIN_MESSAGE_MAP(CRFMapToolView, CFormView)
	//{{AFX_MSG_MAP(CRFMapToolView)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_OPEN_EBP, OnFileOpenEbp)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()







/////////////////////////////////////////////////////////////////////////////
// CRFMapToolView construction/destruction

CRFMapToolView::CRFMapToolView()
	: CFormView(CRFMapToolView::IDD)
{
	//{{AFX_DATA_INIT(CRFMapToolView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CRFMapToolView::~CRFMapToolView()
{
}

void CRFMapToolView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRFMapToolView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CRFMapToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CRFMapToolView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

/////////////////////////////////////////////////////////////////////////////
// CRFMapToolView printing

BOOL CRFMapToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRFMapToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRFMapToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CRFMapToolView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CRFMapToolView diagnostics

#ifdef _DEBUG
void CRFMapToolView::AssertValid() const
{
	CFormView::AssertValid();
}

void CRFMapToolView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CRFMapToolDoc* CRFMapToolView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRFMapToolDoc)));
	return (CRFMapToolDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRFMapToolView message handlers


int CRFMapToolView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	

	
//	g_pDoc->m_cRightTab.Create(IDD_TAB_BORDER,this);
//	g_pDoc->m_cRightTab.MoveWindow(800, 0, 1024-800-12, 600);

	g_pDoc->m_cRender.Create(IDD_DIALOG_RENDER, this);
	RECT rect;
	g_pDoc->m_cRender.GetClientRect(&rect);
	g_pDoc->m_cRender.MoveWindow(0, 0, 800, 600);
//	g_pDoc->m_cRender.MoveWindow(0, 0, 800, 600);
	g_pDoc->m_cRender.ShowWindow(SW_SHOW);

	//CFormView::GetClientRect



	::SetActiveWindow(g_pDoc->m_cRender.m_hWnd);
	// TODO: Add your specialized creation code here
	
	return 0;
}


#include "CmyD3DApp.h"

void CRFMapToolView::OnFileOpen() 
{
	// TODO: Add your command handler code here
	g_pDoc->m_cD3dApp.LoadBsp();
}

void CRFMapToolView::OnFileOpenEbp() 
{
	// TODO: Add your command handler code here
	g_pDoc->m_cD3dApp.LoadEbp();
}

void CRFMapToolView::OnFileSave() 
{
	// TODO: Add your command handler code here
	g_pDoc->m_cD3dApp.SaveEbp();
	
}
