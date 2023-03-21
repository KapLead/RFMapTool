// TabParticle.cpp : implementation file
//

#include "stdafx.h"
#include "rfmaptool.h"
#include "r3input.h"
#include "r3math.h"
#include "TabParticle.h"
#include "R3Particle.h"
#include "R3render.h"
#include "renderParticle.h"
#include "ToolManager.h"
#include "core.h"
#include "console.h"
#include "JError.h"
#include "CommonUtil.h"
#include "R3Particle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CParticle stParticle;	// ��ƼŬ �����ͻ󿡼��ߴ� �ϳ��� ��ƼŬ.
static CEntity *stEntity=NULL;
/*
const COLORREF LTGRAY = RGB(192, 192, 192);
const COLORREF YELLOW = RGB(255, 255, 0);
const COLORREF DKYELLOW = RGB(128, 128, 0);
const COLORREF RED = RGB(255, 0, 0);
const COLORREF DKRED = RGB(128, 0, 0);
static int stNowTrack=0;
*/
/////////////////////////////////////////////////////////////////////////////
// CParticleProperty dialog


CParticleProperty::CParticleProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CParticleProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CParticleProperty)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
void CParticleProperty::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if( stParticle.mFlag & _PARTICLE_ATTR_ZDISABLE )
		CheckDlgButton(IDC_CHECK1,TRUE);	//Ų��.
	else
		CheckDlgButton(IDC_CHECK1,FALSE);	//����.

	if( stParticle.mFlag & _PARTICLE_ATTR_ZWRITE )
		CheckDlgButton(IDC_CHECK2,TRUE);	//Ų��.
	else
		CheckDlgButton(IDC_CHECK2,FALSE);	//����.

	if( stParticle.mFlag & _PARTICLE_ATTR_NO_BILLBOARD )
		CheckDlgButton(IDC_CHECK3,TRUE);	//Ų��.
	else
		CheckDlgButton(IDC_CHECK3,FALSE);	//����.

	if( stParticle.mFlag & _PARTICLE_ATTR_COLLISION )
		CheckDlgButton(IDC_CHECK6,TRUE);	//Ų��.
	else
		CheckDlgButton(IDC_CHECK6,FALSE);	//����.

	if( stParticle.mFlag & _PARTICLE_ATTR_NO_LOOP )
		CheckDlgButton(IDC_CHECK5,TRUE);	//Ų��.
	else
		CheckDlgButton(IDC_CHECK5,FALSE);	//����.

	if( stParticle.mFlag & _PARTICLE_ATTR_ALWAYS_LIVE )
		CheckDlgButton(IDC_CHECK8,TRUE);	//Ų��.
	else
		CheckDlgButton(IDC_CHECK8,FALSE);	//����.

	if( stParticle.mFlag & _PARTICLE_ATTR_ENTITY_ANI )
		CheckDlgButton(IDC_CHECK9,TRUE);	//Ų��.
	else
		CheckDlgButton(IDC_CHECK9,FALSE);	//����.
//--���� Ÿ��.
	if( stParticle.mAlphaType == _BLEND_NONE )
		CheckDlgButton(IDC_CHECK4,TRUE);	//Ų��.
	else
		CheckDlgButton(IDC_CHECK4,FALSE);	//����.

	if( stParticle.mAlphaType == _BLEND_DEFAULT )
		CheckDlgButton(IDC_CHECK10,TRUE);	//Ų��.
	else
		CheckDlgButton(IDC_CHECK10,FALSE);	//����.

	if( stParticle.mAlphaType == _BLEND_BRIGHT )
		CheckDlgButton(IDC_CHECK14,TRUE);	//Ų��.
	else
		CheckDlgButton(IDC_CHECK14,FALSE);	//����.

	if( stParticle.mAlphaType == _BLEND_INV_DEFAULT )
		CheckDlgButton(IDC_CHECK15,TRUE);	//Ų��.
	else
		CheckDlgButton(IDC_CHECK15,FALSE);	//����.

	if( stParticle.mAlphaType == _BLEND_BACK_BRIGHT )
		CheckDlgButton(IDC_CHECK16,TRUE);	//Ų��.
	else
		CheckDlgButton(IDC_CHECK16,FALSE);	//����.

	if( stParticle.mAlphaType == _BLEND_LIGHTMAP )
		CheckDlgButton(IDC_CHECK17,TRUE);	//Ų��.
	else
		CheckDlgButton(IDC_CHECK17,FALSE);	//����.

	if( stParticle.mAlphaType == _BLEND_INV_LIGHTMAP )
		CheckDlgButton(IDC_CHECK18,TRUE);	//Ų��.
	else
		CheckDlgButton(IDC_CHECK18,FALSE);	//����.
}


void CParticleProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParticleProperty)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParticleProperty, CDialog)
	//{{AFX_MSG_MAP(CParticleProperty)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_CHECK6, OnCheck6)
	ON_BN_CLICKED(IDC_CHECK5, OnCheck5)
	ON_BN_CLICKED(IDC_CHECK8, OnCheck8)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	ON_BN_CLICKED(IDC_CHECK10, OnCheck10)
	ON_BN_CLICKED(IDC_CHECK14, OnCheck14)
	ON_BN_CLICKED(IDC_CHECK15, OnCheck15)
	ON_BN_CLICKED(IDC_CHECK16, OnCheck16)
	ON_BN_CLICKED(IDC_CHECK17, OnCheck17)
	ON_BN_CLICKED(IDC_CHECK18, OnCheck18)
	ON_BN_CLICKED(IDC_CHECK9, OnCheck9)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CHECK25, OnCheck25)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParticleProperty message handlers

void CParticleProperty::OnCheck1() //z sorting
{
	// TODO: Add your control notification handler code here

	if( IsDlgButtonChecked(IDC_CHECK1) )
		stParticle.mFlag |= _PARTICLE_ATTR_ZDISABLE;
	else
		stParticle.mFlag &= ~_PARTICLE_ATTR_ZDISABLE;
}

void CParticleProperty::OnCheck2() //z write
{
	// TODO: Add your control notification handler code here
		
	if( IsDlgButtonChecked(IDC_CHECK2) )
		stParticle.mFlag |= _PARTICLE_ATTR_ZWRITE;
	else
		stParticle.mFlag &= ~_PARTICLE_ATTR_ZWRITE;
}

void CParticleProperty::OnCheck3() //NO_BILLBOARD
{
	// TODO: Add your control notification handler code here
	
	if( IsDlgButtonChecked(IDC_CHECK3) )
		stParticle.mFlag |= _PARTICLE_ATTR_NO_BILLBOARD;
	else
		stParticle.mFlag &= ~_PARTICLE_ATTR_NO_BILLBOARD;
}

void CParticleProperty::OnCheck6() //�浹 �˻�.
{
	// TODO: Add your control notification handler code here
	
	if( IsDlgButtonChecked(IDC_CHECK6) )
		stParticle.mFlag |= _PARTICLE_ATTR_COLLISION;
	else
		stParticle.mFlag &= ~_PARTICLE_ATTR_COLLISION;
}

void CParticleProperty::OnCheck5()	//no loop
{
	// TODO: Add your control notification handler code here
	
	if( IsDlgButtonChecked(IDC_CHECK5) )
		stParticle.mFlag |= _PARTICLE_ATTR_NO_LOOP;
	else
		stParticle.mFlag &= ~_PARTICLE_ATTR_NO_LOOP;
}
void CParticleProperty::OnCheck8() //�׻�������
{
	// TODO: Add your control notification handler code here
		
	if( IsDlgButtonChecked(IDC_CHECK8) )
		stParticle.mFlag |= _PARTICLE_ATTR_ALWAYS_LIVE;
	else
		stParticle.mFlag &= ~_PARTICLE_ATTR_ALWAYS_LIVE;
}

void CParticleProperty::OnCheck4() //0�� ����.. ���� �Ⱦ�
{
	stParticle.mAlphaType = _BLEND_NONE;
	CheckDlgButton(IDC_CHECK4,TRUE);	//Ų��.
	CheckDlgButton(IDC_CHECK10,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK14,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK15,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK16,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK17,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK18,FALSE);	//�ٸ� �͵��� �� ����.
}
void CParticleProperty::OnCheck10() //2�� ����..
{
	stParticle.mAlphaType = _BLEND_DEFAULT;
	CheckDlgButton(IDC_CHECK10,TRUE);	//Ų��.
	CheckDlgButton(IDC_CHECK4,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK14,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK15,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK16,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK17,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK18,FALSE);	//�ٸ� �͵��� �� ����.
}
void CParticleProperty::OnCheck14() //3�� ����..
{
	stParticle.mAlphaType = _BLEND_BRIGHT;
	CheckDlgButton(IDC_CHECK14,TRUE);	//Ų��.
	CheckDlgButton(IDC_CHECK4,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK10,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK15,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK16,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK17,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK18,FALSE);	//�ٸ� �͵��� �� ����.
}
void CParticleProperty::OnCheck15() //5�� ����..
{
	stParticle.mAlphaType = _BLEND_INV_DEFAULT;
	CheckDlgButton(IDC_CHECK15,TRUE);	//Ų��.
	CheckDlgButton(IDC_CHECK4,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK14,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK10,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK16,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK17,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK18,FALSE);	//�ٸ� �͵��� �� ����.
}
void CParticleProperty::OnCheck16() //8�� ����..
{
	stParticle.mAlphaType = _BLEND_BACK_BRIGHT;
	CheckDlgButton(IDC_CHECK16,TRUE);	//Ų��.
	CheckDlgButton(IDC_CHECK4,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK14,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK15,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK10,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK17,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK18,FALSE);	//�ٸ� �͵��� �� ����.
}
void CParticleProperty::OnCheck17() //10�� ����..
{
	stParticle.mAlphaType = _BLEND_LIGHTMAP;
	CheckDlgButton(IDC_CHECK17,TRUE);	//Ų��.
	CheckDlgButton(IDC_CHECK4,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK14,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK15,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK16,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK10,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK18,FALSE);	//�ٸ� �͵��� �� ����.
}
void CParticleProperty::OnCheck18() //11�� ����..
{
	stParticle.mAlphaType = _BLEND_INV_LIGHTMAP;
	CheckDlgButton(IDC_CHECK18,TRUE);	//Ų��.
	CheckDlgButton(IDC_CHECK4,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK14,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK15,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK16,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK17,FALSE);	//�ٸ� �͵��� �� ����.
	CheckDlgButton(IDC_CHECK10,FALSE);	//�ٸ� �͵��� �� ����.
}

void CParticleProperty::OnCheck9() //��ƼƼ �ִϸ��̼��� ��.
{
	// TODO: Add your control notification handler code here
	
	if( IsDlgButtonChecked(IDC_CHECK9) )
		stParticle.mFlag |= _PARTICLE_ATTR_ENTITY_ANI;
	else
		stParticle.mFlag &= ~_PARTICLE_ATTR_ENTITY_ANI;
}

void CParticleProperty::OnCheck25() //���Ⱘ�� ����.
{
	// TODO: Add your control notification handler code here
	if( IsDlgButtonChecked(IDC_CHECK25) )
		stParticle.mFlag |= _PARTICLE_ATTR_NO_AIR;
	else
		stParticle.mFlag &= ~_PARTICLE_ATTR_NO_AIR;	
}

/////////////////////////////////////////////////////////////////////////////
// CParticleFile dialog

IMPLEMENT_DYNCREATE(CParticleFile, CDialog)

CParticleFile::CParticleFile(CWnd* pParent /*=NULL*/)
	: CDialog(CParticleFile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CParticleFile)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CParticleFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParticleFile)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParticleFile, CDialog)
	//{{AFX_MSG_MAP(CParticleFile)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParticleFile message handlers
/////////////////////////////////////////////////////////////////////////////
// CParticleEdit dialog

IMPLEMENT_DYNCREATE(CParticleEdit, CDialog)

CParticleEdit::CParticleEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CParticleEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CParticleEdit)
	//}}AFX_DATA_INIT
}


void CParticleEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParticleEdit)
	DDX_Control(pDX, IDC_COMBO1, m_ParticleList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParticleEdit, CDialog)
	//{{AFX_MSG_MAP(CParticleEdit)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_CHECK13, OnCheck13)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON_ENTITY_LOAD, OnButtonEntityLoad)
	ON_WM_CLOSE()
	ON_EN_KILLFOCUS(IDC_EDIT6, OnKillfocusEdit6)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CParticleEdit message handlers

/////////////////////////////////////////////////////////////////////////////
// CParticleEdit message handlers

void CParticleEdit::OnKillfocusEdit6() 
{
}


BOOL CParticleEdit::OnInitDialog() 
{
	int i;

	CDialog::OnInitDialog();
	g_pDoc->ParticleEdit=this;
	mTrackFlag = 0;		//0
	mTrackFlag |= (1<<0);	//0���� 5��Ʈ���� ...
	mTrackFlag |= (1<<5);

	//-------- �̸� ���� ���ִ� ��ƼŬ ����Ʈ�� 
	int p_menu_num = GetPreParticleListNum();
	_PRE_PARTICLE_LIST *p_list = GetPreParticleList(0);

	for(i=0; i<p_menu_num; i++)
		m_ParticleList.InsertString(i,p_list[i].name);
	// TODO: Add extra initialization here
	if( p_menu_num )
		m_ParticleList.SetCurSel(0);

	//-------------���� ��ƼŬ ����..
	stParticle.SetPreCalcParticle(_PARTICLE_TYPE_FALL);
	//���� ��ƼŬ �ʱ�ȭ..
	stParticle.mNum=20;	//������ �������ͼ�
	stParticle.InitParticle();

//	DWORD ii=this->GetStyle();
//	BOOL dsasf=ModifyStyle(0, 0x94ca0000);

	ParticleProperty.Create(IDD_PARTICLE_PROPERTY,this);
	ParticleProperty.ShowWindow(SW_HIDE);

	SetDlgItemText(IDC_EDIT_ATTACH_ENTITY,"none");

	CheckDlgButton(IDC_CHECK19,TRUE);	//0�� Ʈ���� Ų��.
	

/*
	//0���� 5��Ʈ���� Ű���� �ִ�.
	VERIFY(m_Track0.Attach(IDC_TRACK_COLOR0, this, RED ));
	VERIFY(m_Track1.Attach(IDC_TRACK_COLOR1, this, LTGRAY ));
	VERIFY(m_Track2.Attach(IDC_TRACK_COLOR2, this, LTGRAY ));
	VERIFY(m_Track3.Attach(IDC_TRACK_COLOR3, this, LTGRAY ));
	VERIFY(m_Track4.Attach(IDC_TRACK_COLOR4, this, LTGRAY));
	VERIFY(m_Track5.Attach(IDC_TRACK_COLOR5, this, RED ));
*/

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


static BOOL stPosCheck=FALSE;
static BOOL stMoveCheck=FALSE;
static BOOL stCreateAreaCheck=FALSE;
static Vector3f stParticleStartPos;


void CParticleEdit::OnCheck1()	// ��ƼŬ ������ ��°�  pos
{
	// TODO: Add your control notification handler code here

	stPosCheck=!stPosCheck;
	if( stPosCheck )	//�ٸ����� ����. 
	{
		CheckDlgButton(IDC_CHECK2,FALSE);
		stMoveCheck=FALSE;
		CheckDlgButton(IDC_CHECK13,FALSE);
		stCreateAreaCheck=FALSE;
	}
}

void CParticleEdit::OnCheck2()	//��ƼŬ ������ �̵�... move
{
	// TODO: Add your control notification handler code here
	
	stMoveCheck=!stMoveCheck;
	if( stMoveCheck )	//�ٸ����� ����. 
	{
		CheckDlgButton(IDC_CHECK1,FALSE);
		stPosCheck=FALSE;
		CheckDlgButton(IDC_CHECK13,FALSE);
		stCreateAreaCheck=FALSE;
	}
}
void CParticleEdit::OnCheck13() //���۹���
{
	// TODO: Add your control notification handler code here
	stCreateAreaCheck =! stCreateAreaCheck;
	if( stCreateAreaCheck )	//�ٸ����� ����. 
	{
		CheckDlgButton(IDC_CHECK1,FALSE);
		stPosCheck=FALSE;
		CheckDlgButton(IDC_CHECK2,FALSE);
		stMoveCheck=FALSE;
	}
}

//����� �ִ� ��ƼŬ ��ġ�� �ű��.
static void MoveCreatedParticle(BOOL y_axis,BOOL xz_axis,LONG x_diff,LONG y_diff )
{
#define _SXSY_TO_DIST	0.2f
	float y=(float)y_diff*_SXSY_TO_DIST;
	float x=(float)x_diff*_SXSY_TO_DIST;

	if( x == 0 && y == 0 )
		return;

	if( y_axis )
	{
		stParticleStartPos[1]-=y;
	}
	else
	if( xz_axis )
	{
		Vector3f src,tar,temp;
		_R3ENGINE_STATE *state = GetR3State();
		temp[0] = 0;
		temp[1] = 0;
		temp[2] = 100;

		tar[0]=state->mMatView.m[0][2]*temp[2];
		tar[1]=state->mMatView.m[1][2]*temp[2];
		tar[2]=state->mMatView.m[2][2]*temp[2];

		src[0]=0;
		src[1]=0;
		src[2]=0;
		D3DXMATRIX billboard,temp_b;
//		GetYBillboardMatrix(&billboard,src,tar);//	������
		GetYBillboardMatrix(&temp_b,src,tar);//	������
		D3DXMatrixInverse( &billboard, NULL, &temp_b );

		stParticleStartPos[0]+=(billboard.m[0][0]*x+billboard.m[0][2]*-y);
		stParticleStartPos[2]+=(billboard.m[2][0]*x+billboard.m[2][2]*-y);
	}
}
static BOOL stOneClick=FALSE;
static LONG stOldMx,stOldMy;

static CLevel *stLevel;
static CBsp *stBsp;

void ProgressParticleEdit(void *v_level)
{
	Vector3f pos;

	if( v_level == NULL )
		return;

	LONG mouse_x = GetRectMouseX(gMouse.x);	//�ػ󵵿����缭..
	LONG mouse_y = GetRectMouseY(gMouse.y);	//�ػ󵵿� ���缭..

	CLevel *level = (CLevel *)v_level;
	CBsp *bsp = level->mBsp;

	stLevel=level;
	stBsp=bsp;


	int what_mode = WhatEntityMode();

	if( !level->IsLoadedBsp() )
		return;

	if (gMouse.button & _LEFT_BUTTON_DOWN)	//���� ���
	{
//		gMouse.button &= (~_LEFT_BUTTON_DOWN);
		if( stOneClick == FALSE )
		{
			stOneClick=TRUE;
			stOldMx = mouse_x;
			stOldMy = mouse_y;
		}

		if( stPosCheck )	//	��ġ�� ���� ���...
		{
			BOOL is_coll;
			if( GetPickingStyle() == _PICKING_STYLE_FAR )
				is_coll=level->GetPointFromScreenRayFar(mouse_x,mouse_y,&pos);//���콺 �������� ����.
			else
				is_coll=level->GetPointFromScreenRay(mouse_x,mouse_y,&pos);//���콺 �������� ����.

			if( is_coll )
			{
				Vector3fCopy(pos,stParticleStartPos);
			}
		}
	}
	else
	{
		stOneClick=FALSE;
	}

	if( stOneClick )
	{	//�̵� ��ǥ���� �ϳ��� ������ ���.
		if( stMoveCheck )
		{
			MoveCreatedParticle(GetYButton(),GetXZButton(),mouse_x-stOldMx
				,mouse_y-stOldMy);
		}
		else
		if( stCreateAreaCheck )	//������ ũ�� �ٲܶ�...
		{
			if( GetYButton() )
			{
				/*
				stParticle.mCreateArea[1]+=mouse_y-stOldMy;
				if( stParticle.mCreateArea[1] < 0 )
					stParticle.mCreateArea[1] = 0;
					*/
			}
			else
			if( GetXZButton() )
			{
				/*
				stParticle.mCreateArea[0]+=mouse_x-stOldMx;
				stParticle.mCreateArea[2]+=mouse_y-stOldMy;
				if( stParticle.mCreateArea[0] < 0 )
					stParticle.mCreateArea[0] = 0;
				if( stParticle.mCreateArea[2] < 0 )
					stParticle.mCreateArea[2] = 0;
					*/
			}
		}
		stOldMx = mouse_x;
		stOldMy = mouse_y;
	}
}


static BOOL stIsParticleStart=FALSE;
void CParticleEdit::OnCheck3()	//��ƼŬ ���۶Ǵ� ����...
{
	// TODO: Add your control notification handler code here
	
	stIsParticleStart=!stIsParticleStart;

	if( stIsParticleStart )
	{
		stParticle.SetParticleState(_PARTICLE_STATE_START);
	}
	else
		stParticle.SetParticleState(_PARTICLE_STATE_STOP);
}

void DrawCreatedParticle()	//��ƼŬ�޴����� ���۵Ǵ� ��ƼŬ...
{
	if( GetTabMode() != _PARTICLE_MODE )
		return ;

	stParticle.SetBspPtr(stBsp);
	stParticle.SetCreatePos(stParticleStartPos);	//�浹 �Ҷ� �ʿ��ϴ�...
	stParticle.Loop();


	//---��ƼŬ �������� �������..
/*
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity( &matWorld );
	MatrixCopy(stParticle.mRotMat,matWorld.m);
	matWorld.m[3][0] = stParticle.mStartPos[0][0];
	matWorld.m[3][1] = stParticle.mStartPos[0][1];
	matWorld.m[3][2] = stParticle.mStartPos[0][2];
	GetD3dDevice()->SetTransform( D3DTS_WORLD, &matWorld );
*/

	D3DXMATRIX matWorld;
	MatrixIdentity(matWorld.m);
	MatrixCopy(stParticle.mRotMat,matWorld.m);
	GetD3dDevice()->SetTransform( D3DTS_WORLD, &matWorld );
//	MatrixMultiply(w_matrix,s_matrix,w_matrix);

	matWorld.m[3][0] = stParticleStartPos[0];
	matWorld.m[3][1] = stParticleStartPos[1];
	matWorld.m[3][2] = stParticleStartPos[2];


	DrawCParticle(&stParticle,stParticle.mEntity,matWorld.m);	//�̸� ���ؽ����ۿ� �����ؾߵȴ�.
	Vector3f box={1,1,1};

	MatrixIdentity(matWorld.m);
	matWorld.m[3][0] = stParticleStartPos[0];
	matWorld.m[3][1] = stParticleStartPos[1];
	matWorld.m[3][2] = stParticleStartPos[2];
	GetD3dDevice()->SetTransform( D3DTS_WORLD, &matWorld );
	box[0]+=(float)sin(R3GetTime()*5)/4.0f;
	box[1]+=(float)sin(R3GetTime()*5)/4.0f;
	box[2]+=(float)sin(R3GetTime()*5)/4.0f;
extern CLevel *stLevel;	//���� Ŭ���� 
	stLevel->DrawTestBox(box,stParticleStartPos,0xf0ff1f10);

	D3DXMatrixIdentity( &matWorld );
	GetD3dDevice()->SetTransform( D3DTS_WORLD, &matWorld );


	//��ƼŬ ���� �ٿ�� �ڽ� �Ǵ� ��..
	int cnt=0;
	_D3DR3VERTEX_TEX0 vertex[1000];
	DWORD tc;
	Vector3f bb_min;
	Vector3f bb_max;

	bb_min[0] = stParticle.mStartPos[0][0]+stParticleStartPos[0];
	bb_min[1] = stParticle.mStartPos[0][1]+stParticleStartPos[1];
	bb_min[2] = stParticle.mStartPos[0][2]+stParticleStartPos[2];

	bb_max[0] = stParticle.mStartPos[1][0]+stParticleStartPos[0];
	bb_max[1] = stParticle.mStartPos[1][1]+stParticleStartPos[1];
	bb_max[2] = stParticle.mStartPos[1][2]+stParticleStartPos[2];

    D3DXMatrixIdentity( &matWorld );
    GetD3dDevice()->SetTransform( D3DTS_WORLD, &matWorld );
    GetD3dDevice()->SetTexture( 0,NULL);

	tc=(0x80004fff);

#define LineSet(xx,yy,zz,cc) {	vertex[cnt].x = xx;	vertex[cnt].y = yy;	vertex[cnt].z = zz;		vertex[cnt].color = cc; cnt++;};

	cnt=0;
	LineSet(bb_min[0],bb_min[1],bb_min[2],tc);
	LineSet(bb_max[0],bb_min[1],bb_min[2],tc);
	LineSet(bb_min[0],bb_min[1],bb_min[2],tc);
	LineSet(bb_min[0],bb_min[1],bb_max[2],tc);
	LineSet(bb_max[0],bb_min[1],bb_min[2],tc);
	LineSet(bb_max[0],bb_min[1],bb_max[2],tc);
	LineSet(bb_min[0],bb_min[1],bb_max[2],tc);
	LineSet(bb_max[0],bb_min[1],bb_max[2],tc);

	LineSet(bb_min[0],bb_max[1],bb_min[2],tc);
	LineSet(bb_max[0],bb_max[1],bb_min[2],tc);
	LineSet(bb_min[0],bb_max[1],bb_min[2],tc);
	LineSet(bb_min[0],bb_max[1],bb_max[2],tc);
	LineSet(bb_max[0],bb_max[1],bb_min[2],tc);
	LineSet(bb_max[0],bb_max[1],bb_max[2],tc);
	LineSet(bb_min[0],bb_max[1],bb_max[2],tc);
	LineSet(bb_max[0],bb_max[1],bb_max[2],tc);

	LineSet(bb_min[0],bb_min[1],bb_min[2],tc);
	LineSet(bb_min[0],bb_max[1],bb_min[2],tc);
	LineSet(bb_max[0],bb_min[1],bb_min[2],tc);
	LineSet(bb_max[0],bb_max[1],bb_min[2],tc);
	LineSet(bb_min[0],bb_min[1],bb_max[2],tc);
	LineSet(bb_min[0],bb_max[1],bb_max[2],tc);
	LineSet(bb_max[0],bb_min[1],bb_max[2],tc);
	LineSet(bb_max[0],bb_max[1],bb_max[2],tc);

	if( cnt )
	{
		GetD3dDevice()->SetTexture( 0, NULL );
		DrawPrimitiveUPLine(cnt/2,vertex);
	}
}








//-----------���� ������ �ٲ�ġ��� ��...
void CParticleEdit::OnButton3()		//��ƼŬ ������Ƽ..
{
	// TODO: Add your control notification handler code here
	
	ParticleProperty.ShowWindow(SW_SHOWNORMAL);
}

//------ ��ƼŬ ���� xyz

void CParticleEdit::OnSelchangeCombo1() 
{
	int id = m_ParticleList.GetCurSel();
	// TODO: Add your control notification handler code here
	if( id )	//����ڸ�尡 �ƴѰ��.
	{
		stParticle.SetParticleAttr(id);	//��ƼŬ ������ �� ����.

		UpdateData(FALSE);	//�������� ���ڷ� ������.
	}
}
/*
//�����̵� ���� ����...
void CParticleEdit::OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	float alpha=m_AlphaGradient.GetPos()/
		(float)(m_AlphaGradient.GetRangeMax()-m_AlphaGradient.GetRangeMin());

	stParticle.mAlphaGradient = alpha;//0-1���� �ۿ� �ȳ��´�.
	*pResult = 0;
}

void CParticleEdit::FixIntersect(int id)//���� Ʈ���� �����ؼ� ���̰��� ���Ѵ�.
{
	int i,j;

	float a_slope;
	float r_slope;
	float g_slope;
	float b_slope;
	float s_slope;
	if( id != 0 )
	{
		for( i=id-1; i>=0; i-- )	//�������� ����
		{
			if( mTrackFlag & (1<<i) )
				break;
		}
		a_slope = ((float)stParticle.mATrack[i]-(float)stParticle.mATrack[id])/(float)(id-i);
		r_slope = ((float)stParticle.mRTrack[i]-(float)stParticle.mRTrack[id])/(float)(id-i);
		g_slope = ((float)stParticle.mGTrack[i]-(float)stParticle.mGTrack[id])/(float)(id-i);
		b_slope = ((float)stParticle.mBTrack[i]-(float)stParticle.mBTrack[id])/(float)(id-i);
		s_slope = ((float)stParticle.mScaleTrack[i]-(float)stParticle.mScaleTrack[id])/(float)(id-i);
		for( j=id-1; j>0; j-- )	//�������� ����
		{
			if( mTrackFlag & (1<<j) )
				break;
			stParticle.mATrack[j]=(BYTE)((id-j)*a_slope+(float)stParticle.mATrack[id]);
			stParticle.mRTrack[j]=(BYTE)((id-j)*r_slope+(float)stParticle.mRTrack[id]);
			stParticle.mGTrack[j]=(BYTE)((id-j)*g_slope+(float)stParticle.mGTrack[id]);
			stParticle.mBTrack[j]=(BYTE)((id-j)*b_slope+(float)stParticle.mBTrack[id]);
			stParticle.mScaleTrack[j]=((id-j)*s_slope+(float)stParticle.mScaleTrack[id]);
		}
	}

	if( id != 5 )
	{
		for( i=id+1; i<6; i++ )	//�������� ����
		{
			if( mTrackFlag & (1<<i) )
				break;
		}
		a_slope = ((float)stParticle.mATrack[i]-(float)stParticle.mATrack[id])/(float)(i-id);
		r_slope = ((float)stParticle.mRTrack[i]-(float)stParticle.mRTrack[id])/(float)(i-id);
		g_slope = ((float)stParticle.mGTrack[i]-(float)stParticle.mGTrack[id])/(float)(i-id);
		b_slope = ((float)stParticle.mBTrack[i]-(float)stParticle.mBTrack[id])/(float)(i-id);
		s_slope = ((float)stParticle.mScaleTrack[i]-(float)stParticle.mScaleTrack[id])/(float)(i-id);
		for( j=id+1; j<6-1; j++ )	//�������� ����
		{
			if( mTrackFlag & (1<<j) )
				break;
			stParticle.mATrack[j]=(BYTE)((j-id)*a_slope+(float)stParticle.mATrack[id]);
			stParticle.mRTrack[j]=(BYTE)((j-id)*r_slope+(float)stParticle.mRTrack[id]);
			stParticle.mGTrack[j]=(BYTE)((j-id)*g_slope+(float)stParticle.mGTrack[id]);
			stParticle.mBTrack[j]=(BYTE)((j-id)*b_slope+(float)stParticle.mBTrack[id]);
			stParticle.mScaleTrack[j]=((j-id)*s_slope+(float)stParticle.mScaleTrack[id]);
		}
	}
}
*/

BOOL CParticleEdit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if(pMsg->message==WM_KEYDOWN)	// Ű�������޼����ΰ�?
	{
		if(pMsg->wParam==VK_RETURN)	// �װ� ����Ű�ΰ�?
		{
//			UpDateEditBox();
		}
	}
	
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CParticleEdit::AttachEntity(char *name)	//��ƼƼ Attach
{
	strlwr(name);
	if( stEntity )
	{
		stEntity->ReleaseTexMem();	//�ؽ��� �޸𸮿� ������.
		stEntity->ReleaseEntity();
		delete stEntity;
		stEntity=NULL;
	}
	stEntity = new CEntity;

	char entity_name[256]="";

	for(DWORD i=0; i<strlen(name); i++)
	{
		if( name[i] == 'e' 
		&& name[i+1] == 'n' 
		&& name[i+2] == 't' 
		&& name[i+3] == 'i' 
		&& name[i+4] == 't' 
		&& name[i+5] == 'y' )
			break;
	}
	if( i == strlen(name) )
	{
		Warning(name,"��ƼƼ�� ��ƼƼ ���� ������ �־�� �մϴ�.");
		return;
	}
	strcpy(entity_name,&name[i+7]);

	//�ӽ÷� �޸𸮿� �ø��� �Ẹ��...
	if( !stEntity->LoadEntity(name,_LOAD_POLY_IN_MEMORY) )
	{
		delete stEntity;
		stEntity=NULL;
		return;	//�ε�����.
	}
	//��ƼŬ�� ��ƼƼ�̸��� �����Ѵ�.
	strcpy(stParticle.mEntityName,entity_name);

	for( i=strlen(name); i>0; i--)
	{
		if( name[i] == '\\' )
			break;
	}
	SetDlgItemText(IDC_EDIT_ATTACH_ENTITY,&name[i+1]);
	stEntity->RestoreTexMem();	//�ؽ��� ������..
}

void CParticleEdit::OnButtonEntityLoad()	//��ƼƼ Attach
{
	// TODO: Add your control notification handler code here
	OPENFILENAME ofn; // zero the ofn members out
	memset(&ofn,0,sizeof(OPENFILENAME));
	char name[256]=".\\entity\\test.r3e";

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = name;
	ofn.nMaxFile = sizeof(name);
	ofn.lpstrFilter = "R3 R3E file (*.R3E)\0*.R3E\0All (*.*)\0*.*\0";
	ofn.lpstrTitle = "Select R3E(R3 entity) file";
	ofn.Flags = OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (!GetOpenFileName(&ofn))
	{
		return;
	}
	AttachEntity(name);	//��ƼƼ Attach
}

void CParticleEdit::OnOK()	//esc�� enterŰ�� ����Ѵ�.
{
	int dfdf=0;
//	CDialog::OnOK();
}

void CParticleEdit::OnCancel() 
{
//	CDialog::OnCancel();
}


void CParticleEdit::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
	if( stEntity )
	{
//		stEntity->ReleaseTexMem();	//�ؽ��� �޸𸮿� ������.
		stEntity->ReleaseEntity();
	}
	stParticle.ReleaseParticle();
	
}
void CParticleEdit::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnClose();
}

void CParticleEdit::ChangeTrackValue(int id)
{
	UpdateData(FALSE);
}



static char stNowFullPath[256];

void CParticleFile::OnButton1()		//��ƼŬ ���� 
{
	// TODO: Add your control notification handler code here

	OPENFILENAME ofn; // zero the ofn members out
	memset(&ofn,0,sizeof(OPENFILENAME));
	char filename[256]="";
	char path[256];

	GetPrivateProfileString("Path","ParticlePath",".\\Map\\Entity\\",path, 128,".\\RFMapTool.ini");

//	_R3ENGINE_STATE *state = GetR3State();
//	strcpy(filename,path);
//	strcat(filename,"test.spt");

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = filename;
	ofn.nMaxFile = sizeof(filename);
	ofn.lpstrFilter = "R3 Spt file (*.Spt)\0*.Spt\0All (*.*)\0*.*\0";
	ofn.lpstrTitle = "Select Spt(R3 Script) file";
	ofn.Flags = OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (!GetOpenFileName(&ofn))
		return;

	stParticle.ReleaseEntity();
	stParticle.ReleaseParticle();
	if( !stParticle.LoadParticleSPT(filename) )	//�ε� ����.
		return;
	if( stParticle.mEntityName[0] != NULL )
	{
		SetDlgItemText(IDC_EDIT_ATTACH_ENTITY,stParticle.mEntityName);
	}
	strcpy(stNowFullPath,filename);
	StripPath(filename);
	SetDlgItemText(IDC_EDIT_PARTICLE_NAME,filename);

	stParticle.InitParticle();
}

void CParticleFile::OnButton2()		//��ƼŬ ���ε�.
{
	if( stNowFullPath[0] == NULL )
		return;
	stParticle.ReleaseEntity();
	stParticle.ReleaseParticle();
	stParticle.LoadParticleSPT(stNowFullPath);
	stParticle.InitParticle();
}



