#ifndef _LIGHTMAP_H__
#define _LIGHTMAP_H__

#include "world.h"

#include "R3BlockMat.h"


//#define _MAX_TEXTURE_SIDE	256
#define _MAX_TEXTURE_SIDE	2048
#define _FIT_TEXTURE_SIDE	256		//����Ʈ�� �⺻ ������...

class CGenLightMap{	//����Ʈ�� 1���� ���͵�...
private:

public:
	int mLEdge[_MAX_TEXTURE_SIDE];	// ���� �������̺�
	int mREdge[_MAX_TEXTURE_SIDE];	// ������ �������̺�
	int mIndex;
	int mCnt;		// �ϳ��� ����Ʈ�ʿ� ��� ���͸��� ��� ����Ʈ���� ���°�...
	int mBufSize;	//�������Ʈ�� ���ۻ����� ��������� �ƴ϶� �ܼ��� ���μ��λ������.

	CGenLightMap *mNext;	//��ũ�� ����Ʈ
	_LIGHT_MAP mLgtMap;	//����Ʈ�� ����ü..

	CGenLightMap();
	~CGenLightMap();
	void AdjustLightMap(_POLYFACE *face,DWORD *index,DWORD *face_index,int cnt);
	BOOL FillLightMap(_LIGHT_MAP *src,_POLYFACE *face,float *lu,float *lv);
};

CGenLightMap *GetLightmap(void);
int GetLightmapCnt(void);	//����Ʈ�� �Ѱ��� 

//void MakeLightmapsFromPatchs(_MESH *mesh,_MATERIAL *mat);
void MakeLightmapsFromPatchs(_MESH *mesh,_MATERIAL *mat,R3BlockBsp *blockmat,_MATERIAL *com_mat);
void DeleteLightmapsFromPatchs(_MESH *mesh);
void SaveLightMap(_MESH *mesh,_MATERIAL *com_mat);	//������ ���͸���� ����Ʈ���� ���� ����.

#endif