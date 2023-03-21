#ifndef _R3D3DWRAPPER_H__
#define _R3D3DWRAPPER_H__

#include <d3Dx8.h>
#include "R3D3DApp.h"

#define _R3_USE_DINPUT 0x1
//-----------------------------------------------------------------------------
// Name: class R3D3dWrapper
//-----------------------------------------------------------------------------
class R3D3dWrapper : public CD3DApplication
{
protected:
	DWORD mR3Flag;	//���� ���� �÷���...
    HRESULT ConfirmDevice( D3DCAPS8*, DWORD, D3DFORMAT );
    HRESULT InitDeviceObjects();
    HRESULT DeleteDeviceObjects();
    HRESULT Render();
    HRESULT FrameMove();
public:
	//----- �ܺο��� ����ϴ� �������̽�....
	LPDIRECT3DDEVICE8 GetD3dDevice8(){	return m_pd3dDevice;	};	//d3d8 ����̽� 
    R3D3dWrapper();
	HRESULT PrepareLoop();	// ���� ����̽��� �غ�...
	HRESULT EndLoop();	//ȭ�� �ø����� �ִ�.
	HRESULT InitR3D3D(HINSTANCE hInstance,DWORD flag);	// ���̷�Ʈ 3D�ʱ�ȭ 
	HRESULT ReleaseR3D3D();		//������..
    virtual HRESULT RestoreDeviceObjects();		//�ػ� ������ ���� �ε��κ�
    virtual HRESULT InvalidateDeviceObjects();	//�ػ� ������ ���� ������κ�
	virtual LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//----- 
	BOOL IsActive();	//â�� Ȱ��ȭ �Ǿ��°�...
	BOOL IsReady();		//aPP�غ� �Ǿ��°�..
};

#endif