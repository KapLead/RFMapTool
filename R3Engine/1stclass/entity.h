#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "c_vector.h"
#include "animation.h"
#include "R3Material.h"
#include "Renderflag.h"
#include "R3VertexBuffer.h"
#include "r3d3dtex.h"
#include "commonutil.h"

#define _ENTITY_VERSION		113

#pragma	pack(push, 1)		//����Ʈ ����.

typedef struct{
	DWORD offset;		//�ɼ�.
	DWORD size;			//��Ʈ�� ������.
}_E_ENTRY;

#define _IDENTITY_ENTITY	0
#define _IDENTITY_PARTICLE	1

//	���� 8byte�� ������ �˼��ִ�.
//	DWORD version;		//_R3BSP_VERSION
//	DWORD Identity;		//�� ��ƼƼ�� ��ƼŬ�ΰ�.	
typedef struct{
	_E_ENTRY CompHeader;		//	float��
	_E_ENTRY Vertex;		//	float��
	_E_ENTRY VColor;		// vertex �÷�
	_E_ENTRY UV;			// uv��ǥ
	_E_ENTRY Face;		//	��
	_E_ENTRY FaceId;		// �� �ε�������Ʈ
	_E_ENTRY VertexId;	// �� �ε�������Ʈ
	_E_ENTRY MatGroup;	//	���͸��� �׷�..
	_E_ENTRY Object;	//	������Ʈ.
	_E_ENTRY Track;	//	Ʈ��.
}_ENTITY_FILE_HEADER;

//��ƼƼ ���� ���
typedef struct {
	WORD type;		//���ؽ� ����Ÿ�� 
	Vector3s bb_min,bb_max;		//���͸��� �ٿ�� �ڽ�
	Vector3f pos;				//��ġ
	float scale;				//�����ϰ�...
	//������ǥ
	float uv_min,uv_max;
}_ENTITY_COMP_HEADER;

typedef struct {
	WORD face_num;	//�ش� �� ����
	DWORD face_start_id;
	SHORT mtl_id;	//���͸��� ���̵�
	WORD object_id;	//������Ʈ ���̵�... ���̳��� ������Ʈ�� ����...
	Vector3s bb_min,bb_max;		//���͸��� �ٿ�� �ڽ�
}_READ_ENTITY_M_GROUP;

typedef struct{
	WORD v_num;		//�ش� �� ����
	DWORD v_start_id;	//����...
}_READ_ENTITY_FACE;

typedef struct {
	WORD Type;
	WORD TriNum;			//�ش� �ﰢ ����
	SHORT MtlId;			//���͸��� ���̵�
	Vector3f BBMin,BBMax;	//���͸��� �ٿ�� �ڽ�
	DWORD VBMinIndex;		//�� ���͸��� ����� ���۵Ǵ� ���ؽ� ���� �ε���
	DWORD IBMinIndex;		//�ε������� �ּ� ��ȣ.
	DWORD VCnt;				//��׷��� ���ؽ� ����..
	WORD ObjectId;			//���̳��� ������Ʈ ���̵�.
	void *MultiSourceUV;	//��Ƽ���̾ �� �ҽ� ������. uv����
}_ENTITY_M_GROUP;

//----------------------------------------------------




typedef struct{
	DWORD start_id;
	WORD entities_num;
}_LEAF_ENTITIES_LIST_INFO;

typedef struct{
	BYTE IsParticle;
	BYTE IsFileExist;		//�ε��ϰ����ϴ� ������ �����ϴ°�?
	char Name[62];
	float FadeStart,FadeEnd;
	WORD Flag;			//��ƼƼ �÷���..
	WORD ShaderID;		//���̴� ���̵�	//0���� �� ����Ʈ..
	float Factor[2];
}_ENTITY_LIST;

typedef struct{
	WORD ID;		//��ƼƼ ���̵�.
	float Scale;
	Vector3f	Pos;
	float RotX,RotY;	//x,yȸ���� 
	Vector3s BBmin,BBmax;	// �ٿ�� �ڽ�
}_READ_MAP_ENTITIES_LIST;	//���Ϸ� �д� ��ƾ


class CParticle;
typedef struct{
	WORD ID;		//��ƼƼ ���̵�.
	float Scale;
	Vector3f	Pos;
	float RotX,RotY;	//x,yȸ���� 
	Vector3s BBMin,BBMax;	// �ٿ�� �ڽ�
	float AddFrame;			//��ƼƼ���� �ִϸ� Ʋ���� ���ֱ� ���ؼ�.
	CParticle *Particle;	//��ƼŬ ������
	DWORD Color;			//���Ĺ� ����.
	DWORD mMapColor;		//�÷���. �� �ٴ��� �÷���.	//���������� 2x�̴�.
}_MAP_ENTITIES_LIST;		// ���� �޸𸮿��� ������������.


//---------------------------------------------------





#define _LOAD_POLY_IN_MEMORY		0x1		//��ƼƼ �ε��� ���ؽ�������,�������������� �޸𸮿� ������.
#define _USE_VERTEX_SHADER			0x2		//���ؽ� ���̴��� ����ϴ� ���.
#define _DO_NOT_MANAGE_TEXTURE		0x8		//��ƼƼ �ؽ��� �����޴�¡.
#define _ENTITY_Z_DISABLE			0x10	//��ƼƼ �ؽ��� �����޴�¡.
#define _ENTITY_MAP_COLOR			0x20	//��ƼƼ �� �ٴ� �÷� �����ϱ�

//��ƼƼ �÷��� 
#define _EF_BILLBOARD				0x80

class CEntity{
private:
	BOOL mIsUseMemory;		//�޸� ���ؽ� ���μ����� ���. ��ƼŬ�� ��� �̷��� ����...
	BOOL mIsEntityLoad;		//���ؽ� ���۸� �������Ҷ��� �ε��� �ȉ�ٰ� ���´�.
	BOOL mIsAlpha;			//�ϳ��� ���͸���׷��� �����ΰ�� ������ ���� ���õȴ�.
	_ENTITY_FILE_HEADER mHeader;
 	DWORD mVetexBufferSize;	//�� ���ؽ����� ������
//	BYTE *mStaticAlloc;		//���� �Ҵ� (���ϻ� �״�� �޸� �ø� ��)
	Vector2f *mOrgUV;
	DWORD mMapColor;		//�÷���. �� �ٴ��� �÷���.	//���������� 2x�̴�.
public:
	DWORD mFlag;			//��ƼƼ �÷��״�. 
	_R3MATERIAL *mMat;		//���͸���..
//	R3Texture *mR3Tex;		//R3 �ؽ���������.
	
	CVertexBuffer mStaticVertexBuffer;	//�������ؽ� ���� 
	CIndexBuffer mStaticIndexedBuffer;	//�����ε��� ���� 
 	DWORD mTextureSize;			//�ؽ��� ������
	int mMatGroupNum;
	_ENTITY_M_GROUP *mMatGroup;
	int mObjectNum;				//���̳��� ������Ʈ ����.
	_ANI_OBJECT *mObject;		//���̳��� ������Ʈ �ε���.
	BYTE *mTrack;				//���̳��� ������Ʈ Ʈ��.
	int mStartTexID;			//���� �ؽ��� ���̵�.
	int mTexNum;				//���� ��ƼƼ�� ������ �ִ� �ؽ��� ���� 
	void *mIndependencyTex;		//_DO_NOT_MANAGE_TEXTURE �ɼǽ�.. �����ؽ��� �����͸� ���� ����... �����ؼ� ���..
	float mStartTime;			//���� Ÿ�̸� ����.	�ؽ��� �ִ϶���� Ư���� �����ϴ� �κ� ���� �����ֱ����� �ִ�..
	void SetStartTimer()		{	mStartTime = R3GetTime();	};
public:
	Vector3f mBBMin,mBBMax;		//��ü �ٿ�� �ڽ�
	float	mAddFrame;			//��ƼƼ���� Ʋ�� �ִϸ��̼��� �ֱ�����... 

	CEntity::CEntity();
	CEntity::~CEntity();

//----- ������ ����(�޸𸮿� �����ϴ°��.. �浹�̳� ��ƼŬ�� ����� �ϵ���� ���ؽ����۰� �ƴ϶� �޸𸮿� �����ߵȴ�.)
	BOOL LoadEntity(char *filename,DWORD option=0);
	void ReleaseEntity();

	BOOL IsLoadedEntity() {		return mIsEntityLoad;	};
	BOOL IsUseMemory() {		return mIsUseMemory;	};	

	BOOL IsFirstAlpha();	//ù��°���͸���׷��� �����ΰ�? ��ƼƼ ������ �ʿ�.
	BOOL IsAlpha()	{	return mIsAlpha;	};			//�Ѱ��� �����ΰ�? 

//�̰Ŵ� �ѹ� �ѷ��ְ� ���־ߵǴ� ��ƼƼ...  FALSE�� �����ָ� �ִϸ��̼��� ������ ����.
	float mFrame;		//���� ������.�ѹ��Ѹ��� ���ֱ�����.
	BOOL OnceDrawEntity(float world_mat[4][4],DWORD color=0xffffffff);	//�����Ʈ������ �Ѱ��ش�.
//d3dmatrix�� ���ڷ� �Ѱ�����Ѵ�. ���� R3Render.cpp��  R3D3d8������ 
//���̷�Ʈ 3D�� �ٷ���� �ϱ⶧���� ���ڰ� �ʿ��ϴ�.
	void AddFlag(DWORD flag);	//���� �÷��׸� �߰��Ѵ�.
	BOOL DrawEntity(float world_mat[4][4],DWORD color=0xffffffff,float ani_frame=-1);	//�����Ʈ������ �Ѱ��ش�. //����� ��ƼƼ��·�ƾ... //false�ϰ�� �Ѹ����� ����(��ƼŬ)
	BOOL DrawEntityVS(_ENTITY_LIST *e_list,float world_mat[4][4],DWORD color=0xffffffff);	//�����Ʈ����,���ؽ����̴� ���� ��ƼƼ��·�ƾ... 

	void SetVertexShaderID(_ENTITY_LIST *e_list,DWORD color);	//���ý� ���̵�.

	int GetUsedVertexBufferSize();
	void GetAnimationMatrix(float get_ani_mat[4][4],_ANI_OBJECT *obj,float now_frame);
	void DrawOneMatGroup(_ENTITY_M_GROUP *m_group);	//�ϳ��� ���͸��� �׷��� �Ѹ���.
	void DrawOneMatGroupVS(_ENTITY_M_GROUP *m_group);	//���ؽ� ���̴� �ϳ��� ���͸��� �׷��� �Ѹ���.

	void RestoreTexMem();	//�ؽ��� �޸𸮿� �ø���.
	void ReleaseTexMem();	//�ؽ��� �޸𸮿� ������.


	//-----��ī�̹ڽ� �����ؼ� �Ѹ������� �����Ѵ�.
	_ANI_OBJECT *GetObject()	{	return mObject;	}
//	int GetObjectNum()			{	return mObjectNum;	}
	void PrepareAnimation();	//�ִϸ��̼� �ϱ����� �غ��Ұ͵�..
	int GetMatGroupNum()		{	return mMatGroupNum;	}
	int GetMatNum()				{	return mMat->m_iMatNum;	}	//��Ʈ�� ������ �������ֵ�.
	_ENTITY_M_GROUP *GetMatGroup(){	return mMatGroup;}

	void SetMapColor(DWORD color)	{	mMapColor=color;	}
	DWORD GetMapColor()				{	return mMapColor;	}

	CVertexBuffer* GetStaticVertexBuffer(){	return &mStaticVertexBuffer;	}
	CIndexBuffer* GetStaticIndexedBuffer(){	return &mStaticIndexedBuffer;	}
};


#pragma	pack(pop)


#endif