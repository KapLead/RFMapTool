#ifndef __R3BSP_H__
#define __R3BSP_H__

#include <windows.h>
#include <stdio.h>
#include "R3Alpha.h"
#include "C_vector.h"
#include "R3D3d8.h"
#include "ToolCollision.h"
#include "Animation.h"
#include "entity.h"
#include "SoundEntity.h"
#include "R3Particle.h"
#include "CPathFinder.h"

//#define EXPORT extern "C" _declspec (dllexport)

//bsp�߰� ���̺긦 ���� ����ü.
typedef struct{
	//�浹 ������...
	_TOOL_COL_POINT *ColPoint;
	_TOOL_COL_LINE *ColLine;
	WORD *ColLineID;
	_TOOL_COL_LEAF *ColLeaf;
	//��ƼƼ
	_ENTITY_LIST *EntityList;
	_LEAF_ENTITIES_LIST_INFO *LeafEntityList;
	_READ_MAP_ENTITIES_LIST *MapEntitiesList;
	WORD *EntityID;
	//����..(��� ȿ����...)
	_READ_SOUND_ENTITY_LIST *SoundEntityList;
	_LEAF_SOUND_ENTITIES_LIST_INFO *LeafSoundEntityList;
	_READ_SOUND_ENTITIES_LIST *SoundEntitiesList;
	WORD *SoundEntityID;
}_ADD_BSP_SAVE;


#define _READ_BSP_SPARE_CNT	35	//26		//����Ȯ���� ���� ����ī��Ʈ.
#define _FREE_BSP_SPARE_CNT	32  //29


#pragma	pack(push, 1)		//����Ʈ ����.

#define _R3BSP_VERSION			39	// 000���� ���
#define _R3_EXT_BSP_VERSION		20	// 19 ���� ���
//------------------- ���� export�Ǵ� BSP 
typedef struct{
	DWORD offset;		//�ɼ�.
	DWORD size;			//��Ʈ�� ������.
}_ENTRY;

typedef struct{
	DWORD version;		//_R3BSP_VERSION
//------------------- �����͸� ������ �޸𸮿� ������ �ִ°͵�...
	_ENTRY CPlanes;		// �� ���	d���� ����.
	_ENTRY CFaceId;		// �� �ε�������Ʈ	�ӽ÷� ���.... ���߿� ����ȭ
	_ENTRY Node;		// ��� 
	_ENTRY Leaf;		// ����

	_ENTRY MatListInLeaf;	//	������ �ִ� ���͸���׷츮��Ʈ.

	//---	�ִϸ��̼� ����.
	_ENTRY Object;	//����� ���̳��� ������Ʈ�� ����. 
	_ENTRY Track;	//�ִϸ��̼� Ʈ��
	_ENTRY EventObjectID;		//	�̺�Ʈ ������Ʈ ���̵�.	 ���̳��� ������Ʈ�� �̺�Ʈ ������Ʈ �ִϸ��̼� ���̵�.
/*
	//----  �浹���ΰ��� -- ���������� ������ �߰��Ҽ� �ִٴ� ���̴�.
	_ENTRY CFVertex;	//	�浹 ���̽� ��.
	_ENTRY CFLine;		//	�浹 ���̽� ����
	_ENTRY CFLineId;	//	�浹 ���̽� ���̵� ����Ʈ.
	_ENTRY CFLeaf;		//	�浹 ���̽� ����.
	_ENTRY EntityList;		//	��ƼƼ ���� ����Ʈ
	_ENTRY EntityID;		//	 map entities list ���̵� �¾�.
	_ENTRY LeafEntityList;		//	���� ��ƼƼ ����Ʈ. ������ ���� ���̵�� ������ ������ ����.

	_ENTRY SoundEntityID;		//	���� ���� ��ƼƼ ����Ʈ ���̵�
	_ENTRY LeafSoundEntityList;		//	���� ���� ��ƼƼ ����Ʈ. 	������ ���� ���̵�� ������ ������ ����.
*/	

	_ENTRY ReadSpare[_READ_BSP_SPARE_CNT];

//------------------- �����͸� �а��� Free�ϴ� �͵�.
	//-------- ȭ�� ���÷��̿�...
	_ENTRY BVertex;		//	byte ��
	_ENTRY WVertex;		//	word ��
	_ENTRY FVertex;		//	float��
	_ENTRY VertexColor;	// ���ؽ� �÷� 
	_ENTRY UV;			// uv��ǥ
	_ENTRY LgtUV;		// ����Ʈ�� uv��ǥ	//WORD���̴�.
	_ENTRY Face;		//	��
	_ENTRY FaceId;		// �� �ε�������Ʈ
	_ENTRY VertexId;	// �� �ε�������Ʈ
	_ENTRY ReadMatGroup;	//	���͸��� �׷�..	�б������ ���Ŵ�..
/*
	_ENTRY MapEntitiesList;	//	�ʻ� �ѷ����� ��ƼƼ����Ʈ.
	_ENTRY SoundEntityList;	//	���� ��ƼƼ����Ʈ.	
	_ENTRY SoundEntitiesList;	//	���� ��ƼƼ�� ����Ʈ.  �������� �ӳı�?�����..
*/
	_ENTRY FreeSpare[_FREE_BSP_SPARE_CNT];
}_BSP_FILE_HEADER;



#define _READ_EXT_BSP_SPARE_CNT	18			//����Ȯ���� ���� ����ī��Ʈ.
#define _FREE_EXT_BSP_SPARE_CNT	18  

typedef struct{		//������ �߰��� BSP ����.
	DWORD version;		//_R3_EXT_BSP_VERSION
//---������ ���ÿ� �޸𸮿� �������°�.
	// �浹���ΰ��� 
	_ENTRY CFVertex;	//	�浹 ���̽� ��.
	_ENTRY CFLine;		//	�浹 ���̽� ����
	_ENTRY CFLineId;	//	�浹 ���̽� ���̵� ����Ʈ.
	_ENTRY CFLeaf;		//	�浹 ���̽� ����.
	//��ƼƼ 
	_ENTRY EntityList;		//	��ƼƼ ���� ����Ʈ
	_ENTRY EntityID;		//	 map entities list ���̵� �¾�.
	_ENTRY LeafEntityList;		//	���� ��ƼƼ ����Ʈ. ������ ���� ���̵�� ������ ������ ����.
	//����
	_ENTRY SoundEntityID;		//	���� ���� ��ƼƼ ����Ʈ ���̵�
	_ENTRY LeafSoundEntityList;		//	���� ���� ��ƼƼ ����Ʈ. 	������ ���� ���̵�� ������ ������ ����.
	//���� ���� 
	_ENTRY ReadSpare[_READ_EXT_BSP_SPARE_CNT];

//-- �а� ���ο��� ���Ǵ� �޸𸮷� �����ϴ°͵�.
	_ENTRY MapEntitiesList;	//	�ʻ� �ѷ����� ��ƼƼ����Ʈ.
	_ENTRY SoundEntityList;	//	���� ��ƼƼ����Ʈ.	
	_ENTRY SoundEntitiesList;	//	���� ��ƼƼ�� ����Ʈ.  �������� �ӳı�?�����..

	_ENTRY FreeSpare[_FREE_EXT_BSP_SPARE_CNT];
}_EXT_BSP_FILE_HEADER;


/*
typedef struct{
	WORD f_normal_id;		//	���̽� ��� ���̵�
	float d;				//	ax+by+cz = d �� d
	WORD vertex_num;			//	vertex����
	DWORD vertex_start_id;	//	���� vertex id = uv�� ���� id = light��uv ���� id(���� ����.)
}_BSP_C_FACE;
*/

typedef struct{
	DWORD f_normal_id;		//	��带 ������ ��� ������
	float d;				//	ax+by+cz = d �� d
	SHORT front;
	SHORT back;
	Vector3s bb_min,bb_max;	//����� �ٿ�� �ڽ� �뷫 5%���� ��ũ��...
}_BSP_NODE;

typedef struct {
	BYTE type;					//����� Ư��. water�����...
	WORD face_num;				//bsp ���̽� ����
	DWORD face_start_id;		//����bsp face id��ȣ

	WORD m_group_num;			//���͸��� �׷�  ����
	DWORD m_group_start_id;		//���� ���͸��� �׷� id��ȣ.

	Vector3s bb_min,bb_max;		//����� �ٿ�� �ڽ�
}_BSP_LEAF;



//---- �б����� ����ü...
#define _VERTEX_ARC_TYPE_CHAR		0x8000
#define _VERTEX_ARC_TYPE_SHORT		0x4000
#define _VERTEX_ARC_TYPE_FLOAT		0x2000

typedef struct {
	WORD attr;		//���� ����Ʈ�� ����Ÿ�� 0 ����������,1 ����Ŀ��?
	WORD face_num;	//�ش� �� ����
	DWORD face_start_id;
	SHORT mtl_id;	//���͸��� ���̵�
	SHORT lgt_id;	//����Ʈ �� ���̵�
	Vector3s bb_min,bb_max;		//���͸��� �ٿ�� �ڽ�
	Vector3f pos;				//��ġ
	float scale;				//�����ϰ�...
	WORD object_id;		//������Ʈ id= ���� id+1 ����� ���̳��� ������Ʈ�� ���� ���.
}_BSP_READ_M_GROUP;

typedef struct{
	WORD v_num;		//�ش� �� ����
	DWORD v_start_id;	//����...
}_BSP_READ_FACE;

//-----�����Ǵ� ����ü.

#define _MAT_GROUP_TYPE_STANDARD	0x0001	//����Ʈ���� �� �ִ� �Ϲ����� ���ؽ� ����.
#define _MAT_GROUP_TYPE_NORMAL		0x0002	//����� ���־�ߵǴ� ���ؽ� ����.
#define _MAT_GROUP_TYPE_OPPA		0x8000	//���Ĺ׹��������� �� �׷�.
#define _MAT_GROUP_TYPE_REFLECT		0x4000	//�ݻ� �ſ��̴�.

#define _MAT_Y_BILLBOARD				0x0800	//y������ �׷�.
#define _MAT_CORONA						0x0400	//�ڷγ� ��.

typedef struct {
	WORD Type;				//�Ÿ���ΰ�..
	WORD TriNum;			//�ش� �ﰢ ����
	SHORT MtlId;			//���͸��� ���̵�
	SHORT LgtId;			//����Ʈ �� ���̵�
	Vector3f BBMin,BBMax;	//���͸��� �ٿ�� �ڽ�
	Vector3f Origin;		//�ٿ���ڽ��� �߽����̴�.
	DWORD VBMinIndex;		//�� ���͸��� ����� ���۵Ǵ� ���ؽ� ���� �ε���
	DWORD IBMinIndex;		//�ε������� �ּ� ��ȣ.
	DWORD VertexBufferId;	//���ؽ� ���� ���̵�.(��Ƽ ���ؽ����۸� ����..)
	DWORD VCnt;				//��׷��� ���ؽ� ����..
	DWORD CFaceStartVId;		//�浹 ��������� ���踦 ���� ���� ���ؽ����۾��̵�.
	void *MultiSourceUV;		//��Ƽ���̾ �� �ҽ� ������. uv����
	void *MultiSourceST;		//��Ƽ���̾ �� �ҽ� ������. st����
	WORD ObjectId;		//������Ʈ id= ���� id+1 ����� ���̳��� ������Ʈ�� ���� ���.
	union{				//����ü�� ���������� �����Ѵ�.
	float CoronaAlpha;
	};
}_BSP_MAT_GROUP;

#define _CFACE_ATTR_NOT_FLOOR	0x80	//�浹 �������� �ٴ��� �ƴѰ��..(����� ����Ѵ�.)
#define _CFACE_NO_COLLISION		0x40	//�浹 ������ ������.
#define _CFACE_WATER			0x20	//���浹 ������.

typedef struct{				// ���̳��� ����Ʈ�� ����� �� �浹�� ����.
	BYTE Attr;				// ����� �Ӽ�,
	BYTE VNum;				//�ش� �� ����
	DWORD VStartId;			//����...	//edge��ֵ� ����.
	WORD MatGIndex;			//���͸��� �׷� �ε���
	Vector4f Normal;		//	ax+by+cz = d 
}_BSP_C_FACE;

#pragma	pack(pop)


#define _MAX_DYNAMIC_VB_FACE_NUM	1024	//�ѹ� ó���� �ִ� ���̳��� ���ؽ����� �����ﰹ��...
#define	_MAX_VB_CNT		80	//80���� �뷫 �鸸 �������� ó���Ѵ�.

#define _PATH_NOT_FOUND			0
#define _PATH_FOUND				1
#define _PATH_TOO_LONG_CLIP		2		//�ָ����� ��� ��� �����ְ� �ϱ����ؼ�..


#define _EVENT_ANI_STATE_FRONT_STOP			0
#define _EVENT_ANI_STATE_FRONT_START		1
#define _EVENT_ANI_STATE_FRONT_RUN			2
#define _EVENT_ANI_STATE_BACK_START			3
#define _EVENT_ANI_STATE_BACK_RUN			4
#define _EVENT_ANI_STATE_BACK_STOP			5


class  CBsp{
private:
	//�浹���� ���� 
	Vector3f *mCVertex;		// �� 
	Vector3f *mCNNormal;	// �浹 ��� ���	d���� ����.
	DWORD *mCVertexId;		// �� ���̵� ����Ʈ
	_BSP_C_FACE *mCFace;	// �浹 �� ��Ʈ��
	Vector4f *mCNEdgeNormal;	// �浹 ��� �������, �޸𸮶����� ���������� ����.
	DWORD *mCFaceId;			// �浹 �� �ε�������Ʈ	�ӽ÷� �����.... ���߿� ����ȭ

	DWORD *mVertexColor;	//���ؽ� �÷� �迭.
	Vector2s *mLgtUV;		// ����Ʈ�� uv��ǥ�� �������ִ°�.. 
	LONG mNowCFaceId;		// nextypos���� �浹�� ���� �ֱٿ� ���� ���̽� ���̵�. �������� ���Ҷ� ����.

	WORD *MatListInLeafId;		//������ �ִ� ���͸��� �׷� ����Ʈ ���̵�
	DWORD mMatGroupNum;			//���͸��� ��� �Ѱ���.
	_BSP_MAT_GROUP *mMatGroup;		//���͸��� ��� �б�.

	DWORD	mCVertexNum;	//�浹������
	DWORD	mCFaceNum;		//�浹�鰹��

	DWORD	mVertexNum;		//������
	DWORD	mFaceNum;		//�鰹��

	BOOL mIsLoaded;			//�� �ε� �Ǿ��°�?

	//���̳��� ������Ʈ
	DWORD mObjectNum;		//���̳��� ������Ʈ ���� 
	_ANI_OBJECT *mObject;		//���̳��� ������Ʈ 
	WORD *mEventObjectID;		//�̺�Ʈ ������Ʈ�� ���̵�..

	//---Environment ��������.  // ��,��,��Ÿ ��ǳ��...
#define _ENV_PARTICLE			0x0001000
	DWORD mEnvID;				//ȯ�� ���̵�..
	union{
	CEntity *mEnvEntity;		// ȯ���� ��ƼƼ�� ��ƼŬ �� ���ٵȴ�.
	CParticle *mEnvParticle;
	};
public:
	//��ƼƼ ����
	BYTE *mEntityCache;		//�� ��ƼƼ ĳ��
	DWORD mEntityCacheSize;	//�� ��ƼƼ ĳ�� ������ 
	DWORD mNowRenderMatGroupNum;	// ���� ������ �ǰ� �մ� ���͸��� ��� ����.
	DWORD mNowShadowMatGroupNum;	// ���� ������ �ǰ� �մ� ��������͸��� ��� ����.
	inline DWORD GetMatGroupNum()	{	return mMatGroupNum;	};			//���͸��� ��� �Ѱ���.


	CEntity *mEntity;		//�ʻ��� ���� ��ƼƼ
	CParticle *mParticle;	//�ʻ��� ���� ��ƼŬ

	_ENTITY_LIST *mEntityList;		//	��ƼƼ ���� ����Ʈ
	WORD		*mEntityID;		//	 map entities list ���̵� �¾�.
	_LEAF_ENTITIES_LIST_INFO *mLeafEntityList;		//	���� ��ƼƼ ����Ʈ. ������ ���� ���̵�� ������ ������ ����.
	_MAP_ENTITIES_LIST *mMapEntitiesList;	//�ʻ��� ��ƼƼ�� ���� 
	DWORD mEntityListNum;
	DWORD mLeafEntityListNum;
	DWORD mEntityIDNum;
	DWORD mMapEntitiesListNum;
	void LoadEntities( _READ_MAP_ENTITIES_LIST *readMapEntitiesList);	//��ƼƼ���� �ε��Ѵ�.
	void ReleaseEntities();												//��ƼƼ���� �������Ѵ�.
	void CalcEntitiesMainColor();										//��ƼƼ�� �ٴ��÷����.
	void RenderOneEntityRender(WORD entities_id);	//�� ��ƼƼ�ϳ� �Ƿ��� ������.

	BOOL IsInWater(Vector3f src,Vector3f tar,Vector3f *get=NULL,float plane_dist=0);	// ���۰� ���� ���� ���ΰ�.���� src�� ������ǥ�� �����̿��� �Ѵ�.

	//���� ��ƼƼ ����...
	BYTE *mSoundEntityCache;		//���� ��ƼƼ ĳ�� �� ���۴�.
	DWORD mSoundEntityCacheSize;	//���� ��ƼƼ ĳ�� ������ 

	DWORD mSoundEntityIDNum;
	DWORD mLeafSoundEntityListNum;
	DWORD mSoundEntityListNum;		//���� ���� ����.
	DWORD mSoundEntitiesListNum;	//�����ǵ� ����.
	_SOUND_ENTITY_LIST *mSoundEntityList;
	_SOUND_ENTITIES_LIST *mSoundEntitiesList;
	WORD	*mSoundEntityID;		//	 ���� entities list ���̵� �¾�.
	_LEAF_SOUND_ENTITIES_LIST_INFO *mLeafSoundEntityList;
	void LoadSoundEntities(_READ_SOUND_ENTITY_LIST *sound_entity_list ,_READ_SOUND_ENTITIES_LIST *sound_entities_list);	//��ƼƼ���� �ε��Ѵ�.
	void ReleaseSoundEntities();	//��ƼƼ���� �������Ѵ�.
	void HearMapSound();	//�� ���带 ����.(�������� �ƴϹǷ� endscene()���Ŀ� ����..)

	DWORD mTotalAllocSize;	//bsp�޸� ��ü(���ȿ������ �ϴ� ����)
	DWORD mTotalWaveSize;	//���ȿ������
	DWORD mVertexBufferSize;	//�� ���ؽ����� ������ ��ü.

	BOOL IsLoaded()								{	return mIsLoaded;	};
	void SetIsLoaded(BOOL load_ok)				{	mIsLoaded=load_ok;	};

public:
	_BSP_NODE *mNode;		// ��� 
	_BSP_LEAF *mLeaf;		// ����

	DWORD	mLeafNum;		//��������
	DWORD	mNodeNum;		//��尹��

	//--�浹 ���̽�(����) ����.
	DWORD mCFVertexNum;			//	�浹 ���̽� ��.
	DWORD mCFLineNum;			//	�浹 ���̽� ����
	DWORD mCFLineIdNum;			//	�浹 ���̽� ���̵� ����Ʈ����.

	Vector3f *mCFVertex;			//	�浹 ���̽� ��.
	_TOOL_COL_LINE *mCFLine;	//	�浹 ���̽� ����
	WORD *mCFLineId;			//	�浹 ���̽� ���̵� ����Ʈ.
	_TOOL_COL_LEAF *mCFLeaf;	//	�浹 ���̽� ����. ������ ���� �������� ����.

	Vector3f *mCFVNormal;		//�浹 ���ؽ� ���.
	Vector4f *mCFNormal;		//�浹 ���.

	void RenderCollisionLeaf(short leaf_id);//���� �浹 �������� �������Ѵ�.

private:
	void MakeEdgeNormal();	//�������������� ���� �浹 ýũ�� �ʿ��ϴ�.Edge normal�� ������ �ִ´�.

	BOOL IsExistSelfPoint(int id,int face);	//�佺����Ʈ�� �浹 ���̽��� ������ �ֳ�?
	void GetFaceFrontPoint(Vector3f *get_temp,int v_id);	//����� �ణ ������ Ƣ��� ���� ���´�.
	void GetPath(Vector3f from,Vector3f at);
	int GetFinalPath(void *node_p,Vector3f from,Vector3f get[16]);
	int GetPathCrossPoint(Vector3f from,Vector3f at,Vector3f *get,int now_f1,int now_f2);
	BOOL EdgeTest(Vector3f cross,int id);
	void GetLightMapUVFromPoint(Vector2f getuv,int face_id,Vector3f p);	//������� �� ���� �Ѱ��ָ� uv�� �����ش�.
	DWORD GetColorFromPoint(int face_id,Vector3f p);	//���ؽ� �÷����� ������� �� ���� �Ѱ��ָ� �÷��� �����ش�.

public:
	DWORD GetLightFromPoint(Vector3f p,DWORD diff=2); //���� ����ִ� ������� �� ���� �Ѱ��ָ� ���� �������� �����ش�.

	CPathFinder mPathFinder;	//�н����δ� 
	int mColFaceId;	//�浹 ���̽� ���̵�
	DWORD mFindPathCnt;			//�н� ã�� ����.
	void SetCFNormal();			//�浹���̽� ���� ���� ����� ���Ѵ�.
	DWORD GetPathFind(Vector3f from,Vector3f at,Vector3f get[16],DWORD *cnt,int depth=_MAX_PATH_POS);	//�н� ���δ�.��ã���� false�� ���������� �ִ�� �����ִ� ��ǥ�� �־��ش�.
	void DrawCollisionPoly(); //�浹 �������� �Ѹ���.
	void DrawLeafCollisionPoly(short leaf_id);//�������� �浹 �������� �Ѹ���.
	short mNowPlayerNum;		//���� �÷��̾��� ������ȣ�� ������ �ִ�...

	void RenderEnvironment(DWORD id);	//ȯ���� �����ϴ�... ��,��,����ǳ.
private:

	//���ؽ� ���� ���� ���ؽ� ���۴� �� 3������ �ִ�. 
	DWORD mStaticVBCnt;		//����ƽ ���ؽ������� ī��Ʈ.
	CVertexBuffer mStaticVertexBuffer[_MAX_VB_CNT];	//�������ؽ� ���� (������ ��κ��� ���⿡ �ִ�)
	DWORD mVBVertexNum[_MAX_VB_CNT];		//���ؽ������� ���ؽ� ������...
	CIndexBuffer mStaticIndexedBuffer;	//�����ε��� ���� (������ ��κ��� ���⿡ �ִ�)
//	void* mNormalVertexBuffer;	//����� ���Ե� ���� ���ؽ� ���� (��Ż�̳� env����..)
//	void* mNormalIndexedBuffer;	//����� ���Ե� ���� �ε��� ���ؽ� ���� (��Ż�̳� env����..)
//	void* mDynamicVertexBuffer;	// ���̳��� ���ؽ� ����.

	void* mMultiLayerUV;	//��Ƽ���̾�� �� uv
	void* mMultiLayerST;	//��Ƽ���̾�� �� st����

	//���̳��� ������Ʈ ����.
	void PrepareAnimation();		//�ִϸ��̼� �ϱ����� �غ��Ұ͵�..
	void GetBspObjectMatrix(float w_matrix[4][4],WORD id);
public:
	BOOL SetEventAnimationState(WORD id,DWORD flag);
	DWORD GetEventAnimationState(WORD id);
	BOOL SetAllAnimationState(DWORD flag);


	//----�� ���� 
	BYTE *mStaticAlloc;		//���� �Ҵ� (���ϻ� �״�� �޸� �ø� ��)
	DWORD mStaticAllocSize;	//���� �Ҵ�ũ��...
	
	BYTE *mExtBspStaticAlloc;		//���� �Ҵ� (���ϻ� �״�� �޸� �ø� ��)
	DWORD mExtBspStaticAllocSize;	//���� �Ҵ�ũ��...

	//--���콺 Ŭ���� Ŭ���Ǵ� ������...
	int mPickPoly;	//-1�ϰ��� Ŭ���� �ȵǾ���..
private:
	BYTE *mMatGroupCache;	//���͸��� �׷��� �۵��Ǵ� ĳ��
	int mMatGroupCacheSize;	//���͸��� �׷� ĳ�� ������ 

//	Vector3f mPointCamera;	//���ο��� �ӽ÷� ���� ����Ʈ ī�޶�..
	Vector3f mTempCamera;	//���ο��� �ӽ÷� ���� �÷��̾� ��ġ..
	short mTempSearchOk;
	short mNowLeafNum;		//���� ī�޶��� ������ȣ�� ������ �ִ�...

	void GetVertexNormal();

	void RenderShadowMatGroup(WORD mat_g_id);	//���͸��� �׸��ڸ� �׸���.
	void RenderMatGroup(WORD mat_g_id);
	void RenderLeaf(short leaf_id);
	void WalkLeaf(short leaf_id);
	void WalkNode(short node_id);
	void SearchNode(short node_id);
	void GetHeight(Vector3f camera);
	void LoopInitRenderedMatGroup();	//���͸��� ĳ�� �ʱ�ȭ..

	void SubLeafList(float su,_BSP_NODE *node,Vector3f src,Vector3f tar,SHORT *leaf_list,int *leaf_ids);
	void WalkNodeForLeafList(short node_id,Vector3f src,Vector3f tar,SHORT *leaf_list,int *leaf_ids);
	void SubLeafListFromBBox(float su,_BSP_NODE *node,Vector3f *bb_list,SHORT *leaf_list,int *leaf_ids);
	void FastWalkNodeForLeafListFromBBox(short node_id,Vector3f *bb_list,SHORT *leaf_list,int *leaf_ids);
	
	void ReadDynamicDataFillVertexBuffer(FILE *fp);						//bsp���� ���Ϸ��а� �������Ұ͵�.
	void OnlyStoreCollisionStructure(_BSP_READ_M_GROUP *readmatgroup,	//�������� �� �浹�κи� �д´�.
		Vector3c *bvertex,Vector3s *wvertex,Vector3f *fvertex,DWORD *vertexid
		,_BSP_READ_FACE *face,DWORD *faceid );


	void ReadDynamicDataExtBsp(FILE *fp);	//ext bsp���� ���Ϸ��а� �������Ұ͵�.

	//�ܺο��� �����ϴ� bsp����
public:
	_BSP_FILE_HEADER		mBSPHeader;		//bsp���� ���... ���� ������ �ִ�..
	_EXT_BSP_FILE_HEADER	mExtBSPHeader;	//ext bsp���� ���... ���� ������ �ִ�..

	CBsp();
	~CBsp();
	ClearVariable();					//bsp ���� Ŭ����
	void LoadBsp(char *name);			//bsp ���� �ε�...
	void LoadExtBsp(char *name);		//ext bsp�� �ε�����.������� ReleaseBsp()���� ���ش�.
	void ReleaseBsp();					//bsp���� Ŭ����...

	void ReadyBspRender(Vector3f camera);
	void DrawBspRender();
	void DrawMapEntitiesRender();	//�� ��ƼƼ�� ������.
	void DrawAlphaRender(Vector3f camera);
	void DrawLeafBBox();	//������ �ٿ�� �ڽ��� ���.
	void DrawMatBBox();		//���͸��� �ٿ�� �ڽ� ���.

	short GetLeafNum(Vector3f player);	//������ ������ȣ�� ����.
	void GetLeafList(Vector3f src,Vector3f tar,LONG *num,SHORT *leaf_list,DWORD max_num);	//a,b���� �����ϴ� ���� ����Ʈ�� ����.
	void GetFastLeafListFromBBox(Vector3f bb_min,Vector3f bb_max,LONG *num,SHORT *leaf_list,DWORD max_num);// �ٿ�� �ڽ��� �־��ָ� ��������Ʈ�� �ش�.

	//�浹 ���� �Լ�..
	float GetFirstYpos(Vector3f pos,int option);	//���Ҽ� ���°�� 65535�� ����
	float GetFirstYpos(Vector3f pos,Vector3s min,Vector3s max);	//�ٿ�� �ڽ� �����ȿ��� ���Ѵ�.���Ҽ� ���°�� 65535�� ����
	float GetFirstYpos(Vector3f pos,Vector3f min,Vector3f max);	//�ٿ�� �ڽ� �����ȿ��� ���Ѵ�.���Ҽ� ���°�� 65535�� ����

	float GetYposInLeafNoAttr(Vector3f a,Vector3f b,float able_height,float now_height,int leaf_id);// able�� ���� ������ ����,now_���� ����,leaf�� ���� ����
	float GetYposInLeaf(Vector3f a,Vector3f b,float able_height,float now_height,int leaf_id);// able�� ���� ������ ����,now_���� ����,leaf�� ���� ����
	float GetYposInLeafUseEdgeNormal(Vector3f a,Vector3f b,float able_height,float now_height,int leaf_id);// able�� ���� ������ ����,now_���� ����,leaf�� ���� ����
	float GetBestYposInLeaf(Vector3f a,Vector3f b,float best_height,float now_height,int leaf_id);	// best�� �ö󰥼��ִ� ������ ����,now_���� ����,leaf�� ���� ����


	BOOL CanYouGoThere(Vector3f now,Vector3f target,Vector3f *get_point); //���ʱ��� ���� �ִ�? ������ �����ִ� ������ get_point�� �־��ش�.
	BOOL GetPointFromScreenRay(float x,float y,Vector3f tar_point,Vector3f src_point);//���콺 ��ũ����ǥ�� �ѱ��.
	BOOL GetPointFromScreenRayFar(float x,float y,Vector3f tar_point,Vector3f src_point);//���콺 ��ũ����ǥ�� �ѱ��.
	BOOL IsCollisionFace(Vector3f src,Vector3f tar);		//�κ��ͻ��̿� �������� �浹�̵ǳ�? �ڷγ�...
	BOOL IsCollisionFace(Vector3f src,Vector3f tar,Vector3f *get,float plane_dist);	//������ ������ �Ÿ��� ����
	BOOL IsCollisionFace(Vector3f src,Vector3f tar,Vector3f *get_pos,Vector4f *get_normal);	//�浹�� ��ġ�� ����� �������͸� ���� 

	BOOL IsCollisionFaceForServer(Vector3f src,Vector3f tar);	//�κ��ͻ��̿� �������� �浹�̵ǳ�? �ڷγ�...
	BOOL IsCollisionFaceForServer(Vector3f src,Vector3f tar,Vector3f *get,float plane_dist);	//������ ������ �Ÿ��� ����

//	Vector3f *mVNormal;		//���ؽ� ����ε� ���Ͼȿ� ���� �ε��� �����Ѵ�.

	//	���İ��� API�� Ŭ���� ����.
	CAlpha mAlpha;
	void *GetDynamicVertexBuffer();
	_BSP_MAT_GROUP *GetMatGroup();

	float GetMatGroupPoint(WORD group_id,Vector3f get);	// get�� �ٿ���ڽ��� �߽�.�ٿ���ڽ� �����Ǳ��̸� �����Ѵ�.
	void RenderIndepentMatGroup(WORD mat_g_id);
	void CalcObjectLoop();		//������Ʈ���� ������ ������ ��ƾ�� �ִ� ��.

	BOOL IsInViewFrustum(WORD mat_id);  //�ش� ���͸��� ����� ���ÿ����� �ִ°�?

	void PrepareShadowRender(Vector3f ch_pos,void *tex,float intensity,DWORD blur=3,float scale=1,float add_pos=0);	// 
//	void DrawShadowRender(Vector3f ch_pos,void *tex,float intensity,DWORD blur=3,float scale=1,float add_pos=0);	
	void DrawShadowRender(Vector3f ch_pos);	// 
	//�����췻���� �����, ĳ���� ũ��, ������ �̵����� �ؽ��� �ȿ� ������ ũ��� �����.

	void DrawDynamicLightSub(Vector3f bb_min,Vector3f bb_max);
	void DrawMagicLightSub(Vector3f bb_min,Vector3f bb_max);
	void DrawDynamicLights();

public:
	void SaveExtBsp(char *name,_ADD_BSP_SAVE *add_bsp);	//ebp�� ����.
};



#endif
