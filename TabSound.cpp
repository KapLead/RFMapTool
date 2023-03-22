// Sound.cpp : implementation file
//

#include "stdafx.h"
#include "rfmaptool.h"
#include "tabSound.h"

#include "CommonUtil.h"
#include "Jmalloc.h"
#include "console.h"
#include "Jerror.h"
#include "r3input.h"
#include "r3sound.h"
#include "r3math.h"
#include "r3util.h"
#include "r3text.h"
#include "core.h"
#include "toolmanager.h"
#include "R3Engine/1stClass/core.h"
#include "R3Engine/2ndClass/Level.h"
#include "R3Engine/2ndClass/R3Sound.h"
#include "R3Engine/Common/CommonUtil.h"
#include "R3Engine/Common/c_vector.h"
#include "R3Engine/Common/jerror.h"
#include "R3Engine/Common/jmalloc.h"
#include "R3Engine/common/R3Input.h"
#include "R3Engine/Common/R3math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int stCurSoundMode=0;
static BOOL stUnFreezeState=FALSE;

typedef struct{
	WORD id;		// ���� ���̵�.
	DWORD flag;		//���� �÷��� 
	float scale;	// �߽ɰŸ�...
	float attn;		// ����.
	Vector3f	pos;
//--- box�� ����.
	Vector3f box_scale;
	float box_attn;
	float box_rot_x,box_rot_y;
	WORD event_time;		//���ʸ���.
	DWORD spare;	//������ ���� ����...
	Vector3f bb_min,bb_max;

	float next_play_time;	//�̺�Ʈ�÷��� �ð��ϰ�� 
	float GetVolume(Vector3f pos);
	float GetPan(Vector3f pos);
	
	float mInvMat[4][4];						//������� �ʿ�.
	float GetBoxIntensity(Vector3f pos);		//�ڽ� ���������� ������ �˷��ش�. 0-1����
}_SAVE_SOUND_ENTITIES_LIST;

#define _SEL_VERSION	1.1f

_SAVE_SOUND_ENTITIES_LIST *stSoundEntitiesList;
static int stSoundEntitiesNum=0;
static int stMaxSoundEntitiesNum=0;

#define SOUND_IS_HERE		0x80
#define SOUND_IS_SELECTED	0x40
#define SOUND_IS_FREEZE		0x20
#define _SOUND_FLAG_PAN_OFF	0x10
#define _SOUND_FLAG_SPHERE	0x1
#define _SOUND_FLAG_BOX		0x2
#define _SOUND_FLAG_LOOP	0x4
#define _SOUND_FLAG_EVENT	0x8

//�� ����ü�� ���̺긦 ���ؼ�.
typedef struct{
	char Name[64];
	char FileName[64];	//���� �н����� �����̸�. 
	DWORD id;
}_SAVE_SOUND_LIST;
_SAVE_SOUND_LIST *stSoundList;
static int stSoundListNum=0;

static int  stMaxSoundNum;
static BOOL IsOnInitDialog=FALSE;
static CSoundEdit *stEntityEdit=NULL; //<- onintDialog���� �־ ����Ѵ�.


static float stScale=50.0f;
static float stAttn=70.0f;

static BOOL stOneClickEntity = FALSE;
static int stOldEntityMx,stOldEntityMy;
static int stCurEditMode;
Vector3f stCurEntityPos;
int stCurSelectSound=-1;

DWORD stFlag;
WORD stEventTime;
float stBoxRotY,stBoxRotX;
Vector3f stBoxScale;
float stBoxAttn;

/////////////////////////////////////////////////////////////////////////////
// CSoundFile dialog

IMPLEMENT_DYNCREATE(CSoundFile, CDialog)

CSoundFile::CSoundFile(CWnd* pParent /*=NULL*/)
	: CDialog(CSoundFile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSoundFile)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSoundFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSoundFile)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

void ReleaseTabSoundEntity()
{
	int i;
	if( stSoundList )
	{
		for(i=0; i<stSoundListNum; i++)
		{
			if( stSoundList[i].id )
			{
				IM_StopWave(stSoundList[i].id);
				IM_ReleaseWave(stSoundList[i].id);
			}
		}
		Dfree(stSoundList);
	}
	
	if( stSoundEntitiesList )
		Dfree(stSoundEntitiesList);
}
static void FixTooDist();	//�Ÿ��� ���� �ȵǴ°��� �ִ°� �����.

void LoadSound()
{
	// TODO: Add your control notification handler code here
	
	OPENFILENAME ofn; // zero the ofn members out
	memset(&ofn,0,sizeof(OPENFILENAME));
	char name[256]=".\\���߰���\\ToolBin\\Sound\\test.sel";
	int i;

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = name;
	ofn.nMaxFile = sizeof(name);
	ofn.lpstrFilter = "R3 SEL file (*.SEL)\0*.SEL\0All (*.*)\0*.*\0";
	ofn.lpstrTitle = "Select SEL(Sound Entity List) file";
	ofn.Flags = OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (!GetOpenFileName(&ofn))
	{
		return;
	}
	
	FILE *fp;
	if( ( fp = fopen(name,"rb"))==NULL )
	{
		Warning(name,"������ ���������ϴ�.");
		return;
	}
	float version;
	fread(&version,4,1,fp);
	if( _SEL_VERSION != version )
	{
		Warning(name,"������ ���������Դϴ�.");
		fclose(fp);
		return;
	}
	//���� ���� ����...

	ReleaseTabSoundEntity();

	fread(&stSoundListNum,4,1,fp);	//������.

	stMaxSoundNum=stSoundListNum+10;
	stSoundList=(_SAVE_SOUND_LIST *)Dmalloc(sizeof(_SAVE_SOUND_LIST)*stMaxSoundNum);
	memset(stSoundList,0,sizeof(_SAVE_SOUND_LIST)*stMaxSoundNum);

	char entity_name[256];
	char full_name[256];
	char snd_path[256];
	GetPrivateProfileString("Path","SoundPath",".\\snd\\",snd_path, 256,".\\R3Engine.ini");
	for(i=0; i<stSoundListNum; i++)
	{
		fread(&stSoundList[i].Name,64,1,fp);	//�����̸�.

		strcpy(full_name,snd_path);
		strcat(full_name,stSoundList[i].Name);
		stSoundList[i].id = ::IM_LoadWave(full_name,_PLAY_WAVE_LOOP);	//�ε����� ���ϵȴ�.�ϳ��� �ε������� �ȿ��� ���̺� �����ڰ� ����ԵǹǷ� .. �������� IM_ReleaseAllWaves()ȣ�� �Ұ�.
		//��Ʈ�� ó��...
		strcpy(entity_name,stSoundList[i].Name);
		stEntityEdit->m_LoadedSoundList.InsertString(i, entity_name);
		StripPath(entity_name);
		strcpy(stSoundList[i].FileName,entity_name);
	}
	stEntityEdit->m_LoadedSoundList.SetCurSel(0);

	fread(&stSoundEntitiesNum,4,1,fp);	//������.

	stMaxSoundEntitiesNum=stSoundEntitiesNum+10;
	stSoundEntitiesList=(_SAVE_SOUND_ENTITIES_LIST *)Dmalloc(stMaxSoundEntitiesNum*sizeof(_SAVE_SOUND_ENTITIES_LIST));
	memset( stSoundEntitiesList,0,stMaxSoundEntitiesNum*sizeof(_SAVE_SOUND_ENTITIES_LIST));

	for( i=0; i<stSoundEntitiesNum; i++)
	{
		fread(&stSoundEntitiesList[i].id,sizeof(WORD),1,fp);	
		fread(&stSoundEntitiesList[i].event_time,sizeof(WORD),1,fp);	
		fread(&stSoundEntitiesList[i].flag,sizeof(DWORD),1,fp);	
		fread(&stSoundEntitiesList[i].scale,sizeof(float),1,fp);	//������.
		fread(&stSoundEntitiesList[i].attn,sizeof(float),1,fp);	//����.
		fread(stSoundEntitiesList[i].pos,sizeof(Vector3f),1,fp);	//��ġ
		fread(stSoundEntitiesList[i].box_scale,sizeof(Vector3f),1,fp);	//��ġ
		fread(&stSoundEntitiesList[i].box_attn,sizeof(float),1,fp);		//�ڽ� ����.
		fread(&stSoundEntitiesList[i].box_rot_x,sizeof(float),1,fp);	//�ڽ� xȸ��.
		fread(&stSoundEntitiesList[i].box_rot_y,sizeof(float),1,fp);	//�ڽ� yȸ��.
		fread(&stSoundEntitiesList[i].spare,sizeof(DWORD),1,fp);	//����.
	}

	fclose(fp);
}
void SaveSound()
{
	// TODO: Add your control notification handler code here
	OPENFILENAME ofn; // zero the ofn members out
	memset(&ofn,0,sizeof(OPENFILENAME));
	char path[256]=".\\���߰���\\ToolBin\\Sound\\test.sel";

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = path;
	ofn.nMaxFile = sizeof(path);
	ofn.lpstrFilter = "R3 SEL file (*.SEL)\0*.SEL\0All (*.*)\0*.*\0";
	ofn.lpstrTitle = "Select SEL(Sound Entity List) file";
	ofn.Flags = OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (!GetSaveFileName(&ofn))
	{
		return;
	}
	
	FixTooDist();	//�Ÿ��� ���� �ȵǴ°��� �ִ°� �����.

	FILE *fp;

	if(( fp =fopen(path,"wb"))==NULL) 
	{
		Warning(path,"<-�� ������ �� ���ϴ�.");
		return;
	}
	float version = _SEL_VERSION;
	fwrite(&version,4,1,fp);
	fwrite(&stSoundListNum,4,1,fp);	//������ ����.
	for(int i=0; i<stSoundListNum; i++)
	{
		fwrite(&stSoundList[i].Name,64,1,fp);	//�����̸� ����.
	}

	fwrite(&stSoundEntitiesNum,4,1,fp);	//������ ����.

	for(int i=0; i<stSoundEntitiesNum; i++)
	{
		fwrite(&stSoundEntitiesList[i].id,sizeof(WORD),1,fp);
		fwrite(&stSoundEntitiesList[i].event_time,sizeof(WORD),1,fp);
		fwrite(&stSoundEntitiesList[i].flag,sizeof(DWORD),1,fp);	
		fwrite(&stSoundEntitiesList[i].scale,sizeof(float),1,fp);	//������.
		fwrite(&stSoundEntitiesList[i].attn,sizeof(float),1,fp);	//����.
		fwrite(stSoundEntitiesList[i].pos,sizeof(Vector3f),1,fp);	//��ġ
		fwrite(stSoundEntitiesList[i].box_scale,sizeof(Vector3f),1,fp);	//��ġ
		fwrite(&stSoundEntitiesList[i].box_attn,sizeof(float),1,fp);		//�ڽ� ����.
		fwrite(&stSoundEntitiesList[i].box_rot_x,sizeof(float),1,fp);	//�ڽ� xȸ��.
		fwrite(&stSoundEntitiesList[i].box_rot_y,sizeof(float),1,fp);	//�ڽ� yȸ��.
		fwrite(&stSoundEntitiesList[i].spare,sizeof(DWORD),1,fp);	//����.
	}

	fclose(fp);
}

void CSoundFile::OnButton1()	//���� �ε�..
{
	LoadSound();
}

void CSoundFile::OnButton2()	//���� ����.
{
	SaveSound();
}


BEGIN_MESSAGE_MAP(CSoundFile, CDialog)
	//{{AFX_MSG_MAP(CSoundFile)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSoundFile message handlers
/////////////////////////////////////////////////////////////////////////////
// CSoundEdit dialog



static BYTE *stSoundPlayFlag;
static int stMaxSoundPlayFlagCnt=0;

IMPLEMENT_DYNCREATE(CSoundEdit, CDialog)

CSoundEdit::CSoundEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CSoundEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSoundEdit)
	//}}AFX_DATA_INIT
}

CSoundEdit::~CSoundEdit()
{
	if(	stSoundPlayFlag )
	{
		Dfree(stSoundPlayFlag);
	}
}


void CSoundEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSoundEdit)
	DDX_Control(pDX, IDC_EDIT6, m_BoxRotX);
	DDX_Control(pDX, IDC_EDIT5, m_BoxRotY);
	DDX_Control(pDX, IDC_EDIT25, m_EventTime);
	DDX_Control(pDX, IDC_EDIT3, m_BoxAttn);
	DDX_Control(pDX, IDC_EDIT27, m_BoxScaleZ);
	DDX_Control(pDX, IDC_EDIT26, m_BoxScaleY);
	DDX_Control(pDX, IDC_EDIT22, m_BoxScaleX);
	DDX_Control(pDX, IDC_EDIT15, m_Attn);
	DDX_Control(pDX, IDC_EDIT2, m_Scale);
	DDX_Control(pDX, IDC_COMBO1, m_LoadedSoundList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSoundEdit, CDialog)
	//{{AFX_MSG_MAP(CSoundEdit)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_BN_CLICKED(IDC_CHECK10, OnCheck10)
	ON_BN_CLICKED(IDC_CHECK7, OnCheck7)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_CHECK18, OnCheck18)
	ON_BN_CLICKED(IDC_CHECK27, OnCheck27)
	ON_BN_CLICKED(IDC_CHECK15, OnCheck15)
	ON_EN_CHANGE(IDC_EDIT15, OnChangeEdit15)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK5, OnCheck5)
	ON_BN_CLICKED(IDC_CHECK24, OnCheck24)
	ON_EN_CHANGE(IDC_EDIT25, OnChangeEdit25)
	ON_BN_CLICKED(IDC_CHECK16, OnCheck16)
	ON_BN_CLICKED(IDC_CHECK11, OnCheck11)
	ON_BN_CLICKED(IDC_CHECK29, OnCheck29)
	ON_EN_CHANGE(IDC_EDIT22, OnChangeEdit22)
	ON_EN_CHANGE(IDC_EDIT26, OnChangeEdit26)
	ON_EN_CHANGE(IDC_EDIT27, OnChangeEdit27)
	ON_BN_CLICKED(IDC_CHECK28, OnCheck28)
	ON_EN_CHANGE(IDC_EDIT3, OnChangeEdit3)
	ON_BN_CLICKED(IDC_CHECK12, OnCheck12)
	ON_EN_CHANGE(IDC_EDIT5, OnChangeEdit5)
	ON_BN_CLICKED(IDC_CHECK21, OnCheck21)
	ON_EN_CHANGE(IDC_EDIT6, OnChangeEdit6)
	ON_BN_CLICKED(IDC_CHECK22, OnCheck22)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSoundEdit message handlers


float _SAVE_SOUND_ENTITIES_LIST::GetBoxIntensity(Vector3f pos)		//�ڽ� ���������� ������ �˷��ش�. 0-1����
{
	int i;
	float intensity=1;
	Vector3f get;
	Vector3fTransform(get,pos,mInvMat);
	Vector3f bb_min={-1,-1,-1};
	Vector3f bb_max={1,1,1};
	
	if( box_attn == 1 )
	{
		if( IsCollisionBBoxPoint(bb_min,bb_max,get) )
			return 1.0f;
		else
			return 0.0f;
	}


	for(i=0; i<3; i++)
	{
		if( get[i] > 1 )
			return 0;
		if( get[i] > box_attn )
			intensity = min(intensity,((1-get[i])/(1-box_attn)));
		if( get[i] < -1 )
			return 0;
		if( get[i] < -box_attn )
			intensity = min(intensity,((1+get[i])/(1-box_attn)));
	}
	return intensity;
}


float _SAVE_SOUND_ENTITIES_LIST::GetVolume(Vector3f pos)
{
	Vector3f cam_pos,temp;
	float density,dist;

	::GetCameraPos(&cam_pos);
	Vector3fSub(pos,cam_pos,temp);
	dist = Vector3fDist(temp);	//���� ����Ʈ���� �Ÿ�.

	dist = max(dist,scale);
	dist = min(dist,attn);
	density = (attn-dist)/(attn-scale);

//	return 1;	//�׽�Ʈ..
	return density;
}
float _SAVE_SOUND_ENTITIES_LIST::GetPan(Vector3f src)	//�ʻ��� ��ġ�ϳ��� ȿ������ ���� �Ұ��� ���Ѵ�.  -1�� ���ʿ�ġ��ħ.1�� �����ʿ� ġ��ħ.
{
	Vector3f tar_pos;
	Vector3f view_vec;
	_R3ENGINE_STATE *state = GetR3State();

	view_vec[0] = state->mMatView.m[2][2];	//�Ĵٺ����ִ� ������ 
	view_vec[1] = state->mMatView.m[2][1];
	view_vec[2] = state->mMatView.m[2][0];
	Normalize(view_vec);

	tar_pos[0] = state->mInvMatView.m[3][0];
	tar_pos[1] = state->mInvMatView.m[3][1];
	tar_pos[2] = state->mInvMatView.m[3][2];	//���� ���� ��ġ���� ���ͷ�....
	Vector3fSub(tar_pos,src,tar_pos);
	Normalize(tar_pos);

	return -Vector3fDot(tar_pos,view_vec);	//Dot�� �ٷ� ���� �ȴ�.
}


static BOOL IsNearDistExist(Vector3f pos)	//���尡 �ʹ� ��ó�� �ִ°��.
{
	Vector3f temp;
	for(int i=0; i<stSoundEntitiesNum; i++)
	{
		Vector3fSub(stSoundEntitiesList[i].pos,pos,temp);
		if( Vector3fDist(temp) < 5 )
			return TRUE;
	}
	return FALSE;
}
static void SetScale(float su)
{
	char buf [256];
	sprintf(buf,"%3.01f",su);
	if( IsOnInitDialog )
		stEntityEdit->m_Scale.SetWindowText(buf);
}

static void SetAttn(float su)
{
	char buf [256];
	sprintf(buf,"%3.01f",su);
	if( IsOnInitDialog )
		stEntityEdit->m_Attn.SetWindowText(buf);
}

static void SetFlag(DWORD flag)
{
	if( !IsOnInitDialog )
		return;
	if( flag & _SOUND_FLAG_SPHERE )
	{
		stEntityEdit->CheckDlgButton(IDC_CHECK16,TRUE);
		stEntityEdit->CheckDlgButton(IDC_CHECK11,FALSE);
	}
	else
	{
		stEntityEdit->CheckDlgButton(IDC_CHECK11,TRUE);
		stEntityEdit->CheckDlgButton(IDC_CHECK16,FALSE);
	}

	if( flag & _SOUND_FLAG_LOOP )
	{
		stEntityEdit->CheckDlgButton(IDC_CHECK5,TRUE);
		stEntityEdit->CheckDlgButton(IDC_CHECK24,FALSE);
	}
	else
	{
		stEntityEdit->CheckDlgButton(IDC_CHECK24,TRUE);
		stEntityEdit->CheckDlgButton(IDC_CHECK5,FALSE);
	}

	if( flag & _SOUND_FLAG_PAN_OFF )
	{
		stEntityEdit->CheckDlgButton(IDC_CHECK22,TRUE);
	}
	else
	{
		stEntityEdit->CheckDlgButton(IDC_CHECK22,FALSE);
	}
}

static void SetEventTime(DWORD su)
{
	char buf [256];
	sprintf(buf,"%d",su);
	if( IsOnInitDialog )
		stEntityEdit->m_EventTime.SetWindowText(buf);
}
static void SetBoxRotY(float su)
{
	char buf [256];
	sprintf(buf,"%3.01f",su);
	if( IsOnInitDialog )
		stEntityEdit->m_BoxRotY.SetWindowText(buf);
}
static void SetBoxRotX(float su)
{
	char buf [256];
	sprintf(buf,"%3.01f",su);
	if( IsOnInitDialog )
		stEntityEdit->m_BoxRotX.SetWindowText(buf);
}
static void SetBoxAttn(float su)
{
	char buf [256];
	sprintf(buf,"%3.02f",su);
	if( IsOnInitDialog )
		stEntityEdit->m_BoxAttn.SetWindowText(buf);
}
static void SetBoxScale(Vector3f scale)
{
	if( !IsOnInitDialog )
		return;
	char buf [256];
	sprintf(buf,"%3.01f",scale[0]);
	stEntityEdit->m_BoxScaleX.SetWindowText(buf);
	sprintf(buf,"%3.01f",scale[1]);
	stEntityEdit->m_BoxScaleY.SetWindowText(buf);
	sprintf(buf,"%3.01f",scale[2]);
	stEntityEdit->m_BoxScaleZ.SetWindowText(buf);
}


static void SelectGAll(int sound_id)	//�Ѱ��� id�� ��� �����Ѵ�.
{

	for(int i=0; i<stSoundEntitiesNum; i++)	
	{
		if( stSoundEntitiesList[i].flag & SOUND_IS_FREEZE )	//������ȰŴ� ����.
			continue;
		if( stSoundEntitiesList[i].id == sound_id )
			stSoundEntitiesList[i].flag |= SOUND_IS_SELECTED;
	}
}

static int SelectOneEntity(LONG x,LONG y,CLevel *level,Vector3f cam_pos)	//���� ����� ��ƼƼ�� �����Ѵ�.
{
	Vector3f temp;
	int id=-1;
	float dist=100000,temp_dist;

	int state = GetSelectState();

	for(int i=0; i<stSoundEntitiesNum; i++)	//�Ÿ��� �ճ���� ����.
	{
		if( stSoundEntitiesList[i].flag & SOUND_IS_FREEZE ) //������ȰŴ� ����..
			continue;
//		if( !(stSoundEntitiesList[i].flag & SOUND_IS_HERE) )
//			continue;
		if( state > 0 )	//add�ϰ�� 
		{
			if( (stSoundEntitiesList[i].flag & SOUND_IS_SELECTED) )	//���õ� ���� ����.
				continue;
		}
		else
		if( state < 0 )	//������ ��� 
		{
			if( !(stSoundEntitiesList[i].flag & SOUND_IS_SELECTED) )	//���þȵ� ���� ����.
				continue;
		}
		else
		{
			//stSoundEntitiesList[i].flag&=~SOUND_IS_SELECTED;
		}

		if( level->IsCollisionRayAABB(x,y,stSoundEntitiesList[i].bb_min,stSoundEntitiesList[i].bb_max,&temp))
		{
			Vector3fSub(cam_pos,temp,temp);
			temp_dist = Vector3fDist(temp);
			if( temp_dist < dist )
			{
				dist=temp_dist;
				id = i;
			}
		}
	}
	if( id != -1 )
	{
		if( state > 0 )	//add�ϰ�� 
			stSoundEntitiesList[id].flag |= SOUND_IS_SELECTED;
		else
		if( state < 0 )	//������ ��� 
			stSoundEntitiesList[id].flag&=~SOUND_IS_SELECTED;
		else
		{
			for(int i=0; i<stSoundEntitiesNum; i++)
			{
				stSoundEntitiesList[i].flag &= ~SOUND_IS_SELECTED;
			}
			stSoundEntitiesList[id].flag |= SOUND_IS_SELECTED;
			stCurSelectSound = stSoundEntitiesList[id].id;
			stEntityEdit->m_LoadedSoundList.SetCurSel(stCurSelectSound);

			stScale=stSoundEntitiesList[id].scale;
			stAttn=stSoundEntitiesList[id].attn;
			stFlag = stSoundEntitiesList[id].flag;
			stEventTime=stSoundEntitiesList[id].event_time;
			stBoxRotY = stSoundEntitiesList[id].box_rot_y;
			stBoxRotX = stSoundEntitiesList[id].box_rot_x;
			stBoxAttn = stSoundEntitiesList[id].box_attn;
			Vector3fCopy(stSoundEntitiesList[id].box_scale,stBoxScale);

			
			SetScale(stScale);
			SetAttn(stAttn);

			SetFlag(stFlag);
			SetEventTime(stEventTime);
			SetBoxRotY(stBoxRotY);
			SetBoxRotX(stBoxRotX);
			SetBoxAttn(stBoxAttn);
			SetBoxScale(stBoxScale);
 
			stEntityEdit->OnSelchangeCombo1();
		}
	}
	return id;
}
static int UnFreezeOneEntity(LONG x,LONG y,CLevel *level,Vector3f cam_pos)	//���� ����� ������� ��ƼƼ�� �����Ѵ�.
{
	Vector3f temp;
	int id=-1;
	float dist=100000,temp_dist;

	for(int i=0; i<stSoundEntitiesNum; i++)	//�Ÿ��� �ճ���� ����.
	{
//		if( !(stSoundEntitiesList[i].flag & ENTITY_IS_VIEW) )
//			continue;
		if( !(stSoundEntitiesList[i].flag & SOUND_IS_FREEZE) ) //������� �ȵȰŴ� ����..
			continue;
		if( level->IsCollisionRayAABB(x,y,stSoundEntitiesList[i].bb_min,stSoundEntitiesList[i].bb_max,&temp))
		{
			Vector3fSub(cam_pos,temp,temp);
			temp_dist = Vector3fDist(temp);
			if( temp_dist < dist )
			{
				dist=temp_dist;
				id = i;
			}
		}
	}
	if( id != -1 )
	{
		stSoundEntitiesList[id].flag &= ~SOUND_IS_FREEZE;
	}
	return id;
}
//���õ� ��ƼƼ���� ��ġ�� �ű��.
static void MoveSelectedEntities(BOOL y_axis,BOOL xz_axis,LONG x_diff,LONG y_diff )
{
#define _SXSY_TO_DIST	0.2f
	float y=(float)y_diff*_SXSY_TO_DIST;
	float x=(float)x_diff*_SXSY_TO_DIST;

	if( x == 0 && y == 0 )
		return;

	for(int i=0; i<stSoundEntitiesNum; i++)	
	{
		if( !(stSoundEntitiesList[i].flag & SOUND_IS_SELECTED) )	//���õ��� ���� ���� ����.
			continue;
		if( y_axis )
		{
			stSoundEntitiesList[i].pos[1]-=y;
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

			stSoundEntitiesList[i].pos[0]+=(billboard.m[0][0]*x+billboard.m[0][2]*-y);
			stSoundEntitiesList[i].pos[2]+=(billboard.m[2][0]*x+billboard.m[2][2]*-y);
		}
	}
}

static void ChanegeSelectedFlag(DWORD flag)
{

	for(int i=0; i<stSoundEntitiesNum; i++)	
	{
		if( !(stSoundEntitiesList[i].flag & SOUND_IS_SELECTED) )	//���õ��� ���� ���� ����.
			continue;
		stSoundEntitiesList[i].flag=flag;
	}
}

static void ChanegeSelectedEntities(float scale,float attn,float rot_x )
{

	for(int i=0; i<stSoundEntitiesNum; i++)	
	{
		if( !(stSoundEntitiesList[i].flag & SOUND_IS_SELECTED) )	//���õ��� ���� ���� ����.
			continue;
		if( scale > 0 )
		{
			stSoundEntitiesList[i].scale=scale;
		}
		if( attn > 1 )
		{
			stSoundEntitiesList[i].attn=attn;
		}
	}
}

static void ChanegeBoxSelectedEntities(WORD event_time,Vector3f box_scale,float box_attn,float rot_x,float rot_y )
{
	for(int i=0; i<stSoundEntitiesNum; i++)	
	{
		if( !(stSoundEntitiesList[i].flag & SOUND_IS_SELECTED) )	//���õ��� ���� ���� ����.
			continue;
		stSoundEntitiesList[i].event_time = event_time;
		Vector3fCopy(box_scale,stSoundEntitiesList[i].box_scale);
		stSoundEntitiesList[i].box_attn = box_attn;
		stSoundEntitiesList[i].box_rot_x = rot_x;
		stSoundEntitiesList[i].box_rot_y = rot_y;
	}
}

static void DeleteSelectedEntities()	// ���õǾ��� ��ƼƼ���� �����.
{
	//����Ʈ ���̶� �÷������ΰ� �����.
	for( int i=0; i<stSoundEntitiesNum; i++)
	{
		IM_StopWave(stSoundList[stSoundEntitiesList[i].id].id);
	}
	int real_cnt=stMaxSoundPlayFlagCnt/8+1;
	if( stSoundPlayFlag )
		memset( stSoundPlayFlag,0,sizeof(BYTE)*real_cnt);
	int i;
	while( 1 )
	{
		for(int i=0; i<stSoundEntitiesNum; i++)	//�Ÿ��� �ճ���� ����.
		{
			if( !(stSoundEntitiesList[i].flag & SOUND_IS_SELECTED) )	//���õ��� ���� ���� ����.
				continue;
			break;
		}
		if( i == stSoundEntitiesNum )
			break;

		memcpy(&stSoundEntitiesList[i],&stSoundEntitiesList[i+1],sizeof(_SAVE_SOUND_ENTITIES_LIST)*(stSoundEntitiesNum-i-1));
		stSoundEntitiesNum--;
	}
}
static void UnSelectAllEntities()
{
	for(int i=0; i<stSoundEntitiesNum; i++)	//��� un��������.
	{
		stSoundEntitiesList[i].flag&=~SOUND_IS_SELECTED;
	}
}

static void FixTooDist()	//�Ÿ��� ���� �ȵǴ°��� �ִ°� �����.
{
	UnSelectAllEntities();
	for(int i=0; i<stSoundEntitiesNum; i++)	//�Ÿ��� �ճ���� ����.
	{
		if( stSoundEntitiesList[i].pos[0] > 32000 || stSoundEntitiesList[i].pos[0] < -32000 
		|| stSoundEntitiesList[i].pos[1] > 32000 || stSoundEntitiesList[i].pos[1] < -32000 
		|| stSoundEntitiesList[i].pos[2] > 32000 || stSoundEntitiesList[i].pos[2] < -32000 
			)
		{
			stSoundEntitiesList[i].flag |= SOUND_IS_SELECTED;
		}
	}
	DeleteSelectedEntities();
}

static BOOL stIsCurEntity;
void ProgressSoundEdit(void *v_level,Vector3f cam_pos)
{
	if( v_level == NULL )
		return;

	LONG mouse_x = GetRectMouseX(gMouse.x);	//�ػ󵵿����缭..
	LONG mouse_y = GetRectMouseY(gMouse.y);	//�ػ󵵿� ���缭..
	CLevel *level = (CLevel *)v_level;
	CBsp *bsp = level->mBsp;

	int what_mode = WhatSoundMode();

	if( !level->IsLoadedBsp() )
		return;

	BOOL is_coll;
	Vector3f pos;

	stIsCurEntity=FALSE;
	if( what_mode == _ENTITY_EDIT_MODE )
	{
		if( GetPickingStyle() == _PICKING_STYLE_FAR )
			is_coll=level->GetPointFromScreenRayFar(mouse_x,mouse_y,&pos);//���콺 �������� ����.
		else
			is_coll=level->GetPointFromScreenRay(mouse_x,mouse_y,&pos);//���콺 �������� ����.
		if( level->mBsp->mPickPoly == -1 )	//���콺�� Ŭ���� �ȵȰ��̴�.
			is_coll = FALSE;

		if( is_coll )
		{
			stIsCurEntity=TRUE;
			Vector3fCopy(pos,stCurEntityPos);
		}
	}

	if (gMouse.button & _LEFT_BUTTON_DOWN)	//����
	{
//		gMouse.button &= (~_LEFT_BUTTON_DOWN);

		if( stOneClickEntity == FALSE )
		{
			stOneClickEntity=TRUE;
			stOldEntityMx = mouse_x;
			stOldEntityMy = mouse_y;
			if( what_mode == _SOUND_EDIT_MODE )
			{
				if(stCurEditMode == _SOUND_EDIT_POS_MODE )
				{
					if( stSoundEntitiesList == NULL )
					{
						stMaxSoundEntitiesNum=16;
						stSoundEntitiesList=(_SAVE_SOUND_ENTITIES_LIST *)Dmalloc(stMaxSoundEntitiesNum*sizeof(_SAVE_SOUND_ENTITIES_LIST));
					}
					if( IsNearDistExist(stCurEntityPos) )	//��ƼƼ�� �ʹ� ��ó�� �ִ°��.
					{
						Warning("���� ��ġ�� ���� ������!","");
					}
					else
					if( stCurSelectSound != -1 )
					{						
						stSoundEntitiesList[stSoundEntitiesNum].flag = stFlag;
						stSoundEntitiesList[stSoundEntitiesNum].spare = 0;
						stSoundEntitiesList[stSoundEntitiesNum].id = stCurSelectSound;
						stSoundEntitiesList[stSoundEntitiesNum].event_time = (WORD)stEventTime;
						Vector3fCopy(stCurEntityPos,stSoundEntitiesList[stSoundEntitiesNum].pos);
						stSoundEntitiesList[stSoundEntitiesNum].scale =stScale;
						stSoundEntitiesList[stSoundEntitiesNum].attn =stAttn;

						Vector3fCopy(stBoxScale,stSoundEntitiesList[stSoundEntitiesNum].box_scale);
						stSoundEntitiesList[stSoundEntitiesNum].box_attn =stBoxAttn;
						stSoundEntitiesList[stSoundEntitiesNum].box_rot_x = stBoxRotX;
						stSoundEntitiesList[stSoundEntitiesNum].box_rot_y = stBoxRotY;

						stSoundEntitiesNum++;
						if( stSoundEntitiesNum >= stMaxSoundEntitiesNum )
						{
							stSoundEntitiesList=(_SAVE_SOUND_ENTITIES_LIST *)ReAlloc(stSoundEntitiesList,stMaxSoundEntitiesNum*sizeof(_SAVE_SOUND_ENTITIES_LIST),(stMaxSoundEntitiesNum+16)*sizeof(_SAVE_SOUND_ENTITIES_LIST));
							stMaxSoundEntitiesNum+=16;
						}
					}
				}
			}
			else	//select ���
			{
				if( stUnFreezeState )//�������� ��尡 �켱�̴�.
				{
					UnFreezeOneEntity(mouse_x,mouse_y,level,cam_pos);	//���� ����� ��ƼƼ�� �������� �Ѵ�.
				}
				else
				{
					int id=SelectOneEntity(mouse_x,mouse_y,level,cam_pos);	//���� ����� ��ƼƼ�� �����Ѵ�.
					if( id != -1 )
					{
						stScale=stSoundEntitiesList[id].scale;
						SetScale(stScale);
					}
				}
			}
		}
	}
	else
	{
		stOneClickEntity=FALSE;
	}

	if( stOneClickEntity )
	{	//�̵� ��ǥ���� �ϳ��� ������ ���.
		switch(stCurEditMode)
		{
		case _SOUND_EDIT_POS_MODE:
			if( what_mode == _SOUND_SELECT_MODE )
				MoveSelectedEntities(GetYButton(),GetXZButton(),mouse_x-stOldEntityMx
					,mouse_y-stOldEntityMy);
			break;
		case _SOUND_EDIT_SCALE_MODE:
			stScale += (mouse_y-stOldEntityMy)/10.0f;
			if( stScale <1 )
				stScale=1;
			SetScale(stScale);
			if( stScale > stAttn )
			{
				stAttn=stScale+1;
				SetAttn(stAttn);
			}
			ChanegeSelectedEntities(stScale,stAttn,-1 );
			break;
		case _SOUND_EDIT_ATTN_MODE:
			stAttn += (mouse_y-stOldEntityMy)/10.0f;
			if( stAttn <2 )
				stAttn=2;
			SetAttn(stAttn);
			if( stScale > stAttn )
			{
				stScale=stAttn-1;
				SetScale(stScale);
			}
			ChanegeSelectedEntities(stScale,stAttn,-1 );
			break;

		case _SOUND_EDIT_BOX_SCALE:
			if( GetYButton() )
			{
				stBoxScale[1]+=(mouse_y-stOldEntityMy)/10.0f;
				if( stBoxScale[1] < 10 )
					stBoxScale[1] = 10;
			}
			else
			if( GetXZButton() )
			{
				stBoxScale[0]+=(mouse_x-stOldEntityMx)/10.0f;
				stBoxScale[2]+=(mouse_y-stOldEntityMy)/10.0f;
				if( stBoxScale[0] < 10 )
					stBoxScale[0] = 10;
				if( stBoxScale[2] < 10 )
					stBoxScale[2] = 10;
			}
			SetBoxScale(stBoxScale);
			ChanegeBoxSelectedEntities(stEventTime,stBoxScale,stBoxAttn,stBoxRotX,stBoxRotY);
			break;
		case _SOUND_EDIT_BOX_ATTN_MODE:
			stBoxAttn += ((mouse_y-stOldEntityMy)*0.01f);
			if( stBoxAttn < 0.1f )
				stBoxAttn = 0.1f;
			if( stBoxAttn > 1.0f )
				stBoxAttn = 1.0f;
			SetBoxAttn(stBoxAttn);
			ChanegeBoxSelectedEntities(stEventTime,stBoxScale,stBoxAttn,stBoxRotX,stBoxRotY);
			break;
		case _SOUND_EDIT_BOX_YROT_MODE:
			stBoxRotY += (mouse_y-stOldEntityMy)*0.5f;
			SetBoxRotY(stBoxRotY);
			ChanegeBoxSelectedEntities(stEventTime,stBoxScale,stBoxAttn,stBoxRotX,stBoxRotY);
			break;
		case _SOUND_EDIT_BOX_XROT_MODE:
			stBoxRotX += (mouse_y-stOldEntityMy)*0.5f;
			SetBoxRotX(stBoxRotX);
			ChanegeBoxSelectedEntities(stEventTime,stBoxScale,stBoxAttn,stBoxRotX,stBoxRotY);
			break;
		}
		stOldEntityMx = mouse_x;
		stOldEntityMy = mouse_y;
	}
	if( gAsci[VK_DELETE] )
	{
		gAsci[VK_DELETE]=0;
		DeleteSelectedEntities();
	}
}


static void GetSoundName(char *name)
{
	char temp_name[256];
	char snd_path[256],temp[256];
	int i,cnt=0;

	GetPrivateProfileString("Path","SoundPath",".\\snd\\",temp, 256,".\\R3Engine.ini");
	strcpy(snd_path,&temp[2]);

	for( i=0; i<(int)strlen(name); i++)
	{
		temp_name[cnt] = name[i];
		cnt++;
		if( name[i] == '\\' )
		{
			temp_name[cnt]=NULL;
			strlwr(temp_name);
			cnt=0;
			if( !strcmp(snd_path,temp_name) )
			{
				strcpy(temp_name,&name[i]);
//				StripName(temp_name);
				break;
			}
		}
	}
	if( i != (int)strlen(name) )
		strcpy(name,temp_name);

}

void CSoundEdit::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
	ReleaseTabSoundEntity();
}

BOOL CSoundEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CheckDlgButton(IDC_RADIO1,TRUE);	//����Ʈ�� ����Ʈ��..
	CheckDlgButton(IDC_CHECK7,TRUE);//pos��� 
	
	IsOnInitDialog=TRUE;
	stEntityEdit = this;
	m_LoadedSoundList.ResetContent();	//���ڿ�����Ʈ�� �������..

	char buf[256];
	sprintf(buf,"%f",stScale);
	m_Scale.SetWindowText(buf);

	sprintf(buf,"%f",stAttn);
	m_Attn.SetWindowText(buf);

	for( int i=0; i<stSoundListNum; i++)
	{
		m_LoadedSoundList.InsertString(i, stSoundList[i].Name);
	}
	if( stSoundListNum )
	{
		m_LoadedSoundList.SetCurSel(0);
		stCurSelectSound=0;
	}
	CheckDlgButton(IDC_CHECK5,TRUE);	//������ ����Ʈ��.
	CheckDlgButton(IDC_CHECK16,TRUE);	//������ �⺻
	stFlag = _SOUND_FLAG_SPHERE|_SOUND_FLAG_LOOP;

	stEventTime=5;
	stBoxRotY=0;
	stBoxRotX=0;
	stBoxAttn=1.0f;
	SetEventTime(stEventTime);
	SetBoxRotY(stBoxRotY);
	SetBoxRotX(stBoxRotX);
	SetBoxAttn(stBoxAttn);
	
	stBoxScale[0]=40;
	stBoxScale[1]=40;
	stBoxScale[2]=40;
	SetBoxScale(stBoxScale);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSoundEdit::OnButton3() //����.
{
	// TODO: Add your control notification handler code here
	DeleteSelectedEntities();	
}

void CSoundEdit::OnSelchangeCombo1() //�� ü����..
{
	// TODO: Add your control notification handler code here
	
	stCurSelectSound = m_LoadedSoundList.GetCurSel();
	if( stCurSelectSound == -1 )	//����Ʈ�� �ϳ�.
		return;
}

void CSoundEdit::OnButton7()	//���� ����..
{
	OPENFILENAME ofn; // zero the ofn members out
	memset(&ofn,0,sizeof(OPENFILENAME));
	char name[1256]="";

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = name;
	ofn.nMaxFile = sizeof(name);
	ofn.lpstrFilter = "WAV���� (*.WAV)\0*.WAV\0All (*.*)\0*.*\0";
	ofn.lpstrTitle = "Select WAV file";
	ofn.Flags = OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (!GetOpenFileName(&ofn))
	{
		return;
	}
	
	//�н��� ������.
	char entity_name[256];
	int i;

	strcpy(entity_name,name);
	GetSoundName(entity_name);	//��ٱ����� �н��� ���ֶ�..

	if( strlen(entity_name) > 62 )
	{
		Warning(entity_name,"<-�н��� �ʹ���ϴ�.64�ڱ��� �Դϴ�.");
		return;
	}
	for(i=0; i<stSoundListNum; i++)		//������ �ε����ִ� Ȯ���Ѵ�.
	{
		if( !strcmp(entity_name,stSoundList[i].Name) )
		{
			Warning(entity_name, "�̹� �ε��Ǿ� �ֽ��ϴ�.");
			return;
		}
	}

	if( stSoundListNum==0 )	//ó���ϰ�� 
	{
		stMaxSoundNum=8;
		stSoundList=(_SAVE_SOUND_LIST *)Dmalloc(sizeof(_SAVE_SOUND_LIST)*stMaxSoundNum);
		memset(stSoundList,0,sizeof(_SAVE_SOUND_LIST)*stMaxSoundNum);
	}

	if( stMaxSoundNum <= stSoundListNum ) //��ĥ��� ���Ҵ�.
	{
		stSoundList=(_SAVE_SOUND_LIST *)ReAlloc(stSoundList,stMaxSoundNum*sizeof(_SAVE_SOUND_LIST),(stMaxSoundNum+8)*sizeof(_SAVE_SOUND_LIST));
		stMaxSoundNum += 8;
	} 

	memset(&stSoundList[stSoundListNum],0,sizeof(_SAVE_SOUND_LIST));
	strlwr(name);

	stSoundList[stSoundListNum].id = ::IM_LoadWave(name,_PLAY_WAVE_LOOP);	//�ε����� ���ϵȴ�.�ϳ��� �ε������� �ȿ��� ���̺� �����ڰ� ����ԵǹǷ� .. �������� IM_ReleaseAllWaves()ȣ�� �Ұ�.

	if( !stSoundList[stSoundListNum].id )
	{
		return;	//�ε�����.
	}

	m_LoadedSoundList.InsertString(stSoundListNum, entity_name);
	strcpy(stSoundList[stSoundListNum].Name,entity_name);
	StripPath(entity_name);
	strcpy(stSoundList[stSoundListNum].FileName,entity_name);

	stCurSelectSound = stSoundListNum;
	m_LoadedSoundList.SetCurSel(stCurSelectSound);
	stCurSelectSound=stSoundListNum;
	stSoundListNum++;

}

void CSoundEdit::OnButton8()	//���� ����....
{
	// TODO: Add your control notification handler code here
	int i;
	char buf[256];

	if( stCurSelectSound == -1 )
		return;

	sprintf(buf,"%s �ʻ��� �� ��ƼƼ�� ���� ������?",stSoundList[stCurSelectSound].Name);
	int result = MessageBox(buf,"Select",MB_OKCANCEL);
	if( IDOK != result )	//���� ���.
		return;

	//---���� �ش� ��ƼƼ�� �� ����Ʈ���� �� �����...
	SelectGAll(stCurSelectSound);	//���� Ŀ���� �Ȱ� ����.
	DeleteSelectedEntities();

	m_LoadedSoundList.ResetContent();	//���ڿ�����Ʈ�� �ϴ� �������..

	::IM_ReleaseWave(stSoundList[stCurSelectSound].id);
	//---��ĭ�� �ٶ����.
	memcpy(&stSoundList[stCurSelectSound],&stSoundList[stCurSelectSound+1]
		,sizeof(_SAVE_SOUND_LIST)*(stSoundListNum-stCurSelectSound-1));
	stSoundListNum--;

	if( stSoundListNum == 0 )
		stCurSelectSound =-1;
	
	for( i=0; i<stSoundListNum; i++)	//����� �����.
		m_LoadedSoundList.InsertString(i, stSoundList[i].Name);

	for( i=0; i<stSoundEntitiesNum; i++)	//���� ��ϵ� �༮�� id ��ĭ�� �и���....
	{
		if( stCurSelectSound <  stSoundEntitiesList[i].id )
			stSoundEntitiesList[i].id--;
	}

	if( stCurSelectSound > 0 )
	{
		stCurSelectSound--;
	}
	
	m_LoadedSoundList.SetCurSel(stCurSelectSound);
}

void CSoundEdit::OnButton9()	//���� ��ü...
{
	char buf[256];
	int i;

	if( stCurSelectSound == -1 )
		return;
	// TODO: Add your control notification handler code here
	sprintf(buf,"%s �� ���带 �ٸ� ����� ��ü�ұ��?",stSoundList[stCurSelectSound].Name);
	int result = MessageBox(buf,"Select",MB_OKCANCEL);
	if( IDOK != result )	//���� ���.
		return;

	OPENFILENAME ofn; // zero the ofn members out
	memset(&ofn,0,sizeof(OPENFILENAME));
	char name[256]="";

//	_R3ENGINE_STATE *state = GetR3State();
//	strcpy(name,state->SoundPath);
//	strcat(name,"test.r3e");

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = name;
	ofn.nMaxFile = sizeof(name);
	ofn.lpstrFilter = "WAV���� (*.WAV)\0*.WAV\0All (*.*)\0*.*\0";
	ofn.lpstrTitle = "Select WAV file";
	ofn.Flags = OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (!GetOpenFileName(&ofn))
	{
		return;
	}
	char entity_name[256];

	strcpy(entity_name,name);
	GetSoundName(entity_name);	//��ٱ����� �н��� ���ֶ�..

	strcpy(stSoundList[stCurSelectSound].Name,entity_name);
	StripPath(entity_name);
	strcpy(stSoundList[stCurSelectSound].FileName,entity_name);
	m_LoadedSoundList.ResetContent();	//���ڿ�����Ʈ�� �ϴ� �������..
	for( i=0; i<stSoundListNum; i++)	//����� �����.
		m_LoadedSoundList.InsertString(i, stSoundList[i].Name);
	::IM_ReleaseWave(stSoundList[stCurSelectSound].id);
	stSoundList[stCurSelectSound].id = ::IM_LoadWave(name,_PLAY_WAVE_LOOP);	//�ε����� ���ϵȴ�.�ϳ��� �ε������� �ȿ��� ���̺� �����ڰ� ����ԵǹǷ� .. �������� IM_ReleaseAllWaves()ȣ�� �Ұ�.

	if( !stSoundList[stCurSelectSound].id )
	{
		return;	//�ε�����.
	}
	m_LoadedSoundList.SetCurSel(stCurSelectSound);
}


void CSoundEdit::OnCheck7()		//��ġ(������...)
{
	// TODO: Add your control notification handler code here
	stCurEditMode=_SOUND_EDIT_POS_MODE;		//pos��� 

	CheckDlgButton(IDC_CHECK7,TRUE);
	CheckDlgButton(IDC_CHECK10,FALSE);
	CheckDlgButton(IDC_CHECK15,FALSE);

	CheckDlgButton(IDC_CHECK21,FALSE);
	CheckDlgButton(IDC_CHECK29,FALSE);
	CheckDlgButton(IDC_CHECK28,FALSE);
	CheckDlgButton(IDC_CHECK12,FALSE);
}

void CSoundEdit::OnCheck10() //�ߵ鸮�� �Ÿ�...
{
	// TODO: Add your control notification handler code here
	
	stCurEditMode=_SOUND_EDIT_SCALE_MODE;		//scale��� 

	CheckDlgButton(IDC_CHECK7,FALSE);
	CheckDlgButton(IDC_CHECK10,TRUE);
	CheckDlgButton(IDC_CHECK15,FALSE);

	CheckDlgButton(IDC_CHECK21,FALSE);
	CheckDlgButton(IDC_CHECK29,FALSE);
	CheckDlgButton(IDC_CHECK28,FALSE);
	CheckDlgButton(IDC_CHECK12,FALSE);
}

void CSoundEdit::OnCheck15() //������۰Ÿ�...
{
	// TODO: Add your control notification handler code here
	
	stCurEditMode=_SOUND_EDIT_ATTN_MODE;		//������ 
	CheckDlgButton(IDC_CHECK7,FALSE);
	CheckDlgButton(IDC_CHECK10,FALSE);
	CheckDlgButton(IDC_CHECK15,TRUE);

	CheckDlgButton(IDC_CHECK21,FALSE);
	CheckDlgButton(IDC_CHECK29,FALSE);
	CheckDlgButton(IDC_CHECK28,FALSE);
	CheckDlgButton(IDC_CHECK12,FALSE);
}

void CSoundEdit::OnRadio1()		// �������
{
	// TODO: Add your control notification handler code here
	
	SetSoundMode(_SOUND_EDIT_MODE);
}

void CSoundEdit::OnRadio2()		// ���ø��...
{
	// TODO: Add your control notification handler code here
	
	SetSoundMode(_SOUND_SELECT_MODE);
}

static void CalcBBbox() //�ٿ�� �ڽ� �ǽð� ����.
{
	for( int i=0; i<stSoundEntitiesNum; i++)
	{
		stSoundEntitiesList[i].bb_min[0]=stSoundEntitiesList[i].pos[0]-stSoundEntitiesList[i].attn;
		stSoundEntitiesList[i].bb_min[1]=stSoundEntitiesList[i].pos[1]-stSoundEntitiesList[i].attn;
		stSoundEntitiesList[i].bb_min[2]=stSoundEntitiesList[i].pos[2]-stSoundEntitiesList[i].attn;

		stSoundEntitiesList[i].bb_max[0]=stSoundEntitiesList[i].pos[0]+stSoundEntitiesList[i].attn;
		stSoundEntitiesList[i].bb_max[1]=stSoundEntitiesList[i].pos[1]+stSoundEntitiesList[i].attn;
		stSoundEntitiesList[i].bb_max[2]=stSoundEntitiesList[i].pos[2]+stSoundEntitiesList[i].attn;
	}
}

static void GetRotPoint(Vector3f get,Vector3f pos,float xrot,float yrot,float zrot)
{
	float mat[4][4];

	MatrixRotate(mat,xrot,yrot,zrot);
	Vector3fTransform(get,pos,mat);
}


static void GetEntityBBox(Vector3f get_bbmin,Vector3f get_bbmax,Vector3f *bbox)	//��ƼƼ �ٿ�� �ڽ� Ʈ�������� ����.
{
	Vector3f p_min={65000,65000,65000},p_max={-65000,-65000,-65000};
	for(int i=0; i<8; i++)
	{
		p_min[0] = min(p_min[0],bbox[i][0]);
		p_min[1] = min(p_min[1],bbox[i][1]);
		p_min[2] = min(p_min[2],bbox[i][2]);
		p_max[0] = max(p_max[0],bbox[i][0]);
		p_max[1] = max(p_max[1],bbox[i][1]);
		p_max[2] = max(p_max[2],bbox[i][2]);
	}
	Vector3fCopy(p_min,get_bbmin);
	Vector3fCopy(p_max,get_bbmax);
}



#include "r3render.h"

void DrawSoundEntities()	// ���� ��. �ڽ��� �׸���.
{
	int cnt=0,i,k;
	float j;
	_D3DR3VERTEX_TEX0 vertex[1700*3];
	DWORD tc;

	if( !stSoundEntitiesNum )
		return;
    D3DXMATRIX matWorld;
    D3DXMatrixIdentity( &matWorld );
    GetD3dDevice()->SetTransform( D3DTS_WORLD, &matWorld );
    GetD3dDevice()->SetTexture( 0,NULL);
//    GetD3dDevice()->SetRenderState( D3DRS_ZENABLE, FALSE );
    GetD3dDevice()->SetRenderState( D3DRS_ZENABLE, TRUE );

	tc=(0x80ff4f00);

	CalcBBbox();
#define LineSet(v,cc) {	vertex[cnt].x = v[0];	vertex[cnt].y = v[1];	vertex[cnt].z = v[2];		vertex[cnt].color = cc; cnt++;};

	for( i=0; i<stSoundEntitiesNum; i++)
	{
//		if( !(stSoundEntitiesList[i].flag & SOUND_IS_HERE) )
//			continue;

		if( stSoundEntitiesList[i].flag & SOUND_IS_SELECTED )
			tc=(0x80ff009f);
		else
		if( stSoundEntitiesList[i].flag & SOUND_IS_FREEZE )
			tc=(0x8000ffff);
		else
			tc=(0x80ff4f00);
		Vector3f axis_pos,start_pos,temp_pos;

		if( stSoundEntitiesList[i].flag & _SOUND_FLAG_SPHERE )
		{
	#define _SPLIT_ANGLE	20

			//---------------------------- ���� ��.....
			axis_pos[0] = 0;
			axis_pos[1] = 0;
			axis_pos[2] = stSoundEntitiesList[i].scale;
			Vector3fCopy(axis_pos,start_pos);

			Vector3fAdd(start_pos,stSoundEntitiesList[i].pos,start_pos);
			for(j =_SPLIT_ANGLE; j<=360; j+=_SPLIT_ANGLE)	//x���� �����ؼ� ������ �׸���...
			{
				GetRotPoint(temp_pos,axis_pos,j,0,0);
				LineSet(start_pos,tc);
				Vector3fAdd(temp_pos,stSoundEntitiesList[i].pos,temp_pos);
				LineSet(temp_pos,tc);
				Vector3fCopy(temp_pos,start_pos);
			}
			for(j =_SPLIT_ANGLE; j<=360; j+=_SPLIT_ANGLE)	//x���� �����ؼ� ������ �׸���...
			{
				GetRotPoint(temp_pos,axis_pos,0,j,0);
				LineSet(start_pos,tc);
				Vector3fAdd(temp_pos,stSoundEntitiesList[i].pos,temp_pos);
				LineSet(temp_pos,tc);
				Vector3fCopy(temp_pos,start_pos);
			}
			axis_pos[0] = 0;
			axis_pos[1] = stSoundEntitiesList[i].scale;
			axis_pos[2] = 0;
			Vector3fCopy(axis_pos,start_pos);
			Vector3fAdd(start_pos,stSoundEntitiesList[i].pos,start_pos);

			for(j =_SPLIT_ANGLE; j<=360; j+=_SPLIT_ANGLE)	//x���� �����ؼ� ������ �׸���...
			{
				GetRotPoint(temp_pos,axis_pos,0,0,j);
				LineSet(start_pos,tc);
				Vector3fAdd(temp_pos,stSoundEntitiesList[i].pos,temp_pos);
				LineSet(temp_pos,tc);
				Vector3fCopy(temp_pos,start_pos);
			}

			//---------------------------- �ٱ��� ��.....
			if( stSoundEntitiesList[i].flag & SOUND_IS_SELECTED )
				tc=(0x80af006f);
			else
			if( stSoundEntitiesList[i].flag & SOUND_IS_FREEZE )
				tc=(0x8000bfbf);
			else
				tc=(0x80af2f00);

			axis_pos[0] = 0;
			axis_pos[1] = 0;
			axis_pos[2] = stSoundEntitiesList[i].attn;
			Vector3fCopy(axis_pos,start_pos);

			Vector3fAdd(start_pos,stSoundEntitiesList[i].pos,start_pos);
			for(j =_SPLIT_ANGLE; j<=360; j+=_SPLIT_ANGLE)	//x���� �����ؼ� ������ �׸���...
			{
				GetRotPoint(temp_pos,axis_pos,j,0,0);
				LineSet(start_pos,tc);
				Vector3fAdd(temp_pos,stSoundEntitiesList[i].pos,temp_pos);
				LineSet(temp_pos,tc);
				Vector3fCopy(temp_pos,start_pos);
			}
			for(j =_SPLIT_ANGLE; j<=360; j+=_SPLIT_ANGLE)	//x���� �����ؼ� ������ �׸���...
			{
				GetRotPoint(temp_pos,axis_pos,0,j,0);
				LineSet(start_pos,tc);
				Vector3fAdd(temp_pos,stSoundEntitiesList[i].pos,temp_pos);
				LineSet(temp_pos,tc);
				Vector3fCopy(temp_pos,start_pos);
			}
			axis_pos[0] = 0;
			axis_pos[1] = stSoundEntitiesList[i].attn;
			axis_pos[2] = 0;
			Vector3fCopy(axis_pos,start_pos);
			Vector3fAdd(start_pos,stSoundEntitiesList[i].pos,start_pos);

			for(j =_SPLIT_ANGLE; j<=360; j+=_SPLIT_ANGLE)	//x���� �����ؼ� ������ �׸���...
			{
				GetRotPoint(temp_pos,axis_pos,0,0,j);
				LineSet(start_pos,tc);
				Vector3fAdd(temp_pos,stSoundEntitiesList[i].pos,temp_pos);
				LineSet(temp_pos,tc);
				Vector3fCopy(temp_pos,start_pos);
			}
		}
		else	//�ڽ� �����̴�.
		{
			float mat[4][4],temp[4][4];
			MatrixScale(mat,stSoundEntitiesList[i].box_scale[0],stSoundEntitiesList[i].box_scale[1],stSoundEntitiesList[i].box_scale[2]);
			MatrixRotate(temp,stSoundEntitiesList[i].box_rot_x,stSoundEntitiesList[i].box_rot_y,0);
			MatrixMultiply(mat,temp,mat);
			Vector3fCopy(stSoundEntitiesList[i].pos,mat[3]);
			MatrixInvert(stSoundEntitiesList[i].mInvMat,mat);

			Vector3f b_box[8],attn_box[8],f_b_box[8],f_attn_box[8];
			b_box[0][0] = 1;
			b_box[0][1] = 1;
			b_box[0][2] = 1;
			b_box[1][0] = 1;
			b_box[1][1] = -1;
			b_box[1][2] = 1;
			b_box[2][0] = -1;
			b_box[2][1] = -1;
			b_box[2][2] = 1;
			b_box[3][0] = -1;
			b_box[3][1] = 1;
			b_box[3][2] = 1;

			b_box[4][0] = 1;
			b_box[4][1] = 1;
			b_box[4][2] = -1;
			b_box[5][0] = 1;
			b_box[5][1] = -1;
			b_box[5][2] = -1;
			b_box[6][0] = -1;
			b_box[6][1] = -1;
			b_box[6][2] = -1;
			b_box[7][0] = -1;
			b_box[7][1] = 1;
			b_box[7][2] = -1;

			for(k=0; k<8; k++)
			{
				attn_box[k][0] = b_box[k][0]*stSoundEntitiesList[i].box_attn;
				attn_box[k][1] = b_box[k][1]*stSoundEntitiesList[i].box_attn;
				attn_box[k][2] = b_box[k][2]*stSoundEntitiesList[i].box_attn;
				Vector3fTransform(f_b_box[k],b_box[k],mat);
				Vector3fTransform(f_attn_box[k],attn_box[k],mat);
			}
			GetEntityBBox(stSoundEntitiesList[i].bb_min,stSoundEntitiesList[i].bb_max,f_b_box);	//��ƼƼ �ٿ�� �ڽ� Ʈ�������� ����.
			LineSet(f_attn_box[0],tc);
			LineSet(f_attn_box[1],tc);
			LineSet(f_attn_box[1],tc);
			LineSet(f_attn_box[2],tc);
			LineSet(f_attn_box[2],tc);
			LineSet(f_attn_box[3],tc);
			LineSet(f_attn_box[3],tc);
			LineSet(f_attn_box[0],tc);


			LineSet(f_attn_box[4],tc);
			LineSet(f_attn_box[5],tc);
			LineSet(f_attn_box[5],tc);
			LineSet(f_attn_box[6],tc);
			LineSet(f_attn_box[6],tc);
			LineSet(f_attn_box[7],tc);
			LineSet(f_attn_box[7],tc);
			LineSet(f_attn_box[4],tc);

			LineSet(f_attn_box[0],tc);
			LineSet(f_attn_box[4],tc);
			LineSet(f_attn_box[1],tc);
			LineSet(f_attn_box[5],tc);
			LineSet(f_attn_box[2],tc);
			LineSet(f_attn_box[6],tc);
			LineSet(f_attn_box[3],tc);
			LineSet(f_attn_box[7],tc);
			//---------------------------- �ٱ��� �ڽ�.....
			if( stSoundEntitiesList[i].flag & SOUND_IS_SELECTED )
				tc=(0x80af006f);
			else
			if( stSoundEntitiesList[i].flag & SOUND_IS_FREEZE )
				tc=(0x8000bfbf);
			else
				tc=(0x80af2f00);
			LineSet(f_b_box[0],tc);
			LineSet(f_b_box[1],tc);
			LineSet(f_b_box[1],tc);
			LineSet(f_b_box[2],tc);
			LineSet(f_b_box[2],tc);
			LineSet(f_b_box[3],tc);
			LineSet(f_b_box[3],tc);
			LineSet(f_b_box[0],tc);


			LineSet(f_b_box[4],tc);
			LineSet(f_b_box[5],tc);
			LineSet(f_b_box[5],tc);
			LineSet(f_b_box[6],tc);
			LineSet(f_b_box[6],tc);
			LineSet(f_b_box[7],tc);
			LineSet(f_b_box[7],tc);
			LineSet(f_b_box[4],tc);

			LineSet(f_b_box[0],tc);
			LineSet(f_b_box[4],tc);
			LineSet(f_b_box[1],tc);
			LineSet(f_b_box[5],tc);
			LineSet(f_b_box[2],tc);
			LineSet(f_b_box[6],tc);
			LineSet(f_b_box[3],tc);
			LineSet(f_b_box[7],tc);
		}

		_R3ENGINE_STATE *state = GetR3State();
		Vector4f temp;
		state->mMatViewProj=state->mMatView*state->mMatProj;
		if( TransformVertex( temp, state->mMatViewProj.m,stSoundEntitiesList[i].pos)!=-8 )
		{
			temp[0]-=strlen(stSoundList[stSoundEntitiesList[i].id].FileName)*3;
			temp[1]-=10;
			DrawR3Hangul3D(temp,stSoundList[stSoundEntitiesList[i].id].FileName,0xffffffff,R3_HAN_SHADOW);
		}


		if( cnt > 1000)
		{
			GetD3dDevice()->SetTexture( 0, NULL );
			DrawPrimitiveUPLine(cnt/2,vertex);
			cnt=0;
		}
	}
	if( cnt )
	{
		GetD3dDevice()->SetTexture( 0, NULL );
		DrawPrimitiveUPLine(cnt/2,vertex);
	}
}

void CSoundEdit::OnChangeEdit2() 
{
	char buf[256];
	m_Scale.GetWindowText(buf,256);
	
	int i;
	for(i=0; i<(int)strlen(buf); i++)
	{
		if( i >= 5 || (buf[i] < '0' || buf[i] > '9') && buf[i] !='.' )
		{
			buf[i]=NULL;
			m_Scale.SetWindowText(buf);
		}
	}
	stScale = (float)atof(buf);
	if( stScale==0 )
		stScale=1;
	ChanegeSelectedEntities(stScale,stAttn,-1 );
}

void CSoundEdit::OnChangeEdit15() //���� �Ÿ� ����Ʈ....
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
	char buf[256];
	m_Attn.GetWindowText(buf,256);
	
	int i;
	for(i=0; i<(int)strlen(buf); i++)
	{
		if( i >= 5 || (buf[i] < '0' || buf[i] > '9') && buf[i] !='.' )
		{
			buf[i]=NULL;
			m_Attn.SetWindowText(buf);
		}
	}
	stAttn = (float)atof(buf);
	if( stAttn < 2 )
		stAttn = 2;
	ChanegeSelectedEntities(stScale,stAttn,-1 );
}

void CSoundEdit::OnButton1() //������.
{
	// TODO: Add your control notification handler code here
	for(int i=0; i<stSoundEntitiesNum; i++)	//���õȰ��� unfreeze ����.
	{
		if( stSoundEntitiesList[i].flag & SOUND_IS_SELECTED )
		{
			stSoundEntitiesList[i].flag &= ~SOUND_IS_SELECTED;	//������ ����.
			stSoundEntitiesList[i].flag |= SOUND_IS_FREEZE;
		}
	}
}

void CSoundEdit::OnButton2() //���������.
{
	// TODO: Add your control notification handler code here
	for(int i=0; i<stSoundEntitiesNum; i++)	//��� unfreeze ����.
	{
		stSoundEntitiesList[i].flag&=~SOUND_IS_FREEZE;
	}
}

void CSoundEdit::OnCheck18() // ��������.
{
	stUnFreezeState = !stUnFreezeState;
}

static BOOL stHearing = FALSE;
void CSoundEdit::OnCheck27()//���� ����ġ
{
	// TODO: Add your control notification handler code here
	
	stHearing = !stHearing;

	if( stCurSelectSound == -1 )
		return;

	if( stHearing )
	{
		IM_PlayWave(stSoundList[stCurSelectSound].id);
	}
	else
	{
		IM_StopWave(stSoundList[stCurSelectSound].id);
	}
}

#define PlaySoundFlag(num)			(((BYTE*)stSoundPlayFlag)[num>>3]|=(1<<(num&7)))
#define PlaySoundUnFlag(num)		(((BYTE*)stSoundPlayFlag)[num>>3]&=(~(1<<(num&7))) )	//����.
#define IsPlayedSoundFlag(num)		(((BYTE*)stSoundPlayFlag)[num>>3]&(1<<(num&7)))



void HearSoundFromTool()	//���󿡼� ��°�....
{
	int i,j,bit;

	if( stMaxSoundEntitiesNum ==0 )
		return;

	int real_cnt;

	if(	stSoundPlayFlag== NULL )
	{
		stMaxSoundPlayFlagCnt = stMaxSoundEntitiesNum;
		real_cnt=stMaxSoundPlayFlagCnt/8+1;
		stSoundPlayFlag=(BYTE *)Dmalloc(sizeof(BYTE)*real_cnt);
		memset( stSoundPlayFlag,0,sizeof(BYTE)*real_cnt);
	}
	if( stMaxSoundEntitiesNum >= stMaxSoundPlayFlagCnt )
	{
		real_cnt=stMaxSoundPlayFlagCnt/8+1;
		stSoundPlayFlag=(BYTE *)ReAlloc(stSoundPlayFlag,sizeof(BYTE)*real_cnt,sizeof(BYTE)*(real_cnt+16));
		memset( (stSoundPlayFlag+real_cnt), 0, sizeof(BYTE)*16 );
		stMaxSoundPlayFlagCnt += 64;	//bitflags�ϱ�.. 16byte�� 64��...
	}
	
//	real_cnt=stMaxSoundPlayFlagCnt/8+1;
//	memset(stSoundPlayFlag

	Vector3f pos;
	GetCameraPos(&pos);
		
	for( i=0; i<stSoundEntitiesNum; i++)
	{
		if( pos[0] > stSoundEntitiesList[i].bb_min[0] && pos[0] < stSoundEntitiesList[i].bb_max[0]
		&& pos[1] > stSoundEntitiesList[i].bb_min[1] && pos[1] < stSoundEntitiesList[i].bb_max[1]
		&& pos[2] > stSoundEntitiesList[i].bb_min[2] && pos[2] < stSoundEntitiesList[i].bb_max[2] )
		{	//��� ������ ���Դ�..

			if( stSoundEntitiesList[i].flag & _SOUND_FLAG_SPHERE ) //�����´�
			{
				float vol = stSoundEntitiesList[i].GetVolume(stSoundEntitiesList[i].pos);
				float pan;
				if( stSoundEntitiesList[i].flag & _SOUND_FLAG_PAN_OFF )
					pan = 0;
				else
					pan = stSoundEntitiesList[i].GetPan(stSoundEntitiesList[i].pos);
				if( stSoundEntitiesList[i].flag & _SOUND_FLAG_LOOP ) //�����Ǵ�.
				{
					if( !IsPlayedSoundFlag(i) ) //�÷������� �ƴ϶��..
					{
						PlaySoundFlag(i);
						IM_SetLoopCntWave(stSoundList[stSoundEntitiesList[i].id].id,0);
						IM_PlayWave(stSoundList[stSoundEntitiesList[i].id].id,vol,pan);
					}
					else	//�÷������̸� pan�� ������ �������ش�.
					{
						IM_SetWaveVolumeAndPan(stSoundList[stSoundEntitiesList[i].id].id,vol,pan);
					}
				}
				else
				{
					stSoundEntitiesList[i].next_play_time+=R3GetLoopTime();
					if( stSoundEntitiesList[i].next_play_time > stSoundEntitiesList[i].event_time )
					{
						stSoundEntitiesList[i].next_play_time = (float)((stSoundEntitiesList[i].event_time+stSoundEntitiesList[i].event_time%rand())/2);	//���� �ð��� ��´�.
						float vol = stSoundEntitiesList[i].GetVolume(stSoundEntitiesList[i].pos);
						float pan = stSoundEntitiesList[i].GetPan(stSoundEntitiesList[i].pos);
						IM_SetLoopCntWave(stSoundList[stSoundEntitiesList[i].id].id,1);
						IM_PlayWave(stSoundList[stSoundEntitiesList[i].id].id,vol,pan);
					}
				}
			}
			else		//���� �ڽ�����.
			{
				float intensity = stSoundEntitiesList[i].GetBoxIntensity(pos);

				if( intensity > 0 )
				{
					float pan;
					if( stSoundEntitiesList[i].flag & _SOUND_FLAG_PAN_OFF )
						pan = 0;
					else
						pan = stSoundEntitiesList[i].GetPan(stSoundEntitiesList[i].pos);
					if( !(stSoundEntitiesList[i].flag & _SOUND_FLAG_LOOP) ) //�����Ǵ� ���� �ƴ϶��.
					{
						stSoundEntitiesList[i].next_play_time+=R3GetLoopTime();
						if( stSoundEntitiesList[i].next_play_time > stSoundEntitiesList[i].event_time )
						{
							stSoundEntitiesList[i].next_play_time = (float)(rand()%(stSoundEntitiesList[i].event_time+1));	//���� �ð��� ��´�.
							stSoundEntitiesList[i].next_play_time = stSoundEntitiesList[i].event_time - stSoundEntitiesList[i].event_time/4.0f+stSoundEntitiesList[i].next_play_time/2.0f;
							stSoundEntitiesList[i].next_play_time = stSoundEntitiesList[i].event_time - stSoundEntitiesList[i].next_play_time;
							IM_SetLoopCntWave(stSoundList[stSoundEntitiesList[i].id].id,1);
							IM_PlayWave(stSoundList[stSoundEntitiesList[i].id].id,intensity,pan);
						}
					}
					else
					{
						float vol = stSoundEntitiesList[i].GetVolume(stSoundEntitiesList[i].pos);
						if( !IsPlayedSoundFlag(i) ) //�÷������� �ƴ϶��..
						{
							PlaySoundFlag(i);
							IM_SetLoopCntWave(stSoundList[stSoundEntitiesList[i].id].id,0);
							IM_PlayWave(stSoundList[stSoundEntitiesList[i].id].id,vol,pan);
						}
						else	//�÷������̸� pan�� ������ �������ش�.
						{
							IM_SetWaveVolumeAndPan(stSoundList[stSoundEntitiesList[i].id].id,vol,pan);
						}
					}
				}
			}
		}
	}


	// �÷������ε� ������... 
	real_cnt=stMaxSoundPlayFlagCnt/8+1;
	for(i=0 ;i< real_cnt; i++)
	{
		if( !stSoundPlayFlag[i] )
			continue;
		for(j=0,bit=0x1; j<8; j++,bit<<=1)
		{
			if( stSoundPlayFlag[i]&bit )
			{
				int entities_id = (i<<3)+j;

				if( !(stSoundEntitiesList[entities_id].flag & _SOUND_FLAG_LOOP) ) //�����Ǵ°� �ƴϴ�.
					continue;
				if( stSoundEntitiesList[entities_id].flag & _SOUND_FLAG_SPHERE ) //�����´�
				{
					if( pos[0] > stSoundEntitiesList[entities_id].bb_min[0] 
					&& pos[0] < stSoundEntitiesList[entities_id].bb_max[0]
					&& pos[1] > stSoundEntitiesList[entities_id].bb_min[1] 
					&& pos[1] < stSoundEntitiesList[entities_id].bb_max[1]
					&& pos[2] > stSoundEntitiesList[entities_id].bb_min[2] 
					&& pos[2] < stSoundEntitiesList[entities_id].bb_max[2] )
					{
					}
					else
					{
						// �÷������ε� ��� �ִ�... �׷��� ����...
						IM_StopWave(stSoundList[stSoundEntitiesList[entities_id].id].id);
						PlaySoundUnFlag(entities_id);
					}
				}
				else
				{
					float intensity = stSoundEntitiesList[entities_id].GetBoxIntensity(pos);
					if( intensity == 0 )	//�÷������ε� ����ִ�.
					{
						IM_StopWave(stSoundList[stSoundEntitiesList[entities_id].id].id);
						PlaySoundUnFlag(entities_id);
					}
				}
			}
		}
	}
}
static BOOL IsCollisionBBox(Vector3s a_min,Vector3s a_max,Vector3f b_min,Vector3f b_max)
{
	if( a_min[0] <= b_max[0] && b_min[0] <= a_max[0] 
	&& a_min[1] <= b_max[1] && b_min[1] <= a_max[1] 
	&& a_min[2] <= b_max[2] && b_min[2] <= a_max[2] )
		return TRUE;
	return FALSE;
}

void GetSoundEntityInfoInBSP(CBsp *bsp,_ADD_BSP_SAVE *add_bsp)	//�޸� ���.
{
	int i,j;

	if( stSoundListNum==0 || stSoundEntitiesNum==0 )	//�����Ұ� �����ϱ�..
		return;
	_READ_SOUND_ENTITY_LIST *sound_entity_list=(_READ_SOUND_ENTITY_LIST *)Dmalloc(sizeof(_READ_SOUND_ENTITY_LIST)*stSoundListNum );
	memset(sound_entity_list,0,sizeof(_READ_SOUND_ENTITY_LIST)*stSoundListNum );
	_READ_SOUND_ENTITIES_LIST *sound_entities_list=(_READ_SOUND_ENTITIES_LIST *)Dmalloc(sizeof(_READ_SOUND_ENTITIES_LIST)*stSoundEntitiesNum);
	memset(sound_entities_list,0,sizeof(_READ_SOUND_ENTITIES_LIST)*stSoundEntitiesNum);

	_LEAF_SOUND_ENTITIES_LIST_INFO *leaf_sound_entity_list=(_LEAF_SOUND_ENTITIES_LIST_INFO *)Dmalloc(sizeof(_LEAF_SOUND_ENTITIES_LIST_INFO)*bsp->mLeafNum );
	memset(leaf_sound_entity_list,0,sizeof(_LEAF_SOUND_ENTITIES_LIST_INFO)*bsp->mLeafNum );
	WORD *sound_entity_id=(WORD *)Dmalloc(sizeof(WORD)*65535);	//�� ���忩 ����... 

	bsp->mExtBSPHeader.SoundEntityList.size=sizeof(_READ_SOUND_ENTITY_LIST)*stSoundListNum;
	bsp->mExtBSPHeader.SoundEntitiesList.size=sizeof(_READ_SOUND_ENTITIES_LIST)*stSoundEntitiesNum;
	bsp->mExtBSPHeader.LeafSoundEntityList.size=sizeof(_LEAF_SOUND_ENTITIES_LIST_INFO)*bsp->mLeafNum;

	for(i=0; i<stSoundListNum; i++)
	{
		memcpy(sound_entity_list[i].name,stSoundList[i].Name,62);;
	}
	for(i=0; i<stSoundEntitiesNum; i++)
	{
		sound_entities_list[i].id = stSoundEntitiesList[i].id;
		sound_entities_list[i].event_time = stSoundEntitiesList[i].event_time;
		sound_entities_list[i].flag = stSoundEntitiesList[i].flag;
		sound_entities_list[i].scale = stSoundEntitiesList[i].scale;
		sound_entities_list[i].attn = stSoundEntitiesList[i].attn;
		Vector3fCopy( stSoundEntitiesList[i].pos,sound_entities_list[i].pos );

		Vector3fCopy( stSoundEntitiesList[i].box_scale,sound_entities_list[i].box_scale );
		sound_entities_list[i].box_attn = stSoundEntitiesList[i].box_attn;
		sound_entities_list[i].box_rot_x = stSoundEntitiesList[i].box_rot_x;
		sound_entities_list[i].box_rot_y = stSoundEntitiesList[i].box_rot_y;
//		sound_entities_list[i].spare = stSoundEntitiesList[i].spare;
	}

	DWORD id_cnt=0;
	for(i=1; i<(int)bsp->mLeafNum; i++)
	{
		leaf_sound_entity_list[i].start_id = (WORD)id_cnt;
		DWORD temp_id_cnt = id_cnt;
		for( j=0; j<stSoundEntitiesNum; j++)
		{
			if( IsCollisionBBox( bsp->mLeaf[i].bb_min,bsp->mLeaf[i].bb_max	//�ٿ���ڽ� ������ ���.
				,stSoundEntitiesList[j].bb_min,stSoundEntitiesList[j].bb_max) )
			{
				sound_entity_id[id_cnt]=(WORD)j;
				id_cnt++;
			}
		}
		leaf_sound_entity_list[i].entities_num = (WORD)(id_cnt - temp_id_cnt);
	}
	bsp->mExtBSPHeader.SoundEntityID.size = id_cnt*sizeof(WORD);

	add_bsp->SoundEntityList= sound_entity_list;
	add_bsp->LeafSoundEntityList =leaf_sound_entity_list;
	add_bsp->SoundEntitiesList=sound_entities_list;
	add_bsp->SoundEntityID =sound_entity_id;
}


void ReleaseSoundEntityInfoInBSP(_ADD_BSP_SAVE *add_bsp) //�޸� ����...
{
	if( add_bsp->SoundEntityList )
		Dfree( add_bsp->SoundEntityList );
	if( add_bsp->LeafSoundEntityList )
		Dfree( add_bsp->LeafSoundEntityList );
	if( add_bsp->SoundEntitiesList )
		Dfree( add_bsp->SoundEntitiesList );
	if( add_bsp->SoundEntityID )
		Dfree( add_bsp->SoundEntityID );
}

void ReleaseSoundEntity() //�޸� ����...
{
	if( stSoundList )
	{
		for(LONG i=0; i<stSoundListNum; i++)
			IM_ReleaseWave(stSoundList[i].id);
	}
	if( stSoundList )
		Dfree(stSoundList);
	if( stSoundEntitiesList )
		Dfree(stSoundEntitiesList);
	stSoundList=0;
	stSoundEntitiesList=0;
	stSoundListNum=0;
	stSoundEntitiesNum=0;
}
void LoadSoundEntityFromBSP(CBsp *bsp)
{
	int i;

	ReleaseTabSoundEntity();

	stSoundListNum = bsp->mSoundEntityListNum;	//������.

	stMaxSoundNum=stSoundListNum;
	stSoundList=(_SAVE_SOUND_LIST *)Dmalloc(sizeof(_SAVE_SOUND_LIST)*stMaxSoundNum);
	memset(stSoundList,0,sizeof(_SAVE_SOUND_LIST)*stMaxSoundNum);

	char entity_name[256];
	char full_name[256];
	char snd_path[256];
	GetPrivateProfileString("Path","SoundPath",".\\snd\\",snd_path, 256,".\\R3Engine.ini");

	for(i=0; i<stSoundListNum; i++)
	{
		memcpy(stSoundList[i].Name,bsp->mSoundEntityList[i].Name,62);	//�����̸�.

		strcpy(full_name,snd_path);
		strcat(full_name,stSoundList[i].Name);
		stSoundList[i].id = ::IM_LoadWave(full_name,_PLAY_WAVE_LOOP);	//�ε����� ���ϵȴ�.�ϳ��� �ε������� �ȿ��� ���̺� �����ڰ� ����ԵǹǷ� .. �������� IM_ReleaseAllWaves()ȣ�� �Ұ�.
		//��Ʈ�� ó��...
		strcpy(entity_name,stSoundList[i].Name);
		stEntityEdit->m_LoadedSoundList.InsertString(i, entity_name);
		StripPath(entity_name);
		strcpy(stSoundList[i].FileName,entity_name);
	}
	stEntityEdit->m_LoadedSoundList.SetCurSel(0);

	stSoundEntitiesNum = bsp->mSoundEntitiesListNum;	//������.

	stMaxSoundEntitiesNum=stSoundEntitiesNum;
	stSoundEntitiesList=(_SAVE_SOUND_ENTITIES_LIST *)Dmalloc(stMaxSoundEntitiesNum*sizeof(_SAVE_SOUND_ENTITIES_LIST));
	memset( stSoundEntitiesList,0,stMaxSoundEntitiesNum*sizeof(_SAVE_SOUND_ENTITIES_LIST));

	for( i=0; i<stSoundEntitiesNum; i++)
	{
		stSoundEntitiesList[i].id = bsp->mSoundEntitiesList[i].ID;	
		stSoundEntitiesList[i].event_time = bsp->mSoundEntitiesList[i].EventTime;	
		stSoundEntitiesList[i].flag = bsp->mSoundEntitiesList[i].Flag;
		stSoundEntitiesList[i].scale = bsp->mSoundEntitiesList[i].Scale;	//������.
		stSoundEntitiesList[i].attn	= bsp->mSoundEntitiesList[i].Attn;//����.
		Vector3fCopy(bsp->mSoundEntitiesList[i].Pos,stSoundEntitiesList[i].pos);//��ġ

		Vector3fCopy(bsp->mSoundEntitiesList[i].BoxScale,stSoundEntitiesList[i].box_scale);
		stSoundEntitiesList[i].box_attn	= bsp->mSoundEntitiesList[i].BoxAttn;//����.
		stSoundEntitiesList[i].box_rot_x = bsp->mSoundEntitiesList[i].BoxRotX;
		stSoundEntitiesList[i].box_rot_y = bsp->mSoundEntitiesList[i].BoxRotY;
	}
}


//--------------------------���� �߰�...


void CSoundEdit::OnCheck5()		//���� 
{
	CheckDlgButton(IDC_CHECK5,TRUE);
	CheckDlgButton(IDC_CHECK24,FALSE);

	stFlag = (stFlag & ~_SOUND_FLAG_EVENT)|_SOUND_FLAG_LOOP;
	ChanegeSelectedFlag(stFlag);
}

void CSoundEdit::OnCheck24()	//�̺�Ʈ 
{
	CheckDlgButton(IDC_CHECK5,FALSE);
	CheckDlgButton(IDC_CHECK24,TRUE);
	stFlag = (stFlag & ~_SOUND_FLAG_LOOP)|_SOUND_FLAG_EVENT;
	ChanegeSelectedFlag(stFlag);
}

void CSoundEdit::OnCheck22()	//Pan Off
{
	if( stFlag & _SOUND_FLAG_PAN_OFF )
		stFlag = (stFlag & ~_SOUND_FLAG_PAN_OFF);
	else
		stFlag |= _SOUND_FLAG_PAN_OFF;
	ChanegeSelectedFlag(stFlag);
}

void CSoundEdit::OnChangeEdit25()	//�̺�Ʈ �ð�.
{
	char buf[256];
	m_EventTime.GetWindowText(buf,256);
	
	int i;
	for(i=0; i<(int)strlen(buf); i++)
	{
		if( i >= 5 || (buf[i] < '0' || buf[i] > '9') && buf[i] !='.' )
		{
			buf[i]=NULL;
			m_EventTime.SetWindowText(buf);
		}
	}
	stEventTime= (WORD)atoi(buf);
	if( stEventTime==0 )
		stEventTime=1;
	ChanegeBoxSelectedEntities(stEventTime,stBoxScale,stBoxAttn,stBoxRotX,stBoxRotY);
}

void CSoundEdit::OnCheck16()	//������.
{
	CheckDlgButton(IDC_CHECK16,TRUE);
	CheckDlgButton(IDC_CHECK11,FALSE);
	stFlag = (stFlag & ~_SOUND_FLAG_BOX)|_SOUND_FLAG_SPHERE;
	ChanegeSelectedFlag(stFlag);
}

void CSoundEdit::OnCheck11()	//�ڽ� ����.
{
	CheckDlgButton(IDC_CHECK16,FALSE);
	CheckDlgButton(IDC_CHECK11,TRUE);
	stFlag = (stFlag & ~_SOUND_FLAG_SPHERE)|_SOUND_FLAG_BOX;
	ChanegeSelectedFlag(stFlag);
}


void CSoundEdit::OnCheck29()	//��ü�鸮�� ũ��.
{
	CheckDlgButton(IDC_CHECK29,TRUE);
	CheckDlgButton(IDC_CHECK12,FALSE);
	CheckDlgButton(IDC_CHECK21,FALSE);
	CheckDlgButton(IDC_CHECK28,FALSE);	

	CheckDlgButton(IDC_CHECK7,FALSE);
	CheckDlgButton(IDC_CHECK10,FALSE);
	CheckDlgButton(IDC_CHECK15,FALSE);
	stCurEditMode=_SOUND_EDIT_BOX_SCALE;
}

void CSoundEdit::OnChangeEdit22()	//��ü�鸮�� ũ�� x 
{
	char buf[256];
	m_BoxScaleX.GetWindowText(buf,256);
	
	int i;
	for(i=0; i<(int)strlen(buf); i++)
	{
		if( i >= 5 || (buf[i] < '0' || buf[i] > '9') && buf[i] !='.' )
		{
			buf[i]=NULL;
			m_BoxScaleX.SetWindowText(buf);
		}
	}
	stBoxScale[0]= (float)atof(buf);
	if( stBoxScale[0]==0 )
		stBoxScale[0]=1;
	ChanegeBoxSelectedEntities(stEventTime,stBoxScale,stBoxAttn,stBoxRotX,stBoxRotY);
}

void CSoundEdit::OnChangeEdit26() 	//��ü�鸮�� ũ�� y
{
	char buf[256];
	m_BoxScaleY.GetWindowText(buf,256);
	
	int i;
	for(i=0; i<(int)strlen(buf); i++)
	{
		if( i >= 5 || (buf[i] < '0' || buf[i] > '9') && buf[i] !='.' )
		{
			buf[i]=NULL;
			m_BoxScaleY.SetWindowText(buf);
		}
	}
	stBoxScale[1]= (float)atof(buf);
	if( stBoxScale[1]==0 )
		stBoxScale[1]=1;
	ChanegeBoxSelectedEntities(stEventTime,stBoxScale,stBoxAttn,stBoxRotX,stBoxRotY);
}

void CSoundEdit::OnChangeEdit27() 	//��ü�鸮�� ũ�� z 
{
	char buf[256];
	m_BoxScaleZ.GetWindowText(buf,256);
	
	int i;
	for(i=0; i<(int)strlen(buf); i++)
	{
		if( i >= 5 || (buf[i] < '0' || buf[i] > '9') && buf[i] !='.' )
		{
			buf[i]=NULL;
			m_BoxScaleZ.SetWindowText(buf);
		}
	}
	stBoxScale[2]= (float)atof(buf);
	if( stBoxScale[2]==0 )
		stBoxScale[2]=1;
	ChanegeBoxSelectedEntities(stEventTime,stBoxScale,stBoxAttn,stBoxRotX,stBoxRotY);
}

void CSoundEdit::OnCheck28()	//�ߵ鸲.
{
	CheckDlgButton(IDC_CHECK28,TRUE);
	CheckDlgButton(IDC_CHECK12,FALSE);
	CheckDlgButton(IDC_CHECK21,FALSE);
	CheckDlgButton(IDC_CHECK29,FALSE);	

	CheckDlgButton(IDC_CHECK7,FALSE);
	CheckDlgButton(IDC_CHECK10,FALSE);
	CheckDlgButton(IDC_CHECK15,FALSE);
	stCurEditMode=_SOUND_EDIT_BOX_ATTN_MODE;
}

void CSoundEdit::OnChangeEdit3()	//�� ������ �ۼ�Ʈ��.
{
	char buf[256];
	m_BoxAttn.GetWindowText(buf,256);
	
	int i;
	for(i=0; i<(int)strlen(buf); i++)
	{
		if( i >= 5 || (buf[i] < '0' || buf[i] > '9') && buf[i] !='.' )
		{
			buf[i]=NULL;
			m_BoxAttn.SetWindowText(buf);
		}
	}
	stBoxAttn= (float)atof(buf);
	if( stBoxAttn<=0.1f )
		stBoxAttn=0.1f;
	if( stBoxAttn>=1 )
		stBoxAttn=1;
	ChanegeBoxSelectedEntities(stEventTime,stBoxScale,stBoxAttn,stBoxRotX,stBoxRotY);
}

void CSoundEdit::OnCheck12()	//y ������Ʈ 
{
	CheckDlgButton(IDC_CHECK12,TRUE);
	CheckDlgButton(IDC_CHECK21,FALSE);
	CheckDlgButton(IDC_CHECK28,FALSE);
	CheckDlgButton(IDC_CHECK29,FALSE);

	CheckDlgButton(IDC_CHECK7,FALSE);
	CheckDlgButton(IDC_CHECK10,FALSE);
	CheckDlgButton(IDC_CHECK15,FALSE);
	stCurEditMode=_SOUND_EDIT_BOX_YROT_MODE;
	ChanegeBoxSelectedEntities(stEventTime,stBoxScale,stBoxAttn,stBoxRotX,stBoxRotY);
}

void CSoundEdit::OnChangeEdit5() 
{
	char buf[256];
	m_BoxRotY.GetWindowText(buf,256);
	
	int i;
	for(i=0; i<(int)strlen(buf); i++)
	{
		if( i >= 5 || (buf[i] < '0' || buf[i] > '9') && buf[i] !='.' && buf[i] !='-' )
		{
			buf[i]=NULL;
			m_BoxRotY.SetWindowText(buf);
		}
	}
	stBoxRotY= (float)atof(buf);
	ChanegeBoxSelectedEntities(stEventTime,stBoxScale,stBoxAttn,stBoxRotX,stBoxRotY);
}

void CSoundEdit::OnCheck21()	//x ������Ʈ 
{
	CheckDlgButton(IDC_CHECK21,TRUE);
	CheckDlgButton(IDC_CHECK29,FALSE);
	CheckDlgButton(IDC_CHECK28,FALSE);
	CheckDlgButton(IDC_CHECK12,FALSE);

	CheckDlgButton(IDC_CHECK7,FALSE);
	CheckDlgButton(IDC_CHECK10,FALSE);
	CheckDlgButton(IDC_CHECK15,FALSE);
	stCurEditMode=_SOUND_EDIT_BOX_XROT_MODE;
}

void CSoundEdit::OnChangeEdit6() 
{
	char buf[256];
	m_BoxRotX.GetWindowText(buf,256);
	
	int i;
	for(i=0; i<(int)strlen(buf); i++)
	{
		if( i >= 5 || (buf[i] < '0' || buf[i] > '9') && buf[i] !='.' && buf[i] !='-')
		{
			buf[i]=NULL;
			m_BoxRotX.SetWindowText(buf);
		}
	}
	stBoxRotX= (float)atof(buf);
	ChanegeBoxSelectedEntities(stEventTime,stBoxScale,stBoxAttn,stBoxRotX,stBoxRotY);
}

