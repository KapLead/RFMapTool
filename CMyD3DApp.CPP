
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

CLevel *stLevel=NULL;	//레벨 클래스 

#define _MOVE_SPEED		(100.0f)
#define _ADD_Y		16

Vector3f gPlayer={0,0,0};

//테스트 함수 인터페이스용..
R3Camera CameraTest;


#include "R3Input.h"
class Cplayer{
public:
	Vector3f mPos;	//위치.. 그냥 pos[3]이다 
	Vector3f mTPos;	// 타겟 위치(갈곳...)
	BOOL mIsMove;	//	현재 움직이고 있는가?
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
	InitR3Engine();		//R3엔진을 초기화한다.
	InitR3SoundSystem(NULL);

	stLevel = new CLevel;
    m_hWnd = hwnd;

	//-------효과부분을 위한곳...
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
//	GetPrivateProfileString("Path","MagicScriptFile","MagicSptList.spt",name, 128,".\\RFMapTool.ini");
//	strcpy(temp,path);
//	strcat(temp,name);
//	CreateMagicSptList(temp);
	//--------------------------

//	다이렉트 인풋을 사용 안하려면...
	InitR3D3D( AfxGetInstanceHandle() ,0 );   //d3d초기화.. 
//	InitConsole();
	R3LoadTextTexture();	//폰트텍스쳐는따로 올려준다.
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
	R3ReleaseTextTexture();	//폰트텍스쳐를 내려준다.

//	InvalidateDeviceObjects();
	if( stLevel )
	{
		stLevel->ReleaseLevel();
		delete stLevel;
		stLevel=0;
	}
	ReleaseR3SoundSystem();
	ReleaseR3D3D();
	DestroyMagicSptList();	//이펙트들 릴리즈
	DestroyEffectsList();		//이펙트들 릴리즈
	ReleaseR3Engine();		//R3엔진을 초기화한다.
}

static BOOL OneClick=FALSE;
static BOOL FirstCollision=FALSE;
static int stTabIndex;	//현재 탭이어디 있는가...

#define TAB_COLLISION	0	//충돌 
#define TAB_ENTITY		1// 엔티티 
#define TAB_PARTICLE	2//파티클.
#define TAB_MAGIC		3//마법.
#define TAB_SOUND		4//사운드.
#define TAB_DUMMY		5//더미.


//패스파인더 추가부분.
static Vector3f stPathStack[16];
static DWORD stPathStackCnt=0,stNowPathCnt=0;

BOOL GetNextPath(Vector3f *get)		//다음갈곳이 없으면 FALSE
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

	if( !FirstCollision  )	//최초에 y 좌표를 구해줘야된다.
	{
		FirstCollision =TRUE;
		stPlayer.mPos[1] = stLevel->GetFirstYpos(stPlayer.mPos,-1);	//가장 낮은 충돌좌표를 얻어낸다.
		stPlayer.mTPos[1]=stPlayer.mPos[1];
	}

	//----fly와 걷기를 바꿔서 공중에 떠있는 경우를 정리하자 엔진상에서는 필요없다..
	if( 1 )
	{
		Vector3f pos,min,max;
		min[1]=stPlayer.mPos[1]-5;
		max[1]=stPlayer.mPos[1]+5;
		Vector3fCopy(stPlayer.mPos,pos);
		pos[1] = stLevel->GetFirstYpos(pos,min,max);	//	바운딩 박스영역에서 y값을 얻어온다.
		if(pos[1] == -65535)	//공중에 떠있다..
		{
			min[1] = -30000;  
			pos[1] = stLevel->GetFirstYpos(pos,min,max);	//	바운딩 박스영역에서 y값을 얻어온다.
			if(pos[1] != -65535)		//가장 가까운쪽을 얻어낸다.
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
	{	//마우스 땔때 ...
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
	{	//마우스 땔때 ...
		OneClick=FALSE;
	}
	/*
	if( stNowPathCnt )
	{
		stPlayer.mYangle=::GetYAngle(stPlayer.mPos,stPlayer.mTPos);
		stPlayer.mIsMove=TRUE;
		stNowPathCnt--;
	}*/

	if( stPlayer.mIsMove )	//움직여야할때다..
	{
		float dist = ::GetDist(stPlayer.mPos,stPlayer.mTPos);
		if( fabs( dist ) <=::R3GetLoopTime()*_MOVE_SPEED  )	//타겟에 거의 근접했다.오차를 생각해주자..멈춰야 할때다...
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
			//다음 갈곳을 구하고..
			next_pos[0] = stPlayer.mPos[0] + ((float)sin(AngleToPi(stPlayer.mYangle))*speed);
			next_pos[1] = stPlayer.mPos[1];
			next_pos[2] = stPlayer.mPos[2] + ((float)cos(AngleToPi(stPlayer.mYangle))*speed);
			//실제 갈수있는지 첵크해보고..
			float get_ypos;
			BOOL can_go = stLevel->GetNextYpos(next_pos,&get_ypos);
			if( can_go )	//갈수 있다..
			{
				stPlayer.mPos[0] = next_pos[0];
				stPlayer.mPos[1] = get_ypos;
				stPlayer.mPos[2] = next_pos[2];
			}
			else	//갈수 없어서 정지..
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

	if(viewanglex<-179)			//위아래 회전은 제한을 두자 
		viewanglex =-179;
	if(viewanglex> 179)		
		viewanglex = 179;

	//카메라에게 bsp포인터를 넘겨준다. 한번만 호출해도 되는데 항상해도 속도에전혀 상관없다.
	CameraTest.SetBspPtr(stLevel->mBsp);
	//카메라가 향하고 있는 중심점을 지정..  플레이어 위치가 카메라 바라보는 포인트...
	CameraTest.SetSmoothPointLoop(stPlayer.mPos[0],stPlayer.mPos[1]+_ADD_Y,stPlayer.mPos[2]);
	//회전과 포인터와의 거리를 지정..
	CameraTest.SetSmoothThPhDistLoop(viewanglex,viewangley,(float)GetEditCameraDist());
/*
//----딱딱한 카메라 움직임.
	//카메라가 향하고 있는 중심점을 지정..  플레이어 위치가 카메라 바라보는 포인트...
	CameraTest.SetPoint(stPlayer.mPos[0],stPlayer.mPos[1],stPlayer.mPos[2]);
	//회전과 포인터와의 거리를 지정..
	CameraTest.SetThPhDist(viewanglex,viewangley,(float)GetEditCameraDist());
*/
	//모든 값을 지정한후에 카메라의 계산과 view Mat 을위해 한번호출해줘해야한다.
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
			//세이브를 위한 진행.
//			stLevel->mBsp->ReleaseMemoryBsp();	//기존에 있던거 메모리제거 
//			stLevel->mBsp->LoadMemoryFromBspFile(stLevel->GetMapName(),&stBspMemoryH);
			//맵에있는 충돌맵을 데이터를 에디트 충돌맵 구조 안에다 넣는다.
			if( stLevel->mBsp->mCFLineNum > 1 )	//만약 맵에 충돌 정보가 있다면...
				LoadColFaceFromBSP(stLevel->mBsp);

			LoadEntityFromBSP(stLevel->mBsp);
			LoadSoundEntityFromBSP(stLevel->mBsp);
		}
	}

	::R3CalculateTime();	//루프마다 해줘야된다. 내부적으로 타이머처리를 한다.

	if( stLevel->IsLoadedBsp() )
		stLevel->FrameMove();		//맵 렌더링전에 준비할것들...

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
		if( stOldState!=state )	//플라이와 워크와 교체되었다...
		{
			if( stOldState )	//이전이 플라이 였다.
			{
				CameraTest.GetPoint(get_tar);
				Vector3fCopy(get_tar,stPlayer.mPos);
				Vector3fCopy(get_tar,stPlayer.mTPos);
			}
			else
			{
				Vector3fCopy(gPlayer,campos);
				campos[1]+=10;		//너무 바닥에 딱 붙어서 카메라에게 띄운 좌표를 넣어준다.
				CameraTest.SetPoint(gPlayer[0],gPlayer[1]+_ADD_Y,gPlayer[2]);
				R3FlyMoveSetPos(campos);	//이동시 플라이변환 대비...
			}
		}
		stOldState=state;
		if( state )	//플라이...
		{
			float _FLY_SPEED=GetFlySpeed()*_MOVE_SPEED;
			::R3FlyMove(_FLY_SPEED,gPlayer);	//단순유틸리티
			::stLevel->SetCameraPos(gPlayer);
			::stLevel->SetViewMatrix(R3MoveGetViewMatrix());
			CameraTest.SetPos(gPlayer[0],gPlayer[1],gPlayer[2]);
			CameraTest.SetPoint(gPlayer[0],gPlayer[1],gPlayer[2]);
		}
		else
		{
			::TestInterface();
			CameraTest.GetPos(campos);
			::stLevel->SetCameraPos(campos);	//레벨에 카메라 셋팅을 해준다.
			::stLevel->SetViewMatrix(CameraTest.GetViewMatrix());	//레벨에 뷰메트릭스를 셋팅한다.
			Vector3fCopy(stPlayer.mPos,gPlayer);	
		}
	}
	
	if( !stLevel->IsLoadedBsp() )
		return S_OK;
//-------핫 키리스트 
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
	TabEntityHotKey();	//엔티티쪽 핫키..

	stTabIndex = GetTabMode();	//현재 탭이 어디를 가르키고 있는가 
	switch (stTabIndex)
	{
	case TAB_COLLISION:	//충돌 
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
	case TAB_ENTITY:// 엔티티 
		ProgressEntityEdit((void *)stLevel,gPlayer);
		break;
	case TAB_PARTICLE://파티클.
		ProgressParticleEdit((void *)stLevel);
		break;
	case TAB_MAGIC://마법
		FrameMoveAllEffectsList();	//매직과 효과관련프래임 무브리스트
		ProgressMagicEdit((void *)stLevel);
		break;
	case TAB_SOUND://사운드.
		ProgressSoundEdit((void *)stLevel,gPlayer);
		break;
	case TAB_DUMMY://더미.
		ProgressDummyEdit((void *)stLevel,gPlayer);
		break;
	}

//	콘솔은 막아놓는다.
	::ProgressFunctionKey();	//콘솔 핫키	

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

void DrawViewCircle(Vector3f pos,float dist)		//최대 가는 범위(현재 20m) 박스를 출력한다.
{
	Vector3f box={3,3,3};	//가로 세로 높이..
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
		stLevel->DrawTestBox(box,point,0xffff9f00);		//맵
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
		stLevel->DrawSkyBoxRender();	//하늘
		if( stViewMapFace )
			stLevel->DrawMapRender();		//맵 

		Vector3f box={7,7,20};	//가로 세로 높이..
		Vector3f point,pos;
		CameraTest.GetPoint(point);	//카메라 포지션을 항상 가르킨다.
//		stLevel->DrawTestBox(box,point,0xffff9f00);		//맵
		CameraTest.GetPos(pos);		//카메라 위치를 알아낸다.

//		stLevel->DrawLeafBBox();

#ifdef	_CHARACTER
		if( !GetConsoleStateB(OP_FLY) && !GetConsoleStateB(OP_PLAY_CAM))
		{
			//캐릭터
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

			if( stLevel->IsLoadedBsp() && GetTabMode() == TAB_COLLISION && WhatCollisionMode() == _COLLISON_TEST_MODE)	//현재 탭이 어디를 가르키고 있는가 
			{
				DrawViewCircle(point,20);		//최대 가는 범위(현재 20m) 박스를 출력한다.
			}
		}
#endif
		if( stViewColFace )
			DrawCollisionPoly();	//충돌 폴리곤을 뿌린다.
		if( stViewColPoint )
			DrawCollisionPoint();

		if( stTabIndex == TAB_ENTITY )
			DrawCursorEntity();	//엔티티 에딧모드일때 커서의 위치에 뿌려야된다.
		if( stViewEntity )
			DrawEntities(pos);
		if( stViewEntityBBox )
			DrawEntitiesBBox();	//DrawEntities() 호출후 앤티티 바운딩 박스를 그리자.
		if( stViewSoundEntity )
			DrawSoundEntities();
		if( stViewDummyEntity )
			DrawDummyEntities();

		if( stViewMapFace )
			stLevel->DrawMapAlphaRender(pos);	//맵에서 알파..

		HearSoundFromTool();	//툴상에서 효과음 듣는거..

		DrawCreatedParticle();	//파티클상에서 제작되는 파티클...
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
		RenderAllEffectsList();		//이펙트 렌더파트
		FullScreenEffect();

		DrawFadeScreen();


//		stLevel->DrawLeafBBox();
//		stLevel->mBsp->DrawLeafCollisionPoly(stLevel->mBsp->GetLeafNum(pos));	//현재의 리프번호를 얻어낸다.



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
	::ProgressConsole(stLevel);		//콘솔 진행....

	//--화면에 뿌릴 텍스트들
    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: RestoreDeviceObjects()
// Desc: Restore device-memory objects and state after a device is created or
//       resized.
//-----------------------------------------------------------------------------
HRESULT CR3D3DApp::RestoreDeviceObjects(char *name)
{
	R3D3dWrapper::RestoreDeviceObjects();	//상위클래스부터호출하자.
//	stSpriteMouse.LoadSprite(".\\system\\mouse.spr");
	InitConsole();
	R3LoadTextTexture();	//폰트텍스쳐는따로 올려준다.

	stLevel->LoadLevel(name);		//맵과 기타 등등 레벨을 로드한다.이름이 없을경우 리로드를 한다.

/*
	R3LoadTextTexture();	//텍스트도 텍스쳐메모리를 쓴다. 올리고내려줘야된다.
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
	R3ReleaseTextTexture();	//폰트텍스쳐를 내려준다.
	if( stLevel )
		stLevel->ReleaseLevel();	//레벨릴리즈....
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
	//	PartManager추가 
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

	//	Animation 추가
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
	stLevel->mBsp->ReleaseMemoryBsp();	//기존에 있던거 메모리제거 
	stLevel->ReleaseLevel();	//레벨릴리즈....
	//--------- Level 메모리해제
	delete stLevel;

	ReleaseR3Engine();		//R3엔진을 해제한다.

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: Called when the app is exitting, or the device is being changed,
//       this function deletes any device dependant objects.
//-----------------------------------------------------------------------------
HRESULT CR3D3DApp::DeleteDeviceObjects()
{
	CleanMapEntityAndEntities();	//그동안 로딩된 엔티티를 날린다.
	stLevel->ReleaseLevel();	//레벨릴리즈....
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

	//------키보드와 마우스 메시지를 받기위해...
	R3MsgProc( hWnd, uMsg, wParam, lParam );


    return CD3DApplication::MsgProc( hWnd, uMsg, wParam, lParam );
}



void CR3D3DApp::LoadBsp()
{
//	gAsci[VK_F1]=1;
//	::ProgressFunctionKey();	//콘솔 핫키
//	::ProgressConsole(stLevel);		//콘솔 진행....
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
	if( stLevel->IsLoadedBsp() )	//기존에 로딩되었는가?
	{
		CleanMapEntityAndEntities();	//그동안 로딩된 엔티티를 날린다.
	}

	InvalidateDeviceObjects();
//	R3InvalidateDevice();	//텍스쳐 메모리 내리고
//	stLevel->ReleaseLevel();		//맵과 기타 등등 레벨을 릴리즈.

	
//	stLevel->LoadLevel(name);		//맵과 기타 등등 레벨을 로드한다.
//	R3RestoreDevice();				//텍스쳐 전부 올리고..
	RestoreDeviceObjects(name);


	//세이브를 위한 진행.
	LoadSoundEntityFromBSP(stLevel->mBsp);
//	stLevel->mBsp->ReleaseMemoryBsp();	//기존에 있던거 메모리제거 
//	stLevel->mBsp->LoadMemoryFromBspFile(stLevel->GetMapName(),&stBspMemoryH);
	//맵에있는 충돌맵을 데이터를 에디트 충돌맵 구조 안에다 넣는다.
	if( stLevel->mBsp->mCFLineNum > 1 )	//만약 맵에 충돌 정보가 있다면...
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
	GetColface()->LoadCollisonInfoInBSP(level->mBsp,&add_bsp);	//충돌 정보를 넣어준다.
	GetEntityInfoInBSP(level->mBsp,&add_bsp);	//엔티티 정보를 넣어준다.
	GetSoundEntityInfoInBSP(level->mBsp,&add_bsp);	//엔티티 정보를 넣어준다.

	//레벨 저장 
	level->mBsp->SaveExtBsp(name,&add_bsp);

	//각각 메모리 제거.
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
	if( stLevel->IsLoadedBsp() )	//기존에 로딩되었는가?
	{
		CleanMapEntityAndEntities();	//그동안 로딩된 엔티티를 날린다.
	}

	stLevel->mBsp->LoadExtBsp(name);

//	InvalidateDeviceObjects();
//	R3InvalidateDevice();	//텍스쳐 메모리 내리고
//	stLevel->ReleaseLevel();		//맵과 기타 등등 레벨을 릴리즈.

	
//	stLevel->LoadLevel(name);		//맵과 기타 등등 레벨을 로드한다.
//	R3RestoreDevice();				//텍스쳐 전부 올리고..
//	RestoreDeviceObjects(name);


	//세이브를 위한 진행.
	LoadSoundEntityFromBSP(stLevel->mBsp);
//	stLevel->mBsp->ReleaseMemoryBsp();	//기존에 있던거 메모리제거 
//	stLevel->mBsp->LoadMemoryFromBspFile(stLevel->GetMapName(),&stBspMemoryH);
	//맵에있는 충돌맵을 데이터를 에디트 충돌맵 구조 안에다 넣는다.
	if( stLevel->mBsp->mCFLineNum > 1 )	//만약 맵에 충돌 정보가 있다면...
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
		int result = MessageBox(NULL,"덮어씌워도 괜찮을 까요?","Select",MB_OKCANCEL);
		if( IDCANCEL == result )	//닫을 경우.
		{
			return;
		}
	}
	SaveBSPAndCF(name,stLevel);
/*
	if( GetColLineNum() > 1 )//현재 에디트가 새로 된 경우와 기존저장했던 충돌맵을 재저장한다.
	{
		int result = MessageBox(NULL,"현재 있는 충돌맵을 BSP에 포함할까요?","Select",MB_OKCANCEL);
		if( IDOK == result )	//닫을 경우.
		{
			SaveBSPAndCF(name,stLevel);
		}
	}
	else
	{
		MessageBox(NULL,"현재 충돌맵이 하나도 존재하지 않습니다.","저장할것이 없습니다.",MB_OK);
	}
*/
}
void CR3D3DApp::RenderScene()
{
	FrameMoveLoop();

	if(S_OK == PrepareLoop() )	//d3d 렌더링및 루프 준비.
	{
		Rendering();
//				SDKTest_MainLoop();		//모든 메인루프.
		EndLoop();	//d3d 페이지 플리핑.
	}
}
