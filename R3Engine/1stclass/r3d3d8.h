#ifndef __R3D3D8_H__
#define __R3D3D8_H__
#include "d3d8.h"
#include "d3dx8.h"


#define D3DFVF_R3VERTEX_TEX0  (D3DFVF_XYZ |D3DFVF_DIFFUSE)
#define D3DFVF_R3VERTEX_TEX1  (D3DFVF_XYZ |D3DFVF_DIFFUSE| D3DFVF_TEX1)
#define D3DFVF_R3VERTEX_TEX2  (D3DFVF_XYZ |D3DFVF_DIFFUSE| D3DFVF_TEX2 |D3DFVF_NORMAL )

#define D3DFVF_R3TLVERTEX_TEX0 (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)
#define D3DFVF_R3TLVERTEX_TEX1 (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#pragma	pack(push, 1)		//����Ʈ ����.

typedef struct {
	union{
	float x;
	float pos[1];
	};
	float y,z;
	DWORD color;
}_D3DR3VERTEX_TEX0;

typedef struct {
	union{
	float x;
	float pos[1];
	};
	float y,z;
	DWORD color;
	float u,v;
}_D3DR3VERTEX_TEX1;

typedef struct {
	union{
	float x;
	float local[1];
	};
	float y,z;
	
	union{
	float nx;
	float normal[1];
	};
	float ny,nz;

	DWORD color;
	float uv[2][2];
}_D3DR3VERTEX_TEX2;


typedef struct {
	float x,y,z,w;
	DWORD color;
}_D3DR3TLVERTEX_TEX0;


typedef struct {
	float x,y,z,w;
	DWORD color;
	float u,v;
}_D3DR3TLVERTEX_TEX1;
#pragma	pack(pop)

#define _LPD3DR3VERTEX void*


#define _SHADER_TEX1	0
#define _SHADER_TEX2	1





//---����̽�����
HRESULT D3D_R3InitDevice(  LPDIRECT3DDEVICE8 pd3dDevice );
HRESULT D3D_R3ConfirmDevice( D3DCAPS8* pCaps, DWORD dwBehavior,
                                          D3DFORMAT Format );

HRESULT D3D_R3RestoreDevice();
HRESULT D3D_R3InvalidateDevice();
HRESULT D3D_R3DeleteDevice();

LPDIRECT3DDEVICE8 GetD3dDevice();

//----------�޽��� ����.. �Լ��ȿ� Ű����� ���콺���� �Լ��� �ִ�.
void D3D_R3MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam,LPARAM lParam );


//---
void SetViewPortArea(LONG sx,LONG sy,LONG xl,LONG yl);
void SetFov(float pi);	//fov���� ���� ����..
void SetDefaultFov();	//���� 48.29���̴�.


void SetTextureRenderTargetFrameBuffer(BOOL state);	//���� ����Ÿ���� �����ӹ��������� �˷��ִ� �Լ�.
BOOL IsRenderTargetFrameBuffer();	//���� �����ӹ��۰� ���� Ÿ���ΰ�?



//��ƿ..
void SetDefaultFlags();
//y������ ��Ʈ����... ������� view�� �ι��� ��Ʈ�����̴�.
void GetYBillboardMatrix(D3DXMATRIX *get,FLOAT src[3],FLOAT tar[3]);//	������
void GetZBillboardMatrix(D3DXMATRIX *get,FLOAT src[3],FLOAT tar[3]);//	������
void GetBillboardMatrix(D3DXMATRIX *get,FLOAT src[3],FLOAT tar[3]);//	������

void CleanViewPortStack();
void PushViewPortArea();
void PopViewPortArea();



//Ǯ��ũ������Ʈ ( FSE ) ȿ����.
#define _FSE_MOTION_BLUR		0x80000000		//��� ��ο�� ������Ⱑ �����ϴ�.
#define _FSE_IN_WATER			0x40000000		//����.


void InitFullScreenEffect();	//���� ����̽� ������..
void ReleaseFullScreenEffect();
void FullScreenEffect();
void SetStateFullScreenEffect(DWORD run_id);	//0�� �������� �ƴϴ�. 1�� ��Ǻ�ξ�
DWORD GetStateFullScreenEffect();


//������ �ؽ���
void ShadowBeginScene();
void ShadowEndScene();
LPDIRECT3DTEXTURE8 GetShadowTexture();


#endif