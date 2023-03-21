#ifndef __EXTDUMMY_H__
#define __EXTDUMMY_H__

#define _DUMMY_FLAG_MUSIC	0x1	//���ǿ�.
#define _DUMMY_FLAG_FOG		0x2	//���׿�.

typedef struct{
	char mName[32];
	DWORD mFlag;
	Vector3f mBBmin,mBBmax;
	DWORD mID;		//Ư���� �κ��� ���� ���� ���̵�.
	union {
	DWORD mdwArgv[4];
	float mfArgv[1];
	};
	float mMat[4][4];
	float mInvMat[4][4];
}_EXT_DUMMY;

class CExtDummy{
private:
	DWORD mMaxNum;
	DWORD mNum;
	_EXT_DUMMY *mDummy;
public:
	CExtDummy();
	~CExtDummy();
	_EXT_DUMMY *GetDummy( DWORD index );
	BOOL LoadExtDummy(char *name);
	void ReleaseExtDummy();
	BOOL IsInBBox(DWORD index,Vector3f pos);				//pos ��ǥ�� �ش���� �ε����ȿ� �ִ°�?
	//ȸ�� ���� ���� ���̾��� local_pos��ǥ�� �ش� �ε����� �Ѱ��ָ� ������� ��ǥ get_pos�� ����.
	BOOL GetWorldFromLocal(Vector3f *get_pos,DWORD i,Vector3f local_pos);	
	//���� ���� ��ǥ�� �Ѱ��ָ� ���̾��� local_pos��ǥ�� ����.
	BOOL GetLocalFromWorld(Vector3f *get_pos,DWORD i,Vector3f world_pos);	

	//�ش� �÷��׸� ������ �ִ� ���̵� ������ �ε��� ����Ʈ�� ����.
	void GetDummyList(DWORD flag,DWORD *cnt,DWORD *index_list);	
	DWORD GetTotalNum()		{	return mNum;	};

	//���̹ڽ� ������.
	void DrawDummyBBox(DWORD id);
	void DrawAllDummyBBox();
};

#endif