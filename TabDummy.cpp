// TabDummy.cpp : implementation file
//

#define STRICT
#include <stdafx.h>
#include <math.h>
#include "rfmaptool.h"
#include "TabDummy.h"
#include "ToolManager.h"

#include "CommonUtil.h"
#include "Jmalloc.h"
#include "console.h"
#include "Jerror.h"
#include "r3input.h"
#include "r3math.h"
#include "r3util.h"
#include "r3text.h"
#include "r3render.h"
#include "core.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif






#define _DUMMY_EDIT_POS_MODE	0
#define _DUMMY_EDIT_SCALE_MODE	1
#define _DUMMY_EDIT_ROT_MODE	2

#define DUMMY_IS_SELECTED	0x40
#define DUMMY_IS_FREEZE		0x20

static BOOL stUnFreezeState=FALSE;
static CDummyEdit *stEntityEdit=NULL; //<- onintDialog���� �־ ����Ѵ�.


typedef struct{
	BYTE flag;		//���� �÷��� 
	char name[256];
	Vector3f bb_min,bb_max;
	Vector3f pos;
	Vector3f scale;
	float y_rot;
}_SAVE_DUMMY_ENTITIES_LIST;

_SAVE_DUMMY_ENTITIES_LIST *stDummyEntitiesList;
static int stDummyEntitiesNum=0;
static int stMaxDummyEntitiesNum=0;


static char  *stInitScrollBar[]={"UnKnown","MOB ����","MAP�̵� ����","���� ����","NPC(����) ����"
,"�ڿ� ����","����Ʈ ����","�̵� ���� ����","����Ÿ�� ����",""};
static char  *stInitial[]={"unknown","dm","dp","ds","sd","dr","dq","dg","dt"};
static int stDummyAttNum;
static int stCurEditDummyAtt=0,stCurSelectDummyAtt=0;
static int stCurEditMode=0;
static BOOL IsOnInitDialog=FALSE;

static BOOL stOneClickEntity = FALSE;
static float stOldEntityMx,stOldEntityMy;
static Vector3f stCurEntityPos;
static Vector3f stCurScale={20,14,20};
static float stCurRot=0;
static int stCurEditDummy=0;
static int stFinalSelectId=-1;

static BOOL stOneClick=FALSE;





IMPLEMENT_DYNCREATE(CDummyFile, CDialog)

/////////////////////////////////////////////////////////////////////////////
// CDummyFile dialog
CDummyFile::CDummyFile(CWnd* pParent /*=NULL*/)
	: CDialog(CDummyFile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDummyFile)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDummyFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDummyFile)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDummyFile, CDialog)
	//{{AFX_MSG_MAP(CDummyFile)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDummyFile message handlers

static void FixTooDist();	//�Ÿ��� ���� �ȵǴ°��� �ִ°� �����.

void SaveDummy()
{
	OPENFILENAME ofn; // zero the ofn members out
	memset(&ofn,0,sizeof(OPENFILENAME));
	char path[256]=".\\���߰���\\ToolBin\\ServerSPT\\test.spt";

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = path;
	ofn.nMaxFile = sizeof(path);
	ofn.lpstrFilter = "R3 SPT file (*.Spt)\0*.Spt\0All (*.*)\0*.*\0";
	ofn.lpstrTitle = "Select Dummy SPT(Dummy Script) file";
	ofn.Flags = OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (!GetSaveFileName(&ofn))
	{
		return;
	}
	/*
	if( IsExistFile(path) )
	{
		int result = MessageBox(NULL,"������� ������ ���?","Select",MB_OKCANCEL);
		if( IDCANCEL == result )	//���� ���.
		{
			return;
		}
	}*/
	
	FixTooDist();	//�Ÿ��� ���� �ȵǴ°��� �ִ°� �����.

	FILE *fp;

	if(( fp =fopen(path,"wt"))==NULL) 
	{
		Warning(path,"<-�� ������ �� ���ϴ�.");
		return;
	}

	fprintf(fp,"script_begin\n\n\n");

	fprintf(fp,"[HelperObjectBegin]\n");

	for(int i=0; i<stDummyEntitiesNum; i++)
	{/*
		fprintf(fp,"\t*%s\t%d\t%d\t%d\t\t%d\t%d\t%d\n"
			,stDummyEntitiesList[i].name
			,(int)stDummyEntitiesList[i].bb_min[0]
			,(int)stDummyEntitiesList[i].bb_min[1]
			,(int)stDummyEntitiesList[i].bb_min[2]
			,(int)stDummyEntitiesList[i].bb_max[0]
			,(int)stDummyEntitiesList[i].bb_max[1]
			,(int)stDummyEntitiesList[i].bb_max[2]);
*/
		fprintf(fp,"\t*%s\t%d\t%d\t%d\t\t%d\t%d\t%d\n"
			,stDummyEntitiesList[i].name
			,(int)-stDummyEntitiesList[i].scale[0]
			,(int)-stDummyEntitiesList[i].scale[1]
			,(int)-stDummyEntitiesList[i].scale[2]
			,(int)stDummyEntitiesList[i].scale[0]
			,(int)stDummyEntitiesList[i].scale[1]
			,(int)stDummyEntitiesList[i].scale[2]);
		fprintf(fp,"\t-node_tm\n");
		float mat[4][4];
		MatrixRotateY(mat,stDummyEntitiesList[i].y_rot);
		Vector3fCopy(stDummyEntitiesList[i].pos,(mat[3]));
		fprintf(fp,"\t%f %f %f %f\n",mat[0][0],mat[0][1],mat[0][2],mat[0][3]);
		fprintf(fp,"\t%f %f %f %f\n",mat[1][0],mat[1][1],mat[1][2],mat[1][3]);
		fprintf(fp,"\t%f %f %f %f\n",mat[2][0],mat[2][1],mat[2][2],mat[2][3]);
		fprintf(fp,"\t%f %f %f %f\n\n",mat[3][0],mat[3][1],mat[3][2],mat[3][3]);
	}
	fprintf(fp,"[HelperObjectEnd]\n\n");

	fprintf(fp,"script_end\n");
	fclose(fp);
}

void CDummyFile::OnButton2()	//���� ����.
{
	SaveDummy();
}

CExtDummy stExtDummy;

void LoadDummy()
{
	OPENFILENAME ofn; // zero the ofn members out
	memset(&ofn,0,sizeof(OPENFILENAME));
	char path[256]=".\\���߰���\\ToolBin\\ServerSPT\\test.spt";;

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = path;
	ofn.nMaxFile = sizeof(path);
	ofn.lpstrFilter = "R3 SPT file (*.Spt)\0*.Spt\0All (*.*)\0*.*\0";
	ofn.lpstrTitle = "Select Dummy SPT(Dummy Script) file";
	ofn.Flags = OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (!GetOpenFileName(&ofn))
	{
		return;
	}
	stExtDummy.ReleaseExtDummy();
	if( !stExtDummy.LoadExtDummy(path) )
		return;

	if( stDummyEntitiesList == NULL )
	{
		stMaxDummyEntitiesNum=64;
		stDummyEntitiesList=(_SAVE_DUMMY_ENTITIES_LIST *)Dmalloc(stMaxDummyEntitiesNum*sizeof(_SAVE_DUMMY_ENTITIES_LIST));
	}

	DWORD i;
	stDummyEntitiesNum=0;

	for(i=0; i<stExtDummy.GetTotalNum(); i++)
	{
		_EXT_DUMMY *dummy = stExtDummy.GetDummy(i);		

		stDummyEntitiesList[stDummyEntitiesNum].flag = 0;
//		Vector3fSub(dummy->mBBmax,dummy->mBBmin,temp);
//		Vector3fMul(temp,0.5f,temp);	//scale
//		Vector3fCopy(temp,stDummyEntitiesList[stDummyEntitiesNum].scale);

//		Vector3fAdd(temp,dummy->mBBmin,temp);	//pos
		strcpy(stDummyEntitiesList[stDummyEntitiesNum].name,dummy->mName);
		Vector3fCopy(dummy->mMat[3],stDummyEntitiesList[stDummyEntitiesNum].pos);

		Vector3fCopy(dummy->mBBmax,stDummyEntitiesList[stDummyEntitiesNum].scale);

//    get_mat[0][0]=(float)cos(fay); ge			

		float acos_gop = (float)acos(dummy->mMat[0][0]);
		stDummyEntitiesList[stDummyEntitiesNum].y_rot = acos_gop*360.0f/(2.0f*3.141592f);

		stDummyEntitiesNum++;
		if( stDummyEntitiesNum >= stMaxDummyEntitiesNum )
		{
			stDummyEntitiesList=(_SAVE_DUMMY_ENTITIES_LIST *)ReAlloc(stDummyEntitiesList,stMaxDummyEntitiesNum*sizeof(_SAVE_DUMMY_ENTITIES_LIST),(stMaxDummyEntitiesNum+32)*sizeof(_SAVE_DUMMY_ENTITIES_LIST));
			stMaxDummyEntitiesNum+=32;
		}
	}
/*
	FILE *fp;
	char hole[256];

	if(( fp =fopen(path,"rt"))==NULL) 
		Error(path,"<-�� �����̾����ϴ�.");

    while(1)	//�պκ� ��ŵ.
    {
		if( fscanf(fp,"%s",hole)==EOF)	
		{
			Warning(path,"<-��ũ��Ʈ ������ �ƴմϴ�.!");
			fclose(fp);
			return;
		}
		if (!strcmp(hole,"script_begin"))
		{
			break;
		}
	}

	if( stDummyEntitiesList == NULL )
	{
		stMaxDummyEntitiesNum=64;
		stDummyEntitiesList=(_SAVE_DUMMY_ENTITIES_LIST *)Dmalloc(stMaxDummyEntitiesNum*sizeof(_SAVE_DUMMY_ENTITIES_LIST));
	}

	//�ٽ÷ε��ؾߵǴϱ�...
	stDummyEntitiesNum=0;
	Vector3f bb_min,bb_max,temp;

    while(1)	//�պκ� ��ŵ.
    {
		if( fscanf(fp,"%s",hole)==EOF)	
		{
			break;
		}
		if( hole[0] == '*' )
		{
			memset(&stDummyEntitiesList[stDummyEntitiesNum],0,sizeof(_SAVE_DUMMY_ENTITIES_LIST));
			strcpy(stDummyEntitiesList[stDummyEntitiesNum].name,&hole[1] );
			fscanf(fp,"%s",hole);
			bb_min[0] = (float)atof(hole);
			fscanf(fp,"%s",hole);
			bb_min[1] = (float)atof(hole);
			fscanf(fp,"%s",hole);
			bb_min[2] = (float)atof(hole);

			fscanf(fp,"%s",hole);
			bb_max[0] = (float)atof(hole);
			fscanf(fp,"%s",hole);
			bb_max[1] = (float)atof(hole);
			fscanf(fp,"%s",hole);
			bb_max[2] = (float)atof(hole);

			stDummyEntitiesList[stDummyEntitiesNum].flag = 0;
			Vector3fSub(bb_max,bb_min,temp);
			Vector3fMul(temp,0.5f,temp);	//scale
			Vector3fCopy(temp,stDummyEntitiesList[stDummyEntitiesNum].scale);

			Vector3fAdd(temp,bb_min,temp);	//pos
			Vector3fCopy(temp,stDummyEntitiesList[stDummyEntitiesNum].pos);


//			double fay=2.0*g_PI*y_rot/360.0f;			
//    get_mat[0][0]=(float)cos(fay); ge			
			stDummyEntitiesNum++;
			if( stDummyEntitiesNum >= stMaxDummyEntitiesNum )
			{
				stDummyEntitiesList=(_SAVE_DUMMY_ENTITIES_LIST *)ReAlloc(stDummyEntitiesList,stMaxDummyEntitiesNum*sizeof(_SAVE_DUMMY_ENTITIES_LIST),(stMaxDummyEntitiesNum+32)*sizeof(_SAVE_DUMMY_ENTITIES_LIST));
				stMaxDummyEntitiesNum+=32;
			}
		}
	}

	fclose(fp);
*/
}

void CDummyFile::OnButton1()	//���� �ε�.
{
	LoadDummy();
}






/////////////////////////////////////////////////////////////////////////////
// CDummyNameDialog dialog
// ���� �̸� ���̾�α� �ڽ�.....

CDummyNameDialog::CDummyNameDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDummyNameDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDummyNameDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDummyNameDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDummyNameDialog)
	DDX_Control(pDX, IDC_EDIT2, m_DummyName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDummyNameDialog, CDialog)
	//{{AFX_MSG_MAP(CDummyNameDialog)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDummyNameDialog message handlers




/////////////////////////////////////////////////////////////////////////////
// CDummyEdit dialog

IMPLEMENT_DYNCREATE(CDummyEdit, CDialog)

CDummyEdit::CDummyEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CDummyEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDummyEdit)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDummyEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDummyEdit)
	DDX_Control(pDX, IDC_EDIT23, m_Rot);
	DDX_Control(pDX, IDC_EDIT2, m_DummyName);
	DDX_Control(pDX, IDC_EDIT17, m_ScaleZ);
	DDX_Control(pDX, IDC_EDIT16, m_ScaleY);
	DDX_Control(pDX, IDC_EDIT15, m_ScaleX);
	DDX_Control(pDX, IDC_COMBO3, m_SelectScroll);
	DDX_Control(pDX, IDC_COMBO1, m_EditScroll);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDummyEdit, CDialog)
	//{{AFX_MSG_MAP(CDummyEdit)
	ON_BN_CLICKED(IDC_CHECK7, OnCheck7)
	ON_BN_CLICKED(IDC_CHECK10, OnCheck10)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_EN_CHANGE(IDC_EDIT15, OnChangeEdit15)
	ON_EN_CHANGE(IDC_EDIT16, OnChangeEdit16)
	ON_EN_CHANGE(IDC_EDIT17, OnChangeEdit17)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_CHECK18, OnCheck18)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	ON_BN_CLICKED(IDC_CHECK15, OnCheck15)
	ON_EN_CHANGE(IDC_EDIT23, OnChangeEdit23)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON11, OnButton11)
	ON_CBN_SELCHANGE(IDC_COMBO3, OnSelchangeCombo3)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON12, OnButton12)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDummyEdit message handlers
//------------------------------------------------------------------------------------------------//


static int GetAtt(char *name)
{
	char temp[256];

	strcpy(temp,name);
	temp[2]=NULL;
	for(int i=1; i<stDummyAttNum; i++)
	{
		if( !strcmp(temp, stInitial[i]) )
			return i;
	}
	return 0;
}



static BOOL IsNearDistExist(Vector3f pos)	//���̰� �ʹ� ��ó�� �ִ°��.
{
	Vector3f temp;
	for(int i=0; i<stDummyEntitiesNum; i++)
	{
		Vector3fSub(stDummyEntitiesList[i].pos,pos,temp);
		if( Vector3fDist(temp) < 5 )
			return TRUE;
	}
	return FALSE;
}

static int GetSu(char *name)
{
	char buf[1256];
	char temp[1256];

	for(int i=strlen(name)-1; i>0 ; i--)
	{
		buf[i]=name[i];
		temp[0]=name[i];
		temp[1]=NULL;
		if(temp[0] >= '0' && temp[0] <='9' )
			continue;
		break;
	}
	buf[strlen(name)]=NULL;
	
	return atoi(&buf[i+1]);
}
static void SetScaleString(Vector3f scale)
{
	char buf[256];

	sprintf(buf,"%d",(int)scale[0]);
	stEntityEdit->m_ScaleX.SetWindowText(buf);
	sprintf(buf,"%d",(int)scale[1]);
	stEntityEdit->m_ScaleY.SetWindowText(buf);
	sprintf(buf,"%d",(int)scale[2]);
	stEntityEdit->m_ScaleZ.SetWindowText(buf);
}
static void SetRotString(float rot)
{
	char buf[256];

	sprintf(buf,"%.2f",rot);
	stEntityEdit->m_Rot.SetWindowText(buf);
}

//���� ���� �ڽ��� �̸����߿��� �ش�Ǵ� �Ӽ��� ���� 1������ �� �κ��� ī����
static int GetCountDummyName(int id)
{
	int cnt=0;
	char buf[256];

	while( 1 )
	{
		cnt++;	
		BOOL is_find=0;
		for(int i=0; i<stDummyEntitiesNum; i++)
		{
			strcpy(buf,stDummyEntitiesList[i].name);
			buf[2]=NULL;
			int temp_id=GetAtt(buf);
			if( temp_id == id )
			{
				int su = GetSu(stDummyEntitiesList[i].name);
				if( su == cnt )
				{
					is_find=TRUE;
					break;
				}
			}
		}
		//�ش�Ǵ� ī������ �� ã������ �ش���ڰ� ���̵��.
		if( !is_find )
		{
			break;
		}			
	}
	return cnt;
}

static void SetPreDummyString()	//����Ʈ �ڽ��� �����̸��� �̸� ����ش�.
{
	char buf[256];

	int cnt = GetCountDummyName(stCurEditDummyAtt);
	
	sprintf(buf,"%s%d",stInitial[stCurEditDummyAtt],cnt);

	stEntityEdit->DummyNameDialog.m_DummyName.SetWindowText(buf);
}

static DWORD GetLinearDummyNum(DWORD attr)	//���̰����� ���޾� �Ǵ� ù ���ڸ� ���Ѵ�.
{
	//������ ���� ū ���ڸ� ã���� 
	int best_num=-1;
	char buf[256];
	for(int i=0; i<stDummyEntitiesNum; i++)
	{
		strcpy(buf,stDummyEntitiesList[i].name);
		buf[2]=NULL;
		int temp_id=GetAtt(buf);
		if( (DWORD)temp_id == attr )
		{
			int su = GetSu(stDummyEntitiesList[i].name);
			if( su > best_num )
			{
				best_num = su;
			}
		}
	}
	return best_num+1;
}


static void UnSelectAllEntities()
{
	for(int i=0; i<stDummyEntitiesNum; i++)	//��� un��������.
	{
		stDummyEntitiesList[i].flag&=~DUMMY_IS_SELECTED;
	}
	stFinalSelectId=-1;
	stEntityEdit->m_DummyName.SetWindowText("");
}



static void SelectGAll(int id)	//�Ѱ��� id�� ��� �����Ѵ�.
{

	for(int i=0; i<stDummyEntitiesNum; i++)	
	{
		if( stDummyEntitiesList[i].flag & DUMMY_IS_FREEZE )	//������ȰŴ� ����.
			continue;
		if( id == GetAtt(stDummyEntitiesList[i].name) )
			stDummyEntitiesList[i].flag |= DUMMY_IS_SELECTED;
	}
}

static int SelectOneEntity(LONG x,LONG y,CLevel *level,Vector3f cam_pos)	//���� ����� ��ƼƼ�� �����Ѵ�.
{
	Vector3f temp;
	int id=-1;
	float dist=100000,temp_dist;

	int state = GetSelectState();

	for(int i=0; i<stDummyEntitiesNum; i++)	//�Ÿ��� �ճ���� ����.
	{
		if( stDummyEntitiesList[i].flag & DUMMY_IS_FREEZE ) //������ȰŴ� ����..
			continue;
//		if( !(stDummyEntitiesList[i].flag & DUMMY_IS_HERE) )
//			continue;
		if( state > 0 )	//add�ϰ�� 
		{
			if( (stDummyEntitiesList[i].flag & DUMMY_IS_SELECTED) )	//���õ� ���� ����.
				continue;
		}
		else
		if( state < 0 )	//������ ��� 
		{
			if( !(stDummyEntitiesList[i].flag & DUMMY_IS_SELECTED) )	//���þȵ� ���� ����.
				continue;
		}
		else
		{
			//stDummyEntitiesList[i].flag&=~DUMMY_IS_SELECTED;
		}

		if( level->IsCollisionRayAABB(x,y,stDummyEntitiesList[i].bb_min,stDummyEntitiesList[i].bb_max,&temp))
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
			stDummyEntitiesList[id].flag |= DUMMY_IS_SELECTED;
		else
		if( state < 0 )	//������ ��� 
			stDummyEntitiesList[id].flag&=~DUMMY_IS_SELECTED;
		else
		{
			for(i=0; i<stDummyEntitiesNum; i++)
			{
				stDummyEntitiesList[i].flag &= ~DUMMY_IS_SELECTED;
			}
			stDummyEntitiesList[id].flag |= DUMMY_IS_SELECTED;
			stCurEditDummy = GetAtt(stDummyEntitiesList[id].name);
			stEntityEdit->m_SelectScroll.SetCurSel(stCurEditDummy);
 
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

	for(int i=0; i<stDummyEntitiesNum; i++)	//�Ÿ��� �ճ���� ����.
	{
//		if( !(stDummyEntitiesList[i].flag & ENTITY_IS_VIEW) )
//			continue;
		if( !(stDummyEntitiesList[i].flag & DUMMY_IS_FREEZE) ) //������� �ȵȰŴ� ����..
			continue;
		if( level->IsCollisionRayAABB(x,y,stDummyEntitiesList[i].bb_min,stDummyEntitiesList[i].bb_max,&temp))
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
		stDummyEntitiesList[id].flag &= ~DUMMY_IS_FREEZE;
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

	for(int i=0; i<stDummyEntitiesNum; i++)	
	{
		if( !(stDummyEntitiesList[i].flag & DUMMY_IS_SELECTED) )	//���õ��� ���� ���� ����.
			continue;
		if( y_axis )
		{
			stDummyEntitiesList[i].pos[1]-=y;
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

			stDummyEntitiesList[i].pos[0]+=(billboard.m[0][0]*x+billboard.m[0][2]*-y);
			stDummyEntitiesList[i].pos[2]+=(billboard.m[2][0]*x+billboard.m[2][2]*-y);
		}
	}
}

//���õ� ��ƼƼ���� ��ġ�� �ű��.
static void RotSelectedEntities(LONG y_diff )
{
#define _SXSY_TO_ROT	22.2f
	float y=(float)y_diff*_SXSY_TO_ROT;

	if( y == 0 )
		return;

	for(int i=0; i<stDummyEntitiesNum; i++)	
	{
		if( !(stDummyEntitiesList[i].flag & DUMMY_IS_SELECTED) )	//���õ��� ���� ���� ����.
			continue;

		stDummyEntitiesList[i].y_rot += y*R3GetLoopTime();
	}
	stCurRot += y*R3GetLoopTime();
}

static void ChangeSelectedEntities(Vector3f scale,float attn,float rot_y )
{

	for(int i=0; i<stDummyEntitiesNum; i++)	
	{
		if( !(stDummyEntitiesList[i].flag & DUMMY_IS_SELECTED) )	//���õ��� ���� ���� ����.
			continue;
		if( scale )
		{
			stDummyEntitiesList[i].scale[0]=scale[0];
			stDummyEntitiesList[i].scale[1]=scale[1];
			stDummyEntitiesList[i].scale[2]=scale[2];
		}
		stDummyEntitiesList[i].y_rot = rot_y;
	}
}

static void DeleteSelectedEntities()	// ���õǾ��� ��ƼƼ���� �����.
{
	while( 1 )
	{
		for(int i=0; i<stDummyEntitiesNum; i++)	//�Ÿ��� �ճ���� ����.
		{
			if( !(stDummyEntitiesList[i].flag & DUMMY_IS_SELECTED) )	//���õ��� ���� ���� ����.
				continue;
			break;
		}
		if( i == stDummyEntitiesNum )
			break;

		memcpy(&stDummyEntitiesList[i],&stDummyEntitiesList[i+1],sizeof(_SAVE_DUMMY_ENTITIES_LIST)*(stDummyEntitiesNum-i-1));
		stDummyEntitiesNum--;
	}
}
static void FixTooDist()	//�Ÿ��� ���� �ȵǴ°��� �ִ°� �����.
{
	UnSelectAllEntities();
	for(int i=0; i<stDummyEntitiesNum; i++)	//�Ÿ��� �ճ���� ����.
	{
		if( stDummyEntitiesList[i].pos[0] > 32000 || stDummyEntitiesList[i].pos[0] < -32000 
		|| stDummyEntitiesList[i].pos[1] > 32000 || stDummyEntitiesList[i].pos[1] < -32000 
		|| stDummyEntitiesList[i].pos[2] > 32000 || stDummyEntitiesList[i].pos[2] < -32000 
			)
		{
			stDummyEntitiesList[i].flag |= DUMMY_IS_SELECTED;
		}
	}
	DeleteSelectedEntities();
}

void CalcDummyBBox()
{
	for(int i=0; i<stDummyEntitiesNum; i++)	
	{
		stDummyEntitiesList[i].bb_min[0] = stDummyEntitiesList[i].pos[0] - stDummyEntitiesList[i].scale[0];
		stDummyEntitiesList[i].bb_min[1] = stDummyEntitiesList[i].pos[1] - stDummyEntitiesList[i].scale[1];
		stDummyEntitiesList[i].bb_min[2] = stDummyEntitiesList[i].pos[2] - stDummyEntitiesList[i].scale[2];

		stDummyEntitiesList[i].bb_max[0] = stDummyEntitiesList[i].pos[0] + stDummyEntitiesList[i].scale[0];
		stDummyEntitiesList[i].bb_max[1] = stDummyEntitiesList[i].pos[1] + stDummyEntitiesList[i].scale[1];
		stDummyEntitiesList[i].bb_max[2] = stDummyEntitiesList[i].pos[2] + stDummyEntitiesList[i].scale[2];
	}
}

static BOOL stIsCurEntity;
void ProgressDummyEdit(void *v_level,Vector3f cam_pos)
{
	if( v_level == NULL )
		return;

	LONG mouse_x = GetRectMouseX(gMouse.x);	//�ػ󵵿����缭..
	LONG mouse_y = GetRectMouseY(gMouse.y);	//�ػ󵵿� ���缭..
	CLevel *level = (CLevel *)v_level;
	CBsp *bsp = level->mBsp;

	int what_mode = WhatDummyMode();

	if( !level->IsLoadedBsp() )
		return;
/*
	BOOL is_coll;
	Vector3f pos;

	stIsCurEntity=FALSE;
	if( what_mode == _DUMMY_EDIT_MODE )
	{
		if( GetPickingStyle() == _PICKING_STYLE_FAR )
			is_coll=level->GetPointFromScreenRayFar(mouse_x,mouse_y,&pos);//���콺 �������� ����.
		else
			is_coll=level->GetPointFromScreenRay(mouse_x,mouse_y,&pos);//���콺 �������� ����.

		if( is_coll )
		{
			stIsCurEntity=TRUE;
			Vector3fCopy(pos,stCurEntityPos);
		}
	}
*/
	if (gMouse.button & _LEFT_BUTTON_DOWN)	//����
	{           
//		gMouse.button &= (~_LEFT_BUTTON_DOWN);

		if( stOneClickEntity == FALSE )
		{
			stOneClickEntity=TRUE;
			stOldEntityMx = (float)mouse_x;
			stOldEntityMy = (float)mouse_y;


			BOOL is_coll;
			Vector3f pos;

			if( what_mode == _DUMMY_EDIT_MODE )
			{
				if( GetPickingStyle() == _PICKING_STYLE_FAR )
					is_coll=level->GetPointFromScreenRayFar(mouse_x,mouse_y,&pos);//���콺 �������� ����.
				else
					is_coll=level->GetPointFromScreenRay(mouse_x,mouse_y,&pos);//���콺 �������� ����.
				if( level->mBsp->mPickPoly == -1 )	//���콺�� Ŭ���� �ȵȰ��̴�.
					is_coll = FALSE;


				if( is_coll )
				{
					Vector3fCopy(pos,stCurEntityPos);
					if(stCurEditMode == _DUMMY_EDIT_POS_MODE )
					{
						if( IsNearDistExist(stCurEntityPos) )	//��ƼƼ�� �ʹ� ��ó�� �ִ°��.
						{
							Warning("���� ��ġ�� ���� ������!","");
						}
						else
						{
							SetPreDummyString();	//����Ʈ �ڽ��� �����̸��� �̸� ����ش�.
							stEntityEdit->DummyNameDialog.ShowWindow(SW_SHOWNORMAL);
						}
					}
					else
					if(stCurEditMode == _DUMMY_EDIT_SCALE_MODE )
					{
					}
					else
					if(stCurEditMode == _DUMMY_EDIT_ROT_MODE )
					{
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
						//��ƼƼ ���É�����.... ó��������..
						stFinalSelectId=-1;	//�ȿ��� ��Ʈ�� �ٲܶ� ���� ����...
						stEntityEdit->m_DummyName.SetWindowText(stDummyEntitiesList[id].name);
						stCurScale[0]=stDummyEntitiesList[id].scale[0];
						stCurScale[1]=stDummyEntitiesList[id].scale[1];
						stCurScale[2]=stDummyEntitiesList[id].scale[2];
						SetScaleString(stCurScale);
						stCurRot = stDummyEntitiesList[id].y_rot;
						SetRotString(stCurRot);
						stFinalSelectId=id;
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
		case _DUMMY_EDIT_POS_MODE:
			if( what_mode == _DUMMY_SELECT_MODE )
				MoveSelectedEntities(GetYButton(),GetXZButton(),mouse_x-(LONG)stOldEntityMx
					,mouse_y-(LONG)stOldEntityMy);
			break;
		case _DUMMY_EDIT_SCALE_MODE:
			if( GetYButton() )
			{
				stCurScale[1]+=(mouse_y-stOldEntityMy)*0.5f;
				if( stCurScale[1] < 2 )
					stCurScale[1] = 2;
			}
			else
			if( GetXZButton() )
			{
				stCurScale[0]+=(mouse_x-stOldEntityMx)*0.5f;
				stCurScale[2]+=(mouse_y-stOldEntityMy)*0.5f;
				if( stCurScale[0] < 2 )
					stCurScale[0] = 2;
				if( stCurScale[2] < 2 )
					stCurScale[2] = 2;
			}
			ChangeSelectedEntities(stCurScale,-1,stCurRot );
			SetScaleString(stCurScale);
			SetRotString(stCurRot);
			break;
		case _DUMMY_EDIT_ROT_MODE:
			RotSelectedEntities(mouse_y-(LONG)stOldEntityMy);
			SetRotString(stCurRot);
			break;
		}
		stOldEntityMx = (float)mouse_x;
		stOldEntityMy = (float)mouse_y;
	}
	if( gAsci[VK_DELETE] )
	{
		gAsci[VK_DELETE]=0;
		DeleteSelectedEntities();
	}
	CalcDummyBBox();
}

void DrawDummyEntities()
{
	int cnt=0,i;
	_D3DR3VERTEX_TEX0 vertex[1000*3];
	DWORD tc;

	if( !stDummyEntitiesNum )
		return;
    D3DXMATRIX matWorld;
    D3DXMatrixIdentity( &matWorld );
    GetD3dDevice()->SetTransform( D3DTS_WORLD, &matWorld );
    GetD3dDevice()->SetTexture( 0,NULL);
//    GetD3dDevice()->SetRenderState( D3DRS_ZENABLE, FALSE );

	tc=(0x80ff4f00);

#define LineSet(v,cc) {	vertex[cnt].x = v[0];	vertex[cnt].y = v[1];	vertex[cnt].z = v[2];		vertex[cnt].color = cc; cnt++;};

	Vector3f pos,temp,scale,ver_f[8];
	float mat[4][4];
	for( i=0; i<stDummyEntitiesNum; i++)
	{
		if( stDummyEntitiesList[i].flag & DUMMY_IS_SELECTED )
			tc=(0x80ff009f);
		else
		if( stDummyEntitiesList[i].flag & DUMMY_IS_FREEZE )
			tc=(0x8000ffff);
		else
			tc=(0x80ff4f00);
		MatrixRotateY(mat,stDummyEntitiesList[i].y_rot);
		Vector3fCopy(stDummyEntitiesList[i].scale,scale);

		temp[0] = scale[0];		temp[1] = scale[1];		temp[2] = scale[2];
		Vector3fTransform(pos,temp,mat);
		Vector3fAdd(pos,stDummyEntitiesList[i].pos,ver_f[0]);

		temp[0] = -scale[0];		temp[1] = scale[1];		temp[2] = scale[2];
		Vector3fTransform(pos,temp,mat);
		Vector3fAdd(pos,stDummyEntitiesList[i].pos,ver_f[1]);

		temp[0] = -scale[0];		temp[1] = -scale[1];		temp[2] = scale[2];
		Vector3fTransform(pos,temp,mat);
		Vector3fAdd(pos,stDummyEntitiesList[i].pos,ver_f[2]);

		temp[0] = scale[0];		temp[1] = -scale[1];		temp[2] = scale[2];
		Vector3fTransform(pos,temp,mat);
		Vector3fAdd(pos,stDummyEntitiesList[i].pos,ver_f[3]);


		temp[0] = scale[0];		temp[1] = scale[1];		temp[2] = -scale[2];
		Vector3fTransform(pos,temp,mat);
		Vector3fAdd(pos,stDummyEntitiesList[i].pos,ver_f[4]);

		temp[0] = -scale[0];		temp[1] = scale[1];		temp[2] = -scale[2];
		Vector3fTransform(pos,temp,mat);
		Vector3fAdd(pos,stDummyEntitiesList[i].pos,ver_f[5]);

		temp[0] = -scale[0];		temp[1] = -scale[1];		temp[2] = -scale[2];
		Vector3fTransform(pos,temp,mat);
		Vector3fAdd(pos,stDummyEntitiesList[i].pos,ver_f[6]);

		temp[0] = scale[0];		temp[1] = -scale[1];		temp[2] = -scale[2];
		Vector3fTransform(pos,temp,mat);
		Vector3fAdd(pos,stDummyEntitiesList[i].pos,ver_f[7]);

		LineSet(ver_f[0],tc);
		LineSet(ver_f[1],tc);
		LineSet(ver_f[1],tc);
		LineSet(ver_f[2],tc);
		LineSet(ver_f[2],tc);
		LineSet(ver_f[3],tc);
		LineSet(ver_f[3],tc);
		LineSet(ver_f[0],tc);


		LineSet(ver_f[4],tc);
		LineSet(ver_f[5],tc);
		LineSet(ver_f[5],tc);
		LineSet(ver_f[6],tc);
		LineSet(ver_f[6],tc);
		LineSet(ver_f[7],tc);
		LineSet(ver_f[7],tc);
		LineSet(ver_f[4],tc);

		LineSet(ver_f[0],tc);
		LineSet(ver_f[4],tc);
		LineSet(ver_f[1],tc);
		LineSet(ver_f[5],tc);
		LineSet(ver_f[2],tc);
		LineSet(ver_f[6],tc);
		LineSet(ver_f[3],tc);
		LineSet(ver_f[7],tc);
/*
		LineSet(stDummyEntitiesList[i].bb_min[0],stDummyEntitiesList[i].bb_min[1],stDummyEntitiesList[i].bb_min[2],tc);
		LineSet(stDummyEntitiesList[i].bb_max[0],stDummyEntitiesList[i].bb_min[1],stDummyEntitiesList[i].bb_min[2],tc);
		LineSet(stDummyEntitiesList[i].bb_min[0],stDummyEntitiesList[i].bb_min[1],stDummyEntitiesList[i].bb_min[2],tc);
		LineSet(stDummyEntitiesList[i].bb_min[0],stDummyEntitiesList[i].bb_min[1],stDummyEntitiesList[i].bb_max[2],tc);
		LineSet(stDummyEntitiesList[i].bb_max[0],stDummyEntitiesList[i].bb_min[1],stDummyEntitiesList[i].bb_min[2],tc);
		LineSet(stDummyEntitiesList[i].bb_max[0],stDummyEntitiesList[i].bb_min[1],stDummyEntitiesList[i].bb_max[2],tc);
		LineSet(stDummyEntitiesList[i].bb_min[0],stDummyEntitiesList[i].bb_min[1],stDummyEntitiesList[i].bb_max[2],tc);
		LineSet(stDummyEntitiesList[i].bb_max[0],stDummyEntitiesList[i].bb_min[1],stDummyEntitiesList[i].bb_max[2],tc);

		LineSet(stDummyEntitiesList[i].bb_min[0],stDummyEntitiesList[i].bb_max[1],stDummyEntitiesList[i].bb_min[2],tc);
		LineSet(stDummyEntitiesList[i].bb_max[0],stDummyEntitiesList[i].bb_max[1],stDummyEntitiesList[i].bb_min[2],tc);
		LineSet(stDummyEntitiesList[i].bb_min[0],stDummyEntitiesList[i].bb_max[1],stDummyEntitiesList[i].bb_min[2],tc);
		LineSet(stDummyEntitiesList[i].bb_min[0],stDummyEntitiesList[i].bb_max[1],stDummyEntitiesList[i].bb_max[2],tc);
		LineSet(stDummyEntitiesList[i].bb_max[0],stDummyEntitiesList[i].bb_max[1],stDummyEntitiesList[i].bb_min[2],tc);
		LineSet(stDummyEntitiesList[i].bb_max[0],stDummyEntitiesList[i].bb_max[1],stDummyEntitiesList[i].bb_max[2],tc);
		LineSet(stDummyEntitiesList[i].bb_min[0],stDummyEntitiesList[i].bb_max[1],stDummyEntitiesList[i].bb_max[2],tc);
		LineSet(stDummyEntitiesList[i].bb_max[0],stDummyEntitiesList[i].bb_max[1],stDummyEntitiesList[i].bb_max[2],tc);

		LineSet(stDummyEntitiesList[i].bb_min[0],stDummyEntitiesList[i].bb_min[1],stDummyEntitiesList[i].bb_min[2],tc);
		LineSet(stDummyEntitiesList[i].bb_min[0],stDummyEntitiesList[i].bb_max[1],stDummyEntitiesList[i].bb_min[2],tc);
		LineSet(stDummyEntitiesList[i].bb_max[0],stDummyEntitiesList[i].bb_min[1],stDummyEntitiesList[i].bb_min[2],tc);
		LineSet(stDummyEntitiesList[i].bb_max[0],stDummyEntitiesList[i].bb_max[1],stDummyEntitiesList[i].bb_min[2],tc);
		LineSet(stDummyEntitiesList[i].bb_min[0],stDummyEntitiesList[i].bb_min[1],stDummyEntitiesList[i].bb_max[2],tc);
		LineSet(stDummyEntitiesList[i].bb_min[0],stDummyEntitiesList[i].bb_max[1],stDummyEntitiesList[i].bb_max[2],tc);
		LineSet(stDummyEntitiesList[i].bb_max[0],stDummyEntitiesList[i].bb_min[1],stDummyEntitiesList[i].bb_max[2],tc);
		LineSet(stDummyEntitiesList[i].bb_max[0],stDummyEntitiesList[i].bb_max[1],stDummyEntitiesList[i].bb_max[2],tc);

*/
		_R3ENGINE_STATE *state = GetR3State();
		Vector4f temp;
		state->mMatViewProj=state->mMatView*state->mMatProj;
		if( TransformVertex( temp, state->mMatViewProj.m,stDummyEntitiesList[i].pos)!=-8 )
		{
			char temp_buf[256];
			int id_temp = GetAtt(stDummyEntitiesList[i].name);
			sprintf(temp_buf,"%s : %s",stDummyEntitiesList[i].name,stInitScrollBar[id_temp]);
			temp[0]-=strlen(temp_buf)*3;
			temp[1]-=10;
			DrawR3Hangul3D(temp,temp_buf,0xffffffff,R3_HAN_SHADOW);
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
void CDummyEdit::OnDestroy() 
{
	CDialog::OnDestroy();
	
	stExtDummy.ReleaseExtDummy();
	// TODO: Add your message handler code here
	
}


BOOL CDummyEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int i;

	stEntityEdit = this;
	CheckDlgButton(IDC_RADIO1,TRUE);	//����Ʈ�� ����Ʈ��..

	IsOnInitDialog = TRUE;
	m_EditScroll.ResetContent();	//���ڿ�����Ʈ�� �������..
	m_SelectScroll.ResetContent();	//���ڿ�����Ʈ�� �������..

	char buf[256];

	sprintf(buf,"%d",(int)stCurScale[0]);
	m_ScaleX.SetWindowText(buf);
	sprintf(buf,"%d",(int)stCurScale[1]);
	m_ScaleY.SetWindowText(buf);
	sprintf(buf,"%d",(int)stCurScale[2]);
	m_ScaleZ.SetWindowText(buf);

	sprintf(buf,"%f",stCurRot);
	m_Rot.SetWindowText(buf);

	for(i=0; i<10000; i++)
	{
		if( stInitScrollBar[i][0] == NULL )
			break;
		m_EditScroll.InsertString(i, stInitScrollBar[i]);
		m_SelectScroll.InsertString(i, stInitScrollBar[i]);
	}
	m_EditScroll.SetCurSel(0);
	m_SelectScroll.SetCurSel(0);
	stDummyAttNum = i;

	CheckDlgButton(IDC_CHECK7,TRUE);	//pos ���
	
	// �����̸� ���̾�� �ڽ�...
	DummyNameDialog.Create(IDD_DUMMY_DIALOG,this);
	DummyNameDialog.ShowWindow(SW_HIDE);

	DummySearchDialog.Create(IDD_DUMMY_SEARCH_DIALOG,this);
	DummySearchDialog.ShowWindow(SW_HIDE);

	DummySplitDialog.Create(IDD_DUMMY_SPLIT_DIALOG,this);
	DummySplitDialog.ShowWindow(SW_HIDE);

	DummyErrorDialog.Create(IDD_DUMMY_ERROR_DIALOG,this);
	DummyErrorDialog.ShowWindow(SW_HIDE);
//	DummyNameDialog.ShowWindow(SW_SHOWNORMAL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDummyEdit::OnCheck7()		//��ġ �۾� �÷���
{
	// TODO: Add your control notification handler code here
	CheckDlgButton(IDC_CHECK7,TRUE);
	CheckDlgButton(IDC_CHECK10,FALSE);
	CheckDlgButton(IDC_CHECK15,FALSE);
	stCurEditMode=_DUMMY_EDIT_POS_MODE;		//scale��� 
}

void CDummyEdit::OnCheck10()	//������ �۾� �÷���...
{
	// TODO: Add your control notification handler code here
	CheckDlgButton(IDC_CHECK7,FALSE);
	CheckDlgButton(IDC_CHECK15,FALSE);
	CheckDlgButton(IDC_CHECK10,TRUE);
	stCurEditMode=_DUMMY_EDIT_SCALE_MODE;		//scale��� 
}

void CDummyEdit::OnCheck15()	//ȸ�� �۾� �÷���...
{
	// TODO: Add your control notification handler code here
	CheckDlgButton(IDC_CHECK7,FALSE);
	CheckDlgButton(IDC_CHECK10,FALSE);
	CheckDlgButton(IDC_CHECK15,TRUE);
	stCurEditMode=_DUMMY_EDIT_ROT_MODE;		//scale��� 
}

void CDummyEdit::OnRadio2() //���ø��..
{
	// TODO: Add your control notification handler code here
	SetDummyMode(_DUMMY_SELECT_MODE);
}

void CDummyEdit::OnRadio1() //�������
{
	// TODO: Add your control notification handler code here
	SetDummyMode(_DUMMY_EDIT_MODE);
}

void CDummyEdit::OnButton3()	//����..
{
	// TODO: Add your control notification handler code here
	DeleteSelectedEntities();
}

void CDummyEdit::OnSelchangeCombo1() //���� ����Ʈ ���� �Ӽ� ǥ��...
{
	stCurEditDummyAtt = m_EditScroll.GetCurSel();
	if( stCurEditDummyAtt == -1 )	//����Ʈ�� �ϳ�.
	{
		stCurEditDummyAtt=0;
		m_EditScroll.SetCurSel(stCurEditDummyAtt);
		return;
	}
}

void CDummyEdit::OnSelchangeCombo3() //�׷� ���ý� �Ӽ� ǥ��...
{
	stCurSelectDummyAtt = m_SelectScroll.GetCurSel();
	if( stCurSelectDummyAtt == -1 )	//����Ʈ�� �ϳ�.
	{
		stCurSelectDummyAtt=0;
		m_SelectScroll.SetCurSel(stCurSelectDummyAtt);
		return;
	}
	SelectGAll(stCurSelectDummyAtt);//�Ѱ��� id�� ��� �����Ѵ�.
}

void CDummyNameDialog::OnOK()	//�����̸��� �������....
{
	// TODO: Add extra validation here
	CDialog::OnOK();

	if( stDummyEntitiesList == NULL )
	{
		stMaxDummyEntitiesNum=64;
		stDummyEntitiesList=(_SAVE_DUMMY_ENTITIES_LIST *)Dmalloc(stMaxDummyEntitiesNum*sizeof(_SAVE_DUMMY_ENTITIES_LIST));
	}

	memset(&stDummyEntitiesList[stDummyEntitiesNum],0,sizeof(_SAVE_DUMMY_ENTITIES_LIST));

	stDummyEntitiesList[stDummyEntitiesNum].flag = 0;

	// ���⿡�� �̸� �߰� �۾��� �Ұ�...
	//stDummyEntitiesList[stDummyEntitiesNum].id = stCurEditDummy;
	Vector3fCopy(stCurEntityPos,stDummyEntitiesList[stDummyEntitiesNum].pos);
	
	char buf[256];
	m_DummyName.GetWindowText(buf,256);

	for( LONG i=0; i< stDummyEntitiesNum; i++)
	{
		if(!strcmp(stDummyEntitiesList[i].name,buf ))	//���� ���ٸ�..
		{
			Warning(buf,"<- ���� �̸��� ���̰� �̹� �ֽ��ϴ�.");
			return;
		}
	}

	strcpy(stDummyEntitiesList[stDummyEntitiesNum].name,buf );

	stDummyEntitiesList[stDummyEntitiesNum].scale[0]=stCurScale[0];
	stDummyEntitiesList[stDummyEntitiesNum].scale[1]=stCurScale[1];
	stDummyEntitiesList[stDummyEntitiesNum].scale[2]=stCurScale[2];
	stDummyEntitiesList[stDummyEntitiesNum].y_rot=stCurRot;
	
	stDummyEntitiesNum++;
	if( stDummyEntitiesNum >= stMaxDummyEntitiesNum )
	{
		stDummyEntitiesList=(_SAVE_DUMMY_ENTITIES_LIST *)ReAlloc(stDummyEntitiesList,stMaxDummyEntitiesNum*sizeof(_SAVE_DUMMY_ENTITIES_LIST),(stMaxDummyEntitiesNum+32)*sizeof(_SAVE_DUMMY_ENTITIES_LIST));
		stMaxDummyEntitiesNum+=32;
	}
}

void CDummyNameDialog::OnCancel()	//���� ������... 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}


void CDummyNameDialog::OnChangeEdit2()	//���� �̸�...
{
	
}

//������ x,y,z
void CDummyEdit::OnChangeEdit15() 
{
	char buf[256];
	m_ScaleX.GetWindowText(buf,256);

	int i;
	for(i=0; i<(int)strlen(buf); i++)
	{
		if( i >= 5 || (buf[i] < '0' || buf[i] > '9') && buf[i] !='.' )
		{
			buf[i]=NULL;
			m_ScaleX.SetWindowText(buf);
		}
	}
	stCurScale[0] = (float)atof(buf);
	if( stCurScale[0]==0 )
		stCurScale[0]=1;
}

void CDummyEdit::OnChangeEdit16() 
{
	char buf[256];
	m_ScaleY.GetWindowText(buf,256);

	int i;
	for(i=0; i<(int)strlen(buf); i++)
	{
		if( i >= 5 || (buf[i] < '0' || buf[i] > '9') && buf[i] !='.' )
		{
			buf[i]=NULL;
			m_ScaleY.SetWindowText(buf);
		}
	}
	stCurScale[1] = (float)atof(buf);
	if( stCurScale[1]==0 )
		stCurScale[1]=1;
}

void CDummyEdit::OnChangeEdit17() 
{
	char buf[256];
	m_ScaleZ.GetWindowText(buf,256);

	int i;
	for(i=0; i<(int)strlen(buf); i++)
	{
		if( i >= 5 || (buf[i] < '0' || buf[i] > '9') && buf[i] !='.' )
		{
			buf[i]=NULL;
			m_ScaleZ.SetWindowText(buf);
		}
	}
	stCurScale[2] = (float)atof(buf);
	if( stCurScale[2]==0 )
		stCurScale[2]=1;
}

void CDummyEdit::OnChangeEdit23() 
{
	char buf[256];
	m_Rot.GetWindowText(buf,256);

	int i;
	for(i=0; i<(int)strlen(buf); i++)
	{
		if( i >= 7 || (buf[i] < '0' || buf[i] > '9') && buf[i] !='.' && buf[i] !='-')
		{
			buf[i]=NULL;
			m_Rot.SetWindowText(buf);
		}
	}
	stCurRot = (float)atof(buf);
	ChangeSelectedEntities(0,-1,stCurRot );
//	if( stCurRot==0 )
//		stCurRot=1;
}

void CDummyEdit::OnChangeEdit2() //�����̸� ����Ʈ�ϴ°�.
{
	if( stFinalSelectId == -1 )
		return;
	char buf[256];

	m_DummyName.GetWindowText(buf,256);
	for(int i=0; i<stDummyEntitiesNum; i++)	
	{
		if( !strcmp( stDummyEntitiesList[i].name, buf)) 
		{
			Warning("�ٸ� ���̰� ���� �̸��� �ֽ��ϴ�.","");
			return;
		}
	}
	
	strcpy( stDummyEntitiesList[stFinalSelectId].name, buf);
}

void CDummyEdit::OnButton5()	//unselect ��...
{
	// TODO: Add your control notification handler code here
	
	UnSelectAllEntities();
}



void CDummyEdit::OnButton1() //������
{
	for(int i=0; i<stDummyEntitiesNum; i++)	//���õȰ��� freeze ����.
	{
		if( stDummyEntitiesList[i].flag & DUMMY_IS_SELECTED )
		{
			stDummyEntitiesList[i].flag &= ~DUMMY_IS_SELECTED;	//������ ����.
			stDummyEntitiesList[i].flag |= DUMMY_IS_FREEZE;
		}
	}
}

void CDummyEdit::OnCheck18() //��������
{
	// TODO: Add your control notification handler code here
	stUnFreezeState = !stUnFreezeState;
}

void CDummyEdit::OnButton2() //�������� ��.
{
	// TODO: Add your control notification handler code here
	for(int i=0; i<stDummyEntitiesNum; i++)	//��� un������ ����.
	{
		stDummyEntitiesList[i].flag&=~DUMMY_IS_FREEZE;
	}
}
/////////////////////////////////////////////////////////////////////////////
// CDummySearchDialog dialog


CDummySearchDialog::CDummySearchDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDummySearchDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDummySearchDialog)
	//}}AFX_DATA_INIT
}


void CDummySearchDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDummySearchDialog)
	DDX_Control(pDX, IDC_EDIT2, m_DummyName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDummySearchDialog, CDialog)
	//{{AFX_MSG_MAP(CDummySearchDialog)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDummySearchDialog message handlers

extern void SetMainPos(Vector3f pos);

void CDummySearchDialog::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();

	char buf[256];
	m_DummyName.GetWindowText(buf,256);	
	if( buf[0] == NULL )
		return;

	for( LONG i=0; i< stDummyEntitiesNum; i++)
	{
		if(!strcmp(stDummyEntitiesList[i].name,buf ))	//���� ���ٸ�..
		{
			SetMainPos(stDummyEntitiesList[i].pos);
			return;
		}
	}
	Warning(buf,"<- ã���� �ϴ� ���̰� �����ϴ�.");
}

void CDummySearchDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
	
}

void CDummySearchDialog::OnChangeEdit2() 
{
}

void CDummyEdit::OnButton10() 
{
	// TODO: Add your control notification handler code here
	stEntityEdit->DummySearchDialog.ShowWindow(SW_SHOWNORMAL);
}




void CDummyEdit::OnButton11() 
{
	// TODO: Add your control notification handler code here
	stEntityEdit->DummySplitDialog.ShowWindow(SW_SHOWNORMAL);
}
/////////////////////////////////////////////////////////////////////////////
// CDummySplitDialog dialog


CDummySplitDialog::CDummySplitDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDummySplitDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDummySplitDialog)
	//}}AFX_DATA_INIT
}


void CDummySplitDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDummySplitDialog)
	DDX_Control(pDX, IDC_EDIT24, m_Height);
	DDX_Control(pDX, IDC_EDIT15, m_Width);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDummySplitDialog, CDialog)
	//{{AFX_MSG_MAP(CDummySplitDialog)
	ON_EN_CHANGE(IDC_EDIT15, OnChangeEdit15)
	ON_EN_CHANGE(IDC_EDIT24, OnChangeEdit24)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDummySplitDialog message handlers

void CDummySplitDialog::OnChangeEdit15() 
{
	char buf[256];
	m_Width.GetWindowText(buf,256);

	int i;
	for(i=0; i<(int)strlen(buf); i++)
	{
		if( i >= 7 || (buf[i] < '0' || buf[i] > '9'))
		{
			buf[i]=NULL;
			m_Width.SetWindowText(buf);
		}
	}
}

void CDummySplitDialog::OnChangeEdit24() 
{
	char buf[256];
	m_Height.GetWindowText(buf,256);

	int i;
	for(i=0; i<(int)strlen(buf); i++)
	{
		if( i >= 7 || (buf[i] < '0' || buf[i] > '9'))
		{
			buf[i]=NULL;
			m_Height.SetWindowText(buf);
		}
	}
}

void CDummySplitDialog::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();

	char buf1[256];
	char buf2[256];
	LONG w,h;
	m_Width.GetWindowText(buf1,256);	
	m_Height.GetWindowText(buf2,256);	
	if( buf1[0] == NULL || buf2[0] == NULL )
	{
		Warning("���μ��ΰ��� ��� 1�̻��̿��� �մϴ�.","");
		return;
	}
	w=atoi(buf1);
	h=atoi(buf2);
	if( w < 1  || h < 1 )
	{
		Warning("���μ��ΰ��� ��� 1�̻��̿��� �մϴ�.","");
		return;
	}

	LONG i,j,k,cnt=0,id;
	for( i=0; i<stDummyEntitiesNum; i++)	
	{
		if( !(stDummyEntitiesList[i].flag & DUMMY_IS_SELECTED) )	//���õ��� ���� ���� ����.
			continue;
		id=i;
		cnt++;
	}
	if( cnt != 1 )
	{
		Warning("���õǾ��� ���̰� 1���� �Ǿ� �մϴ�..","");
		return;
	}
	DWORD attr = GetAtt(stDummyEntitiesList[id].name);
	DWORD start_index=0;//GetLinearDummyNum(attr);	//���̾� �����ִ� �ε����� ã�´�.

	float garo_leng=(stDummyEntitiesList[id].scale[0]*2.0f)/w;
	float sero_leng=(stDummyEntitiesList[id].scale[2]*2.0f)/h;
	for( i=0; i< h; i++ )
	{
		for( j=0; j< w; j++ )
		{
			char cmp_buf[256];

			sprintf(cmp_buf,"%s_%d",stDummyEntitiesList[id].name,start_index++);
			for(k=0; k<stDummyEntitiesNum; k++)
			{
				if( !strcmp(stDummyEntitiesList[k].name,cmp_buf ))
				{
					Warning(cmp_buf,"<- �����̸��� ������ �̹� �ֽ��ϴ�.");
					return;
				}
			}
		}
	}
	start_index=0;

	Vector3f pos;
	if( stDummyEntitiesList == NULL )
	{
		stMaxDummyEntitiesNum=64;
		stDummyEntitiesList=(_SAVE_DUMMY_ENTITIES_LIST *)Dmalloc(stMaxDummyEntitiesNum*sizeof(_SAVE_DUMMY_ENTITIES_LIST));
	}

	float mat[4][4];

	MatrixRotateY(mat,stDummyEntitiesList[id].y_rot);
	//���� ����...
	for( i=0; i< h; i++ )
	{
		for( j=0; j< w; j++ )
		{
			memcpy(&stDummyEntitiesList[stDummyEntitiesNum],&stDummyEntitiesList[id],sizeof(_SAVE_DUMMY_ENTITIES_LIST));
			pos[0] = garo_leng*j-stDummyEntitiesList[id].scale[0]+garo_leng/2;
			pos[1] = 0;
			pos[2] = sero_leng*i-stDummyEntitiesList[id].scale[2]+sero_leng/2;
			Vector3fTransform(stDummyEntitiesList[stDummyEntitiesNum].pos,pos,mat);
			Vector3fAdd(stDummyEntitiesList[stDummyEntitiesNum].pos,stDummyEntitiesList[id].pos,stDummyEntitiesList[stDummyEntitiesNum].pos);

			stDummyEntitiesList[stDummyEntitiesNum].flag = 0;
			stDummyEntitiesList[stDummyEntitiesNum].scale[0]=garo_leng/2;
			stDummyEntitiesList[stDummyEntitiesNum].scale[2]=sero_leng/2;
			sprintf(stDummyEntitiesList[stDummyEntitiesNum].name,"%s_%d",stDummyEntitiesList[id].name,start_index++);

			stDummyEntitiesNum++;
			if( stDummyEntitiesNum >= stMaxDummyEntitiesNum )
			{
				stDummyEntitiesList=(_SAVE_DUMMY_ENTITIES_LIST *)ReAlloc(stDummyEntitiesList,stMaxDummyEntitiesNum*sizeof(_SAVE_DUMMY_ENTITIES_LIST),(stMaxDummyEntitiesNum+32)*sizeof(_SAVE_DUMMY_ENTITIES_LIST));
				stMaxDummyEntitiesNum+=32;
			}
		}
	}
	DeleteSelectedEntities();	//������ ���� ���̴� ����..
}

void CDummySplitDialog::OnCancel() 
{
//	CDialog::OnCancel();
}

extern CLevel *stLevel;	//���� Ŭ���� 
DWORD stErrorDummyId;
void CDummyEdit::OnButton12()	//���� ýũ
{
	// TODO: Add your control notification handler code here

	DWORD i,j,k;

	if(!stLevel )
		return;
	FixTooDist();	//�Ÿ��� ���� �ȵǸ� �����.

	Vector3f pos,w_pos;
	for( i=0; i<(DWORD)stDummyEntitiesNum; i++)	
	{
		float mat[4][4];
		MatrixRotateY(mat,stDummyEntitiesList[i].y_rot);

#define _DIVIDE_LENG	4.0f
		float garo_leng=stDummyEntitiesList[i].scale[0]*2/_DIVIDE_LENG;
		float sero_leng=stDummyEntitiesList[i].scale[2]*2/_DIVIDE_LENG;
		//���̿� ���� ���ҹ�....
		DWORD c_cnt=0;
		for(j=0; j<(DWORD)sero_leng; j++)
		{
			for(k=0; k<(DWORD)sero_leng; k++)
			{
				pos[0] = _DIVIDE_LENG*k-stDummyEntitiesList[i].scale[0]+_DIVIDE_LENG/2;
				pos[1] = 0;
				pos[2] = _DIVIDE_LENG*j-stDummyEntitiesList[i].scale[2]+_DIVIDE_LENG/2;
				Vector3fTransform(w_pos,pos,mat);
				Vector3fAdd(stDummyEntitiesList[i].pos,w_pos,w_pos);
				
				float y = stLevel->GetFirstYpos(w_pos,stDummyEntitiesList[i].bb_min,stDummyEntitiesList[i].bb_max);
				if( y != -65535 )
					continue;
//					goto _CDummyEdit_;
				c_cnt++;
			}
		}
		if( c_cnt*4 >= (DWORD)(sero_leng*sero_leng)*3 )	//�Ӿ� �̰� 1/4���� ���� �۴�.
		{
			stErrorDummyId = i;
			//��� ó������...
			char buf[256];
			sprintf(buf,"�߸��� ������ġ->%s",stDummyEntitiesList[i].name);
			stEntityEdit->DummyErrorDialog.SetWindowText(buf);
			stEntityEdit->DummyErrorDialog.ShowWindow(SW_SHOWNORMAL);
			return;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
// CDummyErrorDialog dialog


CDummyErrorDialog::CDummyErrorDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDummyErrorDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDummyErrorDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDummyErrorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDummyErrorDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDummyErrorDialog, CDialog)
	//{{AFX_MSG_MAP(CDummyErrorDialog)
	ON_BN_CLICKED(IDOK2, OnOk2)
	ON_BN_CLICKED(IDOK3, OnOk3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDummyErrorDialog message handlers

void CDummyErrorDialog::OnOK() 	//�����.
{
	CDialog::OnOK();
	//�����.
	stDummyEntitiesList[stErrorDummyId].flag |= DUMMY_IS_SELECTED;
	DeleteSelectedEntities();
}

void CDummyErrorDialog::OnOk2()	//�̵��Ѵ�.
{
	CDialog::OnOK();
	
	SetMainPos(stDummyEntitiesList[stErrorDummyId].pos);
}

void CDummyErrorDialog::OnCancel()	//���д�.
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CDummyErrorDialog::OnOk3() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
	
	DWORD i,j,k;

	if(!stLevel )
		return;
	FixTooDist();	//�Ÿ��� ���� �ȵǸ� �����.

	Vector3f pos,w_pos;
	for( i=0; i<(DWORD)stDummyEntitiesNum; i++)	
	{
		float mat[4][4];
		MatrixRotateY(mat,stDummyEntitiesList[i].y_rot);

#define _DIVIDE_LENG	4.0f
		float garo_leng=stDummyEntitiesList[i].scale[0]*2/_DIVIDE_LENG;
		float sero_leng=stDummyEntitiesList[i].scale[2]*2/_DIVIDE_LENG;
		//���̿� ���� ���ҹ�....
		DWORD c_cnt=0;
		for(j=0; j<(DWORD)sero_leng; j++)
		{
			for(k=0; k<(DWORD)sero_leng; k++)
			{
				pos[0] = _DIVIDE_LENG*k-stDummyEntitiesList[i].scale[0]+_DIVIDE_LENG/2;
				pos[1] = 0;
				pos[2] = _DIVIDE_LENG*j-stDummyEntitiesList[i].scale[2]+_DIVIDE_LENG/2;
				Vector3fTransform(w_pos,pos,mat);
				Vector3fAdd(stDummyEntitiesList[i].pos,w_pos,w_pos);
				
				float y = stLevel->GetFirstYpos(w_pos,stDummyEntitiesList[i].bb_min,stDummyEntitiesList[i].bb_max);
				if( y != -65535 )
					continue;
				c_cnt++;
			}
		}
		if( c_cnt*4 >= (DWORD)(sero_leng*sero_leng)*3 )	//�Ӿ� �̰� 1/4���� ���� �۴�.
		{
			stDummyEntitiesList[i].flag |= DUMMY_IS_SELECTED;
		}
	}
	DeleteSelectedEntities();
}
