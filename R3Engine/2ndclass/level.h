#ifndef __LEVEL_H__
#define __LEVEL_H__

//#include "C_Vector.h"
#include "R3Bsp.h"
#include "SkyBox.h"
#include "R3Move.h"
#include "AniCamera.h"
#include "ExtDummy.h"

//----------------------------------Ÿ�̸� Ŭ����: �ʴ� �ϳ��� ������..----------------------------
typedef unsigned __int64 UINT64;
class CTimer{
private:
	//------ ���� ���� Ÿ�̸� 
	float mLoopTime,mTime,mRealTime;
	float mMinFPS,mfLoopHop;
	DWORD mOldTime;
	DWORD mLoopCnt;
	DWORD mLoopHop;
	BOOL m_bUsingQPF;
	BOOL m_bTimerInitialized;
	float mFPS;
	DWORD mLoopFPSCnt;
	float mFPSTime;

	float m_fTicksPerSec;
	float m_fFramesPerSec;
	float m_fAverageFramesPerSec;
	float m_fSecsPerFrame;
	float m_fLamTime;

	UINT64 m_qwTicks;
	UINT64 m_qwStartTicks;
	UINT64 m_qwTicksPerSec;
	UINT64 m_qwTicksPerFrame;

	float GetDuration();
	UINT64 GetTicks();
public:
	CTimer();
	float GetLoopTime();	//�ѷ����� ���� �ð��� ������.
	float GetTime();				//���� �ð��� �˾Ƴ���.
	void SetTime(float time);	//Ÿ���� �� ����...
	void SetMinFPS(float min_fps);	//�ּ� ������ ����Ʈ�� �����Ѵ�.
	void CalculateTime();	//�������� ����ߵȴ�.
	float GetFPS()		{	return mFPS;	};

};


class CLevel{
private:
	char mMapName[256];		//������ ���̸�...
	Vector3f mCamPos;			//ī�޶� ��ġ��ǥ.
	D3DXMATRIX mMatView;	//���� �������ɷ��� ī�޶��� �� ��Ʈ����.. 
	BOOL mIsLoadedBsp;
public:

	CBsp	*mBsp;	//�ش� ������ �� bsp
	CSkyBox	*mSkyBox;	//�ش� ������ ��ī�̹ڽ�
	CAniCamera mAutoAniCam;		//�⺻ �ڵ� ī�޶��̴�.	�Ű澲�� ���ñ�..
	CTimer	mTimer;				//���� Ÿ�̸�....
	CExtDummy mDummy;			//���̴�.

	//----���� ���� �ؽ��� �޸𸮻�����...
	DWORD mLightTexMemSize;
	DWORD mMapTexMemSize;
	DWORD mSkyTexMemSize;
	DWORD mEntityTexMemSize;

	CLevel();
	virtual ~CLevel();

	void LoadLevel(char *name=NULL);		//������ ���Ϸ� �д´� NULL�ϰ������ mMapName�� �����ؼ� �д´�.
	void ReleaseLevel();

	BOOL IsLoadedBsp() const { return mIsLoadedBsp; } //�������� �ε��Ǿ��°�?
	void CalcR3Fog();						//���׸� ����Ѵ�.
	
	void FrameMove();						//�� �������ϱ����� �ݵ�� ȣ���ϴ°�.. �ʷ��� �����غ��̴�.

	void DrawSkyBoxRender();				//�ϴ��� �������Ѵ�.
	void DrawMapRender();					//���� �������Ѵ�.
	void DrawMapEntitiesRender();			//�ʾ��� ��ƼƼ���� �������Ѵ�.
	void DrawMapAlphaRender(Vector3f pos);	//�ʿ��� ���� �κ��� �������Ѵ�.
	void DrawLeafBBox();	//������ �ٿ�� �ڽ��� ���.
	void DrawMatBBox();		//���͸����� �ٿ�� �ڽ� ���.

	void PrepareShadowRender(Vector3f ch_pos,void *tex,float intensity,DWORD blur=3,float scale=1,float add_pos=0);	// 
	void DrawShadowRender(Vector3f ch_pos);

	void HearMapSound();	//�� ���带 ����.(�������� �ƴϹǷ� endscene()���Ŀ� ����..)

	//---- �ʷ��� �ϱ����� �Ʒ�2���� ����������Ѵ�.
	void SetCameraPos(Vector3f camera);	
	void SetViewMatrix(D3DXMATRIX *view_mat);	// ī�޶� �� �̿��ϰ� �ܺο��� ���������ϴ� ���..

	//-------�� ��ƿ�� ���Ѱ�... 
	BOOL GetPointFromScreenRay(LONG x,LONG y,Vector3f *get_point);	//���콺Ŭ���� ������ ����Ʈ�� ����.
	float GetFirstYpos(Vector3f pos,int option);	// ó�� y�·Ḧ 
	float GetFirstYpos(Vector3f pos,Vector3s min,Vector3s max);	//�ٿ�� �ڽ� �����ȿ��� ���Ѵ�.���Ҽ� ���°�� 65535�� ����
	float GetFirstYpos(Vector3f pos,Vector3f min,Vector3f max);	//	�ٿ�� �ڽ��������� y���� ���´�.
	BOOL GetNextYpos(Vector3f tpos,float *ypos);	//������ġ�� y��ǥ��..	
	BOOL GetNextYposNoAttr(Vector3f tpos,float *ypos);	//������ġ�� y��ǥ��..	

	BOOL GetNextYposFarProgress(Vector3f src_pos,Vector3f tar_pos,float *ypos);
	
	//Ÿ�̸�üũ�ؼ� 
	//�������� �ȳ��ð�� �÷��̾ ���߿� �߰ų� ������ �ʰ� �ϱ����� �Լ�.
	BOOL GetNextYposFar(Vector3f src_pos,Vector3f tar_pos,float *ypos);

	BOOL GetNextYposForServer(Vector3f tpos,float *ypos);	//������ġ�� y��ǥ��..	���������� ����. ��������� ������ �����Ƿ� �ӵ� ����.
	BOOL GetNextYposForServerFar(Vector3f src_pos,Vector3f tar_pos,float *ypos);//�������� �����ð��� ȣ���ϴ°Ŵ�. 
/*
	GetNextYposForServerFar()�Լ�������...
	���������� Ypos�� �ӵ����������� �ŷ������ٴ� ĳ���� Ű(����18)�� ����� ���� �Ÿ����� ���Լ��� 
	����ϴ� ���� ������. 
	���� ��� ���Ͱ� 1�ʿ� 40���� �����δٸ� 18�̶� �Ÿ��� 0.45�ʸ��� ���Ƿ� 0.45�� ���� ypos�� �����ϸ� 
	�ǰڴ�. ������ ���� ��Ȯ�ؾߵ� �ʿ䰡 �����Ƿ� ������ ��0.2�������� ��������..
*/
	BOOL IsCollisionRayAABB(LONG x,LONG y,Vector3f BBmin,Vector3f BBmax, Vector3f *get_cross);	//�ٿ�� �ڽ����� �浹..

	void GetFrustumNormalPlane(Vector4f n_plane[6]);	//core�� view frustum ������� �����Ѵ�.

	//��Ÿ ��ƿ...
	void DrawBBox(Vector3f bb_min,Vector3f bb_max,DWORD color);	//�ٿ�� �ڽ��� �������� �׸��� �Լ�.
	void DrawBBox(Vector3s bb_min,Vector3s bb_max,DWORD color);	//�ٿ�� �ڽ��� �������� �׸��� �Լ�.
	void DrawTestBox(Vector3f box_size,Vector3f pos,DWORD color);		//����׽�Ʈ�� �ڽ���ƿ...
	void ScreenShot();
	void ReLoadAllMaterial();	//���͸��󰪸� ��ε��ϴ°�..
	char *GetMapName();		//���� ���̸��� �˾Ƴ���.


	DWORD GetPath(Vector3f from,Vector3f at,Vector3f get[16],DWORD *cnt,int depth=_MAX_PATH_POS);	//�н� ���δ�.
	BOOL GetPathFromDepth(Vector3f from,Vector3f at,int depth,Vector3f get[16],DWORD *cnt);	//�н� ���δ�.
	//���ϰ����� �н��� ��ã����� FALSE�� ���������� �ִ� �����ִ� ��ǥ�� �־��ش�.

	//------ �������� �� API 

	BOOL GetPointFromScreenRayFar(LONG x,LONG y,Vector3f *get_point);	//��ŷ�� ���� �ָ��ִ� �������� ����.
};


#endif