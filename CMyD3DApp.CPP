
#define STRICT
#include "stdafx.h"
//#define D3D_OVERLOADS

#include <math.h>
#include <stdio.h>
#include "CMyD3dApp.h"

#include "DXUtil.h"
#include "resource.h"

//#include "R3D3d8.h"
#include "R3Move.h"
#include "R3Text.h"
#include "R3Util.h"
#include "CommonUtil.h"
#include "Level.h"
#include "R3EngineKernel.h"
#include "Console.h"
#include "RightBack.h"
#include "EffectCore.h"

#include "TabCollision.h"
#include "TabEntity.h"
#include "TabParticle.h"
#include "TabMagic.h"
#include "TabSound.h"
#include "TabDummy.h"
#include "ToolManager.h"
#include "2dsprite.h"
#include "r3sound.h"
#include "rtmovie.h"

#pragma	comment( lib, "d3d8.lib" )
#pragma	comment( lib, "d3dx8.lib" )


#define _CHARACTER

#ifdef	_CHARACTER

#include "Character.h"
ChInterface				*g_chIF	= NULL;
//TextureManager			*g_texture		= NULL;
AnimationManager		*g_animation	= NULL;
CObjectManager			*g_om_body		= NULL;
//CObjectManager			*g_om_sword		= NULL;

CHARACTEROBJECT			*g_bn;

#define		ANI0_NAME	"STOP.ANI"
#define		ANI1_NAME	"WALK_LOOP.ANI"
#define		ANI2_NAME	"Run_LOOP.ANI"

char ANI0_name[256];
char ANI1_name[256];
char ANI2_name[256];

static CHARACTEROBJECT	*co;
static CHARACTEROBJECT	*body;
//static CHARACTEROBJECT	*sword_f;

#pragma	comment( lib, "CharacterMDDLL.lib" )

#endif

/*
//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Entry point to the program. Initializes everything, and goes into a
//       message-processing loop. Idle time is used to render the scene.
//-----------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{

    if( FAILED( g_d3dApp.Create( hInst ) ) )    return 0;

    return g_d3dApp.Run();
}
*/





//-----------------------------------------------------------------------------
// Name: CR3D3DApp()
// Desc: Application constructor. Sets attributes for the app.
//-----------------------------------------------------------------------------
CR3D3DApp::CR3D3DApp()
{
    m_strWindowTitle  = _T( "R3BSP Viewer ver 0.98" );
    m_bUseDepthBuffer   = TRUE;

}
CR3D3DApp::~CR3D3DApp()
{
    Cleanup3DEnvironment();
}
//-----------------------------------------------------------------------------
// Name: OneTimeSceneInit()
// Desc: Called during initial app startup, this function performs all the
//       permanent initialization.
//-----------------------------------------------------------------------------

CLevel *stLevel=NULL;	//���� Ŭ���� 

#define _MOVE_SPEED		(100.0f)
#define _ADD_Y		16

Vector3f gPlayer={0,0,0};

//�׽�Ʈ �Լ� �������̽���..
R3Camera CameraTest;


#include "R3Input.h"
class Cplayer{
public:
	Vector3f mPos;	//��ġ.. �׳� pos[3]�̴� 
	Vector3f mTPos;	// Ÿ�� ��ġ(����...)
	BOOL mIsMove;	//	���� �����̰� �ִ°�?
	float mYangle;

	Cplayer();
	~Cplayer();
};
Cplayer::Cplayer()
{
	mPos[0]=0;	
	mPos[1]=0;
	mPos[2]=0;
	mTPos[0]=0;
	mTPos[1]=0;
	mTPos[2]=0;
	mIsMove=FALSE;
	mYangle=0;
}
Cplayer::~Cplayer()
{
}

Cplayer stPlayer;

void CR3D3DApp::InitD3DApp(HWND hwnd)
{
//    Create( AfxGetInstanceHandle() );
	InitR3Engine();		//R3������ �ʱ�ȭ�Ѵ�.
	InitR3SoundSystem(NULL);

	stLevel = new CLevel;
    m_hWnd = hwnd;

	//-------ȿ���κ��� ���Ѱ�...
	char path[256];
	char temp[256];
	char name[256];
//	GetPrivateProfileString("Path","EffectPath",".\\Entity\\Effect\\",path, 128,".\\RFMapTool.ini");
	_R3ENGINE_STATE *state = GetR3State();
	strcpy(path,state->EffectPath);

	GetPrivateProfileString("Path","EntityScriptFile","EffectEntityList.spt",name, 128,".\\RFMapTool.ini");
	strcpy(temp,path);
	strcat(temp,name);
	CreateEffectsList(temp);		//����Ʈ�� �ʱ�ȭ.
//	GetPrivateProfileString("Path","MagicScriptFile","MagicSptList.spt",name, 128,".\\RFMapTool.ini");
//	strcpy(temp,path);
//	strcat(temp,name);
//	CreateMagicSptList(temp);
	//--------------------------

//	���̷�Ʈ ��ǲ�� ��� ���Ϸ���...
	InitR3D3D( AfxGetInstanceHandle() ,0 );   //d3d�ʱ�ȭ.. 
//	InitConsole();
	R3LoadTextTexture();	//��Ʈ�ؽ��Ĵµ��� �÷��ش�.
//	RestoreDeviceObjects();


	SetConsoleStateB(OP_FLY,TRUE);
}

void CR3D3DApp::ReleaseD3DApp()
{
	/*
	if( stLevel )
	{
		stLevel->ReleaseLevel();
		delete stLevel;
	}*/
//    R3ReleaseAllTextures();	
//	ReleaseConsole();
	R3ReleaseTextTexture();	//��Ʈ�ؽ��ĸ� �����ش�.

//	InvalidateDeviceObjects();
	if( stLevel )
	{
		stLevel->ReleaseLevel();
		delete stLevel;
		stLevel=0;
	}
	ReleaseR3SoundSystem();
	ReleaseR3D3D();
	DestroyMagicSptList();	//����Ʈ�� ������
	DestroyEffectsList();		//����Ʈ�� ������
	ReleaseR3Engine();		//R3������ �ʱ�ȭ�Ѵ�.
}

static BOOL OneClick=FALSE;
static BOOL FirstCollision=FALSE;
static int stTabIndex;	//���� ���̾�� �ִ°�...

#define TAB_COLLISION	0	//�浹 
#define TAB_ENTITY		1// ��ƼƼ 
#define TAB_PARTICLE	2//��ƼŬ.
#define TAB_MAGIC		3//����.
#define TAB_SOUND		4//����.
#define TAB_DUMMY		5//����.


//�н����δ� �߰��κ�.
static Vector3f stPathStack[16];
static DWORD stPathStackCnt=0,stNowPathCnt=0;

BOOL GetNextPath(Vector3f *get)		//���������� ������ FALSE
{
	if( stNowPathCnt >= stPathStackCnt)
	{
		stNowPathCnt=0;
		return FALSE;
	}
	Vector3fCopy(stPathStack[stNowPathCnt],get[0]);
	stNowPathCnt++;
	return TRUE;
}

static LONG stSx,stSy,stEx,stEy;
void SetRenderRect(LONG sx,LONG sy,LONG ex,LONG ey)
{
	stSx=sx;
	stSy=sy;
	stEx=ex;
	stEy=ey;
}
LONG GetRectMouseX(LONG mouse_x)
{
	return (LONG)((gMouse.x*800)/stEx);
}
LONG GetRectMouseY(LONG mouse_y)
{
	return (LONG)((gMouse.y*607)/stEy);
}
static int st_Run=0;
void TestInterface()
{
	char buf[256];

	LONG mouse_x = (LONG)((gMouse.x*800)/stEx);
	LONG mouse_y = (LONG)((gMouse.y*607)/stEy);

	if( stLevel->IsLoadedBsp()==FALSE )
		return;

	Vector3f c_pos;
	static int startx, starty; // used for dragging
	static float viewanglex, viewangley, orgviewanglex, orgviewangley;

	if( !FirstCollision  )	//���ʿ� y ��ǥ�� ������ߵȴ�.
	{
		FirstCollision =TRUE;
		stPlayer.mPos[1] = stLevel->GetFirstYpos(stPlayer.mPos,-1);	//���� ���� �浹��ǥ�� ����.
		stPlayer.mTPos[1]=stPlayer.mPos[1];
	}

	//----fly�� �ȱ⸦ �ٲ㼭 ���߿� ���ִ� ��츦 �������� �����󿡼��� �ʿ����..
	if( 1 )
	{
		Vector3f pos,min,max;
		min[1]=stPlayer.mPos[1]-5;
		max[1]=stPlayer.mPos[1]+5;
		Vector3fCopy(stPlayer.mPos,pos);
		pos[1] = stLevel->GetFirstYpos(pos,min,max);	//	�ٿ�� �ڽ��������� y���� ���´�.
		if(pos[1] == -65535)	//���߿� ���ִ�..
		{
			min[1] = -30000;  
			pos[1] = stLevel->GetFirstYpos(pos,min,max);	//	�ٿ�� �ڽ��������� y���� ���´�.
			if(pos[1] != -65535)		//���� ��������� ����.
			{
				stPlayer.mPos[1] = pos[1];
			}
		}
	}

	if (gMouse.button & _RIGHT_BUTTON_DOWN) 
	{
		if( startx!=mouse_x || starty!=mouse_y )
		{
			viewanglex=orgviewanglex-(starty-mouse_y);
			viewangley=orgviewangley+(startx-mouse_x);
		}
	}
	else
	{	//���콺 ���� ...
		orgviewanglex=viewanglex; 
		orgviewangley=viewangley;
		startx = mouse_x; 
		starty = mouse_y;
	}

	if (gMouse.button & _LEFT_BUTTON_DOWN) 
	{           
//		gMouse.button &= (~_LEFT_BUTTON_DOWN);
		if( OneClick == FALSE )
		{
			OneClick = TRUE;

			if( stLevel->GetPointFromScreenRay(mouse_x,mouse_y,&c_pos)==TRUE)
			{
				GetColface()->GetPathFinder(stPlayer.mPos,c_pos,stPathStack,&stPathStackCnt);
/*
				Vector3f get;
				if( GetColface()->GetCrossPoint(stPlayer.mPos,c_pos,&get))
				{
					stPlayer.mTPos[0]=get[0];
					stPlayer.mTPos[1]=get[1];
					stPlayer.mTPos[2]=get[2];
				}
				else
				{			
					stPlayer.mTPos[0]=c_pos[0];
					stPlayer.mTPos[1]=c_pos[1];
					stPlayer.mTPos[2]=c_pos[2];
				}
*/
				stNowPathCnt=0;
				GetNextPath(&stPlayer.mTPos);
				stPlayer.mYangle=::GetYAngle(stPlayer.mPos,stPlayer.mTPos);
				stPlayer.mIsMove=TRUE;
#ifdef	_CHARACTER
				if( st_Run == 0  )
				{
					st_Run = 1;
					g_chIF->AnimationReset(co);
					sprintf(buf,"%s",ANI2_name);
					ChAnimation		*ani = g_chIF->GetAnimationData( g_animation, buf );
					g_chIF->MatchAnimationToMesh(ani,co );
				}
#endif
			}
		}
	}
	else
	{	//���콺 ���� ...
		OneClick=FALSE;
	}
	/*
	if( stNowPathCnt )
	{
		stPlayer.mYangle=::GetYAngle(stPlayer.mPos,stPlayer.mTPos);
		stPlayer.mIsMove=TRUE;
		stNowPathCnt--;
	}*/

	if( stPlayer.mIsMove )	//���������Ҷ���..
	{
		float dist = ::GetDist(stPlayer.mPos,stPlayer.mTPos);
		if( fabs( dist ) <=::R3GetLoopTime()*_MOVE_SPEED  )	//Ÿ�ٿ� ���� �����ߴ�.������ ����������..����� �Ҷ���...
		{
			if( GetNextPath(&stPlayer.mTPos)==FALSE )
			{
				stPlayer.mIsMove=FALSE;
#ifdef	_CHARACTER
			st_Run = 0;
			g_chIF->AnimationReset(co);
			sprintf(buf,"%s",ANI0_name);
			ChAnimation		*ani = g_chIF->GetAnimationData( g_animation, buf );
			g_chIF->MatchAnimationToMesh(ani,co );
#endif
			}
			else
			{
				stPlayer.mYangle=::GetYAngle(stPlayer.mPos,stPlayer.mTPos);
			}
		}
		else
		{
			float speed = ::R3GetLoopTime()*_MOVE_SPEED;
			Vector3f next_pos;
			//���� ������ ���ϰ�..
			next_pos[0] = stPlayer.mPos[0] + ((float)sin(AngleToPi(stPlayer.mYangle))*speed);
			next_pos[1] = stPlayer.mPos[1];
			next_pos[2] = stPlayer.mPos[2] + ((float)cos(AngleToPi(stPlayer.mYangle))*speed);
			//���� �����ִ��� ýũ�غ���..
			float get_ypos;
			BOOL can_go = stLevel->GetNextYpos(next_pos,&get_ypos);
			if( can_go )	//���� �ִ�..
			{
				stPlayer.mPos[0] = next_pos[0];
				stPlayer.mPos[1] = get_ypos;
				stPlayer.mPos[2] = next_pos[2];
			}
			else	//���� ��� ����..
			{
				stPlayer.mIsMove=FALSE;
				next_pos[0]=stPlayer.mPos[0];
				next_pos[2]=stPlayer.mPos[2];
#ifdef	_CHARACTER
				st_Run = 0;
				g_chIF->AnimationReset(co);
				sprintf(buf,"%s",ANI0_name);
				ChAnimation		*ani = g_chIF->GetAnimationData( g_animation, buf );
				g_chIF->MatchAnimationToMesh(ani,co );
#endif
			}
		}
	}

	if(viewanglex<-179)			//���Ʒ� ȸ���� ������ ���� 
		viewanglex =-179;
	if(viewanglex> 179)		
		viewanglex = 179;

	//ī�޶󿡰� bsp�����͸� �Ѱ��ش�. �ѹ��� ȣ���ص� �Ǵµ� �׻��ص� �ӵ������� �������.
	CameraTest.SetBspPtr(stLevel->mBsp);
	//ī�޶� ���ϰ� �ִ� �߽����� ����..  �÷��̾� ��ġ�� ī�޶� �ٶ󺸴� ����Ʈ...
	CameraTest.SetSmoothPointLoop(stPlayer.mPos[0],stPlayer.mPos[1]+_ADD_Y,stPlayer.mPos[2]);
	//ȸ���� �����Ϳ��� �Ÿ��� ����..
	CameraTest.SetSmoothThPhDistLoop(viewanglex,viewangley,(float)GetEditCameraDist());
/*
//----������ ī�޶� ������.
	//ī�޶� ���ϰ� �ִ� �߽����� ����..  �÷��̾� ��ġ�� ī�޶� �ٶ󺸴� ����Ʈ...
	CameraTest.SetPoint(stPlayer.mPos[0],stPlayer.mPos[1],stPlayer.mPos[2]);
	//ȸ���� �����Ϳ��� �Ÿ��� ����..
	CameraTest.SetThPhDist(viewanglex,viewangley,(float)GetEditCameraDist());
*/
	//��� ���� �������Ŀ� ī�޶��� ���� view Mat ������ �ѹ�ȣ�������ؾ��Ѵ�.
	CameraTest.MakeCameraAndViewMatrix();
#ifdef	_CHARACTER
	g_chIF->FrameStep(co);
	g_chIF->FrameMove(co);
	g_chIF->FrameMove(body);
//	g_chIF->FrameMove(sword_f);
#endif

}
#ifdef	_CHARACTER
void LoadTextureInDir(char *name)
{
	int start = strlen(name);
	while(name[start]!='\\'){start--;}start++;

//	g_texture->AddTexture(".\\tex",&name[start]);
	
}
void FileFindAndProcess( char *fdir,void (*func)( char * ))
{
	char buf[256];
	char dir[256];
	sprintf(buf,"%s\\*.*",fdir);
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	//hFind = FindFirstFile(".\\HEAD\\*", &FindFileData);
	hFind = FindFirstFile(buf , &FindFileData);
	int bWorking = true;
	if (hFind!=INVALID_HANDLE_VALUE)
	{
		while (bWorking)
		{
			bWorking = FindNextFile(hFind,&FindFileData);
			if (!bWorking) break;

			if (strcmp(".",FindFileData.cFileName)==0||strcmp("..",FindFileData.cFileName)==0)
			{
				continue;
			}
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				sprintf(dir,"%s\\%s",fdir,FindFileData.cFileName);
				FileFindAndProcess(dir,func);
			}
			else
			{
				sprintf(dir,"%s\\%s",fdir,FindFileData.cFileName);
				func(dir);
			}
		}
		FindClose(hFind);
	}	 
}
#endif


//-----------------------------------------------------------------------------
// Name: FrameMove()
// Desc: Called once per frame, the call is the entry point for animating
//       the scene.
//-----------------------------------------------------------------------------

BOOL stOldState=FALSE;
BOOL stStartLoad=FALSE;


void SetMainPos(Vector3f pos)
{
	Vector3fCopy(pos,stPlayer.mPos);	
	Vector3fCopy(stPlayer.mPos,gPlayer);	
	CameraTest.SetPoint(gPlayer[0],gPlayer[1],gPlayer[2]);
	::stLevel->SetCameraPos(gPlayer);
	::stLevel->SetViewMatrix(R3MoveGetViewMatrix());
	CameraTest.SetPos(gPlayer[0],gPlayer[1],gPlayer[2]);
	CameraTest.SetPoint(gPlayer[0],gPlayer[1],gPlayer[2]);
	R3FlyMoveSetPos(gPlayer);
}

HRESULT CR3D3DApp::FrameMoveLoop()
{
	if( stStartLoad == FALSE )
	{
		stStartLoad=TRUE;
		char name[256];
		GetPrivateProfileString("Common","Map","",name, 128,".\\RFMapTool.ini");
		if( name[0] != NULL )
		{
			InvalidateDeviceObjects();
			RestoreDeviceObjects(name);
			//���̺긦 ���� ����.
//			stLevel->mBsp->ReleaseMemoryBsp();	//������ �ִ��� �޸����� 
//			stLevel->mBsp->LoadMemoryFromBspFile(stLevel->GetMapName(),&stBspMemoryH);
			//�ʿ��ִ� �浹���� �����͸� ����Ʈ �浹�� ���� �ȿ��� �ִ´�.
			if( stLevel->mBsp->mCFLineNum > 1 )	//���� �ʿ� �浹 ������ �ִٸ�...
				LoadColFaceFromBSP(stLevel->mBsp);

			LoadEntityFromBSP(stLevel->mBsp);
			LoadSoundEntityFromBSP(stLevel->mBsp);
		}
	}

	::R3CalculateTime();	//�������� ����ߵȴ�. ���������� Ÿ�̸�ó���� �Ѵ�.

	if( stLevel->IsLoadedBsp() )
		stLevel->FrameMove();		//�� ���������� �غ��Ұ͵�...

/*	for(int i=0 ;i<256; i++)
	{
		if( gScan[i] )
		{
			//gAsci[i]=0;
			int iiii=0;
		}
	} */

	if(gAsci[VK_SPACE] )
	{
		gAsci[VK_SPACE]=0;
		static DWORD stop_toggle=0;
		stop_toggle=!stop_toggle;
		RTMoviePause(stop_toggle);
	}

	if( GetConsoleStateB(OP_PLAY_CAM) )
	{
		if( !IsRTMoviePlaying())
		{
			DWORD flag = RTMovieGetState();
			flag &= (~RTM_STATE_NO_CAMERAMOVE);
			RTMovieSetState(flag);
			RTMovieStartPlay(TRUE);
			SetOpStack(OP_NON_OP);
		}
		float cam_mat[4][4];
		RTMoiveGetCameraMatrix(cam_mat);
		RTMoiveGetCameraPos(stPlayer.mPos);
//		::R3AniCameraMove(gPlayer);
		::stLevel->SetCameraPos(gPlayer);
		::stLevel->SetViewMatrix(R3MoveGetViewMatrix());
	}
	else
	{
		Vector3f campos,get_tar;
		BOOL state = GetConsoleStateB(OP_FLY);
		if( stOldState!=state )	//�ö��̿� ��ũ�� ��ü�Ǿ���...
		{
			if( stOldState )	//������ �ö��� ����.
			{
				CameraTest.GetPoint(get_tar);
				Vector3fCopy(get_tar,stPlayer.mPos);
				Vector3fCopy(get_tar,stPlayer.mTPos);
			}
			else
			{
				Vector3fCopy(gPlayer,campos);
				campos[1]+=10;		//�ʹ� �ٴڿ� �� �پ ī�޶󿡰� ��� ��ǥ�� �־��ش�.
				CameraTest.SetPoint(gPlayer[0],gPlayer[1]+_ADD_Y,gPlayer[2]);
				R3FlyMoveSetPos(campos);	//�̵��� �ö��̺�ȯ ���...
			}
		}
		stOldState=state;
		if( state )	//�ö���...
		{
			float _FLY_SPEED=GetFlySpeed()*_MOVE_SPEED;
			::R3FlyMove(_FLY_SPEED,gPlayer);	//�ܼ���ƿ��Ƽ
			::stLevel->SetCameraPos(gPlayer);
			::stLevel->SetViewMatrix(R3MoveGetViewMatrix());
			CameraTest.SetPos(gPlayer[0],gPlayer[1],gPlayer[2]);
			CameraTest.SetPoint(gPlayer[0],gPlayer[1],gPlayer[2]);
		}
		else
		{
			::TestInterface();
			CameraTest.GetPos(campos);
			::stLevel->SetCameraPos(campos);	//������ ī�޶� ������ ���ش�.
			::stLevel->SetViewMatrix(CameraTest.GetViewMatrix());	//������ ���Ʈ������ �����Ѵ�.
			Vector3fCopy(stPlayer.mPos,gPlayer);	
		}
	}
	
	if( !stLevel->IsLoadedBsp() )
		return S_OK;
//-------�� Ű����Ʈ 
	if( gAsci[VK_Z] )
	{
		gAsci[VK_Z]=0;
		SetYXZAxis(TRUE,FALSE);	
	}
	if( gAsci[VK_X] )
	{
		gAsci[VK_X]=0;
		SetYXZAxis(FALSE,TRUE);	
	}
	TabEntityHotKey();	//��ƼƼ�� ��Ű..

	stTabIndex = GetTabMode();	//���� ���� ��� ����Ű�� �ִ°� 
	switch (stTabIndex)
	{
	case TAB_COLLISION:	//�浹 
		if( WhatCollisionMode() == _COLLISON_EDIT_MODE )
		{
			ProgressCollisionEdit((void *)stLevel);
		}
		else
		if( WhatCollisionMode() == _COLLISON_SELECT_MODE )
		{
			ProgressSelectEdit((void *)stLevel);
		}
		break;
	case TAB_ENTITY:// ��ƼƼ 
		ProgressEntityEdit((void *)stLevel,gPlayer);
		break;
	case TAB_PARTICLE://��ƼŬ.
		ProgressParticleEdit((void *)stLevel);
		break;
	case TAB_MAGIC://����
		FrameMoveAllEffectsList();	//������ ȿ������������ ���긮��Ʈ
		ProgressMagicEdit((void *)stLevel);
		break;
	case TAB_SOUND://����.
		ProgressSoundEdit((void *)stLevel,gPlayer);
		break;
	case TAB_DUMMY://����.
		ProgressDummyEdit((void *)stLevel,gPlayer);
		break;
	}

//	�ܼ��� ���Ƴ��´�.
	::ProgressFunctionKey();	//�ܼ� ��Ű	

    return S_OK;
}

BOOL stViewColFace,stViewColPoint;
void SetViewColFace(BOOL state)
{
	stViewColFace = state;
}

void SetViewColPoint(BOOL state)
{
	stViewColPoint = state;
}

BOOL stViewMapFace;
void SetViewMapFace(BOOL state)
{
	stViewMapFace = state;
}
BOOL stViewEntity;
void SetViewEntity(BOOL state)
{
	stViewEntity = state;
}
BOOL stViewEntityBBox;
void SetViewEntityBBox(BOOL state)
{
	stViewEntityBBox = state;
}
BOOL stViewSoundEntity;
void SetViewSoundEntity(BOOL state)
{
	stViewSoundEntity = state;
}
BOOL stViewDummyEntity;
void SetViewDummyEntity(BOOL state)
{
	stViewDummyEntity = state;
}



static int stSelectState;

int GetSelectState()
{
	return stSelectState;
}

void DrawViewCircle(Vector3f pos,float dist)		//�ִ� ���� ����(���� 20m) �ڽ��� ����Ѵ�.
{
	Vector3f box={3,3,3};	//���� ���� ����..
	Vector3f point;

	dist*=10;
	static float start_angle=0;
	start_angle+=R3GetLoopTime()*10;

	for(float i=start_angle; i<360+start_angle; i+=15)
	{
		float x = cos( AngleToPi(i) )*dist;
		float z = sin( AngleToPi(i) )*dist;

		point[0] = pos[0]+x;
		point[1] = pos[1]+15;
		point[2] = pos[2]+z;
		stLevel->DrawTestBox(box,point,0xffff9f00);		//��
	}
}

//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Called once per frame, the call is the entry point for 3d
//       rendering. This function sets up render states, clears the
//       viewport, and renders the scene.
//-----------------------------------------------------------------------------
HRESULT CR3D3DApp::Rendering()
{
	LONG mouse_x = (LONG)((gMouse.x*800)/stEx);
	LONG mouse_y = (LONG)((gMouse.y*607)/stEy);

	R3ClearFrameBuffer();
    // Begin the scene
	if( R3BeginScene() )
    {
		stLevel->DrawSkyBoxRender();	//�ϴ�
		if( stViewMapFace )
			stLevel->DrawMapRender();		//�� 

		Vector3f box={7,7,20};	//���� ���� ����..
		Vector3f point,pos;
		CameraTest.GetPoint(point);	//ī�޶� �������� �׻� ����Ų��.
//		stLevel->DrawTestBox(box,point,0xffff9f00);		//��
		CameraTest.GetPos(pos);		//ī�޶� ��ġ�� �˾Ƴ���.

//		stLevel->DrawLeafBBox();

#ifdef	_CHARACTER
		if( !GetConsoleStateB(OP_FLY) && !GetConsoleStateB(OP_PLAY_CAM))
		{
			//ĳ����
			point[1] -= _ADD_Y;
			g_chIF->SetState();
			g_chIF->DrawCharacter(body,gPlayer,stPlayer.mYangle);
			m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_NONE );
//			g_chIF->DrawCharacter(sword_f,gPlayer,stPlayer.mYangle);
			m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_POINT );
			
			m_pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND,D3DVBF_DISABLE);
			m_pd3dDevice->SetRenderState( D3DRS_LIGHTING,     FALSE );
			m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
			m_pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 1 );
			m_pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0xffffffff   );

			if( stLevel->IsLoadedBsp() && GetTabMode() == TAB_COLLISION && WhatCollisionMode() == _COLLISON_TEST_MODE)	//���� ���� ��� ����Ű�� �ִ°� 
			{
				DrawViewCircle(point,20);		//�ִ� ���� ����(���� 20m) �ڽ��� ����Ѵ�.
			}
		}
#endif
		if( stViewColFace )
			DrawCollisionPoly();	//�浹 �������� �Ѹ���.
		if( stViewColPoint )
			DrawCollisionPoint();

		if( stTabIndex == TAB_ENTITY )
			DrawCursorEntity();	//��ƼƼ ��������϶� Ŀ���� ��ġ�� �ѷ��ߵȴ�.
		if( stViewEntity )
			DrawEntities(pos);
		if( stViewEntityBBox )
			DrawEntitiesBBox();	//DrawEntities() ȣ���� ��ƼƼ �ٿ�� �ڽ��� �׸���.
		if( stViewSoundEntity )
			DrawSoundEntities();
		if( stViewDummyEntity )
			DrawDummyEntities();

		if( stViewMapFace )
			stLevel->DrawMapAlphaRender(pos);	//�ʿ��� ����..

		HearSoundFromTool();	//���󿡼� ȿ���� ��°�..

		DrawCreatedParticle();	//��ƼŬ�󿡼� ���۵Ǵ� ��ƼŬ...
		if( gScan[VK_CONTROL] )
		{
			::DrawR3Hangul( mouse_x-8,mouse_y+5, "+",0xffffffff);
			stSelectState=1;
		}
		else
		if( gScan[VK_SHIFT] )
		{
			::DrawR3Hangul( mouse_x-8,mouse_y+5, "-",0xffffffff);
			stSelectState=-1;
		}
		if( !gScan[VK_CONTROL] && !gScan[VK_SHIFT] )
			stSelectState=0;

		if( TAB_MAGIC == GetTabMode() )		
		{
			DrawMagicPosBox(stLevel);
		}
		RenderAllEffectsList();		//����Ʈ ������Ʈ
		FullScreenEffect();

		DrawFadeScreen();


//		stLevel->DrawLeafBBox();
//		stLevel->mBsp->DrawLeafCollisionPoly(stLevel->mBsp->GetLeafNum(pos));	//������ ������ȣ�� ����.



		char t_buf[256];
		sprintf(t_buf,"cam x : %4f cam y: %4f cam z: %4f"
			,GetR3State()->mInvMatView.m[3][0]
			,GetR3State()->mInvMatView.m[3][1]
			,GetR3State()->mInvMatView.m[3][2]);
		::DrawR3Hangul( 2,48+16, t_buf,0xffffffff);//8faf );
		
//		::DrawR3Hangul( mouse_x,mouse_y, "\\",0xffffffff);
/*
	char buf[256];
	extern DWORD src_blend,dest_blend;
	sprintf(buf,"%d %d",src_blend,dest_blend);
	::DrawR3Hangul( 2,48, buf,0xffffffff);//8faf );
*/
		R3EndScene();
    }
	::ProgressConsole(stLevel);		//�ܼ� ����....

	//--ȭ�鿡 �Ѹ� �ؽ�Ʈ��
    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: RestoreDeviceObjects()
// Desc: Restore device-memory objects and state after a device is created or
//       resized.
//-----------------------------------------------------------------------------
HRESULT CR3D3DApp::RestoreDeviceObjects(char *name)
{
	R3D3dWrapper::RestoreDeviceObjects();	//����Ŭ��������ȣ������.
//	stSpriteMouse.LoadSprite(".\\system\\mouse.spr");
	InitConsole();
	R3LoadTextTexture();	//��Ʈ�ؽ��Ĵµ��� �÷��ش�.

	stLevel->LoadLevel(name);		//�ʰ� ��Ÿ ��� ������ �ε��Ѵ�.�̸��� ������� ���ε带 �Ѵ�.

/*
	R3LoadTextTexture();	//�ؽ�Ʈ�� �ؽ��ĸ޸𸮸� ����. �ø���������ߵȴ�.
    if( R3RestoreDevice() == E_FAIL )
 		return E_FAIL;
*/
	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects()
// Desc: Called when the device-dependent objects are about to be lost.
//-----------------------------------------------------------------------------
HRESULT CR3D3DApp::InvalidateDeviceObjects()
{
	R3D3dWrapper::InvalidateDeviceObjects();
	ReleaseConsole();
	R3ReleaseTextTexture();	//��Ʈ�ؽ��ĸ� �����ش�.
	if( stLevel )
		stLevel->ReleaseLevel();	//����������....
    return S_OK;
}

HRESULT CR3D3DApp::InitDeviceObjects()
{
#ifdef	_CHARACTER
	_R3ENGINE_STATE *state = GetR3State();
	char mesh_name[256];
	char bone_name[256];

	g_chIF = new ChInterface;
	g_chIF->InitCharacter(m_pd3dDevice);
	
	//
	//	PartManager�߰� 
	g_chIF->AddPartToMM("BODY");
//	g_chIF->AddPartToMM("SWORD");

	//g_texture   = new TextureManager;
	//g_texture->SetTextureDevice(m_pd3dDevice);
	//FileFindAndProcess(state->CharTexturePath,LoadTextureInDir);

	g_chIF->SetTexPath(state->CharTexturePath);

	strcpy(bone_name,state->CharPath);
	strcat(bone_name,state->BoneName);
	g_chIF->LoadBoneData(bone_name,false);
	g_om_body  = g_chIF->GetPart("BODY");

	g_bn  = g_chIF->GetBoneData( bone_name );

	strcpy(mesh_name,state->CharPath);
	strcat(mesh_name,state->MeshName);
	g_chIF->LoadMeshData(g_om_body ,mesh_name,true,g_bn);

	//	Animation �߰�
	g_animation = new AnimationManager;

	sprintf(ANI0_name,"%s%s",state->CharPath,state->StopAniName);
	sprintf(ANI1_name,"%s%s",state->CharPath,"temp.ani");
	sprintf(ANI2_name,"%s%s",state->CharPath,state->RunAniName);

//	char buf[256];
//	strcpy(buf,ANI1_name);
	g_chIF->AddAnimationData(g_animation,ANI0_name);
	g_chIF->AddAnimationData(g_animation,ANI1_name);
	g_chIF->AddAnimationData(g_animation,ANI2_name);

	ChAnimation		*ani= g_chIF->GetAnimationData(g_animation,ANI0_name);
	
	g_chIF->MatchAnimationToMesh(ani,g_bn);

	co  = g_chIF->GetBoneData( bone_name );
	body  = g_chIF->GetMeshData(g_om_body, mesh_name );
//	sword_f = g_chIF->GetMeshData(g_om_body , ".\\Character\\ENERGYSWORD.msh" );
#endif
//    return S_OK;
	return R3D3dWrapper::InitDeviceObjects();
}

HRESULT CR3D3DApp::DeleteDeviceObjects()
{
#ifdef	_CHARACTER
	SAFE_DELETE( g_animation );
//	SAFE_DELETE( g_texture );
	SAFE_DELETE( g_chIF );
#endif
    return  R3D3dWrapper::DeleteDeviceObjects();
}

/*
//-----------------------------------------------------------------------------
// Name: InitDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name: FinalCleanup()
// Desc: Called before the app exits, this function gives the app the chance
//       to cleanup after itself.
//-----------------------------------------------------------------------------
HRESULT CR3D3DApp::FinalCleanup()
{
	ReleaseR3Text();
	stLevel->mBsp->ReleaseMemoryBsp();	//������ �ִ��� �޸����� 
	stLevel->ReleaseLevel();	//����������....
	//--------- Level �޸�����
	delete stLevel;

	ReleaseR3Engine();		//R3������ �����Ѵ�.

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: Called when the app is exitting, or the device is being changed,
//       this function deletes any device dependant objects.
//-----------------------------------------------------------------------------
HRESULT CR3D3DApp::DeleteDeviceObjects()
{
	CleanMapEntityAndEntities();	//�׵��� �ε��� ��ƼƼ�� ������.
	stLevel->ReleaseLevel();	//����������....
	if( R3DeleteDevice() == E_FAIL)
		return E_FAIL;
    return S_OK;
}
*/

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: Overrrides the main WndProc, so the sample can do custom message 
//       handling (e.g. processing mouse, keyboard, or menu commands).
//-----------------------------------------------------------------------------
LRESULT CR3D3DApp::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam,LPARAM lParam )
{

	//------Ű����� ���콺 �޽����� �ޱ�����...
	R3MsgProc( hWnd, uMsg, wParam, lParam );


    return CD3DApplication::MsgProc( hWnd, uMsg, wParam, lParam );
}



void CR3D3DApp::LoadBsp()
{
//	gAsci[VK_F1]=1;
//	::ProgressFunctionKey();	//�ܼ� ��Ű
//	::ProgressConsole(stLevel);		//�ܼ� ����....
	static OPENFILENAME ofn; // zero the ofn members out
	char name[256];

	_R3ENGINE_STATE *state = GetR3State();
	strcpy(name,state->MapPath);
	strcat(name,"test.bsp");

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = name;
	ofn.nMaxFile = sizeof(name);
	ofn.lpstrFilter = "R3BSP (*.BSP)\0*.BSP\0All (*.*)\0*.*\0";
	ofn.lpstrTitle = "Select BSP file";
	ofn.Flags = OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (!GetOpenFileName(&ofn))
	{
		return;
	}
	if( stLevel->IsLoadedBsp() )	//������ �ε��Ǿ��°�?
	{
		CleanMapEntityAndEntities();	//�׵��� �ε��� ��ƼƼ�� ������.
	}

	InvalidateDeviceObjects();
//	R3InvalidateDevice();	//�ؽ��� �޸� ������
//	stLevel->ReleaseLevel();		//�ʰ� ��Ÿ ��� ������ ������.

	
//	stLevel->LoadLevel(name);		//�ʰ� ��Ÿ ��� ������ �ε��Ѵ�.
//	R3RestoreDevice();				//�ؽ��� ���� �ø���..
	RestoreDeviceObjects(name);


	//���̺긦 ���� ����.
	LoadSoundEntityFromBSP(stLevel->mBsp);
//	stLevel->mBsp->ReleaseMemoryBsp();	//������ �ִ��� �޸����� 
//	stLevel->mBsp->LoadMemoryFromBspFile(stLevel->GetMapName(),&stBspMemoryH);
	//�ʿ��ִ� �浹���� �����͸� ����Ʈ �浹�� ���� �ȿ��� �ִ´�.
	if( stLevel->mBsp->mCFLineNum > 1 )	//���� �ʿ� �浹 ������ �ִٸ�...
		LoadColFaceFromBSP(stLevel->mBsp);

	LoadEntityFromBSP(stLevel->mBsp);
	LoadSoundEntityFromBSP(stLevel->mBsp);

}
static void SaveBSPAndCF(char *name,CLevel *level)
{
	_ADD_BSP_SAVE add_bsp;
	memset(&add_bsp,0,sizeof(_ADD_BSP_SAVE));
//	level->SaveBsp(name,GetColface());
//	GetColface()->
	GetColface()->LoadCollisonInfoInBSP(level->mBsp,&add_bsp);	//�浹 ������ �־��ش�.
	GetEntityInfoInBSP(level->mBsp,&add_bsp);	//��ƼƼ ������ �־��ش�.
	GetSoundEntityInfoInBSP(level->mBsp,&add_bsp);	//��ƼƼ ������ �־��ش�.

	//���� ���� 
	level->mBsp->SaveExtBsp(name,&add_bsp);

	//���� �޸� ����.
	ReleaseSoundEntityInfoInBSP(&add_bsp);
	ReleaseEntityInfoInBSP(&add_bsp);
	GetColface()->ReleaseCollisonInfoInBSP(&add_bsp);
}

void CR3D3DApp::LoadEbp()
{
	static OPENFILENAME ofn; // zero the ofn members out
	char name[256];

	_R3ENGINE_STATE *state = GetR3State();
	strcpy(name,state->MapPath);
	strcat(name,"test.ebp");

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = name;
	ofn.nMaxFile = sizeof(name);
	ofn.lpstrFilter = "R3Ebp (*.Ebp)\0*.Ebp\0All (*.*)\0*.*\0";
	ofn.lpstrTitle = "Select Ebp file";
	ofn.Flags = OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (!GetOpenFileName(&ofn))
	{
		return;
	}
	if( stLevel->IsLoadedBsp() )	//������ �ε��Ǿ��°�?
	{
		CleanMapEntityAndEntities();	//�׵��� �ε��� ��ƼƼ�� ������.
	}

	stLevel->mBsp->LoadExtBsp(name);

//	InvalidateDeviceObjects();
//	R3InvalidateDevice();	//�ؽ��� �޸� ������
//	stLevel->ReleaseLevel();		//�ʰ� ��Ÿ ��� ������ ������.

	
//	stLevel->LoadLevel(name);		//�ʰ� ��Ÿ ��� ������ �ε��Ѵ�.
//	R3RestoreDevice();				//�ؽ��� ���� �ø���..
//	RestoreDeviceObjects(name);


	//���̺긦 ���� ����.
	LoadSoundEntityFromBSP(stLevel->mBsp);
//	stLevel->mBsp->ReleaseMemoryBsp();	//������ �ִ��� �޸����� 
//	stLevel->mBsp->LoadMemoryFromBspFile(stLevel->GetMapName(),&stBspMemoryH);
	//�ʿ��ִ� �浹���� �����͸� ����Ʈ �浹�� ���� �ȿ��� �ִ´�.
	if( stLevel->mBsp->mCFLineNum > 1 )	//���� �ʿ� �浹 ������ �ִٸ�...
		LoadColFaceFromBSP(stLevel->mBsp);

	LoadEntityFromBSP(stLevel->mBsp);
	LoadSoundEntityFromBSP(stLevel->mBsp);

}


void CR3D3DApp::SaveEbp()
{
	if( !stLevel->IsLoadedBsp() )
		return;
	OPENFILENAME ofn; // zero the ofn members out
	memset(&ofn,0,sizeof(OPENFILENAME));
	char name[256];

	_R3ENGINE_STATE *state = GetR3State();
	strcpy(name,state->MapPath);
	strcat(name,"test.ebp");

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = name;
	ofn.nMaxFile = sizeof(name);
	ofn.lpstrFilter = "R3 EXT BSP (*.ebp)\0*.ebp\0All (*.*)\0*.*\0";
	ofn.lpstrTitle = "Select Ext BSP file";
 	ofn.Flags = OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


	if (!GetSaveFileName(&ofn))
	{
		return;
	}
	FILE *fp = fopen(name,"rb");
	if( fp )
	{
		fclose(fp);
		int result = MessageBox(NULL,"������� ������ ���?","Select",MB_OKCANCEL);
		if( IDCANCEL == result )	//���� ���.
		{
			return;
		}
	}
	SaveBSPAndCF(name,stLevel);
/*
	if( GetColLineNum() > 1 )//���� ����Ʈ�� ���� �� ���� ���������ߴ� �浹���� �������Ѵ�.
	{
		int result = MessageBox(NULL,"���� �ִ� �浹���� BSP�� �����ұ��?","Select",MB_OKCANCEL);
		if( IDOK == result )	//���� ���.
		{
			SaveBSPAndCF(name,stLevel);
		}
	}
	else
	{
		MessageBox(NULL,"���� �浹���� �ϳ��� �������� �ʽ��ϴ�.","�����Ұ��� �����ϴ�.",MB_OK);
	}
*/
}
void CR3D3DApp::RenderScene()
{
	FrameMoveLoop();

	if(S_OK == PrepareLoop() )	//d3d �������� ���� �غ�.
	{
		Rendering();
//				SDKTest_MainLoop();		//��� ���η���.
		EndLoop();	//d3d ������ �ø���.
	}
}