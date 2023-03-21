#ifndef __R3FONT_H__
#define __R3FONT_H__
///////////////////////////////////////////////////////////////////
//--����¡����R3������FONT���̺귯���Դϴ�.
//--������:������(���α׷���Ʈ).
//--�ϼ��� �ѱ��Դϴ�.
//--�׷��� ������ ������̵� ���� ���ϴ�.
//
//--1�� ���� ����(CD3DFont�� �״�� ����)
//--2�� �ѱ��� �ؽ�ó���ٰ� �� �׷���
//--3�� �ѱ� �ʹ������ ���� ���� ���� ���� ����.
//		�׷��� ���� ����,����,�ѹ�,Ư�� ���ڴ� �ȵ�
//--4�� ĳ�÷� ��ȯ, �����ϴ���
//(�̹���� ����, ����, ���� Ư�� ���� ���迡�� �����ս��� ������)
//--5�� ĳ�� ���� ������
//--6�� ĳ�� ������ ���� ��� ������. IME�� �׽�Ʈ �ҷ��� ��
//--7�� ĳ�� �ϴ� �������� �����غ�
//--8�� ĳ�� 2���� ���� ����
//--9�� ĳ�� 2���� 512���
//--10�� IME�׽�Ʈ(�ѱ��Է¿Ϻ�)
//--10�� �����߻� Fullscreen��忡��
//--11�� Fullscreen��� �ذ�
//--12�� CacheLock API ����
//--13�� ���� �߻� IME���� ���ڼ��� �������� �Ѿ�� ���� ������
//	delete�κ� ���� �߻�
//--14�� �ذ�
//-- ����ĳ�� ��ƾ �� �ٹٲ� ����... ��98���� �Ϻ� �ٿ������ذ�. �ӵ� 150-300% ���.
//-- ĳ�� �������� �ٲ�.
///////////////////////////////////////////////////////////////////
#include <d3d8.h>

//#define _CONSOLE
#ifdef _CONSOLE
#include "cconsoleex.h"
#endif

#define D3DFONT_CENTERED			    0x0100
#define D3DFONT_TWOSIDED				0x0200
#define D3DFONT_FILTERED				0x0400

#define D3DFONT_BOLD					0x0001
#define D3DFONT_ITALIC					0x0002
#define D3DFONT_ZENABLE					0x0004
//#define D3DFONT_UNDERLINE				0x0008
#define D3DFONT_SHADOW					0x0010
#define D3DFONT_OUTLINE					0x0020
//#define D3DFONT_BRIGHT_BLEND			0x0040		//���� �κ��� ���� ����ϴ� ȿ��...
#define D3DFONT_CLIPPING				0x0080		//Ŭ���� ����. NADIA�ʿ�.

#define D3DFVF_FONT2DVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)


//--2D���ؽ� ����ü RHW������ǥ�� ����Ѵ�. ��ȯ�� ������� �����Ƿ�.
struct FONT2DVERTEX
{
	D3DXVECTOR4 p;
	DWORD color;
	float tu, tv;
};

class CR3Font
{
private:
	//--��Ʈ�κ�
	LPDIRECT3DDEVICE8 m_pd3dDevice;				//--D3D����̽�(��� �Ǵµ�)
	LPDIRECT3DTEXTURE8 m_pTexture;				//--�ؽ�ó

	char m_strFontName[85];						//--��Ʈ�̸�
	float m_fTextScale;							//--�����ϰ�(Ȯ���)
	DWORD m_dwFontHeight;						//--��Ʈ������
	DWORD m_dwFontFlags;						//--�÷��� ���Ÿ�,����� ó��
	DWORD m_dwOutLineColor;						//--�ƿ����� �÷�.
	DWORD m_dwSavedStateBlock;					//--����� ��ϰ�	
	DWORD m_dwDrawTextStateBlock;				//--�ؽ�Ʈ �׸��� ��ϰ�

	HBITMAP m_hbmBitmap;
	HDC		m_hDC;
	HFONT	m_hFont;
	DWORD	*m_pBitmapBits;//--��Ʈ�ʺ�Ʈ������

	DWORD mMaxTextureXSize,mMaxTextureYSize;
	DWORD mFontMaxLines,mFontMaxStringLength;
	DWORD mDeleteLine;
	BYTE *mMemPtr;		//��ǥ�޸� ���������.
	DWORD *m_Start;	//������.
	DWORD *m_Cnt;		//���ڿ� ����.
	DWORD *m_StrCnt;	//�ش���κ��� ī����.
	DWORD *m_LineCnt;	//�ش� ������ ����.
	char *m_CacheString;	//���� �� ���ڴ�.


	DWORD m_dwFontVertexNum;	//�Ѹ� ��Ʈ�� ���ؽ� ����.
	float m_fClipping[2][2];
	FONT2DVERTEX m_Vertices[64];		//9�� ���� ���°Ŷ��� �ִ� 54��.

	void PrivateInit();
	void PrivateRelease();

	void MemAllocate();
	void MemFree();

	void PrepareDrawText();
	void DrawFullText(FONT2DVERTEX *pVertices);

	void ClearCache();
	//str_leng�� NULL�� ������ ����
	void GetBestPosCache(DWORD str_leng,DWORD *x_index,DWORD *y,DWORD *start);
	BOOL IsExistCache(char *ch,DWORD str_leng,DWORD *x_index,DWORD *y);
	void SetCache(char *ch,DWORD str_leng,DWORD x_index,DWORD y,DWORD start);

	HRESULT FillIt(FONT2DVERTEX *pVertices,float xyzw[4], DWORD dwColor, char* strText, DWORD dwFlags);

public:
	
	//--�ؽ�Ʈ �׸��� �Լ�(�ݵ�� BeginScene(), EndScene()���̿� �־�� �׷���)
	HRESULT DrawText(float x, float y, DWORD dwColor, char* strText, DWORD dwFlags=0L);
	HRESULT DrawText(float xyzw[4], DWORD dwColor, char* strText, DWORD dwFlags=0L);
	
	// BY Nadia
	// clipping ���� : 0.0 ~ 1.0
	HRESULT DrawText(float x, float y, DWORD dwColor, char* strText, float clipping[2][2], DWORD dwFlags=0L);

	//--D3D�ý��� �Լ�(D3D���� �̸��� ���� ������ �Լ��� ȣ���ؾ��Ѵ�.)
	HRESULT InitDeviceObjects( LPDIRECT3DDEVICE8 pd3dDevice,DWORD tex_xl,DWORD tex_yl );
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();

	void SetOutLineColor(DWORD color)	{	m_dwOutLineColor = color&0x00ffffff;		};
	DWORD GetOutLineColor()			{	return m_dwOutLineColor;	};
	
	//--����,�Ҹ�(����� 9����Ʈ �ۿ� ���� �ʴ´�)
	CR3Font();
	CR3Font( char* strFontName, DWORD dwHeight, DWORD dwFlags=0L);
	~CR3Font();
};
#endif
