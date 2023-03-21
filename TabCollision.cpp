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


void CCollFile::OnButton1()		//�浹 cf���� �ε�.
{
	// TODO: Add your control notification handler code here
	LoadCFFile();
}

void CCollFile::OnButton2()		//�浹 cf����  ����.
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


//--------------������ʹ� ���� ����Ʈ ����...
#include "ToolCollision.h"
static CToolCollisionFace stColface;

static CCollEdit *CCollEditPtr=NULL; //<- onintDialog���� �־ ����Ѵ�.
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
	if( WhatCollisionMode() == _COLLISON_EDIT_MODE )//��������ϰ��... ���� �������ִ� ������ ����..
	{
		if( stColface.mOnePointCreate )	//������ �߸� ��������.... �װ͸� �����.
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

	LONG mouse_x = GetRectMouseX(gMouse.x);	//�ػ󵵿����缭..
	LONG mouse_y = GetRectMouseY(gMouse.y);	//�ػ󵵿� ���缭..

	if( v_level == NULL )
		return;
	CLevel *level = (CLevel *)v_level;
	CBsp *bsp = level->mBsp;

	if (gMouse.button & _LEFT_BUTTON_DOWN)	//���� ���
	{           
		gMouse.button &= (~_LEFT_BUTTON_DOWN);
		BOOL is_coll;

		if( GetPickingStyle() == _PICKING_STYLE_FAR )
			is_coll=level->GetPointFromScreenRayFar(mouse_x,mouse_y,&pos);//���콺 �������� ����.
		else
			is_coll=level->GetPointFromScreenRay(mouse_x,mouse_y,&pos);//���콺 �������� ����.

		if( GetPickingStyle() == _PICKING_STYLE_BEFORE_POINT && stColface.GetContinueLine() )
		{
			Vector3f bpos;
			stColface.GetVertex(&bpos,stColface.GetBeforeId());
			pos[1]=bpos[1];
		}
		if( is_coll )
		{
			WORD v_id=stColface.IsExistPoint(pos);	//�αٿ� ���� �ִٸ�.
			if( v_id )	//������ ���� �ִ�.
			{
/*
				if( 0 )		//���࿡ ��ġ�� �ȵǴ� ���̶��...
				{
					MessageBox(NULL,"�߸��� ����Ʈ ����Դϴ�. ","Warning",MB_OK);
				}
				else
				*/
				if( stColface.GetContinueLine() == FALSE )
				{
					int result = MessageBox(NULL,"������ ��� �������?","Select",MB_OKCANCEL);
					if( IDOK == result )	//���� ���.
					{
						stColface.SetBeforeId(v_id);
						stColface.SetContinueLine(TRUE);
					}
				}
				else
				{
					//if( v_id )

					int result = MessageBox(NULL,"������ �հų� �׷��� �������?","Select",MB_OKCANCEL);
					if( IDOK == result )	//���� ���.
					{
						stColface.CreateLine(v_id,stColface.GetContinueLine());
						stColface.SetContinueLine(FALSE);
						stColface.AdjustFace();
					}
				}
			}
			if( v_id == 0)	//���� ����.
			{

				v_id = stColface.CreateVertex(pos);
				if( v_id == 0 )	//���ļ� ����Ʈ �Ұ�.
				{
					MessageBox(NULL,"���ؽ��� 65536���� �Ѿ����ϴ�. ����Ʈ�� �ȵ˴ϴ�.","",MB_OK);
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
	if( gAsci[VK_DELETE] )	//���ﶧ
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
	LONG mouse_x = GetRectMouseX(gMouse.x);	//�ػ󵵿����缭..
	LONG mouse_y = GetRectMouseY(gMouse.y);	//�ػ󵵿� ���缭..

	CLevel *level = (CLevel *)v_level;
	CBsp *bsp = level->mBsp;
	if (gMouse.button & _LEFT_BUTTON_DOWN)	//���� ���
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
					v_id=stColface.IsExistPoint(pos);	//�αٿ� ���� �ִٸ�.
					if( v_id )	//������ ���� �ִ�.
					{
						//stColface.GetVertex(&pos,v_id);
						stColface.SetSelectVertex(v_id);	//���ý��� ���� �ϳ��� ���ð��� 
					}
					break;
				case 1:
				case 2: 
					f_id = stColface.GetCrossFace(mouse_x,mouse_y);
					if( f_id != -1 )
					{
						if( stUnFreezeMode )	//�������� ����϶� 
						{
							stColface.UnFreezeFace(f_id);
							if( mode == 2 )	//�׷��ϰ��� ������ ��������.
							{
								if( !stColface.UnFreezeFaceGroup(f_id) )
								{	//���̽���ũ�� ������.
									//�ذ� �Լ�.
									stColface.AdjustFixFaceLink();			//��ũ�ȵ� ���� ��ģ��.
								}
							}
						}
						else
						{
							stColface.SelectFace(f_id);
							if( mode == 2 )	//�׷��ϰ��� ������ ��������.
							{
								if( !stColface.SelectFaceGroup(f_id) )
								{	//���̽���ũ�� ������.
									//�ذ� �Լ�.
									stColface.AdjustFixFaceLink();			//��ũ�ȵ� ���� ��ģ��.
								}
							}
							else	//���̽��ϰ�� ���̽� ���̸� ���Ѵ�.
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
		if( stOneClick )	//���콺 ��ư�� �÷�������.
		{
			if( stNewNormal )	//����� �ٽñ��Ѵ�.
			{
				stNewNormal=FALSE;
				stColface.MakeFnormal();	//���� ���õ��ִ� ���̽������ ���Ѵ�.
				stColface.MakeVnormal();	//���� ���õ��ִ� ���̽��鿡�� ���ؽ������ ���Ѵ�.
			}
		}
		stOneClick=FALSE;
	}
	
	if( stOneClick )
	{	//�̵� ��ǥ���� �ϳ��� ������ ���.
		mode = GetSelectMode();
		if( mode == 0 ) //���ؽ� �ϰ��.
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
	if( gAsci[VK_DELETE] )	//���ﶧ
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
	char name[256]=".\\���߰���\\ToolBin\\CollisionMap\\test.cf";

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
	stColface.MakeVnormal();	//���� ���õ��ִ� ���̽��鿡�� ���ؽ������ ���Ѵ�.
	memset(gAsci,0,256);
}
void SaveCFFile()
{
	OPENFILENAME ofn; // zero the ofn members out
	memset(&ofn,0,sizeof(OPENFILENAME));
	char name[256]=".\\���߰���\\ToolBin\\CollisionMap\\test.cf";

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
	stColface.MakeVnormal();	//���� ���õ��ִ� ���̽��鿡�� ���ؽ������ ���Ѵ�.
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
	CheckDlgButton(IDC_RADIO3,TRUE);	//����Ʈ�� �׽�Ʈ��..
	SetConsoleStateB(OP_FLY,FALSE);
	m_Select.AddString("Vertex");
	m_Select.AddString("Face");
	m_Select.AddString("Group");
	m_Select.SetCurSel(0);
	m_Select.ModifyStyle(WS_TABSTOP, 0);
	m_Select.EnableWindow(FALSE);
//	m_Select.ModifyStyle(0, WS_TABSTOP);
//	m_Select.EnableWindow(TRUE);
	m_Delete.ModifyStyle(WS_TABSTOP, 0);//delete�� disable �س��´�.
	m_Delete.EnableWindow(FALSE);


	m_Height.SetWindowText("60");
	SetHeight(60);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCollEdit::OnRadio1() //�������
{
	// TODO: Add your control notification handler code here
	SetCollisionMode(_COLLISON_EDIT_MODE);
	stColface.UnSelectAll();	//������ ����.
	m_Select.ModifyStyle(WS_TABSTOP, 0);	//������ disable �س��´�.
	m_Select.EnableWindow(FALSE);
	m_Delete.ModifyStyle(0,WS_TABSTOP);//delete�� enable �س��´�.
	m_Delete.EnableWindow(TRUE);

	SetConsoleStateB(OP_FLY,TRUE);
}

void CCollEdit::OnRadio2() //���� ���
{
	// TODO: Add your control notification handler code here
	SetCollisionMode(_COLLISON_SELECT_MODE);
	m_Select.ModifyStyle(0, WS_TABSTOP);	//������ enable �س��´�.
	m_Select.EnableWindow(TRUE);

	m_Delete.ModifyStyle(0,WS_TABSTOP);//delete�� enable �س��´�.
	m_Delete.EnableWindow(TRUE);

	SetSelectMode(m_Select.GetCurSel());
	SetConsoleStateB(OP_FLY,TRUE);
}

void CCollEdit::OnRadio3()		//�׽�Ʈ ���
{
	// TODO: Add your control notification handler code here
	stColface.UnSelectAll();	//������ ����.

	SetCollisionMode(_COLLISON_TEST_MODE);
	m_Select.ModifyStyle(WS_TABSTOP, 0);//������ disable �س��´�.
	m_Select.EnableWindow(FALSE);

	m_Delete.ModifyStyle(WS_TABSTOP, 0);//delete�� disable �س��´�.
	m_Delete.EnableWindow(FALSE);

	stColface.MakeVnormal();	//���� ���õ��ִ� ���̽��鿡�� ���ؽ������ ���Ѵ�.
	if (GetConsoleStateB(OP_FLY))
		SetConsoleStateB(OP_FLY,FALSE);
}

void CCollEdit::OnSelchangeCombo1() //���ؽ�,���̽�,�׷�
{
	// TODO: Add your control notification handler code here
	SetSelectMode(m_Select.GetCurSel());
}

void CCollEdit::OnButton3() //����.
{
	// TODO: Add your control notification handler code here

	DeleteSeleted();
}

void CCollEdit::OnButton4()	//���� ����
{
	// TODO: Add your control notification handler code here
	if( WhatCollisionMode() == _COLLISON_SELECT_MODE )
		stColface.SelectAll();
}

void CCollEdit::OnButton5() //�ȼ��� ����
{
	// TODO: Add your control notification handler code here

	stColface.UnSelectAll();
}

void CCollEdit::OnCheck2()	//Continue ���� 
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


void CCollEdit::OnChangeEdit1()		//���� ����Ʈ..
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
void CCollEdit::OnOK()	//esc�� enterŰ�� ����Ѵ�.
{
	// TODO: Add extra validation here

//	CDialog::OnOK();
}

void CCollEdit::OnCancel() 
{
	// TODO: Add extra cleanup here
	
//	CDialog::OnCancel();
}

void CCollEdit::OnButton1() //������
{
	// TODO: Add your control notification handler code here
	stColface.FreezeFromSelect();
}

void CCollEdit::OnCheck26() //��������
{
	// TODO: Add your control notification handler code here
	stUnFreezeMode = !stUnFreezeMode;	
}

void CCollEdit::OnButton2() //�������� ��
{
	// TODO: Add your control notification handler code here
	
	stColface.UnFreezeAll();
}
