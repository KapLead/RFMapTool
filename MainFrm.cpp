// MainFrm.cpp : implementation of the CMainFrame class
//

#include <stdafx.h>
#include "RFMapTool.h"


#include "MainFrm.h"

#include "TabCollision.h"
#include "TabEntity.h"
#include "TabParticle.h"
#include "TabMagic.h"
#include "TabSound.h"
#include "TabDummy.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#pragma	comment( lib, "..\\R3engine\\JpegLibMTDLL.lib" )


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


#define _ROLL_POS_SX 8
#define _ROLL_POS_SY 195+15
#define _ROLL_POS_EX 200

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
	m_bIsRollCreate=FALSE;

//	g_console.Init("Map colsole");
//	g_console.printf("시작 한다..\n");
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY |CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_bIsRollCreate=TRUE;

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	// 오른쪽 주메뉴를 만든다.
	BOOL bRes = m_wndDialogBar.Create(this, IDD_DIALOGBAR, WS_VISIBLE|WS_CHILD|CBRS_RIGHT, 1);

	//-----오른쪽 주메뉴에다 상단에 항상있는 메뉴를 붙인다.
	g_pDoc->m_RightBack.Create(IDD_ALWAYS_BACK,&m_wndDialogBar);
	g_pDoc->m_RightBack.ShowWindow(SW_SHOWNORMAL);

	// 오른쪽 툴 탭을 메뉴를 붙이자..



//----롤시키는 거 붙이자...
	//충돌 
	g_pDoc->m_wndCollRollupCtrl.Create(WS_VISIBLE|WS_CHILD, CRect(_ROLL_POS_SX,_ROLL_POS_SY,_ROLL_POS_EX,520), &m_wndDialogBar, 2);
	//Add some pages
	g_pDoc->m_wndCollRollupCtrl.InsertPage("Collision File",		IDD_COLL_FILE,	RUNTIME_CLASS(CCollFile), 10);
	g_pDoc->m_wndCollRollupCtrl.InsertPage("Collision Edit",		IDD_COLL_EDIT,	RUNTIME_CLASS(CCollEdit), 11);

	g_pDoc->m_wndCollRollupCtrl.ExpandAllPages(TRUE);
//	g_pDoc->m_wndCollRollupCtrl.ShowWindow(SW_HIDE);//	SW_SHOWNA //SW_SHOWNORMAL 

	//엔티티 
	g_pDoc->m_wndEntityRollupCtrl.Create(WS_VISIBLE|WS_CHILD, CRect(_ROLL_POS_SX,_ROLL_POS_SY,_ROLL_POS_EX,520), &m_wndDialogBar, 3);
	//Add some pages
	g_pDoc->m_wndEntityRollupCtrl.InsertPage("Entity File",		IDD_ENTITY_FILE,	RUNTIME_CLASS(CEntityFile), 20);
	g_pDoc->m_wndEntityRollupCtrl.InsertPage("Entity Edit",		IDD_ENTITY_EDIT,	RUNTIME_CLASS(CEntityEdit), 21);

	g_pDoc->m_wndEntityRollupCtrl.ExpandAllPages(TRUE);
	g_pDoc->m_wndEntityRollupCtrl.ShowWindow(SW_HIDE);//	SW_SHOWNA //SW_SHOWNORMAL 

	//파티클
	g_pDoc->m_wndParticleRollupCtrl.Create(WS_VISIBLE|WS_CHILD, CRect(_ROLL_POS_SX,_ROLL_POS_SY,_ROLL_POS_EX,520), &m_wndDialogBar, 4);
	//Add some pages
	g_pDoc->m_wndParticleRollupCtrl.InsertPage("Particle File",		IDD_PARTICLE_FILE,	RUNTIME_CLASS(CParticleFile), 30);
	g_pDoc->m_wndParticleRollupCtrl.InsertPage("Particle Edit",		IDD_PARTICLE_EDIT,	RUNTIME_CLASS(CParticleEdit), 31);

	g_pDoc->m_wndParticleRollupCtrl.ExpandAllPages(TRUE);
	g_pDoc->m_wndParticleRollupCtrl.ShowWindow(SW_HIDE);//	SW_SHOWNA //SW_SHOWNORMAL 

	//매직
	g_pDoc->m_wndMagicRollupCtrl.Create(WS_VISIBLE|WS_CHILD, CRect(_ROLL_POS_SX,_ROLL_POS_SY,_ROLL_POS_EX,520), &m_wndDialogBar, 5);
	//Add some pages
	g_pDoc->m_wndMagicRollupCtrl.InsertPage("Magic File",		IDD_MAGIC_FILE,	RUNTIME_CLASS(CMagicFile), 40);
	g_pDoc->m_wndMagicRollupCtrl.InsertPage("Magic Edit",		IDD_MAGIC_EDIT,	RUNTIME_CLASS(CMagicEdit), 41);

	g_pDoc->m_wndMagicRollupCtrl.ExpandAllPages(TRUE);
	g_pDoc->m_wndMagicRollupCtrl.ShowWindow(SW_HIDE);//	SW_SHOWNA //SW_SHOWNORMAL 

	//사운드 
	g_pDoc->m_wndSoundRollupCtrl.Create(WS_VISIBLE|WS_CHILD, CRect(_ROLL_POS_SX,_ROLL_POS_SY,_ROLL_POS_EX,520), &m_wndDialogBar, 6);
	//Add some pages
	g_pDoc->m_wndSoundRollupCtrl.InsertPage("Sound File",		IDD_SOUND_FILE,	RUNTIME_CLASS(CSoundFile), 50);
	g_pDoc->m_wndSoundRollupCtrl.InsertPage("Sound Edit",		IDD_SOUND_EDIT,	RUNTIME_CLASS(CSoundEdit), 51);

	g_pDoc->m_wndSoundRollupCtrl.ExpandAllPages(TRUE);
	g_pDoc->m_wndSoundRollupCtrl.ShowWindow(SW_HIDE);//	SW_SHOWNA //SW_SHOWNORMAL 

	
	//더미찍기...
	g_pDoc->m_wndDummyRollupCtrl.Create(WS_VISIBLE|WS_CHILD, CRect(_ROLL_POS_SX,_ROLL_POS_SY,_ROLL_POS_EX,600), &m_wndDialogBar, 7);
	//Add some pages
	g_pDoc->m_wndDummyRollupCtrl.InsertPage("Dummy File",		IDD_DUMMY_FILE,	RUNTIME_CLASS(CDummyFile), 60);
	g_pDoc->m_wndDummyRollupCtrl.InsertPage("Dummy Edit",		IDD_DUMMY_EDIT,	RUNTIME_CLASS(CDummyEdit), 61);

	g_pDoc->m_wndDummyRollupCtrl.ExpandAllPages(TRUE);
	g_pDoc->m_wndDummyRollupCtrl.ShowWindow(SW_HIDE);//	SW_SHOWNA //SW_SHOWNORMAL 
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{

	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	//SendMessage(pMsg->message,pMsg->wParam ,pMsg->lParam );
	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) //사이즈 변경될때 롤창도 바꾸자..
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	if( m_bIsRollCreate )
	{
		RECT Rect={_ROLL_POS_SX,_ROLL_POS_SY,_ROLL_POS_EX,cy-56};
		g_pDoc->m_wndCollRollupCtrl.MoveWindow(&Rect);
		g_pDoc->m_wndParticleRollupCtrl.MoveWindow(&Rect);
		g_pDoc->m_wndEntityRollupCtrl.MoveWindow(&Rect);
		g_pDoc->m_wndMagicRollupCtrl.MoveWindow(&Rect);
		g_pDoc->m_wndSoundRollupCtrl.MoveWindow(&Rect);
		g_pDoc->m_wndDummyRollupCtrl.MoveWindow(&Rect);
		
		RECT temp_rect;
		m_wndDialogBar.GetClientRect(&temp_rect);

		Rect.left=0;
		Rect.top=0;
		Rect.right=cx-4-temp_rect.right;
		Rect.bottom=cy-46;
//		g_pDoc->m_cRender.GetClientRect(&rect);
		g_pDoc->m_cRender.MoveWindow(&Rect);//0, 0, 800, 600);
		SetRenderRect(Rect.left,Rect.top,Rect.right,Rect.bottom);
		g_pDoc->m_cRender.GetClientRect(&Rect);
	}
}
