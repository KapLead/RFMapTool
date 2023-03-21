// TabEntity.cpp : implementation file
//

#include <stdafx.h>
#include "RFMapTool.h"
#include "TabEntity.h"

#include "Entity.h"
#include "R3Particle.h"
#include "renderparticle.h"
#include "CommonUtil.h"
#include "Jmalloc.h"
#include "console.h"
#include "Jerror.h"


#include "ToolManager.h"
#include "r3Input.h"
#include "level.h"
#include "R3math.h"
#include "Core.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CEntityEdit *st_pEntityEdit;

// CEntityFile dialog
IMPLEMENT_DYNCREATE(CEntityFile, CDialog)

CEntityFile::CEntityFile(CWnd* pParent /*=NULL*/)
	: CDialog(CEntityFile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEntityFile)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CEntityFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEntityFile)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEntityFile, CDialog)
	//{{AFX_MSG_MAP(CEntityFile)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEntityFile message handlers
void CEntityFile::OnButton1()	//�� ��ƼƼ ����
{
	// TODO: Add your control notification handler code here
	LoadMapEntityFile();
}
static void FixTooDist();	//�Ÿ��� ���� �ȵǴ°��� �ִ°� �����.

void CEntityFile::OnButton2()	//�� ��ƼƼ ����.
{
	FixTooDist();	//�Ÿ��� ���� �ȵǴ°��� �ִ°� �����.
	// TODO: Add your control notification handler code here
	SaveMapEntityFile();
}

void CEntityFile::OnButton3()	//�� ��ƼƼ ���ϱ�.
{
	// TODO: Add your control notification handler code here
	
	AddMapEntityFile();
}


/////////////////////////////////////////////////////////////////////////////
// CEntityEdit dialog

IMPLEMENT_DYNCREATE(CEntityEdit, CDialog)

CEntityEdit::CEntityEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CEntityEdit::IDD, pParent)
{
	st_pEntityEdit=this;
	//{{AFX_DATA_INIT(CEntityEdit)
	//}}AFX_DATA_INIT
}

void CleanMapEntityAndEntities();	//�׵��� �ε��� ��ƼƼ�� ������.
CEntityEdit::~CEntityEdit()
{
	CleanMapEntityAndEntities();	//�׵��� �ε��� ��ƼƼ�� ������.
}

void CEntityEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEntityEdit)
	DDX_Control(pDX, IDC_EDIT22, m_PosZ);
	DDX_Control(pDX, IDC_EDIT21, m_PosY);
	DDX_Control(pDX, IDC_EDIT20, m_PosX);
	DDX_Control(pDX, IDC_EDIT19, m_Factor2);
	DDX_Control(pDX, IDC_EDIT18, m_Factor1);
	DDX_Control(pDX, IDC_EDIT6, m_EntityDisEnd);
	DDX_Control(pDX, IDC_EDIT5, m_EntityDisStart);
	DDX_Control(pDX, IDC_EDIT4, m_RotX);
	DDX_Control(pDX, IDC_EDIT3, m_RotY);
	DDX_Control(pDX, IDC_EDIT2, m_Scale);
	DDX_Control(pDX, IDC_COMBO1, m_LoadedEntityList);
	DDX_Control(pDX, IDC_COMBO2, m_LoadedShaderList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEntityEdit, CDialog)
	//{{AFX_MSG_MAP(CEntityEdit)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnSelchangeCombo2)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, OnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, OnChangeEdit4)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_EN_CHANGE(IDC_EDIT5, OnChangeEdit5)
	ON_EN_CHANGE(IDC_EDIT6, OnChangeEdit6)
	ON_BN_CLICKED(IDC_CHECK7, OnCheck7)
	ON_BN_CLICKED(IDC_CHECK10, OnCheck10)
	ON_BN_CLICKED(IDC_CHECK11, OnCheck11)
	ON_BN_CLICKED(IDC_CHECK12, OnCheck12)
	ON_BN_CLICKED(IDC_CHECK18, OnCheck18)
	ON_EN_CHANGE(IDC_EDIT18, OnChangeEdit18)
	ON_EN_CHANGE(IDC_EDIT19, OnChangeEdit19)
	ON_EN_CHANGE(IDC_EDIT20, OnChangeEdit20)
	ON_EN_CHANGE(IDC_EDIT21, OnChangeEdit21)
	ON_EN_CHANGE(IDC_EDIT22, OnChangeEdit22)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEntityEdit message handlers
/////////////////////////////////////////////////////////////////////////////




#define _MEL_VERSION	1.2f

static int stMapEntityNum=0,stMaxEntityNum=0;

//�̱���ü�� ���̺긦 ���ؼ�.
typedef struct{
	char Name[63];
	BYTE IsParticle;
	CEntity Entity;
	CParticle Particle;
	float FadeStart,FadeEnd;
	WORD Flag;			//��ƼƼ �÷���..
	WORD ShaderID;		//���̴� ���̵�	//0���� �� ����Ʈ..
	float Factor[2];	//���̴� ���͵�..
}_SAVE_ENTITY_LIST;
_SAVE_ENTITY_LIST *stEntityList;

#define ENTITY_IS_VIEW		0x80
#define ENTITY_IS_SELECTED	0x40
#define ENTITY_IS_FREEZE	0x20

#define _ENTITY_EDIT_POS_MODE	0
#define _ENTITY_EDIT_SCALE_MODE	1
#define _ENTITY_EDIT_YROT_MODE	2
#define _ENTITY_EDIT_XROT_MODE	3

#define _DEFAULT_ENTITY_FADE_START	400
#define _DEFAULT_ENTITY_FADE_END	700


static BOOL stIsCurEntity;
static Vector3f stCurEntityPos;
static int stCurSelectEntity=-1;
static DWORD stCurSelectShader=0;
static int stCurEditMode=_ENTITY_EDIT_POS_MODE;		//pos��� 
static BOOL stUnFreezeState=FALSE;



typedef struct{
	WORD id;		//��ƼƼ ���̵�.
	float scale;
	Vector3f	pos;
	float rot_x,rot_y;	//x,yȸ���� 
//------
//	BYTE is_particle;	//��ƼŬ�ΰ�?
	BYTE flag;		//��ƼƼ �÷��� 
	DWORD color;	//��ƼƼ �÷�,����.
	DWORD map_color;		//�� �÷�.
	Vector3f bb_min,bb_max;
	Vector3f rt_bb_min,rt_bb_max;	//��ƼŬ���� �ǽð����� ��ȣ�ϱ����� �ٿ�� �ڽ�
	float add_frame;	//�ִ� ���̼� �پ��ϰ� �ϱ�����.
	CParticle *particle;
}_SAVE_MAP_ENTITIES_LIST;

_SAVE_MAP_ENTITIES_LIST *stMapEntitiesList;
static int stEntitiesNum=0;
static int stMaxEntitiesNum=0;


static Vector3f mPos={0,0,0};
static float stScale=1.0f;
static float stRotX=0;
static float stRotY=0;
static float stEntityDisStart=400;
static float stEntityDisEnd=700;



static CEntityEdit *stEntityEdit=NULL; //<- onintDialog���� �־ ����Ѵ�.

#define _SHADER_LIST_NUM	3
static char *stShaderName[]={"none","grass1","grass2"};


void CopyCopyEntityList(_ENTITY_LIST *entity,_SAVE_ENTITY_LIST *s_entity)
{
	entity->Factor[0] = s_entity->Factor[0];
	entity->Factor[1] = s_entity->Factor[1];
	entity->FadeEnd = s_entity->FadeEnd;
	entity->FadeStart = s_entity->FadeStart;
	entity->Flag = s_entity->Flag;
	entity->IsFileExist = TRUE;
	entity->IsParticle = s_entity->IsParticle;
	strcpy(entity->Name,s_entity->Name);
	entity->ShaderID = s_entity->ShaderID;
}


/////////////////////////////////////////////////////////////////////////////
// CEntityEdit message handlers
BOOL IsOnInitDialog=FALSE;

BOOL CEntityEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	stEntityEdit = this;
	// TODO: Add extra initialization here
	CheckDlgButton(IDC_RADIO1,TRUE);	//����Ʈ�� ����Ʈ��..

	char buf[256];
	sprintf(buf,"%3.01f",stScale);
	m_Scale.SetWindowText(buf);
	sprintf(buf,"%3.01f",stRotY);
	m_RotY.SetWindowText(buf);
	sprintf(buf,"%3.01f",stRotX);
	m_RotX.SetWindowText(buf);

	sprintf(buf,"%3.01f",stCurEntityPos[0]);
	m_PosX.SetWindowText(buf);
	sprintf(buf,"%3.01f",stCurEntityPos[1]);
	m_PosY.SetWindowText(buf);
	sprintf(buf,"%3.01f",stCurEntityPos[2]);
	m_PosZ.SetWindowText(buf);

	CheckDlgButton(IDC_CHECK7,TRUE);//pos��� 

	m_LoadedEntityList.ResetContent();	//���ڿ�����Ʈ�� �������..
	for( int i=0; i<stMapEntityNum; i++)
	{
		m_LoadedEntityList.InsertString(i, stEntityList[i].Name);
	}
	if( stMapEntityNum )
	{
		m_LoadedEntityList.SetCurSel(0);
		stCurSelectEntity=0;
	}
	IsOnInitDialog=TRUE;

	int temp=stCurSelectEntity;
	stCurSelectEntity=-1;
	sprintf(buf,"%d",(int)stEntityDisStart);
	m_EntityDisStart.SetWindowText(buf);
	sprintf(buf,"%d",(int)stEntityDisEnd);
	m_EntityDisEnd.SetWindowText(buf);
	stCurSelectEntity=temp;


//--------���̴� �κ�..
	m_LoadedShaderList.ResetContent();	//���ڿ�����Ʈ�� �������..
	for( i=0; i<_SHADER_LIST_NUM; i++)
	{
		m_LoadedShaderList.InsertString( i, stShaderName[i] );
	}
	stCurSelectShader=0;
	m_LoadedShaderList.SetCurSel(0);

	sprintf(buf,"%3.01f",stCurSelectShader);
	m_Factor1.SetWindowText(buf);
	sprintf(buf,"%3.01f",stCurSelectShader);
	m_Factor2.SetWindowText(buf);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void SetScale(float su)
{
	char buf [256];
	sprintf(buf,"%3.01f",su);
	if( IsOnInitDialog )
		stEntityEdit->m_Scale.SetWindowText(buf);
}
void SetRotX(float su)
{
	char buf [256];
	sprintf(buf,"%3.01f",su);
	if( IsOnInitDialog )
		stEntityEdit->m_RotX.SetWindowText(buf);
}
void SetRotY(float su)
{
	char buf [256];
	sprintf(buf,"%3.01f",su);
	if( IsOnInitDialog )
		stEntityEdit->m_RotY.SetWindowText(buf);
}
void SetPos(Vector3f pos)
{
	if( !IsOnInitDialog )
		return;

	char buf [256];

	sprintf(buf,"%3.01f",pos[0]);
	stEntityEdit->m_PosX.SetWindowText(buf);
	sprintf(buf,"%3.01f",pos[1]);
	stEntityEdit->m_PosY.SetWindowText(buf);
	sprintf(buf,"%3.01f",pos[2]);
	stEntityEdit->m_PosZ.SetWindowText(buf);
}

static void GetEntityName(char *name)
{
	char temp_name[256];
	int i,cnt=0;

	for( i=0; i<(int)strlen(name); i++)
	{
		temp_name[cnt] = name[i];
		cnt++;
		if( name[i] == '\\' )
		{
			temp_name[cnt]=NULL;
			strlwr(temp_name);
			cnt=0;
			if( !strcmp("entity\\",temp_name) || !strcmp("effect\\",temp_name) )
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

void CEntityEdit::OnButton7() //��ƼƼ ����.
{
	// TODO: Add your control notification handler code here
	OPENFILENAME ofn; // zero the ofn members out
	memset(&ofn,0,sizeof(OPENFILENAME));
	char name[1256]="";

//	_R3ENGINE_STATE *state = GetR3State();
//	strcpy(name,state->EntityPath);
//	strcat(name,"test.r3e");

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = name;
	ofn.nMaxFile = sizeof(name);
	ofn.lpstrFilter = "R3 R3E ��ƼƼ�� ��ƼŬ ��ũ��Ʈ ���� (*.R3E,*.SPT)\0*.R3E;*.spt\0All (*.*)\0*.*\0";
//	ofn.lpstrFilter = "R3 R3E or ��ƼŬ SPT file (*.R3E)\0*.R3E\(*.SPT)\0*.SPT\0All (*.*)\0*.*\0";
	ofn.lpstrTitle = "Select R3E ot ��ƼŬ spt (R3 entity) file";
	ofn.Flags = OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (!GetOpenFileName(&ofn))
	{
		return;
	}
	
	//�н��� ������.
	char entity_name[256];
	int i;

	strcpy(entity_name,name);
	GetEntityName(entity_name);	//��ٱ����� �н��� ���ֶ�..

	if( strlen(entity_name) > 62 )
	{
		Warning(entity_name,"<-�н��� �ʹ���ϴ�.64�ڱ��� �Դϴ�.");
		return;
	}
	for(i=0; i<stMapEntityNum; i++)		//������ �ε����ִ� Ȯ���Ѵ�.
	{
		if( !strcmp(entity_name,stEntityList[i].Name) )
		{
			Warning(entity_name, "�̹� �ε��Ǿ� �ֽ��ϴ�.");
			return;
		}
	}

	if( stMapEntityNum==0 )	//ó���ϰ�� 
	{
		stMaxEntityNum=100;
		stEntityList=(_SAVE_ENTITY_LIST *)Dmalloc(sizeof(_SAVE_ENTITY_LIST)*stMaxEntityNum);
		memset(stEntityList,0,sizeof(_SAVE_ENTITY_LIST)*stMaxEntityNum);
	}

	if( stMaxEntityNum <= stMapEntityNum ) //��ĥ��� ���Ҵ�.
	{
		stEntityList=(_SAVE_ENTITY_LIST *)ReAlloc(stEntityList,stMaxEntityNum*sizeof(_SAVE_ENTITY_LIST),(stMaxEntityNum+20)*sizeof(_SAVE_ENTITY_LIST));
		stMaxEntityNum += 20;
	}

	memset(&stEntityList[stMapEntityNum],0,sizeof(_SAVE_ENTITY_LIST));
	strlwr(name);

	if(name[strlen(name)-3]=='s' && name[strlen(name)-2]=='p' 
		&& name[strlen(name)-1]=='t')	//���� ��ƼŬ�ϰ��.
	{
		if( !stEntityList[stMapEntityNum].Particle.LoadParticleSPT(name) )
			return;	//��ƼŬ spt�� �ƴϴ�.
		stEntityList[stMapEntityNum].Particle.InitParticle();
		stEntityList[stMapEntityNum].Particle.SetParticleState(_PARTICLE_STATE_START);
		stEntityList[stMapEntityNum].IsParticle = TRUE;
	}
	else
	if( stEntityList[stMapEntityNum].Entity.LoadEntity(name,_USE_VERTEX_SHADER|_DO_NOT_MANAGE_TEXTURE) )
//	if( stEntityList[stMapEntityNum].Entity.LoadEntity(".\\entity\\ston\\stone02\\stone02.r3e") )
	{
		stEntityList[stMapEntityNum].Entity.RestoreTexMem();
	}
	else
	{
		return;	//�ε�����.
	}

//	m_LoadedEntityList.AddString(entity_name);
	m_LoadedEntityList.InsertString(stMapEntityNum, entity_name);
	strcpy(stEntityList[stMapEntityNum].Name,entity_name);
	stEntityList[stMapEntityNum].FadeStart = _DEFAULT_ENTITY_FADE_START;
	stEntityList[stMapEntityNum].FadeEnd = _DEFAULT_ENTITY_FADE_END;
/*	if( stMapEntityNum == 0 )
	{
		m_LoadedEntityList.SetCurSel(0);
		stCurSelectEntity=0;
	}
	else
*/	{
		stCurSelectEntity = stMapEntityNum;
		m_LoadedEntityList.SetCurSel(stCurSelectEntity);
	}
	stMapEntityNum++;

}

void CleanMapEntityAndEntities()	//�׵��� �ε��� ��ƼƼ�� ������.
{
	int i;
	
	for( i=0; i<stEntitiesNum; i++)
	{
		if( stEntityList[stMapEntitiesList[i].id].IsParticle )	//���� ��ƼŬ�ϰ��.
		{
			stMapEntitiesList[i].particle->ReleaseParticle();
			delete stMapEntitiesList[i].particle;
		}
	}
	for(i=0; i<stMapEntityNum; i++)
	{
		if( stEntityList[i].IsParticle )	//��ƼŬ�ΰ��.
		{	
			if( stEntityList[i].Particle.mEntity )
			{
				stEntityList[i].Particle.mEntity->ReleaseTexMem();
				stEntityList[i].Particle.mEntity->ReleaseEntity();
				stEntityList[i].Particle.ReleaseEntity();
				stEntityList[i].Particle.mEntity=NULL;
			}
			stEntityList[i].Particle.ReleaseParticle();
		}
		else
		{
			stEntityList[i].Entity.ReleaseTexMem();	//�ؽ��� �޸𸮳���. ���� �����Ÿ� �Ⱦ�������.
			stEntityList[i].Entity.ReleaseEntity();
		}
	}
	if( stEntityList )
		Dfree(stEntityList);
	stEntityList =0;
	stMapEntityNum=0;
	stMaxEntityNum=0;

	if( stMapEntitiesList )
		Dfree( stMapEntitiesList );
	stMapEntitiesList=NULL;

	stEntitiesNum=0;
	stMaxEntitiesNum=0;

}
static void DeleteSelectedEntities()	// ���õǾ��� ��ƼƼ���� �����.
{
	while( 1 )
	{
		for(int i=0; i<stEntitiesNum; i++)	//�Ÿ��� �ճ���� ����.
		{
//			if( !(stMapEntitiesList[i].flag & ENTITY_IS_VIEW) )
//				continue;
			if( !(stMapEntitiesList[i].flag & ENTITY_IS_SELECTED) )	//���õ��� ���� ���� ����.
				continue;
			break;
		}
		if( i == stEntitiesNum )
			break;

		if( stEntityList[stMapEntitiesList[i].id].IsParticle )
			stMapEntitiesList[i].particle->ReleaseParticle();
		memcpy(&stMapEntitiesList[i],&stMapEntitiesList[i+1],sizeof(_SAVE_MAP_ENTITIES_LIST)*(stEntitiesNum-i-1));
		stEntitiesNum--;
	}
}
static void InitBBox(Vector3f bbmin,Vector3f bbmax)
{
	bbmin[0] = 65000;
	bbmin[1] = 65000;
	bbmin[2] = 65000;
	bbmax[0] = -65000;
	bbmax[1] = -65000;
	bbmax[2] = -65000;
}

static void UnSelectAllEntities()
{
	for(int i=0; i<stEntitiesNum; i++)	//��� un��������.
	{
		stMapEntitiesList[i].flag&=~ENTITY_IS_SELECTED;
	}
}
static CLevel *stLevel;

//���õ� ��ƼƼ���� ��ġ�� �ű��.
static void MoveSelectedEntities(BOOL y_axis,BOOL xz_axis,LONG x_diff,LONG y_diff )
{
#define _SXSY_TO_DIST	0.2f
	float y=(float)y_diff*_SXSY_TO_DIST;
	float x=(float)x_diff*_SXSY_TO_DIST;

	if( x == 0 && y == 0 )
		return;

	// ���õȳѵ� ī��Ʈ
	DWORD s_cnt=0;
	for(int i=0; i<stEntitiesNum; i++)	
	{
		if( (stMapEntitiesList[i].flag & ENTITY_IS_SELECTED) )	//���õ��� ���� ���� ����.
			s_cnt++;
	}

	for(i=0; i<stEntitiesNum; i++)	
	{
		if( !(stMapEntitiesList[i].flag & ENTITY_IS_SELECTED) )	//���õ��� ���� ���� ����.
			continue;
		InitBBox(stMapEntitiesList[i].rt_bb_min,stMapEntitiesList[i].rt_bb_max);
		if( y_axis )
		{
			stMapEntitiesList[i].pos[1]-=y;
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

			stMapEntitiesList[i].pos[0]+=(billboard.m[0][0]*x+billboard.m[0][2]*-y);
			stMapEntitiesList[i].pos[2]+=(billboard.m[2][0]*x+billboard.m[2][2]*-y);
		}
		if( s_cnt==1 )
		{
			stCurEntityPos[0]=stMapEntitiesList[i].pos[0];
			stCurEntityPos[1]=stMapEntitiesList[i].pos[1];
			stCurEntityPos[2]=stMapEntitiesList[i].pos[2];
			SetPos(stCurEntityPos);
		}
		if( stLevel )
			stMapEntitiesList[i].map_color = stLevel->mBsp->GetLightFromPoint(stMapEntitiesList[i].pos,30);
	}
}
static void ChanegeSelectedEntities(float scale,float rot_y,float rot_x )
{
	for(int i=0; i<stEntitiesNum; i++)	
	{
		if( !(stMapEntitiesList[i].flag & ENTITY_IS_SELECTED) )	//���õ��� ���� ���� ����.
			continue;
		InitBBox(stMapEntitiesList[i].rt_bb_min,stMapEntitiesList[i].rt_bb_max);
		if( scale > 0 )
		{
			stMapEntitiesList[i].scale=scale;
		}
		if( rot_y >= 0 )
		{
			stMapEntitiesList[i].rot_y=rot_y;
		}
		if( rot_x >= 0 )
		{
			stMapEntitiesList[i].rot_x=rot_x;
		}			
	}
}
static void ChanegeSelectedEntitiesPos(float x ,float y ,float z )
{
	// ���õȳѵ� ī��Ʈ
	DWORD s_cnt=0;
	for(int i=0; i<stEntitiesNum; i++)	
	{
		if( (stMapEntitiesList[i].flag & ENTITY_IS_SELECTED) )	//���õ��� ���� ���� ����.
			s_cnt++;
	}
	if( s_cnt != 1 )	//1���� ���É����� �ٲ۴�.
		return;

	for( i=0; i<stEntitiesNum; i++)	
	{
		if( !(stMapEntitiesList[i].flag & ENTITY_IS_SELECTED) )	//���õ��� ���� ���� ����.
			continue;
		InitBBox(stMapEntitiesList[i].rt_bb_min,stMapEntitiesList[i].rt_bb_max);
		if( x!= -65535 )
			stMapEntitiesList[i].pos[0]=x;
		if( y!= -65535 )
			stMapEntitiesList[i].pos[1]=y;
		if( z!= -65535 )
			stMapEntitiesList[i].pos[2]=z;
	}
}

static void GetTransPoint(Vector3f get_pos,float mat[4][4],Vector3f src_pos)	//��ƼƼ �ٿ�� �ڽ� Ʈ�������� ����.
{
	get_pos[0] = mat[0][0]*src_pos[0]+mat[1][0]*src_pos[1]+mat[2][0]*src_pos[2]+mat[3][0];
	get_pos[1] = mat[0][1]*src_pos[0]+mat[1][1]*src_pos[1]+mat[2][1]*src_pos[2]+mat[3][1];
	get_pos[2] = mat[0][2]*src_pos[0]+mat[1][2]*src_pos[1]+mat[2][2]*src_pos[2]+mat[3][2];
}

static void GetEntityBBox(Vector3f get_bbmin,Vector3f get_bbmax,float mat[4][4],Vector3f src_bbmin,Vector3f src_bbmax)	//��ƼƼ �ٿ�� �ڽ� Ʈ�������� ����.
{
	Vector3f pos[8];
	Vector3f get_pos[8];
	pos[0][0] = src_bbmin[0];	pos[0][1] = src_bbmin[1];	pos[0][2] = src_bbmin[2];
	pos[1][0] = src_bbmax[0];	pos[1][1] = src_bbmin[1];	pos[1][2] = src_bbmin[2];
	pos[2][0] = src_bbmin[0];	pos[2][1] = src_bbmin[1];	pos[2][2] = src_bbmax[2];
	pos[3][0] = src_bbmax[0];	pos[3][1] = src_bbmin[1];	pos[3][2] = src_bbmax[2];

	pos[4][0] = src_bbmin[0];	pos[4][1] = src_bbmax[1];	pos[4][2] = src_bbmin[2];
	pos[5][0] = src_bbmax[0];	pos[5][1] = src_bbmax[1];	pos[5][2] = src_bbmin[2];
	pos[6][0] = src_bbmin[0];	pos[6][1] = src_bbmax[1];	pos[6][2] = src_bbmax[2];
	pos[7][0] = src_bbmax[0];	pos[7][1] = src_bbmax[1];	pos[7][2] = src_bbmax[2];

	Vector3f p_min={65000,65000,65000},p_max={-65000,-65000,-65000};
	for(int i=0; i<8; i++)
	{
		GetTransPoint(get_pos[i],mat,pos[i]);
		p_min[0] = min(p_min[0],get_pos[i][0]);
		p_min[1] = min(p_min[1],get_pos[i][1]);
		p_min[2] = min(p_min[2],get_pos[i][2]);
		p_max[0] = max(p_max[0],get_pos[i][0]);
		p_max[1] = max(p_max[1],get_pos[i][1]);
		p_max[2] = max(p_max[2],get_pos[i][2]);
	}
	Vector3fCopy(p_min,get_bbmin);
	Vector3fCopy(p_max,get_bbmax);
}

static void SelectGAll(int entity_id)	//�Ѱ��� id�� ��� �����Ѵ�.
{

	for(int i=0; i<stEntitiesNum; i++)	
	{
		if( stMapEntitiesList[i].flag & ENTITY_IS_FREEZE )	//������ȰŴ� ����.
			continue;
		if( stMapEntitiesList[i].id == entity_id )
			stMapEntitiesList[i].flag |= ENTITY_IS_SELECTED;
	}
}


static BOOL IsNearDistExist(Vector3f pos)	//��ƼƼ�� �ʹ� ��ó�� �ִ°��.
{
	Vector3f temp;
	for(int i=0; i<stEntitiesNum; i++)
	{
		Vector3fSub(stMapEntitiesList[i].pos,pos,temp);
		if( Vector3fDist(temp) < 3 )
			return TRUE;
	}
	return FALSE;
}


static int SelectOneEntity(LONG x,LONG y,CLevel *level,Vector3f cam_pos)	//���� ����� ��ƼƼ�� �����Ѵ�.
{
	Vector3f temp;
	int id=-1;
	float dist=100000,temp_dist;

	int state = GetSelectState();

	for(int i=0; i<stEntitiesNum; i++)	//�Ÿ��� �ճ���� ����.
	{
		if( !(stMapEntitiesList[i].flag & ENTITY_IS_VIEW) )
			continue;
		if( stMapEntitiesList[i].flag & ENTITY_IS_FREEZE ) //������ȰŴ� ����..
			continue;
		if( state > 0 )	//add�ϰ�� 
		{
			if( (stMapEntitiesList[i].flag & ENTITY_IS_SELECTED) )	//���õ� ���� ����.
				continue;
		}
		else
		if( state < 0 )	//������ ��� 
		{
			if( !(stMapEntitiesList[i].flag & ENTITY_IS_SELECTED) )	//���þȵ� ���� ����.
				continue;
		}
		else
		{
			//stMapEntitiesList[i].flag&=~ENTITY_IS_SELECTED;
		}

		if( level->IsCollisionRayAABB(x,y,stMapEntitiesList[i].bb_min,stMapEntitiesList[i].bb_max,&temp))
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
			stMapEntitiesList[id].flag |= ENTITY_IS_SELECTED;
		else
		if( state < 0 )	//������ ��� 
			stMapEntitiesList[id].flag&=~ENTITY_IS_SELECTED;
		else
		{
			UnSelectAllEntities();
			stMapEntitiesList[id].flag |= ENTITY_IS_SELECTED;
			stCurSelectEntity = stMapEntitiesList[id].id;
			stEntityEdit->m_LoadedEntityList.SetCurSel(stCurSelectEntity);

			st_pEntityEdit->OnSelchangeCombo1();
/*
			stEntityDisStart = stEntityList[stCurSelectEntity].FadeStart;
			stEntityDisEnd = stEntityList[stCurSelectEntity].FadeEnd;
			
			char buf[256];
			sprintf(buf,"%d",(int)stEntityDisStart);
			m_EntityDisStart.SetWindowText(buf);
			sprintf(buf,"%d",(int)stEntityDisEnd);
			m_EntityDisEnd.SetWindowText(buf);
*/
		}
	}
	return id;
}

static int UnFreezeOneEntity(LONG x,LONG y,CLevel *level,Vector3f cam_pos)	//���� ����� ������� ��ƼƼ�� �����Ѵ�.
{
	Vector3f temp;
	int id=-1;
	float dist=100000,temp_dist;

	for(int i=0; i<stEntitiesNum; i++)	//�Ÿ��� �ճ���� ����.
	{
		if( !(stMapEntitiesList[i].flag & ENTITY_IS_VIEW) )
			continue;
		if( !(stMapEntitiesList[i].flag & ENTITY_IS_FREEZE) ) //������� �ȵȰŴ� ����..
			continue;
		if( level->IsCollisionRayAABB(x,y,stMapEntitiesList[i].bb_min,stMapEntitiesList[i].bb_max,&temp))
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
		stMapEntitiesList[id].flag &= ~ENTITY_IS_FREEZE;
	}
	return id;
}

static BOOL stOneClickEntity=FALSE;
static LONG stOldEntityMx,stOldEntityMy;
void ProgressEntityEdit(void *v_level,Vector3f cam_pos)
{
	Vector3f pos;

	if( v_level == NULL )
		return;

	stLevel = (CLevel *)v_level;
	LONG mouse_x = GetRectMouseX(gMouse.x);	//�ػ󵵿����缭..
	LONG mouse_y = GetRectMouseY(gMouse.y);	//�ػ󵵿� ���缭..
	CLevel *level = (CLevel *)v_level;
	CBsp *bsp = level->mBsp;
	int what_mode = WhatEntityMode();

	if( !level->IsLoadedBsp() )
		return;

	BOOL is_coll;

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
			if( what_mode == _ENTITY_EDIT_MODE )
			{
				if(stCurEditMode == _ENTITY_EDIT_POS_MODE )
				{
					if( stMapEntitiesList == NULL )
					{
						stMaxEntitiesNum=4000;
						stMapEntitiesList=(_SAVE_MAP_ENTITIES_LIST *)Dmalloc(stMaxEntitiesNum*sizeof(_SAVE_MAP_ENTITIES_LIST));
					}
					if( IsNearDistExist(stCurEntityPos) )	//��ƼƼ�� �ʹ� ��ó�� �ִ°��.
					{
						Warning("���� ��ġ�� ���� ������!","");
					}
					else
					if( stCurSelectEntity != -1 )
					{
	//					stMapEntitiesList[stEntitiesNum].is_particle=FALSE;
						stMapEntitiesList[stEntitiesNum].flag=0;
						stMapEntitiesList[stEntitiesNum].id = stCurSelectEntity;
						Vector3fCopy(stCurEntityPos,stMapEntitiesList[stEntitiesNum].pos);
						stMapEntitiesList[stEntitiesNum].rot_x =stRotX;
						stMapEntitiesList[stEntitiesNum].rot_y =stRotY;
						stMapEntitiesList[stEntitiesNum].scale =stScale;
						stMapEntitiesList[stEntitiesNum].map_color = 0xff808080;
						
						if( stEntityList[stCurSelectEntity].IsParticle )//��ƼŬ�ΰ�?
						{
							InitBBox(stMapEntitiesList[stEntitiesNum].rt_bb_min,stMapEntitiesList[stEntitiesNum].rt_bb_max);
							CParticle *particle=new CParticle;
							stMapEntitiesList[stEntitiesNum].particle = particle;
							memcpy(particle,&stEntityList[stCurSelectEntity].Particle,sizeof(CParticle));
							particle->InitParticle();
						}
						else
						{
							stMapEntitiesList[stEntitiesNum].map_color = level->mBsp->GetLightFromPoint(stMapEntitiesList[stEntitiesNum].pos,30);
							stMapEntitiesList[stEntitiesNum].particle=NULL;
						}

						/*
						if( stEntityList[stCurSelectEntity].Entity.mObjectNum )
						{
							stMapEntitiesList[stEntitiesNum].add_frame 
								= (float)(rand()%stEntityList[stCurSelectEntity].Entity.mObject[0].frames);
						}
						else
							stMapEntitiesList[stEntitiesNum].add_frame=0;
							*/
						stMapEntitiesList[stEntitiesNum].add_frame=(rand()%256)/4.0f;

						stEntitiesNum++;
						if( stEntitiesNum >= stMaxEntitiesNum )
						{
							stMapEntitiesList=(_SAVE_MAP_ENTITIES_LIST *)ReAlloc(stMapEntitiesList,stMaxEntitiesNum*sizeof(_SAVE_MAP_ENTITIES_LIST),(stMaxEntitiesNum+512)*sizeof(_SAVE_MAP_ENTITIES_LIST));
							stMaxEntitiesNum+=512;
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
						stRotX =stMapEntitiesList[id].rot_x;
						stRotY =stMapEntitiesList[id].rot_y;
						stScale=stMapEntitiesList[id].scale;
						//stCurEntityPos[0] = stMapEntitiesList[id].pos[0];
						//stCurEntityPos[1] = stMapEntitiesList[id].pos[1];
						//stCurEntityPos[2] = stMapEntitiesList[id].pos[2];
						SetScale(stScale);
						SetRotY(stRotY);
						SetRotX(stRotX);
						SetPos(stMapEntitiesList[id].pos);
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
		case _ENTITY_EDIT_POS_MODE:
			if( what_mode == _ENTITY_SELECT_MODE )
				MoveSelectedEntities(GetYButton(),GetXZButton(),mouse_x-stOldEntityMx
					,mouse_y-stOldEntityMy);
			break;
		case _ENTITY_EDIT_SCALE_MODE:
			stScale += (mouse_y-stOldEntityMy)/100.0f;
			if( stScale <0.1f )
				stScale=0.1f;
			SetScale(stScale);
			ChanegeSelectedEntities(stScale,-1,-1 );
			break;
		case _ENTITY_EDIT_YROT_MODE:
			stRotY += (mouse_y-stOldEntityMy)/3.0f;
			if( stRotY <0 )
				stRotY += 360;
			if( stRotY > 360 )
				stRotY -= 360;
			SetRotY(stRotY);
			ChanegeSelectedEntities(-1,stRotY,-1 );
			break;
		case _ENTITY_EDIT_XROT_MODE:
			stRotX += (mouse_y-stOldEntityMy)/3.0f;
			if( stRotX <0 )
				stRotX += 360;
			if( stRotX > 360 )
				stRotX -= 360;
			SetRotX(stRotX);
			ChanegeSelectedEntities(-1,-1,stRotX );
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

static void AdjustBBoxForPatricle(Vector3f get_bb_min,Vector3f get_bb_max,Vector3f bb_min,Vector3f bb_max)
{
	int i;

	for(i=0; i<3; i++)
	{
		get_bb_min[i] = min(get_bb_min[i],bb_min[i] );
		get_bb_max[i] = max(get_bb_max[i],bb_max[i] );
	}
}

#include "R3render.h"
void DrawEntities(Vector3f camera_pos)	//��ƼƼ�� �׸���.
{
	DWORD alpha;
	float dist;
	Vector3f pos;


	float min_dist = 400;
	float max_dist = 700;

	if( !stEntityList )
		return;

//	MutilTexOff();
	for(int i=0; i<stEntitiesNum; i++)	//�Ÿ��� �ճ���� ����.
	{
		stMapEntitiesList[i].flag &= (~ENTITY_IS_VIEW);
		Vector3fSub( camera_pos,stMapEntitiesList[i].pos,pos );
		dist = Vector3fDist( pos );
		max_dist=stEntityList[stMapEntitiesList[i].id].FadeEnd;
		if( dist > max_dist )
			continue;
		min_dist=stEntityList[stMapEntitiesList[i].id].FadeStart;

		stMapEntitiesList[i].flag |= ENTITY_IS_VIEW;
		dist = max(dist,min_dist);
		alpha = (DWORD)((1-(dist - min_dist)/(max_dist - min_dist))*255);
//		stMapEntitiesList[i].color=(0x007f7f7f|(alpha<<24));
		stMapEntitiesList[i].color=(0x00ffffff|(alpha<<24));
	}

	float w_matrix[4][4];
	float s_matrix[4][4];//������ ��Ʈ����
	float r_matrix[4][4];//������Ʈ ��Ʈ����
	for( i=0; i<stEntitiesNum; i++)
	{
		if( !(stMapEntitiesList[i].flag & ENTITY_IS_VIEW) )
			continue;

		MatrixIdentity(w_matrix);
		MatrixScale(s_matrix,stMapEntitiesList[i].scale,stMapEntitiesList[i].scale,stMapEntitiesList[i].scale);
		MatrixRotate(r_matrix,stMapEntitiesList[i].rot_x,stMapEntitiesList[i].rot_y,0);
		MatrixMultiply(w_matrix,s_matrix,w_matrix);
		MatrixMultiply(w_matrix,r_matrix,w_matrix);
		w_matrix[3][0] = stMapEntitiesList[i].pos[0];
		w_matrix[3][1] = stMapEntitiesList[i].pos[1];
		w_matrix[3][2] = stMapEntitiesList[i].pos[2];

		//�ִϸ��̼��� Ʋ���� �ϱ����� �۾�....
		stEntityList[stMapEntitiesList[i].id].Entity.mAddFrame = stMapEntitiesList[i].add_frame;
		
		DWORD color;
		if( stMapEntitiesList[i].flag & ENTITY_IS_SELECTED )
			color = (stMapEntitiesList[i].color&0xff000000)|0x00ff009f;
		else
		if( stMapEntitiesList[i].flag & ENTITY_IS_FREEZE )
			color = (stMapEntitiesList[i].color&0xff000000)|0x0000ffff;
		else
			color = stMapEntitiesList[i].color;

		if( stEntityList[stMapEntitiesList[i].id].IsParticle )	//��ƼŬ�ΰ��.
		{
 			CParticle *particle = stMapEntitiesList[i].particle;
			
			particle->SetCreatePos(w_matrix[3]);
			if( particle->Loop() ==0 )
				return;
			MatrixCopy(particle->mRotMat,r_matrix);
			DrawCParticle(particle,particle->mEntity,w_matrix,color);	//��ƼŬ�ϰ�� ��ƼŬ�� �ѷ�����.
			Vector3f bb_min,bb_max;
			particle->GetBBox(bb_min,bb_max);

			//�������� �Ǵ°�� �ٿ�� �ڽ��� �ǽð����� �����ؾߵǹǷ�...
			//particle->GetBBox(stMapEntitiesList[i].rt_bb_min,stMapEntitiesList[i].rt_bb_max);
			AdjustBBoxForPatricle(stMapEntitiesList[i].rt_bb_min,stMapEntitiesList[i].rt_bb_max
				,bb_min,bb_max);
			GetEntityBBox(stMapEntitiesList[i].bb_min,stMapEntitiesList[i].bb_max,w_matrix
				,stMapEntitiesList[i].rt_bb_min,stMapEntitiesList[i].rt_bb_max);
		}
		else
		{
			GetEntityBBox(stMapEntitiesList[i].bb_min,stMapEntitiesList[i].bb_max,w_matrix
				,stEntityList[stMapEntitiesList[i].id].Entity.mBBMin,stEntityList[stMapEntitiesList[i].id].Entity.mBBMax);
			stEntityList[stMapEntitiesList[i].id].Entity.SetMapColor(stMapEntitiesList[i].map_color);
			stEntityList[stMapEntitiesList[i].id].Entity.mFlag|=_ENTITY_MAP_COLOR;
			if( stEntityList[stMapEntitiesList[i].id].ShaderID )
			{
				_ENTITY_LIST t_entity;
				CopyCopyEntityList(&t_entity,&stEntityList[stMapEntitiesList[i].id]);
				stEntityList[stMapEntitiesList[i].id].Entity.DrawEntityVS(&t_entity,w_matrix,color);
			}
			else
				stEntityList[stMapEntitiesList[i].id].Entity.DrawEntity(w_matrix,color);
		}
	}
}
#include "r3render.h"

void DrawEntitiesBBox()	//DrawEntities() ȣ���� ��ƼƼ �ٿ�� �ڽ��� �׸���.
{
	int cnt=0,i;
	_D3DR3VERTEX_TEX0 vertex[1000*3];
	DWORD tc;

	if( !stEntitiesNum )
		return;
    D3DXMATRIX matWorld;
    D3DXMatrixIdentity( &matWorld );
    GetD3dDevice()->SetTransform( D3DTS_WORLD, &matWorld );
    GetD3dDevice()->SetTexture( 0,NULL);
//    GetD3dDevice()->SetRenderState( D3DRS_ZENABLE, FALSE );

	tc=(0x80ff4f00);

#define LineSet(xx,yy,zz,cc) {	vertex[cnt].x = xx;	vertex[cnt].y = yy;	vertex[cnt].z = zz;		vertex[cnt].color = cc; cnt++;};

	for( i=0; i<stEntitiesNum; i++)
	{
		if( !(stMapEntitiesList[i].flag & ENTITY_IS_VIEW) )
			continue;

		if( stMapEntitiesList[i].flag & ENTITY_IS_SELECTED )
			tc=(0x80ff009f);
		else
		if( stMapEntitiesList[i].flag & ENTITY_IS_FREEZE )
			tc=(0x8000ffff);
		else
			tc=(0x80ff4f00);

		LineSet(stMapEntitiesList[i].bb_min[0],stMapEntitiesList[i].bb_min[1],stMapEntitiesList[i].bb_min[2],tc);
		LineSet(stMapEntitiesList[i].bb_max[0],stMapEntitiesList[i].bb_min[1],stMapEntitiesList[i].bb_min[2],tc);
		LineSet(stMapEntitiesList[i].bb_min[0],stMapEntitiesList[i].bb_min[1],stMapEntitiesList[i].bb_min[2],tc);
		LineSet(stMapEntitiesList[i].bb_min[0],stMapEntitiesList[i].bb_min[1],stMapEntitiesList[i].bb_max[2],tc);
		LineSet(stMapEntitiesList[i].bb_max[0],stMapEntitiesList[i].bb_min[1],stMapEntitiesList[i].bb_min[2],tc);
		LineSet(stMapEntitiesList[i].bb_max[0],stMapEntitiesList[i].bb_min[1],stMapEntitiesList[i].bb_max[2],tc);
		LineSet(stMapEntitiesList[i].bb_min[0],stMapEntitiesList[i].bb_min[1],stMapEntitiesList[i].bb_max[2],tc);
		LineSet(stMapEntitiesList[i].bb_max[0],stMapEntitiesList[i].bb_min[1],stMapEntitiesList[i].bb_max[2],tc);

		LineSet(stMapEntitiesList[i].bb_min[0],stMapEntitiesList[i].bb_max[1],stMapEntitiesList[i].bb_min[2],tc);
		LineSet(stMapEntitiesList[i].bb_max[0],stMapEntitiesList[i].bb_max[1],stMapEntitiesList[i].bb_min[2],tc);
		LineSet(stMapEntitiesList[i].bb_min[0],stMapEntitiesList[i].bb_max[1],stMapEntitiesList[i].bb_min[2],tc);
		LineSet(stMapEntitiesList[i].bb_min[0],stMapEntitiesList[i].bb_max[1],stMapEntitiesList[i].bb_max[2],tc);
		LineSet(stMapEntitiesList[i].bb_max[0],stMapEntitiesList[i].bb_max[1],stMapEntitiesList[i].bb_min[2],tc);
		LineSet(stMapEntitiesList[i].bb_max[0],stMapEntitiesList[i].bb_max[1],stMapEntitiesList[i].bb_max[2],tc);
		LineSet(stMapEntitiesList[i].bb_min[0],stMapEntitiesList[i].bb_max[1],stMapEntitiesList[i].bb_max[2],tc);
		LineSet(stMapEntitiesList[i].bb_max[0],stMapEntitiesList[i].bb_max[1],stMapEntitiesList[i].bb_max[2],tc);

		LineSet(stMapEntitiesList[i].bb_min[0],stMapEntitiesList[i].bb_min[1],stMapEntitiesList[i].bb_min[2],tc);
		LineSet(stMapEntitiesList[i].bb_min[0],stMapEntitiesList[i].bb_max[1],stMapEntitiesList[i].bb_min[2],tc);
		LineSet(stMapEntitiesList[i].bb_max[0],stMapEntitiesList[i].bb_min[1],stMapEntitiesList[i].bb_min[2],tc);
		LineSet(stMapEntitiesList[i].bb_max[0],stMapEntitiesList[i].bb_max[1],stMapEntitiesList[i].bb_min[2],tc);
		LineSet(stMapEntitiesList[i].bb_min[0],stMapEntitiesList[i].bb_min[1],stMapEntitiesList[i].bb_max[2],tc);
		LineSet(stMapEntitiesList[i].bb_min[0],stMapEntitiesList[i].bb_max[1],stMapEntitiesList[i].bb_max[2],tc);
		LineSet(stMapEntitiesList[i].bb_max[0],stMapEntitiesList[i].bb_min[1],stMapEntitiesList[i].bb_max[2],tc);
		LineSet(stMapEntitiesList[i].bb_max[0],stMapEntitiesList[i].bb_max[1],stMapEntitiesList[i].bb_max[2],tc);

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

void DrawCursorEntity()
{
	if( !stEntityList )
		return;
	if( WhatEntityMode() != _ENTITY_EDIT_MODE )
		return;
	if( stCurSelectEntity == -1 )
		return;
//	if( stCurEditMode != 0 )	//pos�� �ƴϸ� 

	if( stIsCurEntity )
	{
		float w_matrix[4][4];
		float s_matrix[4][4];//������ ��Ʈ����
		float r_matrix[4][4];//������Ʈ ��Ʈ����
		MatrixIdentity(w_matrix);
		MatrixScale(s_matrix,stScale,stScale,stScale);
		MatrixRotate(r_matrix,stRotX,stRotY,0);
		MatrixMultiply(w_matrix,s_matrix,w_matrix);
		MatrixMultiply(w_matrix,r_matrix,w_matrix);
		w_matrix[3][0] = stCurEntityPos[0];
		w_matrix[3][1] = stCurEntityPos[1];
		w_matrix[3][2] = stCurEntityPos[2];
 
		if( stEntityList[stCurSelectEntity].IsParticle )	//��ƼŬ�ΰ��.
		{
 			CParticle *particle = &stEntityList[stCurSelectEntity].Particle;
			particle->SetCreatePos(w_matrix[3]);
			if( particle->Loop() ==0 )
				return;
			MatrixCopy(particle->mRotMat,r_matrix);
			DrawCParticle(particle,particle->mEntity,w_matrix,0x8fffffff);	//��ƼŬ�ϰ�� ��ƼŬ�� �ѷ�����.
		}
		else
		{
			if( stEntityList[stCurSelectEntity].ShaderID )
			{
				_ENTITY_LIST t_entity;
				CopyCopyEntityList(&t_entity,&stEntityList[stCurSelectEntity]);
				stEntityList[stCurSelectEntity].Entity.DrawEntityVS(&t_entity,w_matrix,0x8fffffff);
			}
			else
				stEntityList[stCurSelectEntity].Entity.DrawEntity(w_matrix,0x8fffffff);
		}
	}
}

void CEntityEdit::OnRadio1()		//������� 
{
	// TODO: Add your control notification handler code here
	SetEntityMode(_ENTITY_EDIT_MODE);
}


void CEntityEdit::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	
	SetEntityMode(_ENTITY_SELECT_MODE);
}



void CEntityEdit::OnSelchangeCombo1() //��ƼƼ ������ ����.
{
	// TODO: Add your control notification handler code here
	stCurSelectEntity = m_LoadedEntityList.GetCurSel();
	if( stCurSelectEntity == -1 )	//����Ʈ�� �ϳ�.
		return;
	stEntityDisStart = stEntityList[stCurSelectEntity].FadeStart;
	stEntityDisEnd = stEntityList[stCurSelectEntity].FadeEnd;

	int temp = stCurSelectEntity;
	stCurSelectEntity=-1;

	char buf[256];
	sprintf(buf,"%d",(int)stEntityDisStart);
	m_EntityDisStart.SetWindowText(buf);
	sprintf(buf,"%d",(int)stEntityDisEnd);
	m_EntityDisEnd.SetWindowText(buf);
	stCurSelectEntity = temp;
//	stCurSelectEntity = stMapEntityNum-stCurSelectEntity-1;


	//���̴� �κ� 
	m_LoadedShaderList.SetCurSel(stEntityList[stCurSelectEntity].ShaderID);
	sprintf(buf,"%3.01f",stEntityList[stCurSelectEntity].Factor[0]);
	m_Factor1.SetWindowText(buf);
	sprintf(buf,"%3.01f",stEntityList[stCurSelectEntity].Factor[1]);
	m_Factor2.SetWindowText(buf);

}

void CEntityEdit::OnChangeEdit2() //������
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
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
	ChanegeSelectedEntities(stScale,-1,-1 );
}

void CEntityEdit::OnChangeEdit3() //rot Y
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
	char buf[256];
	m_RotY.GetWindowText(buf,256);

	int i;
	for(i=0; i<(int)strlen(buf); i++)
	{
		if( i >= 6 || (buf[i] < '0' || buf[i] > '9') && buf[i] !='.'  )
		{
			buf[i]=NULL;
			m_RotY.SetWindowText(buf);
		}
	}
	stRotY = (float)atof(buf);
	if( stRotY >= 360 )
	{
		m_RotY.SetWindowText("359");
	}
	ChanegeSelectedEntities(-1,stRotY,-1 );
}

void CEntityEdit::OnChangeEdit4() // rot X
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
	char buf[256];
	m_RotX.GetWindowText(buf,256);

	int i;
	for(i=0; i<(int)strlen(buf); i++)
	{
		if( i >= 6 || (buf[i] < '0' || buf[i] > '9') && buf[i] !='.'  )
		{
			buf[i]=NULL;
			m_RotX.SetWindowText(buf);
		}
	}
	stRotX = (float)atof(buf);
	if( stRotX >= 360 )
	{
		m_RotX.SetWindowText("359");
	}
	ChanegeSelectedEntities(-1,-1,stRotX );
}

void CEntityEdit::OnChangeEdit5()	// ������� �Ÿ� ����.
{
	char buf[256];
	m_EntityDisStart.GetWindowText(buf,256);

	if( stCurSelectEntity < 0 )
		return;
	int i;
	for(i=0; i<(int)strlen(buf); i++)
	{
		if( i >= 6 || (buf[i] < '0' || buf[i] > '9') )
		{
			buf[i]=NULL;
			m_EntityDisStart.SetWindowText(buf);
		}
	}
	stEntityDisStart = (float)atoi(buf);
/*
	if( stEntityDisStart < 100 )	//���� 10����
	{
		m_EntityDisStart.SetWindowText("100");
		stEntityDisStart=100;
	}*/
	if( stEntityDisStart < 10 )
		stEntityList[stCurSelectEntity].FadeStart = 10;
	else
		stEntityList[stCurSelectEntity].FadeStart = stEntityDisStart;
}

void CEntityEdit::OnChangeEdit6()	//������� �Ÿ� ��.
{
	char buf[256];
	m_EntityDisEnd.GetWindowText(buf,256);

	if( stCurSelectEntity < 0 )
		return;
	int i;
	for(i=0; i<(int)strlen(buf); i++)
	{
		if( i >= 6 || (buf[i] < '0' || buf[i] > '9') )
		{
			buf[i]=NULL;
			m_EntityDisEnd.SetWindowText(buf);
		}
	}
	stEntityDisEnd = (float)atoi(buf);
	if( stEntityDisEnd > 90000 )	//�ְ� 90000����
	{
		m_EntityDisEnd.SetWindowText("90000");
		stEntityDisEnd=90000;
	}
	if( stEntityDisEnd < 10 )
		stEntityList[stCurSelectEntity].FadeEnd = 10;
	else
		stEntityList[stCurSelectEntity].FadeEnd = stEntityDisEnd;
}

void CEntityEdit::OnButton3() //������ư.
{
	// TODO: Add your control notification handler code here

	DeleteSelectedEntities();
}


void CEntityEdit::OnButton5() //unselect all
{
	// TODO: Add your control notification handler code here
	
	UnSelectAllEntities();
}

void CEntityEdit::OnButton8()	//��ƼƼ ����
{
	// TODO: Add your control notification handler code here
	int i;
	char buf[256];

	if( stCurSelectEntity == -1 )
		return;

	sprintf(buf,"%s �ʻ��� �� ��ƼƼ�� ���� ������?",stEntityList[stCurSelectEntity].Name);
	int result = MessageBox(buf,"Select",MB_OKCANCEL);
	if( IDOK != result )	//���� ���.
		return;
	//---���� �ش� ��ƼƼ�� �� ����Ʈ���� �� �����...
	SelectGAll(stCurSelectEntity);	//���� Ŀ���� �Ȱ� ����.
	DeleteSelectedEntities();

	m_LoadedEntityList.ResetContent();	//���ڿ�����Ʈ�� �ϴ� �������..

	stEntityList[stCurSelectEntity].Entity.ReleaseEntity();
	//---��ĭ�� �ٶ����.
	memcpy(&stEntityList[stCurSelectEntity],&stEntityList[stCurSelectEntity+1]
		,sizeof(_SAVE_ENTITY_LIST)*(stMapEntityNum-stCurSelectEntity-1));
	stMapEntityNum--;

	if( stMapEntityNum == 0 )
		stCurSelectEntity =-1;
	
	for( i=0; i<stMapEntityNum; i++)	//����� �����.
		m_LoadedEntityList.InsertString(i, stEntityList[i].Name);

	for( i=0; i<stEntitiesNum; i++)	//���� ��ϵ� �༮�� id ��ĭ�� �и���....
	{
		if( stCurSelectEntity <  stMapEntitiesList[i].id )
			stMapEntitiesList[i].id--;
	}

	if( stCurSelectEntity > 0 )
	{
		stCurSelectEntity--;

		stEntityDisStart = stEntityList[stCurSelectEntity].FadeStart;
		stEntityDisEnd = stEntityList[stCurSelectEntity].FadeEnd;
		char buf[256];
		sprintf(buf,"%d",(int)stEntityDisStart);
		m_EntityDisStart.SetWindowText(buf);
		sprintf(buf,"%d",(int)stEntityDisEnd);
		m_EntityDisEnd.SetWindowText(buf);
	}
	m_LoadedEntityList.SetCurSel(stCurSelectEntity);
}

void CEntityEdit::OnButton9()	//��ƼƼ ��ü
{
	char buf[256];
	int i;

	if( stCurSelectEntity == -1 )
		return;
	// TODO: Add your control notification handler code here
	sprintf(buf,"%s �� ��ƼƼ�� �ٸ� ��ƼƼ�� ��ü�ұ��?",stEntityList[stCurSelectEntity].Name);
	int result = MessageBox(buf,"Select",MB_OKCANCEL);
	if( IDOK != result )	//���� ���.
		return;

	OPENFILENAME ofn; // zero the ofn members out
	memset(&ofn,0,sizeof(OPENFILENAME));
	char name[256]="";

//	_R3ENGINE_STATE *state = GetR3State();
//	strcpy(name,state->EntityPath);
//	strcat(name,"test.r3e");

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
	char entity_name[256];

	strcpy(entity_name,name);
	GetEntityName(entity_name);	//��ٱ����� �н��� ���ֶ�..

	strcpy(stEntityList[stCurSelectEntity].Name,entity_name);
	m_LoadedEntityList.ResetContent();	//���ڿ�����Ʈ�� �ϴ� �������..
	for( i=0; i<stMapEntityNum; i++)	//����� �����.
		m_LoadedEntityList.InsertString(i, stEntityList[i].Name);

	stEntityList[stCurSelectEntity].Entity.ReleaseEntity();
	if( !stEntityList[stCurSelectEntity].Entity.LoadEntity(name,_USE_VERTEX_SHADER|_DO_NOT_MANAGE_TEXTURE) )
	{
		return;	//�ε�����.
	}
	stEntityList[stCurSelectEntity].Entity.RestoreTexMem();	
	m_LoadedEntityList.SetCurSel(stCurSelectEntity);
}

void CEntityEdit::OnButton4()	//���� G all
{
	// TODO: Add your control notification handler code here
	if( stCurSelectEntity < 0 )
		return;
	UnSelectAllEntities();	//�ϴ� ���õȰ� ����...
	SelectGAll(stCurSelectEntity);	//���� Ŀ���� �Ȱ� ����.
//	DeleteSelectedEntities();
}


void LoadMapEntityFile()
{
	OPENFILENAME ofn; // zero the ofn members out
	memset(&ofn,0,sizeof(OPENFILENAME));
	char name[256]=".\\���߰���\\ToolBin\\MapEntityList\\test.mel";
	int i;

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = name;
	ofn.nMaxFile = sizeof(name);
	ofn.lpstrFilter = "R3 MEL file (*.MEL)\0*.MEL\0All (*.*)\0*.*\0";
	ofn.lpstrTitle = "Select MEL(Map Entity List) file";
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
	float version = _MEL_VERSION;
	fread(&version,4,1,fp);
//	if( _MEL_VERSION-0.1f != version )
	if( _MEL_VERSION != version )		//���� ���׷��̵�.
	{
		Warning(name,"������ ���������Դϴ�.");
		fclose(fp);
		return;
	}
	char path[256]=".\\entity",f_name[256],f_temp[256];

	GetPrivateProfileString("Path","EntityPath",".\\Map\\Entity\\",path, 128,".\\R3Engine.ini");

	CleanMapEntityAndEntities();	//�ε����ִ� ��ƼƼ�� ������.
	if( IsOnInitDialog )
		stEntityEdit->m_LoadedEntityList.ResetContent();	//���ڿ�����Ʈ�� �������..

	fread(&stMapEntityNum,4,1,fp);	//������ ����.

	stMaxEntityNum=100+stMapEntityNum;
	stEntityList=(_SAVE_ENTITY_LIST *)Dmalloc(sizeof(_SAVE_ENTITY_LIST)*stMaxEntityNum);
	memset(stEntityList,0,sizeof(_SAVE_ENTITY_LIST)*stMaxEntityNum);

	for( i=0; i<stMapEntityNum; i++)
	{
		fread(&stEntityList[i].IsParticle,1,1,fp);	//�����̸� ����.
		fread(&stEntityList[i].Name,63,1,fp);	//�����̸� ����.
		fread(&stEntityList[i].FadeStart,sizeof(float),1,fp);	//�����̸� ����.
		fread(&stEntityList[i].FadeEnd,sizeof(float),1,fp);	//�����̸� ����.
		fread(&stEntityList[i].Flag,sizeof(WORD),1,fp);	
		fread(&stEntityList[i].ShaderID,sizeof(WORD),1,fp);
		fread(stEntityList[i].Factor,sizeof(float)*2,1,fp);

/*\\218.55.24.151\����..\..\R3bin\*/

		strcpy(f_temp,stEntityList[i].Name);
		strcpy(f_name,path);
/*
		f_temp[strlen(f_temp)-1]=NULL;
		for(j=strlen(f_temp)-1; j>=0; j--)
		{
			if('\\' == f_temp[j] )
				break;
		}
		temp=&f_temp[j+1];
*/

		strcat(f_name,stEntityList[i].Name);
//		strcat(f_name,temp);
//		strcat(f_name,".r3e");


		if( stEntityList[i].IsParticle )	//���� ��ƼŬ�ϰ��.
		{
			if( !stEntityList[i].Particle.LoadParticleSPT(f_name) )
				return;	//��ƼŬ spt�� �ƴϴ�.
			stEntityList[i].Particle.InitParticle();
			stEntityList[i].Particle.SetParticleState(_PARTICLE_STATE_START);
			stEntityList[i].IsParticle = TRUE;
		}
		else
		{
			if( !stEntityList[i].Entity.LoadEntity(f_name,_USE_VERTEX_SHADER|_DO_NOT_MANAGE_TEXTURE) )
			{
				CleanMapEntityAndEntities();	//�ε����ִ� ��ƼƼ�� ������.
				if( IsOnInitDialog )
					stEntityEdit->m_LoadedEntityList.ResetContent();	//���ڿ�����Ʈ�� �������..
				return;	//�ε�����.
			}
			stEntityList[i].Entity.RestoreTexMem();
		}
		if( IsOnInitDialog )
			stEntityEdit->m_LoadedEntityList.InsertString(i, stEntityList[i].Name);
	}

	fread(&stEntitiesNum,4,1,fp);	//������
	stMaxEntitiesNum=512+stEntitiesNum;
	stMapEntitiesList=(_SAVE_MAP_ENTITIES_LIST *)Dmalloc(stMaxEntitiesNum*sizeof(_SAVE_MAP_ENTITIES_LIST));
	memset(stMapEntitiesList,0,stMaxEntitiesNum*sizeof(_SAVE_MAP_ENTITIES_LIST));

	Vector3s bb_min,bb_max;
	for( i=0; i<stEntitiesNum; i++)
	{
		fread(&stMapEntitiesList[i].id,2,1,fp);	
		fread(&stMapEntitiesList[i].scale,sizeof(float),1,fp);	
		fread(&stMapEntitiesList[i].pos,sizeof(Vector3f),1,fp);
		fread(&stMapEntitiesList[i].rot_x,sizeof(float),1,fp);	
		fread(&stMapEntitiesList[i].rot_y,sizeof(float),1,fp);	
		fread(bb_min,sizeof(Vector3s),1,fp);	//�ٿ�� �ڽ�
		fread(bb_max,sizeof(Vector3s),1,fp);	//�ٿ�� �ڽ�
		stMapEntitiesList[i].bb_min[0] = (float)bb_min[0];
		stMapEntitiesList[i].bb_min[1] = (float)bb_min[1];
		stMapEntitiesList[i].bb_min[2] = (float)bb_min[2];
		stMapEntitiesList[i].bb_max[0] = (float)bb_max[0];
		stMapEntitiesList[i].bb_max[1] = (float)bb_max[1];
		stMapEntitiesList[i].bb_max[2] = (float)bb_max[2];
		stMapEntitiesList[i].add_frame = (rand()%256)/4.0f;;
		if( stEntityList[stMapEntitiesList[i].id].IsParticle )	//���� ��ƼŬ�ϰ��.
		{
			CParticle *particle=new CParticle;
			stMapEntitiesList[i].particle = particle;
			memcpy(particle,&stEntityList[stMapEntitiesList[i].id].Particle,sizeof(CParticle));
			particle->InitParticle();
			stMapEntitiesList[i].map_color = 0xff808080;
		}
		else	//��ƼƼ�� �ٴ��÷� ���
		{
			stMapEntitiesList[i].map_color = stLevel->mBsp->GetLightFromPoint(stMapEntitiesList[i].pos,30);
			if( stMapEntitiesList[i].map_color == 0xff808080 )
			{
				Vector3f temp;
				Vector3fCopy(stMapEntitiesList[i].pos,temp);
				temp[0]+=0.5f;
				stMapEntitiesList[i].map_color = stLevel->mBsp->GetLightFromPoint(stMapEntitiesList[i].pos,30);
			}
		}
	}

	fclose(fp);
}

void AddMapEntityFile()
{
	OPENFILENAME ofn; // zero the ofn members out
	memset(&ofn,0,sizeof(OPENFILENAME));
	char name[256]=".\\���߰���\\ToolBin\\MapEntityList\\test.mel";
	int i;

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = name;
	ofn.nMaxFile = sizeof(name);
	ofn.lpstrFilter = "R3 MEL file (*.MEL)\0*.MEL\0All (*.*)\0*.*\0";
	ofn.lpstrTitle = "Select MEL(Map Entity List) file";
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
	float version = _MEL_VERSION;
	fread(&version,4,1,fp);
	if( _MEL_VERSION != version )		//���� ���׷��̵�.
	{
		Warning(name,"������ ���������Դϴ�.");
		fclose(fp);
		return;
	}
	char path[256]=".\\entity",f_name[256],f_temp[256];

	GetPrivateProfileString("Path","EntityPath",".\\Map\\Entity\\",path, 128,".\\R3Engine.ini");

//	CleanMapEntityAndEntities();	//�ε����ִ� ��ƼƼ�� ������.
//	if( IsOnInitDialog )
//		stEntityEdit->m_LoadedEntityList.ResetContent();	//���ڿ�����Ʈ�� �������..

	LONG addMapEntityNum;
	fread(&addMapEntityNum,4,1,fp);	//�߰�����

	if( !stEntityList )
	{
		stMaxEntityNum=100+addMapEntityNum;
		stEntityList=(_SAVE_ENTITY_LIST *)Dmalloc(sizeof(_SAVE_ENTITY_LIST)*stMaxEntityNum);
		memset(stEntityList,0,sizeof(_SAVE_ENTITY_LIST)*stMaxEntityNum);
	}

	if( stMaxEntityNum <= stMapEntityNum+addMapEntityNum ) //��ĥ��� ���Ҵ�.
	{
		stEntityList=(_SAVE_ENTITY_LIST *)ReAlloc(stEntityList,stMaxEntityNum*sizeof(_SAVE_ENTITY_LIST)
			,(stMaxEntityNum+20+addMapEntityNum)*sizeof(_SAVE_ENTITY_LIST));
		stMaxEntityNum += (20+addMapEntityNum);
	}

	for( i=stMapEntityNum; i<stMapEntityNum+addMapEntityNum; i++)
	{
		fread(&stEntityList[i].IsParticle,1,1,fp);	//�����̸� ����.
		fread(&stEntityList[i].Name,63,1,fp);	//�����̸� ����.
		fread(&stEntityList[i].FadeStart,sizeof(float),1,fp);	//�����̸� ����.
		fread(&stEntityList[i].FadeEnd,sizeof(float),1,fp);	//�����̸� ����.
		fread(&stEntityList[i].Flag,sizeof(WORD),1,fp);	
		fread(&stEntityList[i].ShaderID,sizeof(WORD),1,fp);
		fread(stEntityList[i].Factor,sizeof(float)*2,1,fp);

		strcpy(f_temp,stEntityList[i].Name);
		strcpy(f_name,path);
		strcat(f_name,stEntityList[i].Name);

		if( stEntityList[i].IsParticle )	//���� ��ƼŬ�ϰ��.
		{
			if( !stEntityList[i].Particle.LoadParticleSPT(f_name) )
				return;	//��ƼŬ spt�� �ƴϴ�.
			stEntityList[i].Particle.InitParticle();
			stEntityList[i].Particle.SetParticleState(_PARTICLE_STATE_START);
			stEntityList[i].IsParticle = TRUE;
		}
		else
		{
			if( !stEntityList[i].Entity.LoadEntity(f_name,_USE_VERTEX_SHADER|_DO_NOT_MANAGE_TEXTURE) )
			{
				CleanMapEntityAndEntities();	//�ε����ִ� ��ƼƼ�� ������.
				if( IsOnInitDialog )
					stEntityEdit->m_LoadedEntityList.ResetContent();	//���ڿ�����Ʈ�� �������..
				return;	//�ε�����.
			}
			stEntityList[i].Entity.RestoreTexMem();
		}
		if( IsOnInitDialog )
			stEntityEdit->m_LoadedEntityList.InsertString(i, stEntityList[i].Name);
	}

	LONG addEntitiesNum;
	fread(&addEntitiesNum,4,1,fp);	//������

	if( !stMapEntitiesList )
	{
		stMaxEntitiesNum=512+addEntitiesNum;
		stMapEntitiesList=(_SAVE_MAP_ENTITIES_LIST *)Dmalloc(stMaxEntitiesNum*sizeof(_SAVE_MAP_ENTITIES_LIST));
		memset(stMapEntitiesList,0,stMaxEntitiesNum*sizeof(_SAVE_MAP_ENTITIES_LIST));
	}

	if( stEntitiesNum + addEntitiesNum >= stMaxEntitiesNum )
	{
		stMapEntitiesList=(_SAVE_MAP_ENTITIES_LIST *)ReAlloc(stMapEntitiesList,stMaxEntitiesNum*sizeof(_SAVE_MAP_ENTITIES_LIST)
			,(stMaxEntitiesNum+512+ addEntitiesNum)*sizeof(_SAVE_MAP_ENTITIES_LIST));
		stMaxEntitiesNum+=(512 + addEntitiesNum);
	}

	Vector3s bb_min,bb_max;
	for( i=stEntitiesNum; i<stEntitiesNum+addEntitiesNum; i++)
	{
		fread(&stMapEntitiesList[i].id,2,1,fp);	
		fread(&stMapEntitiesList[i].scale,sizeof(float),1,fp);	
		fread(&stMapEntitiesList[i].pos,sizeof(Vector3f),1,fp);
		fread(&stMapEntitiesList[i].rot_x,sizeof(float),1,fp);	
		fread(&stMapEntitiesList[i].rot_y,sizeof(float),1,fp);	
		fread(bb_min,sizeof(Vector3s),1,fp);	//�ٿ�� �ڽ�
		fread(bb_max,sizeof(Vector3s),1,fp);	//�ٿ�� �ڽ�
		stMapEntitiesList[i].bb_min[0] = (float)bb_min[0];
		stMapEntitiesList[i].bb_min[1] = (float)bb_min[1];
		stMapEntitiesList[i].bb_min[2] = (float)bb_min[2];
		stMapEntitiesList[i].bb_max[0] = (float)bb_max[0];
		stMapEntitiesList[i].bb_max[1] = (float)bb_max[1];
		stMapEntitiesList[i].bb_max[2] = (float)bb_max[2];
		stMapEntitiesList[i].add_frame = (rand()%256)/4.0f;;
		stMapEntitiesList[i].id += stMapEntityNum;
		if( stEntityList[stMapEntitiesList[i].id].IsParticle )	//���� ��ƼŬ�ϰ��.
		{
			CParticle *particle=new CParticle;
			stMapEntitiesList[i].particle = particle;
			memcpy(particle,&stEntityList[stMapEntitiesList[i].id].Particle,sizeof(CParticle));
			particle->InitParticle();
		}
	}
	//���� ���� ����.
	stMapEntityNum += addMapEntityNum;
	stEntitiesNum += addEntitiesNum;
	fclose(fp);
}



void SaveMapEntityFile()
{
	OPENFILENAME ofn; // zero the ofn members out
	memset(&ofn,0,sizeof(OPENFILENAME));
	char name[256]=".\\���߰���\\ToolBin\\MapEntityList\\test.mel";

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = name;
	ofn.nMaxFile = sizeof(name);
	ofn.lpstrFilter = "R3 MEL file (*.MEL)\0*.MEL\0All (*.*)\0*.*\0";
	ofn.lpstrTitle = "Select MEL(Map Entity List) file";
	ofn.Flags = OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (!GetSaveFileName(&ofn))
	{
		return;
	}
	FILE *fp;
	if( ( fp = fopen(name,"wb"))==NULL )
	{
		Warning(name,"������ ���������ϴ�.");
		return;
	}

	float version = _MEL_VERSION;
	fwrite(&version,4,1,fp);
	fwrite(&stMapEntityNum,4,1,fp);	//������ ����.
	for(int i=0; i<stMapEntityNum; i++)
	{
		fwrite(&stEntityList[i].IsParticle,1,1,fp);	
		fwrite(&stEntityList[i].Name,63,1,fp);	//�����̸� ����.
		fwrite(&stEntityList[i].FadeStart,sizeof(float),1,fp);
		fwrite(&stEntityList[i].FadeEnd,sizeof(float),1,fp);	
		fwrite(&stEntityList[i].Flag,sizeof(WORD),1,fp);	
		fwrite(&stEntityList[i].ShaderID,sizeof(WORD),1,fp);
		fwrite(stEntityList[i].Factor,sizeof(float)*2,1,fp);
	}

	fwrite(&stEntitiesNum,4,1,fp);	//������ ����.

	for( i=0; i<stEntitiesNum; i++)
	{
		Vector3s bb_min,bb_max;

		bb_min[0] = (short)MinFixFloatToInt(stMapEntitiesList[i].bb_min[0]);
		bb_min[1] = (short)MinFixFloatToInt(stMapEntitiesList[i].bb_min[1]);
		bb_min[2] = (short)MinFixFloatToInt(stMapEntitiesList[i].bb_min[2]);
		bb_max[0] = (short)MaxFixFloatToInt(stMapEntitiesList[i].bb_max[0]);
		bb_max[1] = (short)MaxFixFloatToInt(stMapEntitiesList[i].bb_max[1]);
		bb_max[2] = (short)MaxFixFloatToInt(stMapEntitiesList[i].bb_max[2]);
		fwrite(&stMapEntitiesList[i].id,2,1,fp);	//�����̸� ����.
		fwrite(&stMapEntitiesList[i].scale,sizeof(float),1,fp);	//�����̸� ����.
		fwrite(&stMapEntitiesList[i].pos,sizeof(Vector3f),1,fp);	//�����̸� ����.
		fwrite(&stMapEntitiesList[i].rot_x,sizeof(float),1,fp);	//�����̸� ����.
		fwrite(&stMapEntitiesList[i].rot_y,sizeof(float),1,fp);	//�����̸� ����.
		fwrite(bb_min,sizeof(Vector3s),1,fp);	//�ٿ�� �ڽ�
		fwrite(bb_max,sizeof(Vector3s),1,fp);	//�ٿ�� �ڽ�
	}

	fclose(fp);
}

static BOOL IsCollisionBBox(Vector3s a_min,Vector3s a_max,Vector3f b_min,Vector3f b_max)
{
	if( a_min[0] <= b_max[0] && b_min[0] <= a_max[0] 
	&& a_min[1] <= b_max[1] && b_min[1] <= a_max[1] 
	&& a_min[2] <= b_max[2] && b_min[2] <= a_max[2] )
		return TRUE;
	return FALSE;
}


void GetEntityInfoInBSP(CBsp *bsp,_ADD_BSP_SAVE *add_bsp)
//	,_ENTITY_LIST **EntityListPtr,_LEAF_ENTITIES_LIST_INFO **LeafEntityListPtr
//	,_READ_MAP_ENTITIES_LIST **MapEntitiesListPtr,WORD **EntityIDPtr) //bsp�� ���Ե� ��ƼƼ������ �ִ´�.
{
	int i,j;

	_ENTITY_LIST *entity_list_ptr;
	_LEAF_ENTITIES_LIST_INFO *leaf_entity_list_ptr;
	_READ_MAP_ENTITIES_LIST *map_entities_list_ptr;
	WORD *entity_ID;
	// �����õǴ� ��ƼƼ���� �����Ѵ�.
	FixTooDist();	//�Ÿ��� ���� �ȵǴ°��� �ִ°� �����.

	//��ƼƼ�� ���� ...
	bsp->mExtBSPHeader.EntityList.size = stMapEntityNum*sizeof(_ENTITY_LIST);
	entity_list_ptr = (_ENTITY_LIST *)Dmalloc(bsp->mExtBSPHeader.EntityList.size);
	for( i=0; i<stMapEntityNum; i++)
	{
		entity_list_ptr[i].IsParticle = stEntityList[i].IsParticle; 
		strcpy(entity_list_ptr[i].Name,stEntityList[i].Name);
		entity_list_ptr[i].FadeStart=stEntityList[i].FadeStart;
		entity_list_ptr[i].FadeEnd=stEntityList[i].FadeEnd;
		entity_list_ptr[i].Factor[0]=stEntityList[i].Factor[0];
		entity_list_ptr[i].Factor[1]=stEntityList[i].Factor[1];
		entity_list_ptr[i].Flag=stEntityList[i].Flag;
		entity_list_ptr[i].ShaderID=stEntityList[i].ShaderID;
	}
	add_bsp->EntityList = entity_list_ptr;

	//�� ��ƼƼ ����Ʈ
	bsp->mExtBSPHeader.MapEntitiesList.size = stEntitiesNum*sizeof(_READ_MAP_ENTITIES_LIST);
	map_entities_list_ptr = (_READ_MAP_ENTITIES_LIST *)Dmalloc(bsp->mExtBSPHeader.MapEntitiesList.size);
	for( i=0; i<stEntitiesNum; i++)
	{
		memcpy(&map_entities_list_ptr[i].ID,&stMapEntitiesList[i].id,2);
		memcpy(&map_entities_list_ptr[i].Scale,&stMapEntitiesList[i].scale,sizeof(float));
		memcpy(&map_entities_list_ptr[i].Pos,&stMapEntitiesList[i].pos,sizeof(Vector3f));
		memcpy(&map_entities_list_ptr[i].RotX,&stMapEntitiesList[i].rot_x,sizeof(float));
		memcpy(&map_entities_list_ptr[i].RotY,&stMapEntitiesList[i].rot_y,sizeof(float));
		Vector3s bb_min,bb_max;
		bb_min[0] = (short)MinFixFloatToInt(stMapEntitiesList[i].bb_min[0]);
		bb_min[1] = (short)MinFixFloatToInt(stMapEntitiesList[i].bb_min[1]);
		bb_min[2] = (short)MinFixFloatToInt(stMapEntitiesList[i].bb_min[2]);
		bb_max[0] = (short)MaxFixFloatToInt(stMapEntitiesList[i].bb_max[0]);
		bb_max[1] = (short)MaxFixFloatToInt(stMapEntitiesList[i].bb_max[1]);
		bb_max[2] = (short)MaxFixFloatToInt(stMapEntitiesList[i].bb_max[2]);
		memcpy(&map_entities_list_ptr[i].BBmin,bb_min,sizeof(Vector3s));
		memcpy(&map_entities_list_ptr[i].BBmax,bb_max,sizeof(Vector3s));
	}
	add_bsp->MapEntitiesList = map_entities_list_ptr;

	leaf_entity_list_ptr=(_LEAF_ENTITIES_LIST_INFO *)Dmalloc(sizeof(_LEAF_ENTITIES_LIST_INFO)*bsp->mLeafNum);
	memset(leaf_entity_list_ptr,0,sizeof(_LEAF_ENTITIES_LIST_INFO)*bsp->mLeafNum);
	add_bsp->LeafEntityList=leaf_entity_list_ptr;
	bsp->mExtBSPHeader.LeafEntityList.size = sizeof(_LEAF_ENTITIES_LIST_INFO)*bsp->mLeafNum;

	DWORD max_entity=10000;
	entity_ID = (WORD *)Dmalloc(max_entity*sizeof(WORD));	//�ִ�ġ�� ���� ������.
	add_bsp->EntityID = entity_ID;
	DWORD id_cnt=0;
	for(i=1; i<(int)bsp->mLeafNum; i++)
	{
		leaf_entity_list_ptr[i].start_id = id_cnt;
		DWORD temp_id_cnt = id_cnt;
		for( j=0; j<stEntitiesNum; j++)
		{
			//memcpy(&map_entities_list_ptr[i].ID,&stMapEntitiesList[i].id,2);	
			if( IsCollisionBBox( bsp->mLeaf[i].bb_min,bsp->mLeaf[i].bb_max	//�ٿ���ڽ� ������ ���.
				,stMapEntitiesList[j].bb_min,stMapEntitiesList[j].bb_max) )
			{
				entity_ID[id_cnt]=(WORD)j;
				id_cnt++;
				if( id_cnt>= max_entity )
				{
					entity_ID = (WORD *)ReAlloc(entity_ID,max_entity*2,(max_entity+10000)*2);	//�ִ�ġ�� ���� ������.
					max_entity += 10000;
					add_bsp->EntityID = entity_ID;
				}
			}
		}
		leaf_entity_list_ptr[i].entities_num = (WORD)(id_cnt - temp_id_cnt);
	}
	bsp->mExtBSPHeader.EntityID.size = id_cnt*sizeof(WORD);
}

void ReleaseEntityInfoInBSP(_ADD_BSP_SAVE *add_bsp) //bsp�� ���Ե� ��ƼƼ������ �ִ´�.
{
	if( add_bsp->EntityList )
		Dfree(add_bsp->EntityList);
	if( add_bsp->LeafEntityList )
		Dfree(add_bsp->LeafEntityList);
	if( add_bsp->MapEntitiesList )
		Dfree(add_bsp->MapEntitiesList);
	if( add_bsp->EntityID )
		Dfree(add_bsp->EntityID);
	add_bsp->MapEntitiesList=NULL;
	add_bsp->LeafEntityList=NULL;
	add_bsp->EntityList=NULL;
	add_bsp->EntityID=NULL;
}

void LoadEntityFromBSP(CBsp *bsp)
{
	int i;

	char path[256],f_name[256],f_temp[256];

	_R3ENGINE_STATE *state = GetR3State();
	strcpy(path,state->EntityPath);

	CleanMapEntityAndEntities();	//�ε����ִ� ��ƼƼ�� ������.
	stEntityEdit->m_LoadedEntityList.ResetContent();	//���ڿ�����Ʈ�� �������..

	stMapEntityNum = bsp->mEntityListNum;//���� ����

	stMaxEntityNum=100+stMapEntityNum;
	stEntityList=(_SAVE_ENTITY_LIST *)Dmalloc(sizeof(_SAVE_ENTITY_LIST)*stMaxEntityNum);
	memset(stEntityList,0,sizeof(_SAVE_ENTITY_LIST)*stMaxEntityNum);

	for( i=0; i<stMapEntityNum; i++)
	{
		stEntityList[i].IsParticle = bsp->mEntityList[i].IsParticle;	//�����̸� ����.
		strcpy(stEntityList[i].Name,bsp->mEntityList[i].Name);	//�����̸� ����.
		stEntityList[i].FadeStart = bsp->mEntityList[i].FadeStart;
		stEntityList[i].FadeEnd = bsp->mEntityList[i].FadeEnd;
		stEntityList[i].Flag = bsp->mEntityList[i].Flag;
		stEntityList[i].ShaderID = bsp->mEntityList[i].ShaderID;
		stEntityList[i].Factor[0] = bsp->mEntityList[i].Factor[0];
		stEntityList[i].Factor[1] = bsp->mEntityList[i].Factor[1];
		strcpy(f_temp,stEntityList[i].Name);
		strcpy(f_name,path);
/*
		f_temp[strlen(f_temp)-1]=NULL;
		for(j=strlen(f_temp)-1; j>=0; j--)
		{
			if('\\' == f_temp[j] )
				break;
		}
		temp=&f_temp[j+1];
*/
		strcat(f_name,stEntityList[i].Name);
//		strcat(f_name,temp);
//		strcat(f_name,".r3e");
		if( stEntityList[i].IsParticle )	//���� ��ƼŬ�ϰ��.
		{
			if( !stEntityList[i].Particle.LoadParticleSPT(f_name) )
				return;	//��ƼŬ spt�� �ƴϴ�.
			stEntityList[i].Particle.InitParticle();
			stEntityList[i].Particle.SetParticleState(_PARTICLE_STATE_START);
			stEntityList[i].IsParticle = TRUE;
		}
		else
		{
			if( !stEntityList[i].Entity.LoadEntity(f_name,_USE_VERTEX_SHADER|_DO_NOT_MANAGE_TEXTURE) )
			{
				CleanMapEntityAndEntities();	//�ε����ִ� ��ƼƼ�� ������.
				stEntityEdit->m_LoadedEntityList.ResetContent();	//���ڿ�����Ʈ�� �������..
				return;	//�ε�����.
			}
			stEntityList[i].Entity.RestoreTexMem();
		}
		stEntityEdit->m_LoadedEntityList.InsertString(i, stEntityList[i].Name);
	}
	if( stMapEntityNum )	//Ŀ�������� �����.
	{
		stEntityEdit->m_LoadedEntityList.SetCurSel(0);
		stCurSelectEntity=0;
	}

	stEntitiesNum = bsp->mMapEntitiesListNum;	//������
	stMaxEntitiesNum=512+stEntitiesNum;
	stMapEntitiesList=(_SAVE_MAP_ENTITIES_LIST *)Dmalloc(stMaxEntitiesNum*sizeof(_SAVE_MAP_ENTITIES_LIST));
	memset(stMapEntitiesList,0,stMaxEntitiesNum*sizeof(_SAVE_MAP_ENTITIES_LIST));

	for( i=0; i<stEntitiesNum; i++)
	{
		stMapEntitiesList[i].id = bsp->mMapEntitiesList[i].ID;	
		stMapEntitiesList[i].scale = bsp->mMapEntitiesList[i].Scale;	
		Vector3fCopy(bsp->mMapEntitiesList[i].Pos,stMapEntitiesList[i].pos);	
		stMapEntitiesList[i].rot_x = bsp->mMapEntitiesList[i].RotX;	
		stMapEntitiesList[i].rot_y = bsp->mMapEntitiesList[i].RotY;	
		stMapEntitiesList[i].bb_min[0] = (float)bsp->mMapEntitiesList[i].BBMin[0];
		stMapEntitiesList[i].bb_min[1] = (float)bsp->mMapEntitiesList[i].BBMin[1];
		stMapEntitiesList[i].bb_min[2] = (float)bsp->mMapEntitiesList[i].BBMin[2];
		stMapEntitiesList[i].bb_max[0] = (float)bsp->mMapEntitiesList[i].BBMax[0];
		stMapEntitiesList[i].bb_max[1] = (float)bsp->mMapEntitiesList[i].BBMax[1];
		stMapEntitiesList[i].bb_max[2] = (float)bsp->mMapEntitiesList[i].BBMax[2];
		stMapEntitiesList[i].add_frame = bsp->mMapEntitiesList[i].AddFrame; 
		stMapEntitiesList[i].map_color = bsp->mMapEntitiesList[i].mMapColor; 

		//��ƼŬ�� ���� �ǽð� �ٿ���ڽ��� �ʱ�ȭ ��Ų��.
//		InitBBox(stMapEntitiesList[i].bb_min,stMapEntitiesList[i].bb_max);
		InitBBox(stMapEntitiesList[i].rt_bb_min,stMapEntitiesList[i].rt_bb_max);

		if( stEntityList[stMapEntitiesList[i].id].IsParticle )	//���� ��ƼŬ�ϰ��.
		{
			CParticle *particle=new CParticle;
			stMapEntitiesList[i].particle = particle;
			memcpy(particle,&stEntityList[stMapEntitiesList[i].id].Particle,sizeof(CParticle));
			particle->InitParticle();
		}
		if( stEntityList[stMapEntitiesList[i].id].Entity.mObjectNum )	//��ƼƼ �ִϸ��̼��� ���� Ʋ���� �ؾߵǴϱ�..
		{
			stMapEntitiesList[i].add_frame 
				= (float)(rand()%stEntityList[stMapEntitiesList[i].id].Entity.mObject[0].frames);
		}
	}
}

void CEntityEdit::OnCheck7()		//pos ����Ű..
{
	// TODO: Add your control notification handler code here
	
	stCurEditMode=_ENTITY_EDIT_POS_MODE;		//pos��� 

	CheckDlgButton(IDC_CHECK7,TRUE);
	CheckDlgButton(IDC_CHECK10,FALSE);
	CheckDlgButton(IDC_CHECK11,FALSE);
	CheckDlgButton(IDC_CHECK12,FALSE);
}

void CEntityEdit::OnCheck10()	// ������ ����Ű...
{
	// TODO: Add your control notification handler code here

	stCurEditMode=_ENTITY_EDIT_SCALE_MODE;		//scale��� 
	CheckDlgButton(IDC_CHECK7,FALSE);
	CheckDlgButton(IDC_CHECK10,TRUE);
	CheckDlgButton(IDC_CHECK11,FALSE);
	CheckDlgButton(IDC_CHECK12,FALSE);
}

void CEntityEdit::OnCheck11()	// yȸ�� ����Ű...
{
	// TODO: Add your control notification handler code here
	
	stCurEditMode=_ENTITY_EDIT_YROT_MODE;		//y rot��� 
	CheckDlgButton(IDC_CHECK7,FALSE);
	CheckDlgButton(IDC_CHECK10,FALSE);
	CheckDlgButton(IDC_CHECK11,TRUE);
	CheckDlgButton(IDC_CHECK12,FALSE);
}

void CEntityEdit::OnOK()	//esc�� enterŰ�� ����Ѵ�.
{
	// TODO: Add extra validation here

//	CDialog::OnOK();
}

void CEntityEdit::OnCancel() 
{
	// TODO: Add extra cleanup here
	
//	CDialog::OnCancel();
}

void CEntityEdit::OnCheck12() // xȸ�� ����Ű...
{
	// TODO: Add your control notification handler code here
	
	stCurEditMode=_ENTITY_EDIT_XROT_MODE;		//x rot��� 
	CheckDlgButton(IDC_CHECK7,FALSE);
	CheckDlgButton(IDC_CHECK10,FALSE);
	CheckDlgButton(IDC_CHECK11,FALSE);
	CheckDlgButton(IDC_CHECK12,TRUE);
}

void CEntityEdit::OnButton1() //������.
{
	// TODO: Add your control notification handler code here
	for(int i=0; i<stEntitiesNum; i++)	//���õȰ��� unfreeze ����.
	{
		if( stMapEntitiesList[i].flag & ENTITY_IS_SELECTED )
		{
			stMapEntitiesList[i].flag &= ~ENTITY_IS_SELECTED;	//������ ����.
			stMapEntitiesList[i].flag |= ENTITY_IS_FREEZE;
		}
	}
}

void CEntityEdit::OnButton2() //���������.
{
	// TODO: Add your control notification handler code here
	for(int i=0; i<stEntitiesNum; i++)	//��� unfreeze ����.
	{
		stMapEntitiesList[i].flag&=~ENTITY_IS_FREEZE;
	}
}

void CEntityEdit::OnCheck18() // ��������.
{
	stUnFreezeState = !stUnFreezeState;
}
static void FixTooDist()	//�Ÿ��� ���� �ȵǴ°��� �ִ°� �����.
{
	UnSelectAllEntities();
	for(int i=0; i<stEntitiesNum; i++)	//�Ÿ��� �ճ���� ����.
	{
		if( stMapEntitiesList[i].pos[0] > 32000 || stMapEntitiesList[i].pos[0] < -32000 
		|| stMapEntitiesList[i].pos[1] > 32000 || stMapEntitiesList[i].pos[1] < -32000 
		|| stMapEntitiesList[i].pos[2] > 32000 || stMapEntitiesList[i].pos[2] < -32000 
			)
		{
			stMapEntitiesList[i].flag |= ENTITY_IS_SELECTED;
		}
	}
	DeleteSelectedEntities();
}



static int stCmode;
/*
BOOL CEntityEdit::OnSetActive()	//Ȱ��ȭ �Ǹ� ..
{
	// TODO: Add your specialized code here and/or call the base class
	
//	SetTabMode(_ENTITY_MODE);
	stCmode = WhatCollisionMode();
	SetCollisionMode(_COLLISON_EDIT_MODE);
	if( stCmode == _COLLISON_TEST_MODE )
		SetCosoleStateB(OP_FLY,TRUE);
	return CPropertyPage::OnSetActive();
}

BOOL CEntityEdit::OnKillActive() //Ȱ��ȭ�� ������� �Ǹ�. 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( stCmode == _COLLISON_TEST_MODE )
		SetCosoleStateB(OP_FLY,FALSE);
	SetCollisionMode(stCmode);
	return CPropertyPage::OnKillActive();
}
*/

void TabEntityHotKey()	//��ƼƼ�� ��Ű..
{
	if( !IsOnInitDialog )
		return;		//���̾�α� �ʱ�ȭ�� �ȵǸ�..
	
	if( gAsci[VK_1] )
	{
		gAsci[VK_1]=0;
		stEntityEdit->OnCheck7();	// pos ����Ű...
	}
	if( gAsci[VK_2] )
	{
		gAsci[VK_2]=0;
		stEntityEdit->OnCheck10();	// ������ ����Ű...
	}
	if( gAsci[VK_3] )
	{
		gAsci[VK_3]=0;
		stEntityEdit->OnCheck11();	// y ȸ�� ����Ű...
	}
	if( gAsci[VK_4] )
	{
		gAsci[VK_4]=0;
		stEntityEdit->OnCheck12();	// x ȸ�� ����Ű...
	}
}

void CEntityEdit::OnSelchangeCombo2() //���̴� ������ ����.
{
	stCurSelectShader = m_LoadedShaderList.GetCurSel();
	if( stCurSelectEntity == -1)
		return;
	stEntityList[stCurSelectEntity].ShaderID = (WORD)stCurSelectShader;
	for( LONG i=0; i<stEntitiesNum; i++)
	{
		//�ִϸ��̼��� Ʋ���� �ϱ����� �۾�....
		stMapEntitiesList[i].add_frame = (rand()%256)/4.0f;;
	}
}

void CEntityEdit::OnChangeEdit18() 
{
	// TODO: Add your control notification handler code here
	char buf[256];
	m_Factor1.GetWindowText(buf,256);

	int i;
	for(i=0; i<(int)strlen(buf); i++)
	{
		if( i >= 5 || (buf[i] < '0' || buf[i] > '9') && buf[i] !='.' )
		{
			buf[i]=NULL;
			m_Factor1.SetWindowText(buf);
		}
	}
	if( stCurSelectEntity == -1)
		return;
	stEntityList[stCurSelectEntity].Factor[0] = (float)atof(buf);
}

void CEntityEdit::OnChangeEdit19() 
{
	char buf[256];
	m_Factor2.GetWindowText(buf,256);

	int i;
	for(i=0; i<(int)strlen(buf); i++)
	{
		if( i >= 5 || (buf[i] < '0' || buf[i] > '9') && buf[i] !='.' )
		{
			buf[i]=NULL;
			m_Factor2.SetWindowText(buf);
		}
	}
	if( stCurSelectEntity == -1)
		return;
	stEntityList[stCurSelectEntity].Factor[1] = (float)atof(buf);
//	stFactor[1] = (float)atof(buf);
}

void CEntityEdit::OnChangeEdit20() 
{
	char buf[256];
	m_PosX.GetWindowText(buf,256);

	int i;
	for(i=0; i<(int)strlen(buf); i++)
	{
		if( i >= 5 || (buf[i] < '0' || buf[i] > '9') && buf[i] !='.'&& (buf[i] != '-') )
		{
			buf[i]=NULL;
			m_PosX.SetWindowText(buf);
		}
	}
	stCurEntityPos[0] = (float)atof(buf);
	ChanegeSelectedEntitiesPos(stCurEntityPos[0],-65535,-65535 );
}

void CEntityEdit::OnChangeEdit21() 
{
	char buf[256];
	m_PosY.GetWindowText(buf,256);

	int i;
	for(i=0; i<(int)strlen(buf); i++)
	{
		if( i >= 5 || (buf[i] < '0' || buf[i] > '9') && buf[i] !='.' && (buf[i] != '-') )
		{
			buf[i]=NULL;
			m_PosY.SetWindowText(buf);
		}
	}
	stCurEntityPos[1] = (float)atof(buf);
	ChanegeSelectedEntitiesPos(-65535,stCurEntityPos[1],-65535 );
}

void CEntityEdit::OnChangeEdit22() 
{
	char buf[256];
	m_PosZ.GetWindowText(buf,256);

	int i;
	for(i=0; i<(int)strlen(buf); i++)
	{
		if( i >= 5 || (buf[i] < '0' || buf[i] > '9') && buf[i] !='.'&& (buf[i] != '-'))
		{
			buf[i]=NULL;
			m_PosZ.SetWindowText(buf);
		}
	}
	stCurEntityPos[2] = (float)atof(buf);
	ChanegeSelectedEntitiesPos(-65535,-65535,stCurEntityPos[2]);
}
