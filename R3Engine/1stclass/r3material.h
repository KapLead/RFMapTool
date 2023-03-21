#ifndef __R3MATERIAL_H__
#define __R3MATERIAL_H__

#define _MAX_MATERIALS		1024
#define _MAX_LAYER_NUM		4	//���̳���,������,����Ʈ���� �����ϰ� �ִ� 4������..
#define _MAX_KEY_TRACK		32	//�ִ� ŰƮ����

#define _TEX_NAME_SIZE		128	//�ؽ��� �ִ� �̸�����
#define _MAX_ANI_TEX_NUM	64	// �ִϸ��̼� �Ǵ� �ִ� �ؽ��ļ�...

#include "C_vector.h"

#define _R3M_VERSION 1.1f	//R3M���� ����..
#define _R3X_VERSION 1.1f	//�ͽ��ٵ� ���͸���(����)R3X���� ����..


//----layer type
#define _LAYER_TYPE_NORMAL_MAP				0
#define _LAYER_TYPE_LIGHT_MAP				1
#define _LAYER_TYPE_TRANSPARENCY_MAP		2
#define _LAYER_TYPE_OPPASITY_MAP			3


#define _ALPHA_SORT_ON		0x80000000


#define _UV_ENV					0x00000001	
//#define _UV_WATER				0x00000002
#define _UV_LAVA				0x00000004
#define _UV_METAL_FLOOR			0x00000002
#define _UV_METAL_WALL			0x00000008
#define _UV_METAL				0x0000000a	//0x2 or 0x8
#define _UV_SCROLL_U			0x00000010
#define _UV_SCROLL_V			0x00000020
#define _UV_ROTATE				0x00000040
#define _UV_SCALE				0x00000080	//��������ü�� �ִϴ�.
#define _UV_GRADIENT_ALPHA_UV	0x00000300	//���ĸ� �۶���̼��Ѵ�.
#define _UV_GRADIENT_ALPHA_U	0x00000100	//���ĸ� �۶���̼��Ѵ�.
#define _UV_GRADIENT_ALPHA_V	0x00000200	//���ĸ� �۶���̼��Ѵ�.

#define _ANI_ALPHA_FLICKER		0x00000400	//���� ����
#define _ANI_TEXTURE			0x00000800  //�ؽ��ľִ�...
#define _ANI_TILE_TEXTURE		0x00001000  //Ÿ�� �ؽ��ľִ�...

#define _MAT_ZWRITE				0x00002000	//�ش� ���͸����� z ����Ʈ�� �Ѵ�. �⺻�� ���ϴ°Ŵ�.
#define _MAT_ENV_BUMP			0x00008000	//Env bump �����̴�.
//#define _MAT_VOLUME_FOG			0x00008000	//���͸����� �������״�.
#define _MAT_TEX_CLAMP			0x00010000	//�ؽ��� Ŭ���� �ɼ�.


#define _MAT_NO_COLLISON		0x00020000	//�� ���͸����� ���� ��������� ��� �浹�˻縦 ���Ѵ�.//0�� ���̾� �÷��׸� ����.
#define _MAT_WATER				0x00040000	//�� ���͸����� ���� ��������� ����ȿ���� ����.
/////////////////////////////////////////////////

#define FixedFloatToShort(a)	((short)((a)*256.0f))
#define FixedShortToFloat(a)	((float)((a)/256.0f))


#pragma	pack(push, 1)		//����Ʈ ����.

typedef struct
{
	SHORT	m_iTileAniTexNum;			//Ÿ�� �ִ� �� ����
	LONG	m_iSurface;
	DWORD	m_dwAlphaType;
	DWORD	m_ARGB;

	DWORD	m_dwFlag;	//���� ������ �ִ����� bit flag.	//env�� ���� �ʿ����..
	SHORT	m_sUVLavaWave;	//��Ͽ��̺��� ũ��

	SHORT	m_sUVLavaSpeed;	// ��� ���ǵ�
	SHORT	m_sUVScrollU;	// U�� �̵�
	SHORT	m_sUVScrollV;	// V�� �̵�
	SHORT	m_sUVRotate;	// ȸ�����ǵ�.
	SHORT	m_sUVScaleStart;	// ������ �ִϽ���...
	SHORT	m_sUVScaleEnd;		// ������ �ִϳ�...
	SHORT	m_sUVScaleSpeed;		// ������ �ִϳ�...
	SHORT	m_sUVMetal;		// ������ 
	SHORT	m_sANIAlphaFlicker;	//���� �ִ�
	WORD	m_sANIAlphaFlickerAni;	//���� �ִϽ��۰� ��,��.
	SHORT	m_sANITexFrame;			//�ؽ��� �ִ��� �����Ӽ�
	SHORT	m_sANITexSpeed;		//�ִϽ��ǵ��.	
	SHORT	m_sGradientAlpha;		//	�۶���̼� ���� 
}_ONE_LAYER;


#define _MAT_FLAG_LIGHT_MAP			0x00000001
#define _MAT_FLAG_NO_LOD_TEXTURE	0x00000002

typedef struct		//���͸������� ���� Ȯ����������. ��κ��� ����� ŰƮ��������� �ٲܿ���.
{
	////////////////////
	// tex
	int m_iMatNum;	//���͸��� �Ѱ��� ���͸����� ��Ʈ���� ���ڸ� ������ �ִ´�.

	char	m_name[_TEX_NAME_SIZE];					//mtrl name
	DWORD	m_dwFlag;		//�ش���͸����� �ñ�.

	LONG	m_iDetailSurface;		//������ �� 
	float	m_fDetailScale;

	DWORD	m_dwLayerNum;
	_ONE_LAYER m_Layer[_MAX_LAYER_NUM];
}_R3MATERIAL;


//ext ���͸��� �ʿ����õ� Ȯ�� ���͸���.
#define EXT_MAT_NO_FOG_SKY			0x00000001
#define EXT_MAT_FOG_RANGE			0x00000002
#define EXT_MAT_EXIST_1ST_FOG		0x00000004	//1������ ������.
#define EXT_MAT_EXIST_2ND_FOG		0x00000008	//2������ ������.
#define EXT_MAT_EXIST_LENS_FLARE	0x00000010	//���� �÷��� ������.
#define EXT_MAT_EXIST_ENV			0x00000020	//ȯ�� ��ƼƼ...������.

typedef struct{
	DWORD mFlag;		//

	float mFogStart;
	float mFogEnd;
	DWORD mFogColor;

	float mFogStart2;
	float mFogEnd2;
	DWORD mFogColor2;
	Vector3f BBMin2,BBMax2;	//2�� �� ���׿���.

	//���� �÷���
	float mLensFlareScale[16];
	char mLensTexName[128];	//�����÷���ξ� �ؽ��� �̸�
	Vector3f mLensFlarePos;

	char mEnvEntityName[128];
	DWORD mEnvID;			//ȯ�� ���̵� ����.. �̰ɷ� ���� �Ѵ�.

	BYTE spare[252+384];		//Ȯ�强�� ���� ����.1024k�� �����.
}_EXT_MAT;

#pragma	pack(pop)

//�ܺο��� ���Լ���...

//�ʸ��͸����� �ε��Ҷ��� ���ϰ��� �Ű� �Ƚᵵ ��..
_R3MATERIAL *LoadMainMaterial(char *matpath);	//�ʿ����õ� ���͸����� �̰����� 
void ReleaseMainMaterial();
BOOL IsLoadedMainMaterial();	//���� ���� ���͸����� �ε�Ǿ��°�?
DWORD	GetMainMaterialNum();
_R3MATERIAL* GetMainMaterial();


_R3MATERIAL* LoadMainR3M(char *name);	//r3m ���̳ʸ� ���� �д°�...
_R3MATERIAL* LoadSubR3M(char *mat_name);	//���θ��͸��� �ܴ̿� �̰Ÿ�.

void SetNoLodTextere();					//lod�ؽ��İ� �ƴѰ�� �÷��� ������ �س��´�.

//���� �ؽ��� ���̵� �߸��Ȱ� ����.
void AdjustIndependenceR3M(_R3MATERIAL *mat,LONG old_base_tex_cnt,LONG new_base_tex_cnt);


//���� ���͸����.
_R3MATERIAL *LoadSubMaterial(char *matpath);	// ��ī�̹ڽ��� ��ƼƼ�� �̰����� �߰��ؼ� ����.
void ReleaseSubMaterial(_R3MATERIAL *mat);	//���͸����� �������ϸ� �ؽ��ĵ� ������ �ȴ�.�߰��� add�� ���͸����� �����Ѵ�.

//���̾���� ��ġ�� �����ϰ������ ���� �������ʰ� ���͸����� ���ε��Ű�� �Լ� ���ε�� True ������ �ݵ�� false��
void SetReLoadState(BOOL state);	
void ReLoadMaterial(char *filename,_R3MATERIAL *now_material);



#endif