#ifndef __EFFEFTCORRE_H__
#define __EFFEFTCORRE_H__

#include <windows.h>
#include "C_vector.h"
#include "entity.h"
#include "CMagicScript.h"
#include "../global.h"

#define _MAX_MAGIC_ID_STACK 16	//ȭ�� ���÷��̿�. �������̵��ð���.





//����Ʈ ����Ʈ
class CEffectList{
public:
	DWORD mState;		//���´�. _EF_STATE_AUTO_DISAPEAR�ϰ�� �ڵ����� �ִϸ��̼ǿ� ���� ������Եȴ�.
	DWORD mColor;		//�÷���.
	float mScale;		//�������̴�.
	float mMat[4][4];	//�ش�Ǵ� ȸ���� �������� �� ��Ʈ������.
	float mDisapearFrame;	//������� �����Ҷ� ������.
	float mFrame;
	union{
	float mAlphaDisapear;	// ��ü �����ӿ��� ���� ������� ������ ���Ѵ�. 0-1����. 1�� ���� ������ �����.
	float mDisapearTime;
	};
	float mAlphaApear;		// ��ü �����ӿ��� ���� ��Ÿ���� ������ ���Ѵ�. 0-1����. 1�� ���� ������ �����.
	DWORD mEffectID;	//����Ʈ ���̵��..
	DWORD mUniqueID;	//����ũ ���̵��..
	DWORD mFlag;		//��ƼŬ�ΰ�?�Ǵ� �������ΰ�.? �÷��״� ������ũ��Ʈ�� ���󰣴�. _MT_FLAG_PARTICLE

	DWORD mSpecialID;			//������� ��쿡 ����Ѵ�.
	Vector3f mVectorArgv1,mVectorArgv2;

	union{
	CEntity *mEntity;	//�̰Ŵ� �����ͷ� �ذ�����...
	CParticle *mParticle;	//��ƼŬ�̴�.
	};
};
//���� Ŭ������ 
class CMagicList{
public:
	DWORD mState;		//���´�. _EF_STATE_AUTO_DISAPEAR�ϰ�� �ڵ����� �ִϸ��̼ǿ� ���� ������Եȴ�.
	DWORD mEntityUniqueIDCnt;	//���� Ʈ���� ������ �ִ� ��ƼƼ�� ��Ʈ���ϱ�����.
	DWORD mEntityUniqueID[_MAX_MAGIC_TRACK];	//���� Ʈ���� ������ �ִ� ��ƼƼ�� ��Ʈ���ϱ�����.
	float mMat[4][4];	//���� ���� ��ġ�� ȸ��...
	float mOldFrame;	//���� �������� ����ϰ� �־�� �ش� �������� ���´�.
	float mFrame;		//���� ������.
	DWORD mMagicID;		//����Ʈ ���̵��..
	DWORD mUniqueID;		//���� ���̵��..
	void *mMagicPtr;	//���� ������..

	DWORD mCharTypeID;			// ĳ���� Ÿ��
	DWORD mCharIndex;			// ���� ������ ���̺��� ���ڵ� �ε���

	DWORD mSpecialID;			//������� ��쿡 ����Ѵ�.
	Vector3f mVectorArgv1,mVectorArgv2;

	EffectInfo mEInfo;	//����Ʈ ���� �����Ұ�츦 ����ؼ�.
	// �׶��� �����۶����� �ϴ� �����ִ°Ŵ�.	���߿� ���ٰ�.
	float mYOldAngle;		//y ���� ..
	float mYAngle;		//y ���� ..
	DWORD mYAngleCnt;		//y ���� ..	
};

//���� �Ѹ��� ����Ʈ ��Ȳ�� �˷��� �Ѵٸ�.
CEffectList* GetEffectList();
DWORD GetEffectListCnt();
CMagicList* GetMagicList();
DWORD GetMagicListCnt();
void DeleteMagicList(DWORD id);	//�̰��� ���� �ؾ� �Ѵ�.

void SpecialARGV1(Vector3f pos);		//Ư���� ���ڸ� ���η� �����Ҷ�ȣ���Ѵ�.
void GetSpecialARGV1(Vector3f pos);	
void SpecialARGV2(Vector3f pos);		//Ư���� ���ڸ� ���η� �����Ҷ�ȣ���Ѵ�.
void GetSpecialARGV2(Vector3f pos);	
BOOL SetSpecialIDFromMagic(DWORD unique_id,DWORD special_id);
DWORD GetSpecialIDFromMagic(DWORD unique_id);
BOOL CopySpecialIDFromMagic(DWORD unique_id);



//--------------------------------------------------------------------------
//���� �̿ϼ��� �κ��� ��ƼƼ�κ��� ĳ���� ��������ĳ������ 
//�׸��� MoveMatrixMagicEffect()�Լ��κ��� �ʿ��ϴ�.
//--------------------------------------------------------------------------


//����Ʈ�� ������ �Ӽ�.
#define _EF_STATE_AUTO_DISAPEAR		0	//�ڵ����� ������� �����Ѵ�.(�ִϸ��̼ǿ� ����Ѵ�.) ��� ����Ʈ�� ����Ʈ�� �̰��� ����.
#define _EF_STATE_ALWAYS_LIVE		1	//�׻� ��� �ִ´�.
#define _EF_STATE_START_DISAPEAR	2	//�׻� ����ִٰ� �̰ž��� �׶����� ������� �����Ѵ�. 
#define _EF_STATE_DEATH				3	//��� �׿����°Ŵ�. 
#define _EF_STATE_MYSELF_LIVE		4	//������ ��� �ִ°��. (������ ��� �����鼭 �������ߵǴ°��.)

#define _GROUND_ITEM_FLAG	0xf0000000
//#define _TRACE_FLAG			0x10000000

//------------------------ ����Ʈ �⺻���� ���� �Լ�.
/* ��� ����Ʈ�� ������ �Լ��ѹ� ȣ���ϱ⸸ �ϸ� �Ҹ�ɶ����� �ڵ����� �۵��Ǵ� ������.
������ �����Ӹ��� �����̰� �ǰų� ��ġ�� �ٲ�� ���( ȭ���) ��Ư������ �Ҹ��̵Ǵ� ����
���ϰ��� DWORD���� unique_id �� ���Ϲ޾� ������ ��ǥ�� MovePosEffect�Լ��� ����ؼ� �Ű��ش�.
*/

DWORD GetNextUniqueID();	//  StartEffect�� unique_id�� ������ ���Լ� ���ϰ��� �־��ش�.

//���ϰ���  unique id���̴�. �������Ӹ��� ��ǥ�� �ٲܰ�� unique id�� �����س��� ��ǥ�� �ٲ۴�.
void StartEffect(Vector3f pos,float y_angle, DWORD effect_id,DWORD state=_EF_STATE_AUTO_DISAPEAR
	,DWORD color=0xffffffff,float scale=1.0f,DWORD flag=0,float frame=0,DWORD unique_id=0);//�ܼ� ����Ʈ�� ������ġ�� ���̵�� �̰͵� ��.
void StartEffect(float mat[4][4], DWORD effect_id,DWORD state=_EF_STATE_AUTO_DISAPEAR
	,DWORD color=0xffffffff,float scale=1.0f,DWORD flag=0,float frame=0,DWORD unique_id=0);//�ܼ� ����Ʈ�� ������ġ�� ���̵�� �̰͵� ��.

//������ ������ġ�� �������̵�� ������ �˸��� ���� ��. �������Ӹ��� ��ǥ�� �ٲܰ�� ���ϰ��� unique id�� ����
DWORD StartMagicEffect(Vector3f pos,float y_angle, DWORD magic_id,void *e_Info=NULL,DWORD state=_EF_STATE_AUTO_DISAPEAR);
DWORD StartMagicEffect(float mat[4][4], DWORD magic_id,void *e_Info=NULL,DWORD state=_EF_STATE_AUTO_DISAPEAR);

//ĳ���� ���̵� ����Ѵ�. Ư���� ��Ȳ�̸� ���߿� ���ֱ� ����.
BOOL SetAttachCharMagicEffect(DWORD unique_id,DWORD char_type,DWORD char_index);

BOOL DelMagicEffect(DWORD magic_id);//ĳ���� �ִ� ������ �����. Tool������ ���°�..


void CreateEffectsList(char *filename);		//����Ʈ�� �ʱ�ȭ.
void DestroyEffectsList();		//����Ʈ�� ������
void CreateMagicSptList(char *name);	//������ũ��Ʈ ����Ʈ�ʱ�ȭ 
void DestroyMagicSptList();		//���� ��ũ��Ʈ ����Ʈ ���ֱ�

void FrameMoveAllEffectsList();	//�����ӹ��� ��Ʈ
void RenderAllEffectsList();		//������Ʈ

void DestroyEffectsCache();		//ĳ������ ��� ����Ʈ���� ������ ��Ų��. ���ο� �����ε��� �ʿ�.
void ResetEffectManager();	//����Ʈ �޴������� �����Ѵ�.


//--------------------- ����Ʈ �������� ���� �Լ� ----------------
/*
BOOL SetMagicEffectState(DWORD magic_id,DWORD state);	//��������Ʈ ���� �����Ѵ�.
DWORD GetMagicEffectState(DWORD magic_id);	//��������Ʈ ���¸� ���´�.
*/

//BOOL SetMagicEffectState(DWORD magic_id,DWORD state);	//��������Ʈ ���� �����Ѵ�.

//���� ���� ���� �Լ�. ���ϰ��� �ش� unique_id�� ���� ���.
BOOL SetMagicState(DWORD unique_id,DWORD state);

//����Ʈ ���� ���� �Լ�. ���ϰ��� �ش� unique_id�� ���� ���.
BOOL SetEffectState(DWORD unique_id,DWORD state);

//�������� ������� �����Ҷ� ����Ѵ�. �ð������� ��...
BOOL SetDisapearTimeEffect(DWORD unique_id,float time);
BOOL SetDisapearTimeMagicEffect(DWORD unique_id,float time);

//Ư�� ����Ʈ���� �����϶� ����Ѵ�.  ȭ���̳� ��Ÿ �׷��͵�...
BOOL MoveMatrixEffect(DWORD unique_id,float mat[4][4]);	//������ ���о��̵��� ����Ʈ�� �����Ѵ�.
BOOL MovePosEffect(DWORD unique_id,Vector3f pos,float y_angle=-1);	//������ ���о��̵��� ����Ʈ�� �����Ѵ�.

BOOL MovePosMagicEffect(DWORD unique_id,Vector3f pos,float y_angle=-1);	//������ ���о��̵��� ����Ʈ�� �����Ѵ�.
BOOL MoveMatrixMagicEffect(DWORD unique_id,float matrix[4][4]);	//������ ���о��̵��� ����Ʈ�� �����Ѵ�.

BOOL GetMagicCTRLPoint012(DWORD unique_id,Vector3f *get_pos);	//���̾� Ŀ�� 4���� ������(Ÿ��) ���� �����ϰ� �־��ش�. 
_MAGIC_TRACK *GetMagicTrack(DWORD unique_id);
CProcessMagic *GetMagic(DWORD unique_id);
CMagicList* GetMagicListFromID(DWORD unique_id);
void SetCurveMagicTrackRandom(_MAGIC_TRACK *mtrack);	//���� �������� ���� Ŀ�긦 �������� ����Ѵ�.

DWORD *GetMagicIDList();

void SetBspPtr(void *bsp);

DWORD GetEffectCacheSize();			//���� ���ǰ��մ� ����Ʈ ĳ�� ����� �˾Ƴ���.
DWORD GetEffectCacheTotalNum();		//���� ���ǰ��մ� ����Ʈ ĳ�� ������ �˾Ƴ���.
DWORD GetEffectCacheNum();			//���� ����Ʈ ĳ�� �� ������ �˾Ƴ���.

DWORD GetMagicSptNum();
DWORD GetAllMemorySizeMagic();	//���� �뷫 ���Ǵ� ���� �޸� ��. 


#endif