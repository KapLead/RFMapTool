#ifndef _CMAGICSCRIPT_H__
#define _CMAGICSCRIPT_H__

#include <windows.h>
#include <stdio.h>
#include <jerror.h>
#include <jmalloc.h>
#include "Entity.h"

#define _MAX_MAGIC_TRACK 16
#define _SPECIAL_EFFECT_FLAG	0x8000000	//�����̳� ��ǳ�� ��Ÿ �ٸ� ȿ����...

//MAGIC TRACK FLAG
#define _MT_FLAG_Y_RANDOM		0x400	//yȸ�� ���� ����.
#define _MT_FLAG_Z_DISABLE		0x200	//z���� �Ⱦ���.
#define _MT_FLAG_MAGIC_LIGHT	0x100	//�������ΰ�? �������� ��� ���̳��� ����Ʈ �Ӽ��� ������.
#define _MT_FLAG_QUAKE			0x80
#define _MT_FLAG_BILLBOARD		0x40
#define _MT_FLAG_PARTICLE		0x20	//��ƼŬ�� ������ �ִ� Ʈ���̴�.
#define _MT_FLAG_DYNAMIC_LIGHT	0x10	//���̳��� ����Ʈ��..
#define _MT_FLAG_SKY_FADE		0x08	//�ϴ� ���̵���..
#define _MT_FLAG_TEX_FADE		0x04	//�ؽ��� ���̵���..
#define _MT_FLAG_ENTITY_FADE	0x02	//��ƼƼ ���̵���..
#define _MT_FLAG_CHECK_DAMAGE	0x01	//�浹 ýũ �ؾߵ� Ÿ�ֽ̹�...����Ʈ�� ��ȯ�ϱ�����..

#pragma	pack(push, 1)		//����Ʈ ����.


typedef struct{
	WORD Flag;				// �÷���.
	float Time;
	Vector3f Pos;			// ��Ʈ���� ��ġ.
	union{
		union{
		float Yangle;		//y����.
		WORD Yrand[2];
		};
	float QuakeTime;		// ���� �ð�...
	float SkyFadeTime;		// �̽ð����� �ϴ��� ��ο��� �������.
	float TexFadeTime;		// �̽ð����� ȭ�鿡 �ؽ��ĸ� ������� �ٽ� �����·εȴ�.
	float Rand1[1];
	};
	union{
	float AlphaDisapear;	//�ι����ϱ� �����ϱ�.. ��ü �����ӿ��� ���� ������� ������ ���Ѵ�. 0-1����. 0�� ���� ������ �����.
	float QuakeDensity;		//��������.
	};
	float AlphaApear;		//�ι����ϱ� �����ϱ�.. ��ü �����ӿ��� ���� ��Ÿ���� ������ ���Ѵ�. 0-1����. 0�� ���� ���ϰ� ��Ÿ��.
	DWORD EntityID;
	union{
	float Scale;
	float Rand2[1];
	};
	DWORD Color;
	DWORD DynamicLight;		//����������...LRGB  Length,Red,Green,Blue  �뷮 ����.^^
}_MAGIC_TRACK;

#pragma	pack(pop)


//�������� Ŭ���� 
class CProcessMagic{
public:
	WORD mFlag;			//���� �÷���...
	BYTE mState;		//����
	BYTE mCreateType;	// �����߻� ����.
	BOOL mIsLoaded;		//���Ϸε� �Ǿ���?
	DWORD mTrackNum;	//Ʈ�� ����.
	float mSpeed;		//���Ÿ� ���� �϶� ���ǵ��.
	Vector3f mCreatePos;		//���� ��� ��ġ��.
	_MAGIC_TRACK mMagicTrack[_MAX_MAGIC_TRACK];	//���� Ʈ��.

	BOOL IsLoaded(){ return mIsLoaded; };
	void CreateMagicTrack(char *name);
	void DestoryMagicTrack();
	// ���ϰ��� -1�ΰ�� ������ ��ƼƼ���̵� ����. 
	// -2�ΰ�� �������� Ʈ���� ��� ������ ������Ѵ�.
	// �׿ܿ� ��ƼƼ�� ������ ���ϵȴ�. 
// �迭 ���� ����� ��Ƽ� �־��ֱ�...
//	LONG GetEntityIDs(float old_frame,float now_frame,float *get_start_frame,_MAGIC_TRACK **get_magic_track);
	LONG GetEntityIDs(float old_frame,float now_frame,LONG *start_loop);
};

DWORD GetRealIDFromMagicSpt(DWORD id);	//�����Ⱦ��̵��� ���� �ε����� ����.
DWORD GetRealIDFromMagicSptSkipWarning(DWORD id);	//�����Ⱦ��̵��� ���� �ε����� ����.��� �ȶ��.

DWORD GetIDFromRealID(DWORD id);		//���� �ε����� �����Ⱦ��̵� ����.
DWORD GetIDFromMagicSpt(char *name);	//������ �����̸����� ���̵� ����.

//���� ���̵� �����ϴ��� ����
BOOL IsExistFromMagicSpt(DWORD id);	//�����Ⱦ��̵� ��������Ʈ�� �ִ°�..

char *GetFilenameFromMagicSpt(DWORD id);	//�����Ⱦ��̵��� ���� �̸��� ����.
void CreateMagicSptList(char *name);
void DestroyMagicSptList();
float GetRandNum(float a,float b);

DWORD GetMagicSptNum();
DWORD GetAllMemorySizeMagic();	//���� �뷫 ���Ǵ� ���� �޸� ��. 


#endif