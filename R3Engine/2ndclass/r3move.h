#ifndef __R3Move_H__
#define __R3Move_H__

#include "C_vector.h"
#include "R3D3d8.h"
//#define EXPORT extern "C" _declspec (dllexport)


class R3Camera{
private:
	Vector3f mPos,mPoint;	// 
	D3DXMATRIX mMatView;	//  ��ī�޶��� �� ��Ʈ������..
	float mTh;				//	ī�޶� ���Ʒ� ȸ��...	�ѹ����� 360.0 ���� 
	float mPh;				//  ī�޶� ����Ʈ�� �߽����� �¿� ȸ��...
	float mDist;			//  ī�޶��� ����Ʈ�� �� �Ÿ�...

	//auto ī�޶� ����.
	float mTime;	//�ð� ������ ����.
	float mOldPh;				//  ī�޶� ����Ʈ�� �߽����� �¿� ȸ��...
	float mPhDist;				//Ph�� �̵��� ����.
	float mPhSign;				//Ph�� ������ ���� ��ȣ.
	float mCharYangle;			//ĳ���� yangle���� �̿��Ѵ�.
	float mCharYangleForTime;	//ĳ���� yangle�� ���� �ð��� �����Ѵ�.
	//smooth ī�޶� ����
	float mPointSmooth,mRotSmooth;
	Vector3f mTarPoint;
	float mTarTh;
	float mTarPh;
	float mTarDist;
	void *mBsp;	//bsp������...
	void MakeViewMatrix(D3DXMATRIX *mat);
public:
	//-----�ܺο��� �����Ҽ��ִ� API
	R3Camera();
	~R3Camera();
	float GetTh();			//  ī�޶󿡼� �ʿ��Ҷ� ���ð� ��...
	float GetPh();
	float GetDist();
	void GetPoint(Vector3f get);	//float ���ϰ��� [3]�迭�̴�.
	void GetPos(Vector3f get);		//float [3]�迭�̴�.���������� pos��ǥ�� dist,th,ph������ �����ȴ�.
	D3DXMATRIX *GetViewMatrix();	//���Ʈ������ ����.

	void SetPos(float x,float y,float z);		//ī�޶� ��ġ��ǥ�� ������� �����Ѵ�...(����)
	void SetPoint(float x,float y,float z);		//����Ʈ��ǥ�� �����Ҽ��ִ�...
	void SetThPhDist(float th,float ph,float dist);	//ȸ���� �Ÿ��� ����..

	//���� ī�޶�.
	void SetAutoCameraThPh(float *th,float *ph,float ch_y_angle);	//�ڵ�ī�޶� ȸ���� ����..
	void SetAutoSmoothThPhDistLoop(float *th,float *ph,float dist);	//ȸ���� �Ÿ��� ����..�ݵ�� ��������
	//------Smooth ī�޶� ȸ��,�ʰ��� �浹�� ���Եȴ�.
	void SetSmoothValue(float point,float rot);	// 0-1 ����... ���ڰ� �������� ī�޶� �ε巯������.0�̸� ������ ��������.  
	void SetSmoothPointLoop(float x,float y,float z);			//����Ʈ��ǥ �ݵ�� �������� 
	void SetSmoothThPhDistLoop(float th,float ph,float dist);	//ȸ���� �Ÿ��� ����..�ݵ�� ��������
	
	void SetSmoothTarPoint(float x,float y,float z); //ī�޶� �̵��� Ÿ����ǥ ����...(����)
	void GetSmoothTarPoint(Vector3f get); //ī�޶� �̵��� Ÿ����ǥ ��� ..(����)
	void SetBspPtr(void *bsp);

	void MakeCameraAndViewMatrix();		//��� ���� �������Ŀ� �ѹ�ȣ�������ؾ��Ѵ�.  ���ο��� pos���� ���Ʈ������ �����Ѵ�.
	void SetViewMatrix();
};


/*
	ī�޶󾲴� �ΰ��� ���
	1. ī�޶� �ٱ����� �����Ϸ��� R3Camera Ŭ������ �̿��ؼ� �����Ѵ�.
	2. �Ʒ� ��ƿ��Ƽ�� �̿��Ѵ�. �� �������̽��� ����Ǿ��־� �ٱ������ǵ帮�� ���Ѵ�.
*/

//--- ī�޶� ��ƿ��Ƽ....����� �����Ӱ���
void R3FlyMoveSetPos(Vector3f pos);
void R3FlyMove(float m_speed,Vector3f player);
D3DXMATRIX *R3MoveGetViewMatrix();	//ī�޶󾴰��� ���´�.
void GetPointCamera(Vector3f *pos);


void R3SetCameraMatrix(Vector3f get_cam_pos,float cam_mat[4][4]);

#endif