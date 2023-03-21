#ifndef __R3ALPHA_H__
#define __R3ALPHA_H__

#include <windows.h>
//#include "RevisitedRadix.h"
#include "C_vector.h"

#define _MAX_ALPHA_FACE		8192	//��ȭ�鿡�� �ִ�Ѹ��� �ִ� ����...

#define _ALPHA_TYPE_M_GROUP		0x80000000
#define _ALPHA_TYPE_CORONA		0x40000000
#define _ALPHA_TYPE_ENTITY		0x20000000

class CAlpha{
private:
//	RadixSorter mSortBase;	//��ƮŬ���� ����.
	DWORD mAlphaFaceCnt,*mAlphaFace;
	SHORT *mAlphaFaceZ;
	DWORD *mTempAlphaFace;
	SHORT *mTempAlphaFaceZ;
	int mAlphaSize;
	void *mBsp;
	void CheckAlphaAlloc(void);	//���� alloc
public:	
	CAlpha();
	~CAlpha();

	void InitAlpha(void* bsp);

	void LoopInitAlphaStack(void);	//���� ����Ʈ ���� �ʱ�ȭ
	DWORD GetAlphaFaceCnt();	//���� ���̽������� ����.
	DWORD *GetAlphaFace();		//���� ���̽����۸� ����.
	DWORD *SortAlphaStack(Vector3f camera);	//������ ���Ľ��ÿ� ���ΰ��� �����ѵ� ���ĵȹ��۸� �Ѱ��ش�.
	void SetAlphaStack(WORD group_id);
	
	void SetCoronaStack(WORD group_id);	//�ڷγ���..
	void SetAlphaEntityStack(WORD entities_id);//��ƼƼ��.
};




#endif