#include <windows.h>
#include <assert.h>

#include "R3D3d8.h"
#include "R3D3dtex.h"
#include "core.h"
#include "R3Text.h"
#include "R3Render.h"
#include "R3EngineKernel.h"
#include "Jmalloc.h"
#include "Console.h"
#include "MultiLayer.h"
#include "2dsprite.h"
#include "R3Sound.h"

static BOOL Is_server_mode	= FALSE;
static LONG *Temp			= NULL;		//�����󿡼� ���� ��������
static BOOL Is_init_R3engine= FALSE;

BOOL IsServerMode()
{
	return Is_server_mode;
}

void ForceFullScreen()	//���������� Ǯ��ũ���� �����.initR3engine���Ŀ� �Ұ�.)
{
	GetR3State()->mbFullScreen = TRUE;
}

void InitR3Engine(int use)
{
//	assert( IsInitR3Engine() == FALSE );

	Is_init_R3engine=TRUE;
	if( use == USE_SERVER )
		Is_server_mode=TRUE;
	else
	if( use == USE_TOOL )
	{
//		SetToolMode();//���뵵�϶� ���� ó���Ұ����ִ�.
	}
	InitCore();

	//���峪 ���� �÷��� ó��..
	SetPlayWaveState(GetR3State()->mSoundOn);
	SetPlayMusicState(GetR3State()->mMusicOn);

	if( !Is_server_mode )	//������尡 �ƴϸ�...
	{
		InitFunctionKey();
		InitSpriteManager();	//��������Ʈ �޴��� ���� �ʱ�ȭ�� ȣ��.
		InitR3Text();		//�ؽ�Ʈ ���� 
	}
}

BOOL IsInitR3Engine(void)
{
	return Is_init_R3engine;
}

void ReleaseR3Engine(void)
{
	if( !Is_server_mode )	//������尡 �ƴϸ�...
	{
		ReleaseFunctionKey();
		ReleaseSpriteManager();
		ReleaseR3Text();	//�ؽ�Ʈ..
		ReleaseConsole();
	}
	Is_init_R3engine=FALSE;
	if ( Temp != NULL )
	{
		Dfree(Temp);
		Temp = NULL;
	}
	ReleaseCore();
}

//---���� ����̽�����
HRESULT R3InitDevice(  void *pDevice ,int sc_width,int sc_height )
{
	GetR3State()->mScreenXsize = (float)sc_width;
	GetR3State()->mScreenYsize = (float)sc_height;
//	SetViewPortArea(0,0,sc_width,sc_height);
	CleanViewPortStack();
//	PushViewPortArea();
	return D3D_R3InitDevice((LPDIRECT3DDEVICE8)pDevice);
}
HRESULT R3ConfirmDevice(D3DCAPS8* pCaps, DWORD dwBehavior,D3DFORMAT Format )
{
	return D3D_R3ConfirmDevice(pCaps,dwBehavior,Format);
}
HRESULT R3RestoreDevice()
{
//	R3LoadTextTexture();	//�ؽ�Ʈ�� �ؽ��ĸ޸𸮸� ����. �ø�������ߵȴ�.
	return D3D_R3RestoreDevice();
}
HRESULT R3InvalidateDevice()
{
	return D3D_R3InvalidateDevice();
}
HRESULT R3DeleteDevice()
{
	return D3D_R3DeleteDevice();
}

void R3SetViewPort(LONG sx,LONG sy,LONG xl,LONG yl)
{
	SetViewPortArea(sx,sy,xl,yl);
}
void R3GetViewPort(LONG *sx,LONG *sy,LONG *xl,LONG *yl)
{
	_R3ENGINE_STATE *state = GetR3State();
	*sx = (LONG)state->mViewSx;
	*sy = (LONG)state->mViewSy;
	*xl = (LONG)state->mViewXL;
	*yl = (LONG)state->mViewYL;
}


//----------���� �޽��� ����.. �Լ��ȿ� Ű����� ���콺���� �Լ��� �ִ�.
void R3MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam,LPARAM lParam )
{
	D3D_R3MsgProc( hWnd,uMsg,wParam,lParam );
}

DWORD GetFogColor()
{
	return GetR3State()->mFogColor;
}



