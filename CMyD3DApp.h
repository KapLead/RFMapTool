#ifndef __CMYD3DAPP_H__
#define __CMYD3DAPP_H__

#include "d3Dx8.h"
#include "R3D3DWRAPPER.H"
/*-----------------------------------------------------------------------------
단순히 d3d 레퍼를 위한 클래스로서 엔진과 별개로 동작된다고 보면 된다.
단 초기화 순서를 지켜준다. APP초기화 순서는  
1. R3엔진 초기화  ( R3사운드 초기화.포함)
2. D3d초기화 (래퍼함수 사용 R3Input초기화 )

순서를 지킵시다..
*/
class CR3D3DApp : public R3D3dWrapper
{
protected:
    HRESULT RestoreDeviceObjects(char *name=NULL);		//래퍼클래스의 버쳐함수 선언되었음.
    HRESULT InvalidateDeviceObjects();	//래퍼클래스의 버쳐함수 선언되었음.
	LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );//래퍼클래스의 버쳐함수 선언되었음.
	HRESULT Rendering();
    HRESULT FrameMoveLoop();
	HRESULT InitDeviceObjects();
	HRESULT DeleteDeviceObjects();

public:
	void InitD3DApp(HWND hwnd);
	void ReleaseD3DApp();
    CR3D3DApp();
    ~CR3D3DApp();
	void LoadBsp();
	void LoadEbp();
	void SaveEbp();
    void RenderScene();
};

/*
class CMyD3DApplication : public CD3DApplication
{
    HRESULT ConfirmDevice( D3DCAPS8*, DWORD, D3DFORMAT );

protected:
    HRESULT OneTimeSceneInit();
    HRESULT InitDeviceObjects();
    HRESULT RestoreDeviceObjects();
    HRESULT InvalidateDeviceObjects();
    HRESULT DeleteDeviceObjects();
    HRESULT FinalCleanup();
    HRESULT Render();
    HRESULT FrameMove();

public:
	void InitD3DApp(HWND hwnd);
	void ReleaseD3DApp();
	void LoadBsp();
	void SaveBsp();

    VOID RenderScene() { Render3DEnvironment(); }
    CMyD3DApplication();
	~CMyD3DApplication();
    BOOL IsReady() { return m_bReady; }
	LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
};
*/
void SetViewColPoint(BOOL state);
void SetViewColFace(BOOL state);
void SetViewMapFace(BOOL state);
void SetViewEntity(BOOL state);
void SetViewEntityBBox(BOOL state);
void SetViewSoundEntity(BOOL state);
void SetViewDummyEntity(BOOL state);

int GetSelectState();
void SetRenderRect(LONG sx,LONG sy,LONG ex,LONG ey);
LONG GetRectMouseX(LONG mouse_x);	//해상도에맞춰서..
LONG GetRectMouseY(LONG mouse_y);	//해상도에 맞춰서..

#endif
