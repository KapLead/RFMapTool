#ifndef __ANICAMERA_H__
#define __ANICAMERA_H__

#include "Animation.h"
#define _ANICAMERA_VERSION 1.2

typedef struct{
	char mName[64];		//ī�޶� �̸�.
	DWORD h_num;	//	hirerachy ����
	float fov;
	float tdist;
	_ANI_OBJECT *obj;
}_ANI_CAMERA;

#define _CAM_FLAG_LOOP			0x0
#define _CAM_FLAG_FINAL_STOP	0x1	//������ �������ӿ� �����Ѵ�.

#define _CAM_PLAY_FULL_FRAME 	-1

class CAniCamera{
private:
	_ANI_CAMERA *AniCamera;
	_ANI_OBJECT *mDummy;		//���̿�����Ʈ��..
	DWORD mDummyNum;
	DWORD mAniCameraNum;
	DWORD mStartFrame,mEndFrame;
	DWORD mPlayStartFrame,mPlayEndFrame;
	int mPlayIndex;		//�ī�޶� �����ϴ°�..
	BOOL mIsSetPerspect;
	float mStartTick;	//���� ƽ�̴�.
	DWORD mFlag;	//�÷��״�.

	float mOldFrame;	//���� �����Ӱ�.
	float mNowFrame;		//���� �������� ������.

	float *GetMatrixAniCamera(float frame); //�뷫 ī��Ʈ ��Ȱ...ī��Ʈ������ ī�޶� ��Ʈ������ ����� �����Ѵ�.
	void GetDummyMatrixSub(float mat[4][4],DWORD id,float frame);

public:
	CAniCamera();
	~CAniCamera();

	void GetDummyMatrix(float mat[4][4],DWORD id,float frame); //frmae������ ��Ʈ������ ����� �����Ѵ�.
	char *GetDummyParentID(DWORD id);
	DWORD GetDummyID(char *name,BOOL check_lwr=FALSE);	//�̸��� �ҹ��� ýũ��������. 

	DWORD GetStartFrame()	{	return mStartFrame;	};	//��ü ī�޶� ���� ������.
	DWORD GetEndFrame()		{	return mEndFrame;	};	//��ü ī�޶� �� ������ 

	void SetNowFrame( DWORD frame);
	float GetNowFrame()					{	return mNowFrame;	};		//���� �������� ������.
	float GetOldFrame()					{	return mOldFrame;	};		//���� ������.

	DWORD GetCameraNum()	{	return mAniCameraNum;	};	//�ϳ��� ���Ͽ� �������� ī�޶������� �����ϴ�.

	char *GetCameraName(DWORD id);	//ī�޶� �̸��� �����Ѵ�. ����ġ������ NULL
	DWORD GetCameraIndex(char *name);	//ī�޶� �ε����� �����Ѵ�. ����ġ������ -1

	BOOL IsLoadedAniCamera();
	void LoadAniCamera(char *name);
	void ReleaseAniCamera();

	void SetPrePlayCamera(DWORD id,DWORD start_frame=_CAM_PLAY_FULL_FRAME
		,DWORD end_frame=_CAM_PLAY_FULL_FRAME,DWORD flag=_CAM_FLAG_LOOP);
	BOOL PlayAniCamera(float matrix[4][4],float speed_scale=1.0f,BOOL pause=FALSE);		//������ �� �÷��̰� �������FALSE�� �����Ѵ�.(loop�ϰ�� �׻� true���ȴ�.)
};

#endif