// TabCollision.cpp : implementation file
//

#include "stdafx.h"
#include "RFMapTool.h"
#include "TabCollision.h"
#include "ToolManager.h"
#include "r3Input.h"
#include "level.h"
#include "console.h"
#include "ToolManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCollFile dialog

IMPLEMENT_DYNCREATE(CCollFile, CDialog)


CCollFile::CCollFile(CWnd* pParent /*=NULL*/)
	: CDialog(CCollFile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCollFile)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCollFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCollFile)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCollFile, CDialog)
	//{{AFX_MSG_MAP(CCollFile)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCollFile::OnButton1()		//충돌 cf파일 로딩.
{
	// TODO: Add your control notification handler code here
	LoadCFFile();
}

void CCollFile::OnButton2()		//충돌 cf파일  저장.
{
	// TODO: Add your control notification handler code here
	
	SaveCFFile();
}

/////////////////////////////////////////////////////////////////////////////
// CCollFile message handlers
/////////////////////////////////////////////////////////////////////////////
// CCollEdit dialog

IMPLEMENT_DYNCREATE(CCollEdit, CDialog)

CCollEdit::CCollEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CCollEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCollEdit)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCollEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCollEdit)
	DDX_Control(pDX, IDC_EDIT1, m_Height);
	DDX_Control(pDX, IDC_CHECK2, m_ContinueLine);
	DDX_Control(pDX, IDC_BUTTON3, m_Delete);
	DDX_Control(pDX, IDC_COMBO1, m_Select);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCollEdit, CDialog)
	//{{AFX_MSG_MAP(CCollEdit)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_CHECK26, OnCheck26)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//--------------여기부터는 실제 에디트 관련...
#include "ToolCollision.h"
static CToolCollisionFace stColface;

static CCollEdit *CCollEditPtr=NULL; //<- onintDialog에서 넣어서 사용한다.
static BOOL stUnFreezeMode=FALSE;

void CheckContinueLine(BOOL state)
{
	if( CCollEditPtr )
		CCollEditPtr->m_ContinueLine.SetCheck(state);
}

BOOL GetCheckContinueLine(BOOL state)
{
	if( CCollEditPtr )
	{
		return CCollEditPtr->m_ContinueLine.GetCheck();
	}
	return FALSE;
}

int GetEditHeight()
{
	char buf[256];

	CCollEditPtr->m_Height.GetWindowText(buf,256);
	return atoi(buf);
}
void SetEditHeight(int su)
{
	char buf[256];
	itoa(su,buf,10);
	CCollEditPtr->m_Height.SetWindowText(buf);
}



static void DeleteSeleted()
{
	if( WhatCollisionMode() == _COLLISON_EDIT_MODE )//에딧모드일경우... 현재 만지고있는 라인을 삭제..
	{
		if( stColface.mOnePointCreate )	//한점을 잘못 찍었을경우.... 그것만 지운다.
		{
			stColface.mOnePointCreate=0;
			if( stColface.mColPointCnt )
				stColface.mColPointCnt--;	
//			m_ContinueLine.SetCheck(FALSE);
			stColface.mbContinueLine = FALSE;
//			if( mColLine[mColLineCnt].end_v == 0)
			{
			}
		}
		else
			stColface.DeleteCurLine();
	}
	else
	if( WhatCollisionMode() == _COLLISON_SELECT_MODE )
	{
		int mode = GetSelectMode();
		switch(mode)
		{
			case 0:
				break;
			case 1:
			case 2: 
				stColface.DeleteSelectFace();
				break;
		}
	}
}

void ProgressCollisionEdit(void *v_level)
{
	Vector3f pos;

	LONG mouse_x = GetRectMouseX(gMouse.x);	//해상도에맞춰서..
	LONG mouse_y = GetRectMouseY(gMouse.y);	//해상도에 맞춰서..

	if( v_level == NULL )
		return;
	CLevel *level = (CLevel *)v_level;
	CBsp *bsp = level->mBsp;

	if (gMouse.button & _LEFT_BUTTON_DOWN)	//에딧 모드
	{           
		gMouse.button &= (~_LEFT_BUTTON_DOWN);
		BOOL is_coll;

		if( GetPickingStyle() == _PICKING_STYLE_FAR )
			is_coll=level->GetPointFromScreenRayFar(mouse_x,mouse_y,&pos);//마우스 찝은곳이 성공.
		else
			is_coll=level->GetPointFromScreenRay(mouse_x,mouse_y,&pos);//마우스 찝은곳이 성공.

		if( GetPickingStyle() == _PICKING_STYLE_BEFORE_POINT && stColface.GetContinueLine() )
		{
			Vector3f bpos;
			stColface.GetVertex(&bpos,stColface.GetBeforeId());
			pos[1]=bpos[1];
		}
		if( is_coll )
		{
			WORD v_id=stColface.IsExistPoint(pos);	//인근에 점이 있다면.
			if( v_id )	//인접된 점이 있다.
			{
/*
				if( 0 )		//만약에 합치면 안되는 점이라면...
				{
					MessageBox(NULL,"잘못된 에디트 방식입니다. ","Warning",MB_OK);
				}
				else
				*/
				if( stColface.GetContinueLine() == FALSE )
				{
					int result = MessageBox(NULL,"라인을 계속 이을까요?","Select",MB_OKCANCEL);
					if( IDOK == result )	//닫을 경우.
					{
						stColface.SetBeforeId(v_id);
						stColface.SetContinueLine(TRUE);
					}
				}
				else
				{
					//if( v_id )

					int result = MessageBox(NULL,"라인을 잇거나 그룹을 닫을까요?","Select",MB_OKCANCEL);
					if( IDOK == result )	//닫을 경우.
					{
						stColface.CreateLine(v_id,stColface.GetContinueLine());
						stColface.SetContinueLine(FALSE);
						stColface.AdjustFace();
					}
				}
			}
			if( v_id == 0)	//새로 생성.
			{

				v_id = stColface.CreateVertex(pos);
				if( v_id == 0 )	//넘쳐서 에디트 불가.
				{
					MessageBox(NULL,"버텍스가 65536개를 넘었습니다. 에디트가 안됩니다.","",MB_OK);
				}
				else
				{
					stColface.CreateLine(v_id,stColface.GetContinueLine());
					stColface.SetContinueLine(TRUE);
				}
			}
		}
	}
	CheckContinueLine(stColface.GetContinueLine());
	if( gAsci[VK_DELETE] )	//지울때
	{
		gAsci[VK_DELETE]=0;
		DeleteSeleted();
	}
}

static BOOL stOneClick=FALSE;
static LONG stOldMx,stOldMy;
BOOL stNewNormal=FALSE;

void ProgressSelectEdit(void *v_level)
{
	Vector3f pos;
	int mode;

	if( v_level == NULL )
		return;
	LONG mouse_x = GetRectMouseX(gMouse.x);	//해상도에맞춰서..
	LONG mouse_y = GetRectMouseY(gMouse.y);	//해상도에 맞춰서..

	CLevel *level = (CLevel *)v_level;
	CBsp *bsp = level->mBsp;
	if (gMouse.button & _LEFT_BUTTON_DOWN)	//에딧 모드
	{           
		if( stOneClick == FALSE )
		{
			stOneClick=TRUE;
			stOldMx = mouse_x;
			stOldMy = mouse_y;
			mode = GetSelectMode();
			WORD v_id;
			int f_id;
			switch(mode)
			{
				case 0:
					level->GetPointFromScreenRay(mouse_x,mouse_y,&pos);
					v_id=stColface.IsExistPoint(pos);	//인근에 점이 있다면.
					if( v_id )	//인접된 점이 있다.
					{
						//stColface.GetVertex(&pos,v_id);
						stColface.SetSelectVertex(v_id);	//버택스는 오직 하나만 선택가능 
					}
					break;
				case 1:
				case 2: 
					f_id = stColface.GetCrossFace(mouse_x,mouse_y);
					if( f_id != -1 )
					{
						if( stUnFreezeMode )	//언프리즈 모드일때 
						{
							stColface.UnFreezeFace(f_id);
							if( mode == 2 )	//그룹일경우는 선택을 더해주자.
							{
								if( !stColface.UnFreezeFaceGroup(f_id) )
								{	//페이스링크가 꼬였다.
									//해결 함수.
									stColface.AdjustFixFaceLink();			//링크안된 면을 고친다.
								}
							}
						}
						else
						{
							stColface.SelectFace(f_id);
							if( mode == 2 )	//그룹일경우는 선택을 더해주자.
							{
								if( !stColface.SelectFaceGroup(f_id) )
								{	//페이스링크가 꼬였다.
									//해결 함수.
									stColface.AdjustFixFaceLink();			//링크안된 면을 고친다.
								}
							}
							else	//페이스일경우 페이스 높이를 구한다.
							{
								SetHeight((int)stColface.GetFaceHeght(f_id));
								SetEditHeight((int)stColface.GetFaceHeght(f_id));
							}
						}
					}
					break;
			}
		}
	}
	else
	{
		if( stOneClick )	//마우스 버튼이 올려질때다.
		{
			if( stNewNormal )	//노멀을 다시구한다.
			{
				stNewNormal=FALSE;
				stColface.MakeFnormal();	//현재 셋팅되있는 페이스노멀을 구한다.
				stColface.MakeVnormal();	//현재 셋팅되있는 페이스들에서 버텍스노멀을 구한다.
			}
		}
		stOneClick=FALSE;
	}
	
	if( stOneClick )
	{	//이동 좌표축이 하나라도 켜지는 경우.
		mode = GetSelectMode();
		if( mode == 0 ) //버텍스 일경우.
		{
			stColface.MoveSelectVertex(GetYButton(),GetXZButton(),mouse_x-stOldMx
				,mouse_y-stOldMy);
			stNewNormal=TRUE;
		}
		else
		{
			stColface.MoveSelectFace(GetYButton(),GetXZButton(),mouse_x-stOldMx
				,mouse_y-stOldMy);
			stNewNormal=TRUE;
		}
		stOldMx = mouse_x;
		stOldMy = mouse_y;
	}
	if( gAsci[VK_DELETE] )	//지울때
	{
		gAsci[VK_DELETE]=0;
		DeleteSeleted();
	}
}
void DrawCollisionPoly()
{
	stColface.DrawCollisionPoly();
}
void DrawCollisionPoint()
{
	stColface.DrawCollisionPoint();
}

void LoadCFFile()
{
	OPENFILENAME ofn; // zero the ofn members out
	memset(&ofn,0,sizeof(OPENFILENAME));
	char name[256]=".\\개발관련\\ToolBin\\CollisionMap\\test.cf";

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = name;
	ofn.nMaxFile = sizeof(name);
	ofn.lpstrFilter = "R3 CF file (*.CF)\0*.CF\0All (*.*)\0*.*\0";
	ofn.lpstrTitle = "Select CF(collsion face) file";
	ofn.Flags = OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (!GetOpenFileName(&ofn))
	{
		return;
	}
	stColface.LoadCF(name);	
	stColface.MakeVnormal();	//현재 셋팅되있는 페이스들에서 버텍스노멀을 구한다.
	memset(gAsci,0,256);
}
void SaveCFFile()
{
	OPENFILENAME ofn; // zero the ofn members out
	memset(&ofn,0,sizeof(OPENFILENAME));
	char name[256]=".\\개발관련\\ToolBin\\CollisionMap\\test.cf";

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = name;
	ofn.nMaxFile = sizeof(name);
	ofn.lpstrFilter = "R3 CF file (*.CF)\0*.CF\0All (*.*)\0*.*\0";
	ofn.lpstrTitle = "Select CF(collsion face) file";
	ofn.Flags = OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (!GetSaveFileName(&ofn))
	{
		return;
	}
	stColface.SaveCF(name);	
	memset(gAsci,0,256);
}

int GetColLineNum()
{
	return stColface.GetColLineCnt();
}



void LoadColFaceFromBSP(CBsp *bsp)
{
	stColface.LoadColFaceFromBSP(bsp);
	stColface.MakeVnormal();	//현재 셋팅되있는 페이스들에서 버텍스노멀을 구한다.
}

CToolCollisionFace *GetColface()
{
	return &stColface;
}

/////////////////////////////////////////////////////////////////////////////
// CCollEdit message handlers

BOOL CCollEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CCollEditPtr = this;
	CheckDlgButton(IDC_RADIO3,TRUE);	//디폴트를 테스트로..
	SetConsoleStateB(OP_FLY,FALSE);
	m_Select.AddString("Vertex");
	m_Select.AddString("Face");
	m_Select.AddString("Group");
	m_Select.SetCurSel(0);
	m_Select.ModifyStyle(WS_TABSTOP, 0);
	m_Select.EnableWindow(FALSE);
//	m_Select.ModifyStyle(0, WS_TABSTOP);
//	m_Select.EnableWindow(TRUE);
	m_Delete.ModifyStyle(WS_TABSTOP, 0);//delete를 disable 해놓는다.
	m_Delete.EnableWindow(FALSE);


	m_Height.SetWindowText("60");
	SetHeight(60);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCollEdit::OnRadio1() //에딧모드
{
	// TODO: Add your control notification handler code here
	SetCollisionMode(_COLLISON_EDIT_MODE);
	stColface.UnSelectAll();	//선택을 끈다.
	m_Select.ModifyStyle(WS_TABSTOP, 0);	//선택을 disable 해놓는다.
	m_Select.EnableWindow(FALSE);
	m_Delete.ModifyStyle(0,WS_TABSTOP);//delete를 enable 해놓는다.
	m_Delete.EnableWindow(TRUE);

	SetConsoleStateB(OP_FLY,TRUE);
}

void CCollEdit::OnRadio2() //선텍 모드
{
	// TODO: Add your control notification handler code here
	SetCollisionMode(_COLLISON_SELECT_MODE);
	m_Select.ModifyStyle(0, WS_TABSTOP);	//선택을 enable 해놓는다.
	m_Select.EnableWindow(TRUE);

	m_Delete.ModifyStyle(0,WS_TABSTOP);//delete를 enable 해놓는다.
	m_Delete.EnableWindow(TRUE);

	SetSelectMode(m_Select.GetCurSel());
	SetConsoleStateB(OP_FLY,TRUE);
}

void CCollEdit::OnRadio3()		//테스트 모드
{
	// TODO: Add your control notification handler code here
	stColface.UnSelectAll();	//선택을 끈다.

	SetCollisionMode(_COLLISON_TEST_MODE);
	m_Select.ModifyStyle(WS_TABSTOP, 0);//선택을 disable 해놓는다.
	m_Select.EnableWindow(FALSE);

	m_Delete.ModifyStyle(WS_TABSTOP, 0);//delete를 disable 해놓는다.
	m_Delete.EnableWindow(FALSE);

	stColface.MakeVnormal();	//현재 셋팅되있는 페이스들에서 버텍스노멀을 구한다.
	if (GetConsoleStateB(OP_FLY))
		SetConsoleStateB(OP_FLY,FALSE);
}

void CCollEdit::OnSelchangeCombo1() //버텍스,페이스,그룹
{
	// TODO: Add your control notification handler code here
	SetSelectMode(m_Select.GetCurSel());
}

void CCollEdit::OnButton3() //삭제.
{
	// TODO: Add your control notification handler code here

	DeleteSeleted();
}

void CCollEdit::OnButton4()	//선택 전부
{
	// TODO: Add your control notification handler code here
	if( WhatCollisionMode() == _COLLISON_SELECT_MODE )
		stColface.SelectAll();
}

void CCollEdit::OnButton5() //안선택 전부
{
	// TODO: Add your control notification handler code here

	stColface.UnSelectAll();
}

void CCollEdit::OnCheck2()	//Continue 라인 
{
	// TODO: Add your control notification handler code here
	DWORD result=m_ContinueLine.GetCheck();
	if( result == 1 )
	{
		m_ContinueLine.SetCheck(FALSE);
		return;
	}
	stColface.SetContinueLine(result);
}


void CCollEdit::OnChangeEdit1()		//숫자 에디트..
{
	char buf[256];
	m_Height.GetWindowText(buf,256);

	int i,s;
	for(i=0; i<(int)strlen(buf); i++)
	{
		if( buf[i] < '0' || buf[i] > '9' )
		{
			buf[i]=NULL;
			m_Height.SetWindowText(buf);
		}
	}
	s= atoi(buf);
	SetHeight(s);
	stColface.AdjustSelectFaceHeight();
//	memset(gAsci,0,256);
}
void CCollEdit::OnOK()	//esc와 enter키를 흡수한다.
{
	// TODO: Add extra validation here

//	CDialog::OnOK();
}

void CCollEdit::OnCancel() 
{
	// TODO: Add extra cleanup here
	
//	CDialog::OnCancel();
}

void CCollEdit::OnButton1() //프리즈
{
	// TODO: Add your control notification handler code here
	stColface.FreezeFromSelect();
}

void CCollEdit::OnCheck26() //언프리즈
{
	// TODO: Add your control notification handler code here
	stUnFreezeMode = !stUnFreezeMode;	
}

void CCollEdit::OnButton2() //언프리즈 올
{
	// TODO: Add your control notification handler code here
	
	stColface.UnFreezeAll();
}
