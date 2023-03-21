#ifndef __R3PARTICLE_H__
#define __R3PARTICLE_H__

#include "windows.h"
#include "c_vector.h"
#include "entity.h"

//------------��ƼŬ ������ҵ��� �ʹ��� ���� �⺻���ð����� ���� �Ѵ�.

#define _PARTICLE_TYPE_FALL			1	//�ܼ� �߶����� �����Ѵ�.
#define _PARTICLE_TYPE_SMOG			2	//����.
#define _PARTICLE_TYPE_EXPLOSION	3	//����Ÿ�ٿ��� �����Ѵ�.
#define _PARTICLE_TYPE_DRAGON		4	//���� ��.

//�� ��ƼŬ�� ���Ϸ� ����ȴ�.
typedef struct {
	char name[64];	//��ƼŬ ���� �̸�.
	DWORD mFlag;		//��ƼŬ�� �Ӽ�... 
	LONG mCreateArea[3];	//	�� ������ ���..[0]�� �迭�� ��������...�ִ�. �ڽ��� x,y,z 
	float mLiveTime;	//?�ʰ� ����ִ´�.
	float mStartGravity;
	float mEndGravity;
	Vector3f mPowerNormal;	//���� ����.
	float mTimeSpeed;
	BYTE mATrack[6];	//6���� ���� Ʈ��.
	BYTE mRTrack[6];	//6���� ���� Ʈ��.
	BYTE mGTrack[6];	//6���� ���� Ʈ��.
	BYTE mBTrack[6];	//6���� ���� Ʈ��.
	float mScaleTrack[6];	//6���� ���� Ʈ��.
}_PRE_PARTICLE_LIST;


//�� ��ƼŬ�� ���Ϸ� ����ȴ�.
#define _PARTICLE_SPARE_SIZE		127
typedef struct {
	char mEntityName[128];	//Attach�� ��ƼƼ �̸�.	
	DWORD mFlag;		//��ƼŬ�� �Ӽ�... 
	DWORD mNum;			//��ƼŬ�� ����... 
	DWORD mAlphaType;		//����Ÿ��... 
	LONG mCreateArea[3];	//	�� ������ ���..[0]�� �迭�� ��������...�ִ�. �ڽ��� x,y,z 
	float mLiveTime;	//?�ʰ� ����ִ´�.
	float mGravity[3];			//�߷�.
	Vector3f mPowerNormal;	//���� ����.
	float mTimeSpeed;
	BYTE mATrack[6];	//6���� ���� Ʈ��.
	BYTE mRTrack[6];	//6���� ���� Ʈ��.
	BYTE mGTrack[6];	//6���� ���� Ʈ��.
	BYTE mBTrack[6];	//6���� ���� Ʈ��.
	float mScaleTrack[6];	//6���� ���� Ʈ��.
	DWORD spare[_PARTICLE_SPARE_SIZE];
}_SAVE_PARTICLE;		

int GetPreParticleListNum();	//�̸� ��ġ�� ����ִ� ��ƼŬ ����Ʈ ���� 
_PRE_PARTICLE_LIST *GetPreParticleList(int id);	//�ش� ���̵��� ������ ����



//-------------------------------------------------------------------------------

#define _PARTICLE_ATTR_NO_LOOP			0x00000001	//������ �ȵǴ� ���. 
#define _PARTICLE_ATTR_ZWRITE			0x00000002	// z write�� �ϴ°�.. no zwrite�� �⺻�̴�.
//#define _PARTICLE_ATTR_ZSORT			0x00000004	//��ƼŬ�� ������ �ݵ�� �ʿ��Ѱ��.
#define _PARTICLE_ATTR_ZDISABLE			0x00000004	//Z �񱳸� �ƿ� ���Ѵ�.
#define _PARTICLE_ATTR_NO_BILLBOARD		0x00000008	//��ƼŬ�� ������ ��Ÿ���� �ƴϴ�.
#define _PARTICLE_ATTR_COLLISION		0x00000010	//��ƼŬ�� �浹�˻簡 �ʿ��Ѱ��.
#define _PARTICLE_ATTR_ALWAYS_LIVE		0x00000020	//�׻� �������.
#define _PARTICLE_ATTR_ENTITY_ANI		0x00000040	//��ƼƼ�� �ִϸ��̼��� ���.
#define _PARTICLE_ATTR_NO_AIR			0x00000080	//���Ⱘ�� ����.
#define _PARTICLE_ATTR_END				0x00000800	//��� ��.
#define _PARTICLE_ATTR_Y_BILLBOARD		0x00000400	//��ƼŬ�� Y ������.
#define _PARTICLE_ATTR_Z_BILLBOARD		0x00000200	//��ƼŬ�� Z ������.
#define _PARTICLE_ATTR_Z_ROT			0x00000100	//��ƼŬ�� Zȸ���� �ִ°��.
#define _PARTICLE_ATTR_FREE				0x00008000	//��ƼŬ ��Ʈ���� ������ �ƴ� �����ΰ��.
#define _PARTICLE_ATTR_ZFRONT			0x00004000	//��ƼŬ z���� �������ʿ䰡 �ִ°��.
#define _PARTICLE_ATTR_EMIT_TIME		0x00002000	//���� ����� �ð� ����.
#define _PARTICLE_ATTR_Y_ROT			0x00001000	//��ƼŬ�� yȸ���� �ִ°��.
#define _PARTICLE_ATTR_BOUNDED_BOX		0x00080000	//��ƼŬ�� �ڽ������� ���д�.
#define _PARTICLE_ATTR_SPECIAL_ID		0x00040000	//Ư���� ���� ��ƼŬ�̴�. ��� ��ƼŬ�� Ư����ƼŬ ���̵� ���� �����δ�.
#define _PARTICLE_ATTR_FLICKER			0x00020000	//���ۺ��� �����δ�.

#define _PARTICLE_ATTR_IS_NOT_BILLBOARD	0x00000608	// �����尡 �ƴѰ��..


#define _PARTICLE_ATTR_SPHERE			0x80000000	//�� ��Ÿ���� ������
#define _PARTICLE_ATTR_SPHERE_EDGE		0x40000000	//�� �׵θ� ������.


#define _ELEMENT_ATTR_FLICKER			0x01

#pragma	pack(push, 1)		//����Ʈ ����.
typedef struct{
//	float mVelosity[3];	//�� ��ƼŬ�� ���� �ӵ�.
	float mDirStep[3];	//�� ��ƼŬ ����.
	float mDir[3];		//�� ��ƼŬ ����.
	float mPos[3];
	float mTime;		//�������� ����ð�.

	float mARGBStep[4];	//�ʿ��Ѱ͵�.
	float mARGB[4];
	float mScaleStep;
	float mScale;
	float mZRotStep;
	float mZRot;
	float mYRotStep;
	float mYRot;

	BYTE mFlag;			//��� �÷���.
	BYTE mIsLive;
	WORD mNowTrack;		//���� ����� Ʈ�����̵� ó���� 0�� ī��Ų��.
	float mNowFrame;	//�ִϸ��̼ǵǴ°�...

	void UpDate(float time);//,float mTimeSpeed);
}_PARTICLE_ELEMENT;
#pragma	pack(pop)

#define _PARTICLE_STATE_STOP				0	//��ƼŬ ���� �� �����ִ� ��ƼŬ�� ����������� �����Ѵ�.
#define _PARTICLE_STATE_START				1	//��ƼŬ�� ������ �˸���.
#define _PARTICLE_STATE_START_ONE_ELEMENT	2	//������ ����ϳ��� ��������. �� ���� �������ִ� ��ü ���ں��� ������� ��������� ��Ҹ� ���ְ� �����Ѵ�.
#define _PARTICLE_STATE_ALWAYS_LIVE			3	//��ƼŬ�� �׻� ����ְ��Ϸ��� Ÿ�Ӱ����� �缳������� �ȴ�.

#define _PATICLE_MAX_TRACK	12

#define _TRACK_FLAG_ALPHA		0x80
#define _TRACK_FLAG_COLOR		0x40
#define _TRACK_FLAG_SCALE		0x20
#define _TRACK_FLAG_POWER		0x10
#define _TRACK_FLAG_ZROT		0x08
#define _TRACK_FLAG_YROT		0x04
#define _TRACK_FLAG_FLICKER		0x02


class CParticle{
public:
	char mEntityName[128];		//�� ��ƼŬ�� � ��ƼƼ�� �����ϴ°�...	//
	LONG mNum;					//��ƼŬ ������Ʈ �� ����
	_PARTICLE_ELEMENT *mElement;		//��ƼŬ �ϳ��ϳ� ���

	CParticle();
	~CParticle();

	void SetCreatePos(Vector3f pos)		{	Vector3fCopy(pos,mCreatePos);	};
	LONG Loop();			//��ƼŬ�� �ش� ����... ���ϰ��� ��ƼŬ�� ����ִ� ������.
	LONG RealLoop();		//���� ��ƼŬ�� ������.���ϰ��� ��ƼŬ�� ����ִ� ������.
	LONG SpecialLoop();		//Ư�� ��ƼŬ�� �ش� ����... ���ϰ��� -1�ΰ�� ���������� ����.
	LONG SpecialLoop2();		//Ư�� ��ƼŬ�� �ش� ����... ���ϰ��� -1�ΰ�� ���������� ����.
	void SetParticleState(DWORD state);	// ����,��,���� ����� ����.
	DWORD GetParticleState();			// ����,��,���� ���¸� �˾ƺ�
	
	void SetStartBoxArea();	//���۹ڽ��ȿ��� ��� ��ƼŬ�� ������ �ű��. //�� ��ó�� ȯ�濡�� ��������

	//���� ��ƼŬ�� ���� ��ƼŬ���� ����.
	void CopyParticleToSaveParticle(_SAVE_PARTICLE *s_particle);
	void CopySaveParticleToParticle(_SAVE_PARTICLE *s_particle);

	void SetParticleAttr(DWORD attr)	{	mFlag=attr;		};			//��ƼŬ �÷��״�.
	DWORD GetParticleAttr()				{	return mFlag;	};			//��ƼŬ �÷��״�.
	void SetPreCalcParticle(DWORD type = _PARTICLE_TYPE_FALL);	//������ �ݵ�� ���º������� �����ؾߵȴ�.

	BOOL LoadParticleSPT(char *name,DWORD option=0);
	void InitParticle();//��ƼŬ ����� ���� �ʱ�ȭ mNum������ �ʱ�ȭ �ȴ�.
	void ReInitParticle(int num);//����  ������ ���Ҵ� ��Ų��.  �ŷ������ٴ� ����...(malloc�� �ִ�.)
	void ReleaseParticle();//��ƼŬ ������ 

	CEntity *mEntity;	//��ƼŬ�� �����ϰ��ִ� ��ƼƼ
	void ReleaseEntity();	//Ŭ�е��� �������ϸ� ū�� ����.  Ŭ�� ����  ���������� �ʴ´�.
	void GetBBox(Vector3f get_min,Vector3f get_max);			//�ش���ƼŬ �׷��� �ٿ�� �ڽ��� ����Ѵ�.
	void GetFlickerARGB(int i,DWORD *dw_argb);	//�ø�Ŀ�� �ִٸ� �÷����� ���Ѵ�.

public:	//���� �ǵ�� ������... 
	float mLiveTime;			// ����ִ� �ð� �ʴ���
	float mStartTimeRange;		// 0�϶� ���̺�Ÿ�ӿ� �°� ������ 1�� �������� ��ƼŬ�� �Ѳ����� ���´�.
	float mTimeSpeed;			//�ð� ����.
	float mGravity[3];			//�߷�.
	float mStartPower[2][3];			//���������� ��ƼŬ�� �����̴� ���� ����.
	float mStartScale[2];		//������ ������ ���� ������ 
	float mStartZRot[2];		//���� Zrot�� 
	float mStartYRot[2];		//���� Yrot�� 
	BYTE  mFlickerAlpha;		//�ø�Ŀ�Ҷ� ������.
	float mFlickerTime;			//�ø�Ŀ �����ӽð�.
	float mStartARGB[4][2];		//������ ARGB
	float mOnePerTimeEpsilon;	//1�� ���� �ɶ� ��������... ���� 0�̸� ����������.  �ʴ��� �ʱ� �����
	float mRotMat[4][4];		//����ƼŬ ��ü ȸ�� ��Ʈ������.
	union{
	float mElasticity;			// ź����.... �浹�� �ε������ �ݹ߷��̴�.
	float mTexRepeat;			// �ؽ��� ����Ʈ��ġ... �浹���� ���Ƿ�..
	};
	float mZFront;				//ȭ������� �ѷ�����Ҷ� z ���� �ռ��׽�Ʈ... �浹�� �ε������ �ݹ߷��̴�.
	float mEmitTime;			//����ϴ� ���� �ð�.

//	float mAlphaGradient;		//����Ŭ����Ʈ 0-1���� 0�̸� ���ۺ��� ������� ����...
	WORD mSpecialID;			//Ư�����̵��.

	WORD mTrackCnt;	//Ʈ���� �ִ���.
	float mTimeTrack[_PATICLE_MAX_TRACK];	//x���� ���� Ʈ��.
	BYTE mTrackFlag[_PATICLE_MAX_TRACK];	//���� Ʈ���� ��������� �ִ°� �÷���.
	BYTE mATrack[_PATICLE_MAX_TRACK];		//x���� ���� Ʈ��.
	BYTE mRTrack[_PATICLE_MAX_TRACK][2];	//x���� ���� Ʈ��.
	BYTE mGTrack[_PATICLE_MAX_TRACK][2];	//x���� ���� Ʈ��.
	BYTE mBTrack[_PATICLE_MAX_TRACK][2];	//x���� ���� Ʈ��.
	float mScaleTrack[_PATICLE_MAX_TRACK][2];	//x���� ���� Ʈ��.
	float mZRotTrack[_PATICLE_MAX_TRACK][2];			// x���� ���� Ʈ��. Zrot�� 
	float mYRotTrack[_PATICLE_MAX_TRACK][2];			// x���� ���� Ʈ��. Zrot�� 
	Vector3f mPowerTrack[_PATICLE_MAX_TRACK][2];	//x���� ���� Ʈ��. ���� ����.
	Vector3f mSpecialARGV[2];						//Ư�� ����.

	void *mBsp;						//bsp������..
	void SetBspPtr(void *bsp)	{		mBsp = bsp;	};	//�浹�˻翡�� bsp �����͸� ����.
	DWORD mFlag;					//��ƼŬ�� �Ӽ�... 
	DWORD mAlphaType;				//��ƼŬ ���� Ÿ��.. 
//	LONG mCreateArea[3];			//	�� ������ ���..[0]�� �迭�� ��������...�ִ�. �ڽ��� x,y,z ������ �ڽ��� ���.
	float mStartPos[2][3];			// Vector3f  ��ƼŬ ���� ��ġ���� ���������� �׻�[2]�迭������.
	float mCreatePos[3];			// Vector3f  ��ƼŬ ���� ��ġ mCreatePos+mStartPos = world��ġ.
	void ResetOnePerTime();			//�ϳ��� ����� �ð� ����...

private:
	BOOL mState;					//��ƼŬ�� ���۹� ��Ÿ ����?
	
	float mOnePerTime;				//��ƼŬ 1�� �����Ǵ� �ð�...
	float mOnePerTimeEpsilonTemp;	//1�� ���� �ɶ� ��������...mOnePerTimeEpsilon���� ���ο��� ���� ��ġ..
	float mParticleTimer;			//��ƼŬ Ÿ�̸�.
	float mNextCreatorTime;			//���� ���� Ÿ��.

//	float mMass;					// ��ƼŬ ���� 

	void CheckCollision(int id,float time);	//��ƼŬ�� ���� ��ǥ�� �ش��ϴ� ����..
	void GetPartcleStep(int id,float time);	//��ƼŬ�� ���� ��ǥ�� �ش��ϴ� ����..
//	void GetPartcleAbsoluteTime(int id,float time);	//��ƼŬ�� ����Ÿ�ӿ� ������ǥ ���Ѵ�.
	void InitElement(int id,float time); //������Ʈ �ʱ�ȭ

};

float GetGravity( float num );     // 0�� �� ����	-�� �ö󰣴� +�� ��������.

//Get Snake Vertex list	������� ���� strip ���ؽ� ����Ʈ �̴�.  ���̴� �������� �׸��� ���̴�..
DWORD CalcSnakeVertexList(_D3DR3VERTEX_TEX1 *get_v,Vector3f *org_v,DWORD cnt,float f_length,DWORD color);


/*
	%����    ��ƼŬ �Լ� ȣ�� ������ ����...
	��ƼŬ Ŭ������ �Ѹ��� ��ƾ�� ����... ���� ��ǥ������ �������� ��Ÿ����.

	CParticle	test;
	test.SetParticleAttr(_PARTICLE_TYPE_FALL);
	test.InitParticle();
	test.SetParticleState(_PARTICLE_STATE_START);
	while(1)
	{
		-beginRender-
		test.Loop();	
		renderParticle();
		-endRender-
	}
	test.ReleaseParticle();
  */



#endif
