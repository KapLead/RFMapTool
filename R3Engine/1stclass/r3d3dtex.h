#ifndef __R3D3DTEX_H__
#define __R3D3DTEX_H__
#include "d3d8.h"
#include "d3dx8.h"
#include "r3lightmap.h"

#define _R3T_VERSION 1.2f	//R3T���� ����..

#define _R3TEXFLAG_NONE_R3T			0x8000	//�ؽ��� ���������� r3t�� �ƴϴ�.
#define _R3TEXFLAG_DO_NOT_MANAGE	0x4000	//�ؽ��İ� �������̴�..
#define _R3TEXFLAG_MIPMAP			0x2000	//�ؽ��İ� MIPMAP�� �ִ�.

#define _R3TEX_MANAGE_FLAG_NO_SAME_CHECK	0x1


typedef struct{
	char mName[128];
	DWORD mStartID;
	DWORD mTexNum;
	DWORD mFlag;
	DWORD mSameCnt;		//���� �ؽ��ĸ� ����ϴ� ī��Ʈ... Ư�� �ؽ��ĸ� ������ �����ϴ�.
}R3Texture;


//   �ؽ��� ���� ����...
BOOL IsAniTex(char *hole);	//�ִϸ��̼� �ؽ������� �˾Ƴ���.
R3Texture *R3GetTexInfoR3T(char *name,DWORD flag);	//R3T�ؽ��� ������ �����Ѵ�.
_LIGHTMAP **LoadR3TLightMap(R3Texture *tex,D3DFORMAT desire_format);

char *R3GetTexName(int id);	//���̵� �־��ָ� �ؽ��� �̸��� �˷��ش�.
int R3GetPreTextureId(char *name);//ó���� �ε��� �ؽ��� ���̵� ����.������ �ؽ��� �޸𸮿� ������ �ʴ´�.
void R3GetPreAniTextureId(char *path,char *name,LONG *start_id,LONG *num); //�ε��� �ִ� �ؽ��� ���� ���̵�� ������ ����.������ �ؽ��� �޸𸮿� ������ �ʴ´�.
void R3ReleasePreTextures();	//�ε��� �ؽ��ĵ��� �����ϴ� _R3Texture�� ���ش�.
void R3ReleaseAllTextures();	//�ؽ��ĸ޸𸮸� ���� Ŭ�����Ѵ�.
void R3RestoreAllTextures();	//_R3Texture�� �����ؼ� �ε��� �ؽ��ĸ� �ؽ��ĸ޸𸮿� ��´�.

LPDIRECT3DTEXTURE8 R3GetSurface(int id);	//id�� �־��ָ� ǥ���� �������ش�.
LPDIRECT3DTEXTURE8 R3GetSurface(char* name);	
int GetNowR3D3DTexCnt();			//���� �ؽ��� ī��Ʈ�� ����
void SetNowR3D3DTexCnt(int cnt);	//����� ����% �𸣰� ���� å�Ӹ���...

int GetNowR3TexCnt();			//���� r3�ؽ��� ī��Ʈ
void SetNowR3TexCnt(int cnt);	//r3�ؽ��� ����, �𸣰� ���� å�Ӹ���...

void R3LoadTextureMem(int id);	//Ư�� �ؽ��ĸ� �ؽ��ĸ޸𸮿� ��´�.
void R3ReleaseTextureMem(int id);	//Ư�� �ؽ��ĸ� �ؽ��ĸ޸𸮿� ������.
void LoadR3T(R3Texture *tex);	//r3t������ ��� 

DWORD GetNowTexMemSize();	//��� �ε��ߴ� �ؽ��ĸ޸𸮿뷮�� ����.
void ResetTexMemSize();


void SetR3TexManageFlag(DWORD mFlag);
DWORD GetR3TexManageFlag();


LPDIRECT3DTEXTURE8 R3LoadDDSAndTextureMem(char *name);	//�ؽ��ĸ� �׳� �����.

//�ؽ��� �÷��� ����.
#define _R3D3DTEXTURE_FLAG_NO_LOD	0x80000000	//lod�����ϸ� �ȵǴ� �ؽ���.
void SetR3D3DTexture(DWORD id,DWORD flag);


//--2d��������Ʈ ����

void *GetDDSTexFromBuffer(WORD xl,WORD yl,DWORD format,BYTE *buf);
void InitSpriteManager();	//���� �ʱ�ȭ�� ȣ��.
void ReleaseSpriteManager();
class CSprite;
void RestoreSpriteManager(CSprite *spr);
void ReleaseSpriteManager(CSprite *spr);



//----------------------------- �ؽ��� ���� Ŭ���� 
class CTextureRender{
    LPD3DXRENDERTOSURFACE  m_pRenderToSurface;
    LPDIRECT3DTEXTURE8     m_pCausticTex;
    LPDIRECT3DSURFACE8     m_pCausticSurf;
    LPDIRECT3DSURFACE8     m_pZBuffer;

	BOOL m_IsLoaded;
	BOOL m_ClearAtOnce;
	BOOL m_bUseZbuffer;
	LPDIRECT3DDEVICE8 m_pd3dDevice;
	DWORD m_dwXsize,m_dwYsize;
public:
	CTextureRender();
	~CTextureRender();
	void ReleaseTexture();
	BOOL CreateTexture(LPDIRECT3DDEVICE8 pd3dDevice,DWORD x_size,DWORD y_size,BOOL use_zbuffer);	//z���۸� ���� �Ⱦ��� ���̰� �մ�.
	void ClearFrameBuffer( DWORD color = 0);
	BOOL BeginScene(DWORD sub_view_port=0);	//����Ʈ�� �Ϻθ� ���ִ��� Ȯ��.
	void EndScene();
	DWORD GetXSize()	{	return m_dwXsize;	};
	DWORD GetYSize()	{	return m_dwYsize;	};
	LPDIRECT3DTEXTURE8 GetD3DTexture()	{	return m_pCausticTex;	};
};

CTextureRender *GetShadowRenderTexture();

#endif