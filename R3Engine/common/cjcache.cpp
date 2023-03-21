#include "CJcache.h"

#define SetCache(num)	(mCacheBitBuf[num>>3]|=(1<<(num&7)))
#define UnSetCache(num)	(mCacheBitBuf[num>>3]&=(~(1<<(num&7))))
#define IsCached(num)	(mCacheBitBuf[num>>3]&(1<<(num&7)))

void CJcache::CreateJcache(DWORD cache_num,DWORD struct_size)		//���ѵ� ĳ�� ������ �ʱ�ȭ�ؾ���.
{
	mCacheNum = cache_num;
	mStructSize = struct_size;
	mCacheBitBuf=(BYTE *)Dmalloc((cache_num/8+1)*sizeof(BYTE));
	memset(mCacheBitBuf,0,(cache_num/8+1)*sizeof(BYTE));

	mCacheBuf = (BYTE *)Dmalloc(cache_num*struct_size);
	memset(mCacheBuf,0,cache_num*struct_size);
	mContent = (_CJCACHE_CONTENT*)Dmalloc(sizeof(_CJCACHE_CONTENT)*cache_num);
	memset(mContent,0,sizeof(_CJCACHE_CONTENT)*cache_num);
}

DWORD CJcache::GetCacheLoopID()
{
	DWORD i,j;
	for(i=0; i<(mCacheNum/8+1); i++)
	{
		if( mCacheBitBuf[i] != 0xff )	//����� �ִ�.
		{
			for(j=0; j<8; j++)
			{
				if( !(mCacheBitBuf[i] & ( 1<<j )) ) //�����.
				{
					return (i*8+j);
					//return mCacheBuf[mStructSize*(i*8+j)];
				}
			}
		}
	}
	return -1;
}

void CJcache::DestroyJcache()
{
	if( mCacheBitBuf )
	{
		Dfree(mCacheBitBuf);
		mCacheBitBuf=NULL;
	}
	if( mCacheBuf )
	{
		Dfree(mCacheBuf);
		mCacheBuf=NULL;
	}
	if( mContent )
	{
		Dfree(mContent);
		mContent=NULL;
	}

	if( mCacheList.size() )
	{
		/*
		for( JCache::iterator i = mCacheList.begin(); i != mCacheList.end(); i++ )
		{
			i = mCacheList.erase( i );
		}*/
		mCacheList.clear();
	}
}

CJcache::CJcache()
{
	mCacheNum=0;
	mCacheBuf=NULL;
}
CJcache::~CJcache()
{
	if( mCacheBuf )
		Dfree(mCacheBuf);
}

LONG CJcache::HitCache(DWORD id,void **get_cache_ptr)	//ĳ���� ����ϴ� ��Ʈ, ���ϰ��� TRUE�ϰ�� ������ ĳ���� �Ŵ�.
{
	_CJCACHE_CONTENT *content;
	int j=0;

	for( JCache::iterator i = mCacheList.begin(); i != mCacheList.end(); i++,j++ )
	{
		content = *i;
		
		if( content->mID == id )
		{
			*get_cache_ptr = &mCacheBuf[mStructSize*content->mLoopID];
			mCacheList.erase(i);		//����� ���� �տ��� �д�.
			mCacheList.push_front( content );
			return _HITCACHE_RET_OLD;
		}
	}
	if( j >= mCacheNum )	//���� ��ģ�ٸ� ���� �����ȰŸ� �����Ѵ�. false�ϰ��� �ݵ�� ����� ����Ѵ�.
	{
		i=mCacheList.end();
		i--;
		content=*i;
		*get_cache_ptr = &mCacheBuf[mStructSize*content->mLoopID];

		content->mID = id;
		mCacheList.erase(i);		//����� �� ������ �س���.
//		mCacheList.push_back( content );
		mCacheList.push_front( content );
		return _HITCACHE_RET_DEL_AND_NEW;  //������� ���� �����ؾ���.
	}
	
	// ���ο� ���۸� ������. �����ؾߵȴ�.
	DWORD l_id = GetCacheLoopID();
	if( l_id ==-1 )
		Error("ĳ�� ����!","ġ������...");

	mContent[l_id].mID = id;
	mContent[l_id].mLoopID = l_id;
	SetCache(l_id);

	mCacheList.push_front( &mContent[l_id] );

	*get_cache_ptr = &mCacheBuf[mStructSize*l_id];
	memset(*get_cache_ptr,0,mStructSize);
	return _HITCACHE_RET_NEW;
}

BOOL CJcache::DelCache(DWORD id)						//ĳ���� ���۸� �����. ����ü ���� �͵��� ��������� GetCache���� ������� ���Լ� ȣ��.
{
	_CJCACHE_CONTENT *content;

	for( JCache::iterator i = mCacheList.begin(); i != mCacheList.end(); i++)
	{
		content = *i;
		
		if( content->mID == id )
		{
//			get_cache_ptr = &mCacheBuf[mStructSize*content->mLoopID];
			i = mCacheList.erase( i );
			UnSetCache(content->mLoopID);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CJcache::GetCache(DWORD id,void **get_cache_ptr)	//ĳ���� ���۸� ��´�. ������ FALSE�� ����
{
	_CJCACHE_CONTENT *content;

	for( JCache::iterator i = mCacheList.begin(); i != mCacheList.end(); i++)
	{
		content = *i;
		
		if( content->mID == id )
		{
			*get_cache_ptr = &mCacheBuf[mStructSize*content->mLoopID];
			return TRUE;
		}
	}
	return FALSE;
}


DWORD CJcache::GetCacheCnt()  //ĳ��ī��Ʈ����
{
	_CJCACHE_CONTENT *content;

	DWORD j=0;
	for( JCache::iterator i = mCacheList.begin(); i != mCacheList.end(); i++)
		j++;

	return j;
}


