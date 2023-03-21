#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include <windows.h>
#include "R3material.h"
#include "entity.h"



class CSkyBox{
private:
	BOOL mIsSkyLoad;
/*
	_ENTITY_FILE_HEADER mHeader;
 	int mVetexBufferSize;	//�� ���ؽ����� ������
	void* mStaticVertexBuffer;	//�������ؽ� ���� 
	void* mStaticIndexedBuffer;	//�����ε��� ���� 
//	BYTE *mStaticAlloc;		//���� �Ҵ� (���ϻ� �״�� �޸� �ø� ��)
	_ENTITY_M_GROUP *mMatGroup;
	Vector2f *mOrgUV;
*/
	CEntity	mEntity;				//��ī�̹ڽ� ��ƼƼ 
	_ENTITY_M_GROUP *mMatGroup;		//��ƼƼ�Ÿ� �޾ƿ� ������.
	int mMatNum;				//���͸��� ����.
	int mMatGroupNum;
	short mMatGroupSort[512];		//���͸��� �׷��� ������� �ѷ��ֱ� ���� ����.
public:
	CSkyBox::CSkyBox();
	CSkyBox::~CSkyBox();

	void LoadSkyBox(char *filename);
	void ReleaseSkyBox();
//d3dmatrix�� ���ڷ� �Ѱ�����Ѵ�. ���� R3Render.cpp��  R3D3d8������ 
//���̷�Ʈ 3D�� �ٷ���� �ϱ⶧���� ���ڰ� �ʿ��ϴ�.
	void DrawSkyBox(void *view_mat);	
	int GetUsedVertexBufferSize();
};


									


#endif