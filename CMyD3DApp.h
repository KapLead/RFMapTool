#ifndef __CMYD3DAPP_H__
#define __CMYD3DAPP_H__

#include "d3Dx8.h"
#include "R3D3DWRAPPER.H"
/*-----------------------------------------------------------------------------
�ܼ��� d3d ���۸� ���� Ŭ�����μ� ������ ������ ���۵ȴٰ� ���� �ȴ�.
�� �ʱ�ȭ ������ �����ش�. APP�ʱ�ȭ ������  
1. R3���� �ʱ�ȭ  ( R3���� �ʱ�ȭ.����)
2. D3d�ʱ�ȭ (�����Լ� ��� R3Input�ʱ�ȭ )

������ ��ŵ�ô�..
*/
class CR3D3DApp : public R3D3dWrapper
{
protected:
    HRESULT RestoreDeviceObjects(char *name=NULL);		//����Ŭ������ �����Լ� ����Ǿ���.
    HRESULT InvalidateDeviceObjects();	//����Ŭ������ �����Լ� ����Ǿ���.
	LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );//����Ŭ������ �����Լ� ����Ǿ���.
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
LONG GetRectMouseX(LONG mouse_x);	//�ػ󵵿����缭..
LONG GetRectMouseY(LONG mouse_y);	//�ػ󵵿� ���缭..

#endif
