#ifndef __CPATHFINDER_H__
#define __CPATHFINDER_H__

#include "C_Vector.h"
#include "windows.h"

//--�н� ���δ�.
#define _MAX_FIND_PATH	10	// �ִ� ���� ���.
#define _MAX_PATH_POS	16	//�ִ� �н� ����


typedef struct{
	float TotalLeng;
	Vector3f Path[_MAX_PATH_POS+1];
	int WhatDirection;	//��� ����.?
	WORD FrontLinetId;
	WORD BackLineId;
	int PathCnt;
	BOOL IsFind;	//ã�ҳ�?
}_PATH_NODE;


class CPathFinder{
public:
	int mMaxDepth;
	_PATH_NODE mPathNode[_MAX_FIND_PATH+2];
	Vector3f mStart;
	_PATH_NODE mStackPathNode[_MAX_FIND_PATH+10];
	int mStackPoint;
//	int stColFaceId;	//�浹 ���̽� ���̵�

	CPathFinder()					{	memset(this,0,sizeof(CPathFinder));	}

public:
	BOOL AddPath(Vector3f pos,int id);
	void CopyPath(int tar,int src)			{	memcpy(&mPathNode[tar],&mPathNode[src],sizeof(_PATH_NODE));	}
	void PushPathStack(int id);	//���������� �н��� ���ÿ� �ִ´�.
	void PopPathStack(int id);	//���������� �н��� ���ÿ� ������.

	void CompletePath(int id)				{		mPathNode[id].IsFind = TRUE;	}//�ѽ� ��ã�Ҵ�.
	int GetPathCnt(int id)					{		return mPathNode[id].PathCnt;	}
	void SetPathDirection(int id,int dir)	{		mPathNode[id].WhatDirection = dir;		}
	int GetPathDirection(int id)			{		return mPathNode[id].WhatDirection;	}
	void SetFrontLineId(int id,int l_id)	{		mPathNode[id].FrontLinetId = l_id;	}
	void SetBackLineId(int id,int l_id)		{		mPathNode[id].BackLineId = l_id;	}
	WORD GetFrontLineId(int id)				{		return mPathNode[id].FrontLinetId;	}
	WORD GetBackLineId(int id)				{		return mPathNode[id].BackLineId;	}
};

#endif
