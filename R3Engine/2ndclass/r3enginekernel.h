#ifndef __R3ENGINEKERNEL_H__
#define __R3ENGINEKERNEL_H__

#include "core.h"


#define USE_CLIENT	0
#define USE_SERVER	1
#define USE_TOOL	2


//-- �ʱ�ȭ �� ������ ����
void InitR3Engine(int use = USE_CLIENT);	//������ ���������� ����Ϸ��� Use_server�� �ִ´�.
void ReleaseR3Engine();
BOOL IsInitR3Engine();
void ForceFullScreen();	//���������� Ǯ��ũ���� �����.InitR3engine���Ŀ� �Ұ�.)

//---�����ӹ��� Ŭ����� ������ ���۰� ��
void R3ClearFrameBuffer();
void DrawLogo();	//�ΰ� �Ѹ���.
BOOL R3BeginScene();
void R3EndScene();

//_R3ENGINE_STATE *GetR3State();
extern _R3ENGINE_STATE stState;
#define GetR3State() (&stState)

DWORD GetFogColor();

//-- �������
BOOL IsServerMode();	//���� ��������ΰ�?

//---���� ����̽�����
HRESULT R3InitDevice(  void *pDevice ,int sc_width,int sc_height );
HRESULT R3ConfirmDevice(D3DCAPS8* pCaps, DWORD dwBehavior,D3DFORMAT Format );

HRESULT R3RestoreDevice();
HRESULT R3InvalidateDevice();
HRESULT R3DeleteDevice();

//����Ʈ �����Լ� ....
void R3SetViewPort(LONG sx,LONG sy,LONG xl,LONG yl);
void R3GetViewPort(LONG *sx,LONG *sy,LONG *xl,LONG *yl);
void PushViewPortArea();
void PopViewPortArea();

//----------���� �޽��� ����.. �Լ��ȿ� �⺻ Ű����� ���콺���� �Լ��� �ִ�.
void R3MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam,LPARAM lParam );


#endif