#ifndef _CSWORDTRACE_H__
#define _CSWORDTRACE_H__

#define _MAX_TRACE_CNT	60
#define _MAX_REAL_TRACE_CNT	10

#include "r3d3dtex.h"
#include "r3math.h"
#include "entity.h"


class CSwordTrace{
	int mCnt;	//
	int mQueue;		//�̰� ť��.	ť�� ���� ����Ʈ
	DWORD mColor;		//�ܻ��� �÷���.
	Vector3f mStartPos[_MAX_TRACE_CNT];
	Vector3f mEndPos[_MAX_TRACE_CNT];
	
	int mRealCnt;	//
	Vector3f mRealStartPos[_MAX_REAL_TRACE_CNT];	//���������� ���� ����..
	Vector3f mRealEndPos[_MAX_REAL_TRACE_CNT];		//���� ������ ���� ����.

	BOOL mIsTrace;
	LONG mIsRealTrace;
public:
	CSwordTrace();
	void SetTraceState(BOOL trace);	//Į���� ���� ����Ҷ� TRUE ������ FALSE..
	BOOL IsTrace();	//���� ������ �׸��� ������...
	BOOL IsDraw();	//���� ������ �׸��� ������...
	BOOL IsRealTrace() {	return (mIsRealTrace);	};	//���������� ���� Ʈ���̽��� ���������ΰ�?
	void InsertSwordTraceQueue(Vector3f s_pos,Vector3f e_pos);
	void UpdateSwordTrace();
	void SetTraceColor(DWORD color){ mColor = color; };
	void SetDisable()	{	mCnt=0;	};	//���� ������ �־ Ʈ���̽��� ���� ���� ��..
	CEntity *mEntity;	// �ؽ��ĸ� ���� ��ƼƼ�� ������ �־��.
};

#endif