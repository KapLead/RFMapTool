// TabMagic.cpp : implementation file
//

#include "stdafx.h"
#include "rfmaptool.h"
#include "TabMagic.h"

#include "CommonUtil.h"
#include "Jmalloc.h"
#include "console.h"
#include "Jerror.h"
#include "Core.h"

#include "ToolManager.h"

#include "r3input.h"
#include "CMagicScript.h"
#include "EffectCore.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMagicFile dialog

IMPLEMENT_DYNCREATE(CMagicFile, CDialog)

CMagicFile::CMagicFile(CWnd* pParent /*=NULL*/)
	: CDialog(CMagicFile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMagicFile)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMagicFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMagicFile)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMagicFile, CDialog)
	//{{AFX_MSG_MAP(CMagicFile)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static char stMagicName[256];
static DWORD stMagicID=-1;	//현재 보여주는 마법 한개..
static Vector3f stPos;	//마법 시작 포인트 
extern void ForToolUseSetMagicSptName(char *name);	//툴에서만 쓰는 위험한 방법.


static BOOL StripNowPath(char *path)
{
	char buf[256];
	char now_path[256];

	if( !GetNowPath(now_path) )
		return FALSE;
	
	strlwr(path);
	strlwr(now_path);
	for(int i=0; strlen(path); i++)
	{
		if( path[i] == '\\' )
		{
			strcpy(buf,path);
			buf[i]='\\';
			buf[i+1]=NULL;
			if( !strcmp( buf,now_path ) )
			{
				strcpy(buf,path);
				path[0]='.';
				path[1]=NULL;
				strcat(path,&buf[i]);
				return TRUE;
			}
			buf[i]=NULL;
			if( !strcmp( buf,now_path ) )
			{
				strcpy(buf,path);
				path[0]='.';
				path[1]='\\';
				path[2]=NULL;
				strcat(path,&buf[i]);
				return TRUE;
			}
		}
	}
	return FALSE;
}
BOOL StripEffectPath(char *path)
{
	char buf[256];
	char now_path[256];

	_R3ENGINE_STATE *state = GetR3State();
	strcpy(now_path,state->EffectPath);	//이펙트 패스 얻어오기...
	
	strlwr(path);
	strlwr(now_path);
	int leng=strlen(path);
	leng=min(254,leng);
	for(int i=0; leng; i++)
	{
		if( path[i] == '\\' )
		{
			strcpy(buf,path);
			buf[i]='\\';
			buf[i+1]=NULL;
			if( !strcmp( buf,now_path ) )
			{
				strcpy(buf,path);
				path[0]=NULL;
				strcat(path,&buf[i]);
				return TRUE;
			}
			buf[i]=NULL;
			if( !strcmp( buf,now_path ) )
			{
				strcpy(buf,path);
				path[0]='\\';
				path[1]=NULL;
				strcat(path,&buf[i]);
				return TRUE;
			}
		}
	}
	return FALSE;
}

void CMagicFile::OnButton2() //마법열기
{
	OPENFILENAME ofn; // zero the ofn members out
	memset(&ofn,0,sizeof(OPENFILENAME));
	char path[256];
	char temp[256]="";

//	_R3ENGINE_STATE *state = GetR3State();
//	strcpy(path,state->EffectPath);
//	strcpy(temp,path);
//	strcat(temp, "test.spt" );

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = temp;
	ofn.nMaxFile = sizeof(temp);
	ofn.lpstrFilter = "R3 SPT file (*.Spt)\0*.Spt\0All (*.*)\0*.*\0";
	ofn.lpstrTitle = "Select Magic SPT(Magic Script) file";
	ofn.Flags = OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (!GetOpenFileName(&ofn))
	{
		return;
	}

	if( stMagicID!=-1 )	
		DelMagicEffect(stMagicID);//캐쉬상에 있는 마법을 지운다.

	StripNowPath(temp);
	strcpy(path,temp);
	stMagicID = 0;//GetIDFromMagicSpt(path);
	
	SetDlgItemText(IDC_EDIT_MAGIC_NAME,path);
	strcpy(stMagicName,path);
	StripEffectPath(stMagicName);

	DestroyMagicSptList();
	ForToolUseSetMagicSptName(stMagicName);
}

static void CreateEntityList()
{
	char path[256];
	char temp[256];
	char name[256];
//	GetPrivateProfileString("Path","EffectPath",".\\Entity\\Effect\\",path, 128,".\\RFMapTool.ini");
	_R3ENGINE_STATE *state = GetR3State();
	strcpy(path,state->EffectPath);

	GetPrivateProfileString("Path","EntityScriptFile","EffectEntityList.spt",name, 128,".\\RFMapTool.ini");
	strcpy(temp,path);
	strcat(temp,name);
	CreateEffectsList(temp);		//이펙트들 초기화.
}
void CMagicFile::OnButton1() //마법 리로드
{
	if( stMagicName[0] == NULL )
		return;
	DestroyEffectsList();
	CreateEntityList();
	ForToolUseSetMagicSptName(stMagicName);
	DelMagicEffect(stMagicID);//캐쉬상에 있는 마법을 지운다.
	ResetEffectManager();
}

BOOL CMagicFile::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetDlgItemText(IDC_EDIT_MAGIC_NAME,"none");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CMagicFile::OnDestroy() 
{
	CDialog::OnDestroy();
	DestroyMagicSptList();
	
	// TODO: Add your message handler code here
	
}


/////////////////////////////////////////////////////////////////////////////
// CMagicEdit dialog

IMPLEMENT_DYNCREATE(CMagicEdit, CDialog)

CMagicEdit::CMagicEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CMagicEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMagicEdit)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMagicEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMagicEdit)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMagicEdit, CDialog)
	//{{AFX_MSG_MAP(CMagicEdit)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMagicEdit message handlers


static DWORD stMagicUniqueID;
static BOOL stAlwaysLive=FALSE;
static CProcessMagic stProcessMagic;

void CMagicEdit::OnButton1() 
{
	//시작	

	if( stMagicUniqueID )	//현재 유닉크 아이디가 살아 있어서 사라지기 시작하게 만든다.
	{
		SetDisapearTimeMagicEffect(stMagicUniqueID,1);
		return;
	}
	DWORD state;

	if( stAlwaysLive )
		state = _EF_STATE_ALWAYS_LIVE;
	else
		state = _EF_STATE_AUTO_DISAPEAR;
	if( stMagicID != -1 )
	{
		stMagicUniqueID=StartMagicEffect(stPos,0,stMagicID,0,state);
//		stProcessMagic.CreateMagicTrack(".\\Entity\\Effect\\MagicScript_dc.spt");
	}
	if( !stAlwaysLive )
		stMagicUniqueID=0;
}

static BOOL stPosCheck=FALSE;
static BOOL stMoveCheck=FALSE;
static BOOL stOneClick=FALSE;
static LONG stOldMx,stOldMy;

static void MoveMagicPoint(BOOL y_axis,BOOL xz_axis,LONG x_diff,LONG y_diff )
{
#define _SXSY_TO_DIST	0.2f
	float y=(float)y_diff*_SXSY_TO_DIST;
	float x=(float)x_diff*_SXSY_TO_DIST;

	if( x == 0 && y == 0 )
		return;

	if( y_axis )
	{
		stPos[1]-=y;
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
//		GetYBillboardMatrix(&billboard,src,tar);//	빌보드
		GetYBillboardMatrix(&temp_b,src,tar);//	빌보드
		D3DXMatrixInverse( &billboard, NULL, &temp_b );

		stPos[0]+=(billboard.m[0][0]*x+billboard.m[0][2]*-y);
		stPos[2]+=(billboard.m[2][0]*x+billboard.m[2][2]*-y);
	}
}

void DrawMagicPosBox(void *v_level)
{
	CLevel *level = (CLevel *)v_level;
	CBsp *bsp = level->mBsp;

	if( !level->IsLoadedBsp() )
		return;
	Vector3f box={1,1,1};

	box[0]+=(float)sin(R3GetTime()*10)/4.0f;
	box[1]+=(float)sin(R3GetTime()*10)/4.0f;
	box[2]+=(float)sin(R3GetTime()*10)/4.0f;

	level->DrawTestBox(box,stPos,0xffff3f00);
}

void ProgressMagicEdit(void *v_level)
{
	if( v_level == NULL )
		return;

	LONG mouse_x = GetRectMouseX(gMouse.x);	//해상도에맞춰서..
	LONG mouse_y = GetRectMouseY(gMouse.y);	//해상도에 맞춰서..
	CLevel *level = (CLevel *)v_level;
	CBsp *bsp = level->mBsp;

	if( !level->IsLoadedBsp() )
		return;

	Vector3f pos;

	if (gMouse.button & _LEFT_BUTTON_DOWN)	//에딧 모드
	{
//		gMouse.button &= (~_LEFT_BUTTON_DOWN);
		if( stOneClick == FALSE )
		{
			stOneClick=TRUE;
			stOldMx = mouse_x;
			stOldMy = mouse_y;
		}

		if( stPosCheck )	//	위치를 찝을 경우...
		{
			BOOL is_coll;
			if( GetPickingStyle() == _PICKING_STYLE_FAR )
				is_coll=level->GetPointFromScreenRayFar(mouse_x,mouse_y,&pos);//마우스 찝은곳이 성공.
			else
				is_coll=level->GetPointFromScreenRay(mouse_x,mouse_y,&pos);//마우스 찝은곳이 성공.

			if( is_coll )
			{
				Vector3fCopy(pos,stPos);
			}
		}
	}
	else
	{
		stOneClick=FALSE;
	}

	if( stOneClick )
	{	//이동 좌표축이 하나라도 켜지는 경우.
		if( stMoveCheck )
		{
			MoveMagicPoint(GetYButton(),GetXZButton(),mouse_x-stOldMx
				,mouse_y-stOldMy);
		}
		stOldMx = mouse_x;
		stOldMy = mouse_y;
	}	
	if( stMagicUniqueID )	//현재 유닉크 아이디가 살아 있는 경우는 움직여줘보자.
	{
		if( !MovePosMagicEffect(stMagicUniqueID,stPos,0) )
		{
			//수동으로 더이상의 마법 움직임이 없다 없애자.
			stMagicUniqueID=0;
		}
	}
}


void CMagicEdit::OnCheck1() //pos
{
	stPosCheck=!stPosCheck;
	if( stPosCheck )	//다른데는 끈다. 
	{
		CheckDlgButton(IDC_CHECK2,FALSE);
		stMoveCheck=FALSE;
	}	
}

void CMagicEdit::OnCheck2() //mov
{
	stMoveCheck=!stMoveCheck;
	if( stMoveCheck )	//다른데는 끈다. 
	{
		CheckDlgButton(IDC_CHECK1,FALSE);
		stPosCheck=FALSE;
	}	
}

void CMagicEdit::OnCheck3() //항상 살아있음 첵크 
{
	stAlwaysLive=!stAlwaysLive;

	if( stAlwaysLive && stMagicUniqueID )	//현재 유닉크 아이디가 살아 있어서 사라지기 시작하게 만든다.
	{
		SetDisapearTimeMagicEffect(stMagicUniqueID,0);
	}
}

