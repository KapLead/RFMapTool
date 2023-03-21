#ifndef __CORE_H__
#define __CORE_H__

#include "R3d3d8.h"
#include "C_vector.h"

#define _FRUSTUM_NEAR	4
#define _FRUSTUM_FAR	5
#define _FRUSTUM_RIGHT	1
#define _FRUSTUM_LEFT	0
#define _FRUSTUM_TOP	2
#define _FRUSTUM_BOTTOM	3


//#define _DEFINE_FOV		(D3DX_PI/(2.8f));		//48.29���̴�.	tan(����)��
#define _DEFINE_FOV		1.12f //(0.9f);		//tan(42). 


typedef struct{
//---���� �н� �κ�.
	char MapPath[128];
	char SystemPath[128];
	char EntityPath[128];
	char EffectPath[128];
	char ScreenShotsPath[128];
	char SoundPath[128];	//ȿ���� �н�

	char CharPath[128];
	char CharTexturePath[128];
	char MeshName[128];
	char BoneName[128];
	char StopAniName[128];
	char RunAniName[128];
	char SwordAniName[128];
	char SwordMeshName[128];
	char SwordTexturePath[128];

//---- ������ ȯ��κ�.
	BOOL mbStartCamera;			//���ۺ��� �������� ī�޶��ΰ�??
	float mNear,mFar;	//�Ͼ�� ���÷���...
	float mMipMapBias;		//�Ӹ� ��ī�ο� ����..
	float mFov;			//ī�޶� ����...
	float mAsfectRatio;			//ȭ�� ��Ⱦ��..
	float mScreenXsize;			//ȭ�� ���� �ػ�.
	float mScreenYsize;			//ȭ�� ���� �ػ�.
	float mFlySpeed;			//���� �ٴϴ°� �ӵ�
	DWORD mRenderBits;			//�����������ӹ��� ��Ʈ��..
	DWORD mZbufferBits;			//Z���� ��Ʈ��..
	BOOL mbFullScreen;			//Ǯ��ũ���ΰ�??
	DWORD mAvailableTextureMem;	//������ ���� ��밡�� �ؽ��� ������
	BOOL mIsUsedVertexShader;	//���ؽ����̴� ��� �����Ѱ�?
	BOOL mWideOn;				//16:9�� ���̵� �ΰ�..
	BOOL mIsDrawLogo;			//�ΰ� �Ѹ��ǰ�..
	DWORD mTextureUnitNum;		//�ϵ��� ���ÿ� �Ѹ��� �ؽ��İ���.

	//---CFG���Ϸ� ���� �о� ���̴°�...
	float mGamma;				//���� ������.
	DWORD mTextureDetail;		//�ؽ��� ������ ��...	//ĳ���Ϳ��ش�.
	BOOL mDetailTexture;		//�ؽ��� ������ ��...	//ĳ���Ϳ��ش�.
	DWORD mSceneTextureDetail;		//�ؽ��� ������ ��...	//��濡 �ش�.
	DWORD mShadowDetail;		//�׸��� ����..
	DWORD mDynamicLight;		//���������ü�...
	float mSeeDistance;			//���°Ÿ� ����... ����=1.0   0.8=8������ 0%�Ÿ�..
	BOOL mSoundOn;				//ȿ����..
	BOOL mMusicOn;				//���ǲ�..
	float mSoundVol;			//ǥ��������
	float mMusicVol;			//���� ����
	BOOL mMouseAccelation;		//���콺 ���ӿ���.
	DWORD mThisVidMemLevel;		//���� ��밡���� �ؽ��Ļ����� ���� 0,1,2	��,��,��


	DWORD mMaxTexSize;		//��ī���� �ִ� �ؽ��� ���ι� ���� ������
	DWORD mTempSize;

	BOOL mShowLightMap;		//����Ʈ���� ���δ�.
	//���װ���.
	BOOL mIsFog;			//���װ� �ִ°�?
	BOOL mIsNoFogSky;		//��ī�̹ڽ� ���װ� ���°�?
	BOOL mFogRangeEnable;	//���� ����ų��?
	float mFogStart;			//�ϵ���� ���� ����.
	float mFogEnd;				//��
	DWORD mFogColor;

	float mMainFogStart;			//���� ���� ���� ����.
	float mMainFogEnd;				//���� ���� ��
	DWORD mMainFogColor;			//���� ���� ��

	//---�����÷������.
	char mLensTexName[128];
	int	 mLensFlareTex;			//�����÷��� �ؽ��� �ȿ� 16���� �ش�Ǵ� �÷��� �������� �ִ�.
	float mLensFlareScale[16];	//�����÷���� �����ϰ�.
	float mLensFlarePos[3];		//�����÷���� ��ġ��. x,y,z

	//---Environment ���� ��ƼƼ�̸� ����.  // ��,��,��Ÿ ��ǳ��...
	char mEnvEntityName[128];		// ȯ���� ��ƼƼ�� ���ٵȴ�.
	DWORD mEnvID;					// ȯ�� ���̵� 0 �� ����, 1���� �޸� �𷡸���.

	//��Ʈ���� ����.
	D3DXMATRIX mMatWorld;	//���� ��Ʈ������..
	D3DXMATRIX mMatView;	//�� ��Ʈ������..
	D3DXMATRIX mMatViewProj;	//��*�������� ��Ʈ����
	D3DXMATRIX mInvMatView;	//�ι��� �� ��Ʈ������..
	D3DXMATRIX mMatProj;	//�������� ��Ʈ������..

	Vector3f mLightNomalForShadow;	//�����츦 ���� ����Ʈ����̴�..
	Vector3f mMainLightNomal;		//���ζ���Ʈ����̴�..
	float	 mMainLightYAngle;		//���ζ���Ʈ Yangle���̴�.������ ����.

	//�ؽ��� ���˰���.
	D3DFORMAT mBestTextureFormat;		//���� ������ �⺻ �ؽ�������
//	D3DFORMAT mBest2ndTextureFormat;		//���� ������ 2��° �ؽ������� bmp�ǰ��.32��Ʈ�� ���ϱ� �̰����� ��ü�Ѵ�.

	D3DFORMAT mBestSpriteTextureFormat;	//��������Ʈ�� �� ���� ������ �ؽ�������
	D3DFORMAT mBestAlphaTextureFormat;	//���İ� ���� ���� ������ �ؽ�������


	// ��ũ�� ������� ��������.
	LONG mViewSx,mViewSy,mViewXL,mViewYL;
	Vector4f mFrustumPlane[6];//Vector4f;	//�������� Ŭ���� �غ�.

	//�ý��� �ؽ���..
//	LPDIRECT3DTEXTURE8 mVFogTexture;			//���� �����ؽ���
	LPDIRECT3DTEXTURE8 mLogoTexture;			//�ΰ� �ؽ���
	LPDIRECT3DTEXTURE8 mDLightTexture;			//���̳��� ����Ʈ �ؽ���
}_R3ENGINE_STATE;

void InitCore();
//_R3ENGINE_STATE *GetR3State();
extern _R3ENGINE_STATE stState;
#define GetR3State() (&stState)

BOOL IsInitCore();
void ReleaseCore();
void SetFrustumNormalPlane();	//core�� view frustum ����� 
BOOL IsBBoxInFrustum( Vector3s bb_min,Vector3s bb_max );	//�ٿ�� �ڽ��� �������Ҿȿ� ��°�?
BOOL IsBBoxInFrustum( Vector3f bb_min,Vector3f bb_max );	//�ٿ�� �ڽ��� �������Ҿȿ� ��°�?
void GetFrustumNormalPlane(Vector4f n_plane[6]);	//core�� view frustum ����� 

void *GetTempBuffer();		//���ÿ� ��⿡�� ����� ũ�� ��� �� ���۸� ���� 
DWORD GetTempBufferSize();
void RestoreSystemTexture();	//�ý��� �ؽ��ĸ� �ø���.
void ReleaseSystemTexture();	//�ý��� �ؽ��ĸ� ������.

void SetGamma(float gamma);
void SetMainLight(Vector3f lgt_pos);	//���� ����Ʈ ��ġ�� �����Ѵ�.(�׸��ڿ� ��������.)

#endif