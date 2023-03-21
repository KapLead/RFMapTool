#include "collision.h"
#include "R3bsp.h"
#include "Core.h"
#include "jerror.h"
#include "R3lightmap.h"


#define _ON_EPSILON	0.01



//-----�н� ���ε� ���� API

#define _NO_COLLISION		0	//�浹�� �ȵɰ��.
#define _NORMAL_COLLISION	1	//����� ������ ���� �浹���.
#define _CONTINUE_NODE		2	//���� �������� ���� ã�ư��ߵɰ��.
#define _ONE_NORMAL_COLLISION	3	//���ѹ� ������ ���� ���� �浹�Ǵ� ���.
#define _CANT_GO			4	//������ ����.

#define _COL_ON_EPSILON		0.5

#define _ADD_PLANE_DIST		1.0f


#define _LINE_FRONT	1		//���� ����.
#define _LINE_BACK	-1


static void GetCrossPointToDistFromPlane(Vector3f *get,Vector3f from,Vector3f at,Vector4f f_normal)
{
	Vector3f l_normal;
	Vector3fSub(from,at,l_normal);
	Normalize(l_normal);
	float su=Vector3fDot(l_normal,f_normal);
	su=_ADD_PLANE_DIST/su;
	get[0][0] = l_normal[0]*su+get[0][0];
	get[0][1] = l_normal[1]*su+get[0][1];
	get[0][2] = l_normal[2]*su+get[0][2];
}
BOOL CPathFinder::AddPath(Vector3f pos,int id)
{
	if( mPathNode[id].PathCnt+1 >= mMaxDepth )
		return FALSE;

	Vector3fCopy(pos,mPathNode[id].Path[mPathNode[id].PathCnt]);

	Vector3f dist;
	//�Ÿ��� ���Ѵ�.
	if( mPathNode[id].PathCnt == 0 )
	{
		Vector3fSub(pos,mStart,dist);
	}
	else
	{	//�� �������ϰ� �Ÿ��� ���Ѵ�.
		Vector3fSub(pos,mPathNode[id].Path[mPathNode[id].PathCnt-1],dist);
	}
	mPathNode[id].TotalLeng += Vector3fDist(dist);

	mPathNode[id].PathCnt++;
	return TRUE;
}



void CPathFinder::PushPathStack(int id)	//���������� �н��� ���ÿ� �ִ´�.
{
	memcpy(&mStackPathNode[mStackPoint],&mPathNode[id],sizeof(_PATH_NODE));
	mStackPoint++;
	if( mStackPoint >= _MAX_FIND_PATH+10 )
	{
		Error("pathfinder stack overflow","");
	}
}
void CPathFinder::PopPathStack(int id)	//���������� �н��� ���ÿ� ������.
{
	mStackPoint--;
	if( mStackPoint < 0)
	{
		Error("pathfinder stack ����","");
	}
	memcpy(&mPathNode[id],&mStackPathNode[mStackPoint],sizeof(_PATH_NODE));
}

#define _COL_EPSILON	2
#define CmpVEpsilon(a,b,ep)	(( a[0]+ep > b[0] && a[0]-ep < b[0] )&& ( a[1]+ep > b[1] && a[1]-ep < b[1] )&& ( a[2]+ep > b[2] && a[2]-ep < b[2] ))

BOOL CBsp::EdgeTest(Vector3f cross,int id)
{
	Vector3f a,b;
	a[0] = mCFVertex[mCFLine[id].start_v][0];
	a[1] = mCFVertex[mCFLine[id].start_v][1];
	a[2] = mCFVertex[mCFLine[id].start_v][2];
	b[0] = mCFVertex[mCFLine[id].start_v][0];
	b[1] = mCFVertex[mCFLine[id].start_v][1] + mCFLine[id].height;
	b[2] = mCFVertex[mCFLine[id].start_v][2];
	if( CheckEdgeEpsilon( a , b , cross ,  mCFNormal[id],mCFNormal[id][3]) == 0)	
		return FALSE;

	a[0] = mCFVertex[mCFLine[id].end_v][0];
	a[1] = mCFVertex[mCFLine[id].end_v][1] + mCFLine[id].height;
	a[2] = mCFVertex[mCFLine[id].end_v][2];
	if( CheckEdgeEpsilon( b , a , cross ,  mCFNormal[id],mCFNormal[id][3]) == 0)	
		return FALSE;

	b[0] = mCFVertex[mCFLine[id].end_v][0];
	b[1] = mCFVertex[mCFLine[id].end_v][1];
	b[2] = mCFVertex[mCFLine[id].end_v][2];
	if( CheckEdgeEpsilon( a , b , cross ,  mCFNormal[id],mCFNormal[id][3]) == 0)	
		return FALSE;

	a[0] = mCFVertex[mCFLine[id].start_v][0];
	a[1] = mCFVertex[mCFLine[id].start_v][1];
	a[2] = mCFVertex[mCFLine[id].start_v][2];
	if( CheckEdgeEpsilon( b , a , cross ,  mCFNormal[id],mCFNormal[id][3]) == 0)	
		return FALSE;
	return TRUE;
}

int CBsp::GetPathCrossPoint(Vector3f from,Vector3f at,Vector3f *get,int now_f1,int now_f2)
{
	int i,l,k;
	Vector3f cross;
	BOOL is_col=FALSE,back_ok;
	int back_crash=0;
	int front_crash=0;
	float dist = 100000,front,back;
	float ret=-65536;
	mColFaceId=-1;
	int front_epsilon[8];
	int f_ep_cnt=0;

#define _EYE_POINT 16
	from[1]+=_EYE_POINT;
	at[1]+=_EYE_POINT;

	long leaf_num;
	SHORT leaf_id[32000];//=(SHORT*)GetTempBuffer();

	GetLeafList(from,at,&leaf_num,leaf_id,32000);

	if( mCFLineNum < 2 )
	{
		from[1]-=_EYE_POINT;
		at[1]-=_EYE_POINT;
		return _NO_COLLISION;
	}
	for(l=0; l<leaf_num; l++)
	for(k=0; k<mCFLeaf[leaf_id[l]].line_num; k++)
	{
		i = mCFLineId[mCFLeaf[leaf_id[l]].start_id+k];
//	for( i=1; i<(int)mCFLineNum; i++)
		if( now_f1 == i || now_f2 == i )
			continue;

		front = mCFNormal[i][0]*from[0] + mCFNormal[i][1]*from[1] + mCFNormal[i][2]*from[2] -mCFNormal[i][3];
		back  = mCFNormal[i][0]*at[0] + mCFNormal[i][1]*at[1] + mCFNormal[i][2]*at[2] -mCFNormal[i][3];

		if( front > -_ON_EPSILON && front <_ON_EPSILON )	//������ �Ǵ� ����Ʈ
			front_epsilon[f_ep_cnt++]=i;

		if( (front > 0 && back > 0 ) || (front < -_ON_EPSILON && back < -_ON_EPSILON)) 
     			continue;

		if( front > -_ON_EPSILON && back <= 0 )
			back_ok = FALSE;
		else
		if( front < 0 && back > 0 )
			back_ok = TRUE;
		else
		if ( front > -_ON_EPSILON && back > -_ON_EPSILON )
			continue;
		else
		if ( front < -_ON_EPSILON && back > -_ON_EPSILON )
			back_ok = TRUE;
		else
			back_ok = FALSE;

		front = front / (front-back);	
/*
		//�տ� ������ ������ �����.
		cross[0] = from[0] + front*(at[0]-from[0]) + _ADD_PLANE_DIST*mNormal[i][0];
		cross[1] = from[1] + front*(at[1]-from[1]) + _ADD_PLANE_DIST*mNormal[i][1];
		cross[2] = from[2] + front*(at[2]-from[2]) + _ADD_PLANE_DIST*mNormal[i][2];
*/
		cross[0] = from[0] + front*(at[0]-from[0]);
		cross[1] = from[1] + front*(at[1]-from[1]);
		cross[2] = from[2] + front*(at[2]-from[2]);

		if( EdgeTest(cross,i) == FALSE )
			continue;

		Vector3f vec_dist;
		Vector3fSub(cross,from,vec_dist);
		float temp_d = sqrtf(Vector3fDot(vec_dist,vec_dist));
		if(dist > temp_d)
		{
			if( back_ok )
			{
				is_col=FALSE;
				dist = temp_d;
				back_crash++;
				continue;
			}
			mColFaceId=i;
			dist = temp_d;
			is_col=TRUE;
			get[0][0]=cross[0];
			get[0][1]=cross[1];
			get[0][2]=cross[2];
			front_crash++;
		}
		else
		{
			if( back_ok )
				back_crash++;
			else
				front_crash++;
		}
	}
	from[1]-=_EYE_POINT;
	at[1]-=_EYE_POINT;

	if( is_col )
	{
		GetCrossPointToDistFromPlane(get,from,at,mCFNormal[mColFaceId]);

		//���� �Ʒ���ƾ�� �������� �ִ� ��ƾ�̴�.
//		if( front_crash - back_crash > 0)
//			return _ONE_NORMAL_COLLISION;
		if( front_crash && back_crash == 0)
			return _ONE_NORMAL_COLLISION;
		if( now_f1 || now_f2 )
		{
			front = mCFNormal[now_f1][0]*get[0][0] + mCFNormal[now_f1][1]*get[0][1] 
				+ mCFNormal[now_f1][2]*get[0][2] -mCFNormal[now_f1][3];
			back = mCFNormal[now_f2][0]*get[0][0] + mCFNormal[now_f2][1]*get[0][1] 
				+ mCFNormal[now_f2][2]*get[0][2] -mCFNormal[now_f2][3];
			if( front < 0 && back < 0 )
			return _CONTINUE_NODE;
		}

		return _NORMAL_COLLISION;
	}
	else
	{
		if( (now_f1 || now_f2) && back_crash  )
			return _CONTINUE_NODE;
		return _NO_COLLISION;
	}
}

BOOL CBsp::IsExistSelfPoint(int id,int face)	//�佺����Ʈ�� �浹 ���̽��� ������ �ֳ�?
{
	if( mPathFinder.GetPathDirection(id) == _LINE_BACK )
	{
		int st=mCFLine[face].start_v;
		for(int i=0; i<mPathFinder.mPathNode[id].PathCnt; i++)
		{
			if( CmpVEpsilon(mCFVertex[st],mPathFinder.mPathNode[id].Path[i],_ADD_PLANE_DIST))
				return TRUE;
		}
	}
	else
	{
		int st=mCFLine[face].end_v;
		for(int i=0; i<mPathFinder.mPathNode[id].PathCnt; i++)
		{
			if( CmpVEpsilon(mCFVertex[st],mPathFinder.mPathNode[id].Path[i],_ADD_PLANE_DIST))
				return TRUE;
		}
	}
	return FALSE;
}

void CBsp::GetFaceFrontPoint(Vector3f *get_temp,int v_id)	//����� �ణ ������ Ƣ��� ���� ���´�.
{
	Vector3f add_pos;
	Vector3fScale(mCFVNormal[v_id],_ADD_PLANE_DIST,add_pos);
	Vector3fAdd(add_pos,mCFVertex[v_id],get_temp[0]);
}


void CBsp::GetPath(Vector3f from,Vector3f at)
{
	if( mFindPathCnt >= _MAX_FIND_PATH )	//���ٴ� �׸� ã��..
		return;
	if( mPathFinder.GetPathCnt(mFindPathCnt)+1 >= mPathFinder.mMaxDepth )	//�н��ִ°� ���ى��.
		return;

	Vector3f get,get_temp,get2;

	int type = GetPathCrossPoint(from,at,&get,mPathFinder.GetFrontLineId(mFindPathCnt),mPathFinder.GetBackLineId(mFindPathCnt));
	int backup_face = mColFaceId;

	if( type == _NORMAL_COLLISION )
	{
		if( IsExistSelfPoint(mFindPathCnt,backup_face) )
		{
			type = _CONTINUE_NODE;
		}
	}

	switch( type )
	{
		case _NORMAL_COLLISION:
			mPathFinder.AddPath(get,mFindPathCnt);	//�浹�� ���� ����.
			mPathFinder.PushPathStack(mFindPathCnt);	//���������� �н��� ���ÿ� �ִ´�.

			//����� ������ ������ �ִ´�.
			mPathFinder.SetFrontLineId(mFindPathCnt,backup_face);//��� ����Ʈ�� ������ �ִ´�.
			mPathFinder.SetBackLineId(mFindPathCnt,mCFLine[backup_face].back);
			mPathFinder.SetPathDirection(mFindPathCnt,_LINE_BACK);
			GetFaceFrontPoint(&get_temp,mCFLine[backup_face].start_v);	//����� �ణ ������ Ƣ��� ���� ���´�.
			get_temp[1] = from[1];

			type = GetPathCrossPoint(get,get_temp,&get2,mPathFinder.GetFrontLineId(mFindPathCnt),mPathFinder.GetBackLineId(mFindPathCnt));
			if( type && mColFaceId !=-1 )
			{
				mPathFinder.AddPath(get2,mFindPathCnt);	//�浹�� ���� ����.
				mPathFinder.SetFrontLineId( mFindPathCnt, mColFaceId );	//��� ����Ʈ�� ������ �ٽ� �ִ´�.
				mPathFinder.SetBackLineId( mFindPathCnt, mCFLine[mColFaceId].back);
//				FixBeforePoint(mColFaceId,mCFLine[mColFaceId].end_v,mFindPathCnt);	//���� ������ �߰� ��ġ�� �� ��� �� �ִ´�.
				GetFaceFrontPoint(&get_temp,mCFLine[mColFaceId].start_v);	//����� �ణ ������ Ƣ��� ���� ���´�.
				get_temp[1] = from[1];
			}
			mPathFinder.AddPath(get_temp,mFindPathCnt);	//���߰� 
			GetPath(get_temp,at);

			mFindPathCnt++;	//�н��ϳ��� ����..
			mPathFinder.PopPathStack(mFindPathCnt);//���������� �н��� ���ÿ� ������.
			if( mFindPathCnt >= _MAX_FIND_PATH )
			{
				mFindPathCnt--;
				return;
			}
			//����Ʈ ���������� ������ �ִ´�.
			mPathFinder.SetFrontLineId(mFindPathCnt,mCFLine[backup_face].front);//��� ����Ʈ�� ������ �ִ´�.
			mPathFinder.SetBackLineId(mFindPathCnt,backup_face);
			mPathFinder.SetPathDirection(mFindPathCnt,_LINE_FRONT);
			GetFaceFrontPoint(&get_temp,mCFLine[backup_face].end_v);	//����� �ణ ������ Ƣ��� ���� ���´�.
			get_temp[1] = from[1];

			type = GetPathCrossPoint(get,get_temp,&get2,mPathFinder.GetFrontLineId(mFindPathCnt),mPathFinder.GetBackLineId(mFindPathCnt));
			if( type && mColFaceId !=-1 )
			{
				mPathFinder.AddPath(get2,mFindPathCnt);	//�浹�� ���� ����.
				mPathFinder.SetBackLineId( mFindPathCnt, mColFaceId );	//��� ����Ʈ�� ������ �ִ´�.
				mPathFinder.SetFrontLineId( mFindPathCnt, mCFLine[mColFaceId].front);
//				FixBeforePoint(mColFaceId,mCFLine[mColFaceId].start_v,mFindPathCnt);	//���� ������ �߰� ��ġ�� �� ��� �� �ִ´�.
				GetFaceFrontPoint(&get_temp,mCFLine[mColFaceId].end_v);	//����� �ణ ������ Ƣ��� ���� ���´�.
				get_temp[1] = from[1];
			}
			mPathFinder.AddPath(get_temp,mFindPathCnt);	//���߰� 
			GetPath(get_temp,at);
			break;
		case _CONTINUE_NODE:
			if( mPathFinder.GetPathDirection(mFindPathCnt) == _LINE_BACK)
			{
				int bk = mPathFinder.GetBackLineId(mFindPathCnt);	//�ٲ�� ���� ���.
				mPathFinder.SetFrontLineId( mFindPathCnt, bk );	//��� ����Ʈ�� ������ �ִ´�.
				mPathFinder.SetBackLineId( mFindPathCnt, mCFLine[bk].back);
				//FixBeforePoint(bk,mCFLine[bk].end_v,mFindPathCnt);	//���� ������ �߰� ��ġ�� �� ��� �� �ִ´�.
				GetFaceFrontPoint(&get_temp,mCFLine[bk].start_v);	//����� �ణ ������ Ƣ��� ���� ���´�.
				get_temp[1] = from[1];

				type = GetPathCrossPoint(from,get_temp,&get,mPathFinder.GetFrontLineId(mFindPathCnt),mPathFinder.GetBackLineId(mFindPathCnt));
				if( type  && mColFaceId !=-1  )
				{
					mPathFinder.AddPath(get,mFindPathCnt);	//�浹�� ���� ����.
					mPathFinder.SetFrontLineId( mFindPathCnt, mColFaceId );	//��� ����Ʈ�� ������ �ٽ� �ִ´�.
					mPathFinder.SetBackLineId( mFindPathCnt, mCFLine[mColFaceId].back);
//					FixBeforePoint(mColFaceId,mCFLine[mColFaceId].end_v,mFindPathCnt);	//���� ������ �߰� ��ġ�� �� ��� �� �ִ´�.
					GetFaceFrontPoint(&get_temp,mCFLine[mColFaceId].start_v);	//����� �ణ ������ Ƣ��� ���� ���´�.
					get_temp[1] = from[1];					
				}
				mPathFinder.AddPath(get_temp,mFindPathCnt);	//�� �߰� 
				GetPath(get_temp,at);	//�� ����..
			}
			else
			{
				int bk = mPathFinder.GetFrontLineId(mFindPathCnt);	//�ٲ�� ���� ���.
				mPathFinder.SetBackLineId( mFindPathCnt, bk );	//��� ����Ʈ�� ������ �ִ´�.
				mPathFinder.SetFrontLineId( mFindPathCnt, mCFLine[bk].front);
//				FixBeforePoint(bk,mCFLine[bk].start_v,mFindPathCnt);	//���� ������ �߰� ��ġ�� �� ��� �� �ִ´�.
				GetFaceFrontPoint(&get_temp,mCFLine[bk].end_v);	//����� �ణ ������ Ƣ��� ���� ���´�.
				get_temp[1] = from[1];

				type = GetPathCrossPoint(from,get_temp,&get,mPathFinder.GetFrontLineId(mFindPathCnt),mPathFinder.GetBackLineId(mFindPathCnt));
				if( type && mColFaceId !=-1 )
				{
					mPathFinder.AddPath(get,mFindPathCnt);	//�浹�� ���� ����.
					mPathFinder.SetBackLineId( mFindPathCnt, mColFaceId );	//��� ����Ʈ�� ������ �ִ´�.
					mPathFinder.SetFrontLineId( mFindPathCnt, mCFLine[mColFaceId].front);
//					FixBeforePoint(mColFaceId,mCFLine[mColFaceId].start_v,mFindPathCnt);	//���� ������ �߰� ��ġ�� �� ��� �� �ִ´�.
					GetFaceFrontPoint(&get_temp,mCFLine[mColFaceId].end_v);	//����� �ణ ������ Ƣ��� ���� ���´�.
					get_temp[1] = from[1];
				}
				mPathFinder.AddPath(get_temp,mFindPathCnt);	//���߰� 
				GetPath(get_temp,at);	//�ǰ���..
			}
			break;
		case _NO_COLLISION:
			if( mPathFinder.AddPath(at,mFindPathCnt) == FALSE )	//���̻�� �ִ´�.
				break;
			mPathFinder.CompletePath(mFindPathCnt);//�ѽ� ��ã�Ҵ�.
			break;
		case _ONE_NORMAL_COLLISION:	//�� front����� �浹�� �ִ°��..
			if( mPathFinder.AddPath(get,mFindPathCnt) == FALSE )	//���̻�� �ִ´�.
				break;
			mPathFinder.CompletePath(mFindPathCnt);//�ѽ� ��ã�Ҵ�.
			break;
	}
}

int CBsp::GetFinalPath(void *node_p,Vector3f from,Vector3f get[16])
{
	int i,j,id;
	_PATH_NODE *node=(_PATH_NODE *)node_p;
/*
	Vector3f sub;	//ó�� �������� �ʹ� ��ó�� �ִٸ�..
	node->Path[0][1]=from[1];
	Vector3fSub(from,node->Path[0],sub);
	
	float dist = Vector3fDist(sub);
	int ss=0;
	if( dist < 1 )
		ss=1;
*/
	Vector3f temp;
	if( node->PathCnt == 1)
	{
		Vector3fCopy(node->Path[0],get[0]);
		return 1;
	}
	int index[16],index_cnt=0;

	index[index_cnt++] = node->PathCnt-1;
	for(i=node->PathCnt-2; i>0; i--)
	{
		id=-2;
		if( !GetPathCrossPoint(from,node->Path[i],&temp,0,0) )
		{
			id=-1;
			index[index_cnt++]=i;
			break;
		}
		for(j=i-2; j>0 ;j-- )
		{
			if( !GetPathCrossPoint(node->Path[j],node->Path[i],&temp,0,0) )
			{
				id=j;
			}
		}
		if( id == -2 )
		{
			index[index_cnt++]=i;
			continue;
		}
		
		index[index_cnt++]=i;
		index[index_cnt++]=id;
		i=id+1;
	}
	if( id!=-1 )
		index[index_cnt++]=0;

	for(i=0; i<index_cnt; i++)
		Vector3fCopy(node->Path[index[index_cnt-i-1]],get[i]);


	return index_cnt;
	/*	����ȭ�� �ƴҶ�
	for(i=ss; i<node->PathCnt; i++)
	{
		Vector3fCopy(node->Path[i],get[i-ss]);
	}
	return ( node->PathCnt-ss );
	*/
}
DWORD CBsp::GetPathFind(Vector3f from,Vector3f at,Vector3f get[16],DWORD *cnt,int depth )
{
	DWORD i,min_i=-1;
	float min_dist=1000000;
	BOOL too_long=FALSE;
	Vector3f target_at;

	Vector3fCopy(at,target_at);
	target_at[1] = from[1];
	Vector3f temp_f,temp_at;
	Vector3fSub(target_at,from,temp_f);


	float dist = Vector3fDist(temp_f);
	if( dist > 200 )	//�ʹ� �� �ڸ���.
	{
		too_long=TRUE;		
		Vector3fCopy(target_at,temp_at);
		Normalize(temp_f);
		Vector3fScale(temp_f,200,temp_f);	//������ �ִ� 20���ͱ����� ����.
		Vector3fAdd(temp_f,from,target_at);
		Normalize(temp_f);
		Vector3fScale(temp_f,210,temp_f);	//TEMP Ÿ���� ���� 21���Ͱ��� ����.
		temp_at[1] = from[1];
	}

	memset(mPathFinder.mPathNode,0,sizeof(_PATH_NODE)*_MAX_FIND_PATH);
	mFindPathCnt=0;	//�н��� ����..
	mPathFinder.mStackPoint=0;	//���õ� ����..

	Vector3fCopy(from,mPathFinder.mStart);
	mPathFinder.mMaxDepth = depth;		//ã�����ϴ� �ִ� ���̸� ���´�.
	GetPath(from,target_at);
	mFindPathCnt++;	//�����̹Ƿ�...
	for(i=0; i<	mFindPathCnt; i++)
	{
		if( mPathFinder.mPathNode[i].IsFind == FALSE )
			continue;
		if( min_dist > mPathFinder.mPathNode[i].TotalLeng )
		{
			min_dist = mPathFinder.mPathNode[i].TotalLeng;
			min_i = i;
		}
	}
	/*
	if( mFindPathCnt == 0 )	//�̷� ������ ��찡 ����� �ȵ�..
	{
		Vector3fCopy(at,get[i]);
		*cnt = 1;
		return;
	}*/
	if( min_i == -1 )	//���� ��ã��.
	{
		Vector3f can_go;
		if( GetPathCrossPoint(from,target_at,&can_go,0,0) )
		{
			Vector3fCopy(can_go,get[0]);
//			Vector3fCopy(from,get[0]);
		}
		else
		{
			Vector3fCopy(target_at,get[0]);
		}
		*cnt = 1;
		return _PATH_NOT_FOUND;
	}
	
	*cnt=GetFinalPath(&mPathFinder.mPathNode[min_i],from,get);	//�߰��� ���� �ִ� ���� ã�Ƽ� ����ȭ ���ش�. 

	if( too_long && *cnt == 1 && Vector3fCmp(target_at,get[0]) )
	{	//�ָ� �����κ��� �������ؼ�
		*cnt = 2;
		Vector3fCopy(temp_at,get[1]);
		return _PATH_TOO_LONG_CLIP;
	}

	return _PATH_FOUND;
}


