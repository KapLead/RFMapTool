// RightBack.cpp : implementation file
//

#include "stdafx.h"
#include "rfmaptool.h"
#include "RightBack.h"
#include "ToolManager.h"
#include "console.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRightBack dialog


CRightBack::CRightBack(CWnd* pParent /*=NULL*/)
	: CDialog(CRightBack::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRightBack)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CRightBack::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRightBack)
	DDX_Control(pDX, IDC_EDIT4, m_FlySpeed);
	DDX_Control(pDX, IDC_EDIT3, m_CameraDist);
	DDX_Control(pDX, IDC_COMBO1, m_PikedStyle);
	DDX_Control(pDX, IDC_TAB_COLL_BUTTON, m_TabCollButton);
	DDX_Control(pDX, IDC_TAB_ENTITY_BUTTON, m_TabEntity);
	DDX_Control(pDX, IDC_TAB_PARTICLE_BUTTON, m_TabParticle);
	DDX_Control(pDX, IDC_TAB_MAGIC_BUTTON, m_TabMagic);
	DDX_Control(pDX, IDC_TAB_SOUND_BUTTON, m_TabSound);
	DDX_Control(pDX, IDC_TAB_DUMMY_BUTTON, m_TabDummy);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRightBack, CDialog)
	//{{AFX_MSG_MAP(CRightBack)
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	ON_BN_CLICKED(IDC_CHECK5, OnCheck5)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_EN_CHANGE(IDC_EDIT3, OnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, OnChangeEdit4)
	ON_BN_CLICKED(IDC_CHECK6, OnCheck6)
	ON_BN_CLICKED(IDC_CHECK7, OnCheck7)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_BN_CLICKED(IDC_CHECK8, OnCheck8)
	ON_BN_CLICKED(IDC_CHECK9, OnCheck9)
	ON_BN_CLICKED(IDC_TAB_COLL_BUTTON, OnTabCollButton)
	ON_BN_CLICKED(IDC_TAB_ENTITY_BUTTON, OnTabEntityButton)
	ON_BN_CLICKED(IDC_TAB_PARTICLE_BUTTON, OnTabParticleButton)
	ON_BN_CLICKED(IDC_TAB_MAGIC_BUTTON, OnTabMagicButton)
	ON_BN_CLICKED(IDC_TAB_SOUND_BUTTON, OnTabSoundButton)
	ON_BN_CLICKED(IDC_TAB_DUMMY_BUTTON, OnTabDummyButton)
	ON_BN_CLICKED(IDC_CHECK10, OnCheck10)
	ON_BN_CLICKED(IDC_CHECK27, OnCheck27)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRightBack message handlers


static BOOL stViewColFace = TRUE,stViewMapFace = TRUE,stViewColPoint = TRUE,stViewEntity=TRUE,stViewEntityBBox=TRUE;
static BOOL stYButton,stXZButton;

static BOOL stViewDummy =TRUE,stViewSoundEntity=TRUE;

BOOL GetYButton()
{
	return stYButton;
}
BOOL GetXZButton()
{
	return stXZButton;
}
int GetEditCameraDist()
{
	char buf[256];

	g_pDoc->m_RightBack.m_CameraDist.GetWindowText(buf,256);
	return atoi(buf);
}
void SetEditCameraDist(int su)
{
	char buf[256];
	itoa(su,buf,10);
	g_pDoc->m_RightBack.m_CameraDist.SetWindowText(buf);
}

BOOL CRightBack::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetViewColPoint(stViewColPoint);
	SetViewMapFace(stViewMapFace);
	SetViewColFace(stViewColFace);
	SetViewEntity(stViewEntity);
	SetViewEntityBBox(stViewEntityBBox);
	SetViewSoundEntity(stViewSoundEntity);
	SetViewDummyEntity(stViewDummy);

	//맵,충돌맵,충돌점,엔티티 옵션키고..
	CheckDlgButton(IDC_CHECK3,TRUE);
	CheckDlgButton(IDC_CHECK4,TRUE);
	CheckDlgButton(IDC_CHECK5,TRUE);
	CheckDlgButton(IDC_CHECK8,TRUE);
	CheckDlgButton(IDC_CHECK9,TRUE);
	CheckDlgButton(IDC_CHECK10,TRUE);
	CheckDlgButton(IDC_CHECK27,TRUE);

	m_PikedStyle.AddString("가장가까운곳");
	m_PikedStyle.AddString("가장먼곳");
	m_PikedStyle.AddString("바로전높이");
	m_PikedStyle.SetCurSel(0);

	char buf[256]="200";
	m_CameraDist.SetWindowText(buf);

	char buf1[256]="5";
	SetFlySpeed(5/5.0f);
	m_FlySpeed.SetWindowText(buf1);

	//xz 축이동 키고...
	CheckDlgButton(IDC_CHECK7,TRUE);
	stXZButton=TRUE;

	//탭 비트맵 로딩..
	m_TabCollButton.LoadBitmaps(IDB_TAB_COLL_DN,IDB_TAB_COLL_UP,IDB_TAB_COLL_UP,IDB_TAB_COLL_DN);	
	m_TabEntity.LoadBitmaps(IDB_TAB_ENTITY_DN,IDB_TAB_ENTITY_UP,IDB_TAB_ENTITY_UP,IDB_TAB_ENTITY_DN);
	m_TabParticle.LoadBitmaps(IDB_TAB_PARTICLE_DN,IDB_TAB_PARTICLE_UP,IDB_TAB_PARTICLE_UP,IDB_TAB_PARTICLE_DN);
	m_TabMagic.LoadBitmaps(IDB_TAB_MAGIC_DN,IDB_TAB_MAGIC_UP,IDB_TAB_MAGIC_UP,IDB_TAB_MAGIC_DN);
	m_TabSound.LoadBitmaps(IDB_TAB_SOUND_DN,IDB_TAB_SOUND_UP,IDB_TAB_SOUND_UP,IDB_TAB_SOUND_DN);
	m_TabDummy.LoadBitmaps(IDB_TAB_DUMMY_DN,IDB_TAB_DUMMY_UP,IDB_TAB_DUMMY_UP,IDB_TAB_DUMMY_DN);
/*
	CheckDlgButton(IDC_TAB_COLL_BUTTON,TRUE);
	CWnd* c_wnd = GetDlgItem(IDC_TAB_COLL_BUTTON);
	c_wnd->SetFocus();//SetActiveWindow();
*/
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRightBack::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
void CRightBack::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CRightBack::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CRightBack::OnCheck4() //맵표시 
{
	// TODO: Add your control notification handler code here
	
	if( stViewMapFace )
		stViewMapFace =FALSE;
	else
		stViewMapFace =TRUE;
	SetViewMapFace(stViewMapFace);
}

void CRightBack::OnCheck5() //충돌점표시 
{
	// TODO: Add your control notification handler code here
	
	if( stViewColPoint )
		stViewColPoint =FALSE;
	else
		stViewColPoint =TRUE;
	SetViewColPoint(stViewColPoint);
}

int CRightBack::GetNowTab() //현재탭이 어디에 있는지 알아낸다. 
{
//	return m_pcTabSheet->GetActiveIndex();
	return 0;
}
void CRightBack::OnCheck3() //충돌맵 표시 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	if( stViewColFace )
		stViewColFace =FALSE;
	else
		stViewColFace =TRUE;
	SetViewColFace(stViewColFace);
	
}
void CRightBack::OnCheck8()	//엔티티 표시
{
	// TODO: Add your control notification handler code here
	if( stViewEntity )
		stViewEntity =FALSE;
	else
		stViewEntity =TRUE;
	SetViewEntity(stViewEntity);
}

void CRightBack::OnChangeEdit3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	char buf[256];
	m_CameraDist.GetWindowText(buf,256);

	int i;
	for(i=0; i<(int)strlen(buf); i++)
	{
		if( buf[i] < '0' || buf[i] > '9' )
		{
			buf[i]=NULL;
			m_CameraDist.SetWindowText(buf);
		}
	}	
}

void CRightBack::OnChangeEdit4() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	char buf[256];
	m_FlySpeed.GetWindowText(buf,256);

	int i;
	for(i=0; i<(int)strlen(buf); i++)
	{
		if( buf[i] < '0' || buf[i] > '9' )
		{
			buf[i]=NULL;
			m_FlySpeed.SetWindowText(buf);
		}
	}
	int speed = atoi(buf);
	SetFlySpeed(speed/5.0f);
}


void CRightBack::OnCheck6() // y
{
	stYButton=!stYButton;
	// TODO: Add your control notification handler code here
	
	if( stXZButton && stYButton )
	{
		CheckDlgButton(IDC_CHECK7,FALSE);
		stXZButton=FALSE;
	}	
}

void CRightBack::OnCheck7()	//xz
{
	// TODO: Add your control notification handler code here

	stXZButton=!stXZButton;
	if( stXZButton && stYButton )
	{
		CheckDlgButton(IDC_CHECK6,FALSE);
		stYButton=FALSE;
	}
}

void CRightBack::OnSelchangeCombo1() // 픽킹스타일.
{
	// TODO: Add your control notification handler code here
	SetPickingStyle(m_PikedStyle.GetCurSel());
}

void SetYXZAxis(BOOL y_axis,BOOL xz_axis)
{
	if( y_axis )
	{
		//y 축이동 키고...
		g_pDoc->m_RightBack.CheckDlgButton(IDC_CHECK6,TRUE);
		stYButton=TRUE;
		g_pDoc->m_RightBack.CheckDlgButton(IDC_CHECK7,FALSE);
		stXZButton=FALSE;
	}
	else
	{
		//xz 축이동 키고...
		g_pDoc->m_RightBack.CheckDlgButton(IDC_CHECK7,TRUE);
		stXZButton=TRUE;
		g_pDoc->m_RightBack.CheckDlgButton(IDC_CHECK6,FALSE);
		stYButton=FALSE;
	}
}

void CRightBack::OnCheck9()	//엔티티 바운딩 박스....
{
	// TODO: Add your control notification handler code here
	
	if( stViewEntityBBox )
		stViewEntityBBox =FALSE;
	else
		stViewEntityBBox =TRUE;
	SetViewEntityBBox(stViewEntityBBox);
}
void CRightBack::OnOK()	//esc와 enter키를 흡수한다.
{
	// TODO: Add extra validation here

//	CDialog::OnOK();
}

void CRightBack::OnCancel() 
{
	// TODO: Add extra cleanup here
	
//	CDialog::OnCancel();
}

//-------버튼 탭으로 메뉴를 바꾸어준다.

static LONG stCmode;
void CRightBack::OnTabCollButton() 
{
	// TODO: Add your control notification handler code here
	SetTabMode(_COLLISION_MODE);
	stCmode = WhatCollisionMode();
	if( stCmode == _COLLISON_TEST_MODE )
		SetConsoleStateB(OP_FLY,FALSE);
//	CheckDlgButton(IDC_TAB_COLL_CHECK,TRUE);
//	CheckDlgButton(IDC_TAB_ENTITY_CHECK,FALSE);
//	CheckDlgButton(IDC_TAB_PARTICLE_CHECK,FALSE);
}

void CRightBack::OnTabEntityButton() 
{
	// TODO: Add your control notification handler code here
	SetTabMode(_ENTITY_MODE);	

	stCmode = WhatCollisionMode();
	SetCollisionMode(_COLLISON_EDIT_MODE);
	if( stCmode == _COLLISON_TEST_MODE )
		SetConsoleStateB(OP_FLY,TRUE);
	SetCollisionMode(stCmode);
//	CheckDlgButton(IDC_TAB_COLL_CHECK,FALSE);
//	CheckDlgButton(IDC_TAB_ENTITY_CHECK,TRUE);
//	CheckDlgButton(IDC_TAB_PARTICLE_CHECK,FALSE);
}
void CRightBack::OnTabParticleButton() 
{
	// TODO: Add your control notification handler code here
	SetTabMode(_PARTICLE_MODE);

	stCmode = WhatCollisionMode();
	SetCollisionMode(_COLLISON_EDIT_MODE);
	if( stCmode == _COLLISON_TEST_MODE )
		SetConsoleStateB(OP_FLY,TRUE);
	SetCollisionMode(stCmode);
//	CheckDlgButton(IDC_TAB_COLL_CHECK,FALSE);
//	CheckDlgButton(IDC_TAB_ENTITY_CHECK,FALSE);
//	CheckDlgButton(IDC_TAB_PARTICLE_CHECK,TRUE);
}	
void CRightBack::OnTabMagicButton() 
{
	// TODO: Add your control notification handler code here
	SetTabMode(_MAGIC_MODE);

	stCmode = WhatCollisionMode();
	SetCollisionMode(_COLLISON_EDIT_MODE);
	if( stCmode == _COLLISON_TEST_MODE )
		SetConsoleStateB(OP_FLY,TRUE);
	SetCollisionMode(stCmode);
}	
void CRightBack::OnTabSoundButton() 
{
	// TODO: Add your control notification handler code here
	SetTabMode(_SOUND_MODE);

	stCmode = WhatCollisionMode();
	SetCollisionMode(_COLLISON_EDIT_MODE);
	if( stCmode == _COLLISON_TEST_MODE )
		SetConsoleStateB(OP_FLY,TRUE);
	SetCollisionMode(stCmode);
}	
void CRightBack::OnTabDummyButton() 
{
	// TODO: Add your control notification handler code here
	SetTabMode(_DUMMY_MODE);

	stCmode = WhatCollisionMode();
	SetCollisionMode(_COLLISON_EDIT_MODE);
	if( stCmode == _COLLISON_TEST_MODE )
		SetConsoleStateB(OP_FLY,TRUE);
	SetCollisionMode(stCmode);
}	

void CRightBack::OnCheck10() //더미 표시..
{
	// TODO: Add your control notification handler code here
	
	if( stViewDummy )
		stViewDummy =FALSE;
	else
		stViewDummy =TRUE;
	SetViewDummyEntity(stViewDummy);
}

void CRightBack::OnCheck27() //사운드 표시..
{
	// TODO: Add your control notification handler code here
	
	if( stViewSoundEntity )
		stViewSoundEntity =FALSE;
	else
		stViewSoundEntity =TRUE;
	SetViewSoundEntity(stViewSoundEntity);
}
