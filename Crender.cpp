// Render.cpp : implementation file
//

#include "stdafx.h"
#include "RFMapTool.h"
#include "crender.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRender dialog


CRender::CRender(CWnd* pParent /*=NULL*/)
	: CDialog(CRender::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRender)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CRender::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRender)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRender, CDialog)
	//{{AFX_MSG_MAP(CRender)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRender message handlers

BOOL CRender::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	g_pDoc->m_cD3dApp.InitD3DApp( m_hWnd );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//--------------------------- 인터페이스 연결...
#include "r3engineKernel.h"

void CRender::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	R3MsgProc( 0, WM_LBUTTONDOWN ,0 ,0 );

	CDialog::OnLButtonDown(nFlags, point);
}

void CRender::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	R3MsgProc( 0, WM_LBUTTONUP ,0 ,0 );

	CDialog::OnLButtonUp(nFlags, point);
}

void CRender::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	LPARAM lParam=point.y<<16|point.x;
	R3MsgProc( 0, WM_MOUSEMOVE ,0 ,lParam );
	
	CDialog::OnMouseMove(nFlags, point);
}

void CRender::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	R3MsgProc( 0, WM_RBUTTONDOWN ,0 ,0 );

	CDialog::OnRButtonDown(nFlags, point);
}

void CRender::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	R3MsgProc( 0, WM_RBUTTONUP ,0 ,0 );

	CDialog::OnRButtonUp(nFlags, point);
}

void CRender::OnDestroy() 
{
	g_pDoc->m_cD3dApp.ReleaseD3DApp();
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}


void CRender::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	R3MsgProc( 0, WM_KEYDOWN, nChar,0 );
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CRender::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	R3MsgProc( 0, WM_KEYUP, nChar,0 );

	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}
BOOL CRender::PreTranslateMessage(MSG* pMsg)	//enter키로 인한 것 처리..
{
	
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		R3MsgProc( 0, WM_KEYDOWN, pMsg->wParam,0 );
		return TRUE;
	}
	if (pMsg->message == WM_KEYUP)
	{
		R3MsgProc( 0, WM_KEYUP, pMsg->wParam,0 );
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
