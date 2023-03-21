#ifndef __CJCACHE_H__
#define __CJCACHE_H__

#include <Windows.h>
#include <List>
#include "jmalloc.h"
#include "jerror.h"

/*
	Jcache ver 0.5
	���󵵿ʹ� ������� �� ĳ���� ���� �ֱٿ� ����ߴ� ���� �������� �켱���� �ִ� 
	������� ���Ǵ� ĳ���̴�.
	����ϰ� �ŷ������� ��� ���Ǵ°� ���ٴ� ��������ϴ� ���� ����.
*/

typedef struct{
	DWORD mID;	//���̵�..
	DWORD mLoopID;	//���� ���̵�..  i������..
//	DWORD mCnt;	//��� ī��Ʈ
}_CJCACHE_CONTENT;

using namespace std;
typedef list<_CJCACHE_CONTENT*>		JCache;



#define _HITCACHE_RET_NEW	1			//������ ��� ���� �����ؾߵȴ�.
#define _HITCACHE_RET_DEL_AND_NEW	0	//���� �Ѱ��ִ°Ÿ� �ϳ� ����� �ű⿡�� ���� �����ؾߵȴ�.
#define _HITCACHE_RET_OLD	-1			//������ �����ϹǷ� �ٷ� ����.

class CJcache{
public:
	CJcache();
	~CJcache();

	void CreateJcache(DWORD cache_num,DWORD struct_size);		//���ѵ� ĳ�� ������ �ʱ�ȭ�ؾ���. hitcache()���� id�� 0 ~ cache_num-1���� ���̴�.
	void DestroyJcache();	//��������� ���������� ����ϴ� ����ü���� malloc���� �˾Ƽ� Ŭ���� �ؾ���.

	BOOL GetCache(DWORD id,void **get_cache_ptr);	//ĳ���� ���۸� ��´�. ������ FALSE�� ����
	BOOL DelCache(DWORD id);						//ĳ���� ���۸� �����. ����ü ���� �͵��� ��������� GetCache���� ������� ���Լ� ȣ��.
	virtual LONG HitCache(DWORD id,void **get_cache_ptr);	//ĳ���� ����ϴ� ��Ʈ, ���ϰ��� _HITCACHE_RET ����.

	DWORD GetCacheCnt();	//ĳ��ī��Ʈ����

	void BeginCacheList();	//ĳ���� ����Ʈ�� ������� �Լ�.
	DWORD GetNextCache();	//Begin������ �������� ���ϰ��� ���̵�...  
	void EndCacheList();	//ĳ���� ����Ʈ�� ������� �Լ�.
private:
	BYTE *mCacheBuf;
	BYTE *mCacheBitBuf;	//��Ʈ �÷���ĳ��
	DWORD mCacheNum;	//ĳ�� ���� ����.
	DWORD mStructSize;	//�ʱ�ȭ�� �޾��ִ� ����ü������
	JCache mCacheList;
	_CJCACHE_CONTENT *mContent;
	DWORD GetCacheLoopID();
};

#endif