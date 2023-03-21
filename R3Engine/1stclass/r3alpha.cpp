#include "R3Render.h"
#include "R3Alpha.h"
#include "jmalloc.h"
#include "jerror.h"
#include "R3Bsp.h"
#include "b_sort.h"

CAlpha::CAlpha()
{
	mAlphaFace=(DWORD *)Dmalloc(_MAX_ALPHA_FACE*sizeof(DWORD));
//	mAlphaFaceZ=(float *)Dmalloc(_MAX_ALPHA_FACE*sizeof(float));
	mAlphaFaceZ=(SHORT *)Dmalloc(_MAX_ALPHA_FACE*sizeof(SHORT));

	mTempAlphaFace=(DWORD *)Dmalloc(_MAX_ALPHA_FACE*sizeof(DWORD));
	mTempAlphaFaceZ=(SHORT *)Dmalloc(_MAX_ALPHA_FACE*sizeof(SHORT));
	
	mAlphaFaceCnt = 0;
	mAlphaSize=_MAX_ALPHA_FACE;
}

CAlpha::~CAlpha()
{
	Dfree(mTempAlphaFaceZ);
	Dfree(mTempAlphaFace);
	Dfree(mAlphaFaceZ);
	Dfree(mAlphaFace);
}

void CAlpha::InitAlpha(void* bsp)
{
	mBsp = bsp;
}


void CAlpha::LoopInitAlphaStack(void)	//���� ����Ʈ ���� �ʱ�ȭ
{
	mAlphaFaceCnt = 0;
}

void CAlpha::CheckAlphaAlloc(void)	//���� alloc
{
	if(mAlphaFaceCnt>=(DWORD)mAlphaSize)//_MAX_ALPHA_FACE)
	{
		int Size =mAlphaSize/2+mAlphaSize;
		mAlphaFace=(DWORD *)ReAlloc(mAlphaFace,mAlphaSize*sizeof(DWORD),Size*sizeof(DWORD));
		mAlphaFaceZ=(SHORT *)ReAlloc(mAlphaFaceZ,mAlphaSize*sizeof(SHORT),Size*sizeof(SHORT));
		mTempAlphaFace=(DWORD *)ReAlloc(mTempAlphaFace,mAlphaSize*sizeof(DWORD),Size*sizeof(DWORD));
		mTempAlphaFaceZ=(SHORT *)ReAlloc(mTempAlphaFaceZ,mAlphaSize*sizeof(SHORT),Size*sizeof(SHORT));
		mAlphaSize=Size;
	}
}


void CAlpha::SetAlphaStack(WORD group_id)	//�ʿ����� ����ƽ���� ������.
{
	mAlphaFace[mAlphaFaceCnt++]=_ALPHA_TYPE_M_GROUP|group_id;
	CheckAlphaAlloc();
}

DWORD CAlpha::GetAlphaFaceCnt()
{
	return mAlphaFaceCnt;
}
DWORD *CAlpha::GetAlphaFace()
{
	return mAlphaFace;
}

DWORD *CAlpha::SortAlphaStack(Vector3f camera)	//������ ���Ľ��ÿ� ���ΰ��� �����ѵ� ���ĵȹ��۸� �Ѱ��ش�.
{
	DWORD i;
	CBsp *bsp = (CBsp *)mBsp;
	Vector3f total;

	for(i=0; i<mAlphaFaceCnt; i++)
	{
		if( mAlphaFace[i]&_ALPHA_TYPE_ENTITY)
		{
			Vector3fCopy(bsp->mMapEntitiesList[(mAlphaFace[i]&0x0000ffff)].Pos,total);
		}
		else
		{
			float add_dist=bsp->GetMatGroupPoint((WORD)(mAlphaFace[i]&0x0000ffff),total);
		}
		Vector3fSub(total,camera,total);
		//Vector3fMul(total,3,total);
//		mAlphaFaceZ[i] = Vector3fDist(total)/(float)bsp->face[f].vertex_num;	//������ ���� �Ÿ��� ������..
		mAlphaFaceZ[i] = (SHORT)Vector3fDist(total);//+add_dist;
	}

//	udword *sorted=NULL;
//	if( mAlphaFaceCnt != 0 )
//		sorted = mSortBase.Sort(mAlphaFaceZ, mAlphaFaceCnt).GetIndices();
//	return (DWORD *)sorted;


	if( mAlphaFaceCnt != 0 )
		 ByteSortForShort(mAlphaFaceCnt,mAlphaFace,mAlphaFaceZ,mTempAlphaFace,mTempAlphaFaceZ);
	else
		return NULL;

	return mAlphaFace;
}

//----�ѽ� �ڷγ�.. �ڷγ��� ������ �ؾ���...
void CAlpha::SetCoronaStack(WORD group_id)
{
	mAlphaFace[mAlphaFaceCnt++]=_ALPHA_TYPE_CORONA|group_id;
	CheckAlphaAlloc();
}

//----�ѽ� ���� ��ƼƼ.. ���� ��ƼƼ�� ������ �ؾ���...
void CAlpha::SetAlphaEntityStack(WORD entities_id)
{
	mAlphaFace[mAlphaFaceCnt++]=_ALPHA_TYPE_ENTITY|entities_id;
	CheckAlphaAlloc();
}
