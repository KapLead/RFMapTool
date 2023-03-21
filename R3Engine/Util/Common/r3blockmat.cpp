#include "world.h"
#include "jerror.h"
#include "jmalloc.h"
#include "R3BlockMat.h"

/*
	���̷�Ʈ 8.0���� �߽��� �Ǵ� ���͸��� ���� ������ �ʿ��ϱ⶧���� 
	�޽��� ��Ƽ����¡�� ���ش�.
*/


#define _MAX_DIST_BB_BOX	512.0f //200.0f

static WORD GetDynamicObjectId(_MESH *mesh,int n_id)
{
	int i;

	WORD cnt=0;
	for(i=0 ;i<n_id+1; i++)
	{
		if( mesh->object[i].type & _DYNAMIC_OBJECT)
			cnt++;
		if( i==n_id+1)
			break;
	}
	return cnt;
}
WORD GetObjectId(_MESH *mesh,int d_id)
{
	int i;

	WORD cnt=0;
	for(i=0 ;i<mesh->objectnum; i++)
	{
		if( mesh->object[i].type & _DYNAMIC_OBJECT)
			cnt++;
		if( cnt==d_id)
			break;
	}
	return i;
}


// ü������ �ؾߵɱ�.. ���̷� �ؾߵɱ�...�ϴ� ���̷� ����...
BOOL IsNearBoundingBox(_OptObject *obj1,_OptObject *obj2)
{
	if( obj1->mtlid != obj2->mtlid )	//���͸����� �⺻���� ���ƾ�...
		return FALSE;
	float min_x = min( obj1->bb_min[0],obj2->bb_min[0] );
	float min_y = min( obj1->bb_min[1],obj2->bb_min[1] );
	float min_z = min( obj1->bb_min[2],obj2->bb_min[2] );

	float max_x = max( obj1->bb_max[0],obj2->bb_max[0] );
	float max_y = max( obj1->bb_max[1],obj2->bb_max[1] );
	float max_z = max( obj1->bb_max[2],obj2->bb_max[2] );

	if( fabs(max_x - min_x) < _MAX_DIST_BB_BOX 
	&& fabs(max_y - min_y) < _MAX_DIST_BB_BOX
	&& fabs(max_z - min_z) < _MAX_DIST_BB_BOX )
		return TRUE;

	return FALSE;
}

void MergeBoundingBox(_OptObject *obj1,_OptObject *obj2)
{
	float min_x = min( obj1->bb_min[0],obj2->bb_min[0] );
	float min_y = min( obj1->bb_min[1],obj2->bb_min[1] );
	float min_z = min( obj1->bb_min[2],obj2->bb_min[2] );

	float max_x = max( obj1->bb_max[0],obj2->bb_max[0] );
	float max_y = max( obj1->bb_max[1],obj2->bb_max[1] );
	float max_z = max( obj1->bb_max[2],obj2->bb_max[2] );

	obj1->bb_min[0] = min_x;
	obj1->bb_min[1] = min_y;
	obj1->bb_min[2] = min_z;

	obj1->bb_max[0] = max_x;
	obj1->bb_max[1] = max_y;
	obj1->bb_max[2] = max_z;
}

void R3BlockBsp::MergeOptObject()
{
	BOOL is_merge=FALSE;	//���� ������ �̷������.
	DWORD i;
	int p_cnt;
	_OptObject *cmp_object;
	_OptObject *now_object;
	_OptObject *pre_object;
/*
	p_cnt=0;
	for(now_object = mOptObject; now_object != NULL; now_object= now_object->next)//�Ѹ�ΰ� ����..
		p_cnt++;
*/
	
	p_cnt=0;
	_POLYFACE **temp_p = (_POLYFACE **)Dmalloc(sizeof(_POLYFACE *)*mPolys);	//�ε��� ���� ����...
	DWORD *temp_what_obj = (DWORD *)Dmalloc(sizeof(DWORD)*mPolys);	//������Ʈ �ε��� ���� ����ī��...

	p_cnt=0;
	for(now_object = mOptObject; now_object != NULL; now_object= now_object->next)//�Ѹ�ΰ� ����..
	{
		p_cnt+=now_object->f_cnt;
	}
	p_cnt=0;
	for(now_object = mOptObject; now_object != NULL; now_object= now_object->next)
	{
		BOOL search_ok=FALSE;
		pre_object = now_object;
		for(cmp_object = now_object; cmp_object != NULL; cmp_object = cmp_object->next)
		{

			if( now_object == cmp_object )
				continue;
			if( now_object->type&_NO_MERGE_OBJECT || cmp_object->type&_NO_MERGE_OBJECT )
			{
				pre_object = cmp_object;
				continue;	
			}
			if( (now_object->type & _NO_LIGHT_MAP)^(now_object->type & _NO_LIGHT_MAP) )	//�����ϳ��� ���ú�off ��..
			{
				pre_object = cmp_object;
				continue;	
			}
			if( !IsNearBoundingBox(now_object,cmp_object) )	//�ٿ�� �ڽ��� ������ ũ�⿡������ ����...
			{
				pre_object = cmp_object;
				continue;
			}
			if( now_object->f_cnt + cmp_object->f_cnt >= 12768 )	//���� �ʹ� ������ ��ġ�� �ʴ´�.
				continue;

			//������ ������ ��ģ��.
			search_ok=TRUE;
			is_merge=TRUE;
			int start_id = p_cnt;
			for(i=0; i<now_object->f_cnt; i++)
			{
				temp_p[p_cnt] = mPolyFace[now_object->start_id+i];
				temp_what_obj[p_cnt] = mPolyWhatObj[now_object->start_id+i];
				p_cnt++;
			}
			for(i=0; i<cmp_object->f_cnt; i++)
			{
				temp_p[p_cnt] = mPolyFace[cmp_object->start_id+i];
				temp_what_obj[p_cnt] = mPolyWhatObj[cmp_object->start_id+i];
				p_cnt++;
			}

			now_object->type = now_object->type | cmp_object->type;//Ÿ�� ��ġ��.
			cmp_object->type = now_object->type;	//Ÿ�� ��ġ��.
			now_object->start_id = start_id;
			now_object->f_cnt = now_object->f_cnt + cmp_object->f_cnt;
			pre_object->next = cmp_object->next;	//��ũ�� ����Ʈ����...
			//�ٿ�� �ڽ� ����...
			MergeBoundingBox(now_object, cmp_object);
			// ���ŵǴ°Ŵ� free����..
			Dfree(cmp_object);
			cmp_object=pre_object;
			break;	//�ѹ� ��ģ�Ŵ� �ϴ� ������...
		}
		if( search_ok == FALSE )
		{
			//�״�� �״´�.
			int start_id = p_cnt;
			for(i=0; i<now_object->f_cnt; i++)
			{
				temp_p[p_cnt] = mPolyFace[now_object->start_id+i];
				temp_what_obj[p_cnt] = mPolyWhatObj[now_object->start_id+i];
				p_cnt++;
			}
			now_object->start_id = start_id;
		}
	}
	if( p_cnt != mPolys )
		Error("���ο���! �ȸ¾�... ","");
	memcpy(mPolyFace,temp_p,sizeof(_POLYFACE *)*mPolys);	//�ε��� ���� ����ī��...
	memcpy(mPolyWhatObj,temp_what_obj,sizeof(DWORD)*mPolys);	//�ε��� ���� ����ī��...
	Dfree(temp_what_obj);
	Dfree(temp_p);

	p_cnt=0;
	for(now_object = mOptObject; now_object != NULL; now_object= now_object->next)//�Ѹ�ΰ� ����..
		p_cnt++;

	if( is_merge == TRUE )	//�ϳ��� ����������... �� ���ĺ���.
		MergeOptObject();
}

//R3BlockBsp::R3BlockBsp(_MESH *mesh)
R3BlockBsp::R3BlockBsp(int face_num)
{
	mPolys=face_num;
	mOptObject=NULL;
	mPolyFace = (_POLYFACE **)Dmalloc(sizeof(_POLYFACE *)*mPolys);	//�ε��� ���� ����...
	mPolyWhatObj = (DWORD*)Dmalloc(sizeof(DWORD)*mPolys);			//obj �ε��� ���� ����...
}

R3BlockBsp::~R3BlockBsp()
{
	Dfree(mPolyWhatObj);
	Dfree(mPolyFace);

	_OptObject *now_object = mOptObject;
	_OptObject *temp = mOptObject;

	for(;now_object != NULL; temp = now_object)
	{
//		temp = now_object;
		now_object = now_object->next;
		Dfree(temp);
	}
}
void R3BlockBsp::TestFree()
{
	_OptObject *now_object = mOptObject;
	_OptObject *temp = mOptObject;

	for(;now_object != NULL; temp = now_object)
	{
//		temp = now_object;
		now_object = now_object->next;
		Dfree(temp);
	}
}

void R3BlockBsp::PrepareBlockBsp(_MESH *mesh,_MATERIAL *material)
{	
	int i,j,k;
	DWORD mat_rcnt[1024];
	DWORD *mat_cnt=&mat_rcnt[1];
	_OptObject *now_object;
	int start_index=0;
	int start_id=0;
	if(mesh->poly_type != TRUE )	//�ﰢ�޽���.
	{
		Error("������ ���������� �ٲ����","");
	}
	if( material==NULL )
	{
		material=(_MATERIAL *)Dmalloc(sizeof(_MATERIAL));
		material->all_num=0;
	}
	if( material->all_num > 1023 )
		Error("�� ���͸����� �̷� ����...","");

	//��Ƽ������ ������Ʈ ����� �غ�.  
	//�ϴ� ������Ʈ���� ���͸��� �� ��ũ�� ����Ʈ�� �����.
//	int debug_cnt=0;
	for(i=0; i<mesh->objectnum; i++)
	{
		memset(mat_rcnt,0,(material->all_num+1)*sizeof(DWORD));
		for(j=0; j<mesh->object[i].facenum; j++)
		{
			if( mesh->object[i].p_face[j].mtlid != -1 )
			{
				mat_cnt[mesh->object[i].p_face[j].mtlid]++;
			}
			else
				mat_cnt[-1]++;	//��ó�� �迭 -1�� �ִ´�.
		}
		int old_f_cnt;
		for(j=-1; j<material->all_num; j++)	//������ �迭�� -1...
		{
			if(mat_cnt[j]>0)
			{
				if( mOptObject == NULL )	//�ʱ⿡ �����Ұ�...
				{
					mOptObject = (_OptObject *)Dmalloc(sizeof(_OptObject));
					memset(mOptObject,0,sizeof(_OptObject));
					now_object = mOptObject;
					old_f_cnt=now_object->f_cnt;
				}
				else
				{
					now_object->next = (_OptObject *)Dmalloc(sizeof(_OptObject));
					now_object = now_object->next;
					old_f_cnt=now_object->f_cnt;
				}
				memset(now_object,0,sizeof(_OptObject));
				now_object->i = start_index++;
				now_object->mtlid = j;
				//----�����︮��Ʈ���� ����� ����.
				now_object->start_id = start_id;

				//Ÿ�� ����.
				now_object->type = (WORD)mesh->object[i].type;

				if(_OBJECT_TYPE_COMPOSITE & mesh->object[i].type )
				{
					now_object->type|=_COMPOSITE_LIGHT_MAP;
					now_object->type|=_NO_MERGE_OBJECT;
					now_object->lgtid = (SHORT)mesh->object[i].composite_id;
				}
				if(_SHADOW_RECEIVE_OFF & mesh->object[i].type )
				{
					now_object->type|=_NO_LIGHT_MAP;
					now_object->type|=_NO_MERGE_OBJECT;
				}
				if( mesh->object[i].type & _DYNAMIC_OBJECT)
					now_object->object_id=GetDynamicObjectId(mesh,i);

				float min_x=60000,max_x=-60000;
				float min_y=60000,max_y=-60000;
				float min_z=60000,max_z=-60000;

				for(k=0; k<mesh->object[i].facenum; k++)
				{
					if( mesh->object[i].p_face[k].mtlid == j )
					{
						mPolyFace[start_id+now_object->f_cnt] = &mesh->object[i].p_face[k];
						mPolyWhatObj[start_id+now_object->f_cnt] = i;	//������Ʈ �ε���
						now_object->f_cnt++;

						for(int l=0; l<mesh->object[i].p_face[k].vertex_num; l++)
						{
							int v_id = mesh->object[i].p_face[k].vertex_id[l];
							if( min_x > mesh->object[i].vertex[v_id].x )
								min_x = mesh->object[i].vertex[v_id].x;
							if( min_y > mesh->object[i].vertex[v_id].y )
								min_y = mesh->object[i].vertex[v_id].y;
							if( min_z > mesh->object[i].vertex[v_id].z )
								min_z = mesh->object[i].vertex[v_id].z;
							if( max_x < mesh->object[i].vertex[v_id].x )
								max_x = mesh->object[i].vertex[v_id].x;
							if( max_y < mesh->object[i].vertex[v_id].y )
								max_y = mesh->object[i].vertex[v_id].y;
							if( max_z < mesh->object[i].vertex[v_id].z )
								max_z = mesh->object[i].vertex[v_id].z;
						}
					}
				}
				//�ٿ�� �ڽ� ä���.
				now_object->bb_min[0] = min_x;
				now_object->bb_min[1] = min_y;
				now_object->bb_min[2] = min_z;
				now_object->bb_max[0] = max_x;
				now_object->bb_max[1] = max_y;
				now_object->bb_max[2] = max_z;
				start_id+=now_object->f_cnt;
			}
		}
//		debug_cnt += mesh->object[i].facenum;
	}

	int all_f_cnt=0;
	for(now_object = mOptObject; now_object != NULL; now_object= now_object->next)
		all_f_cnt += now_object->f_cnt;
	int all_f_cnt2=0;
	for(i=0; i<mesh->objectnum; i++)
		all_f_cnt2 += mesh->object[i].facenum;
	if( all_f_cnt2 != all_f_cnt )
		Error("�����Դϴ�.","");
	
	//----------------------------------������Ʈ�� ��ģ��.
	MergeOptObject();
	
	//-----�ε����� �����Ѵ�.
	int p_cnt=0;
	for(now_object = mOptObject; now_object != NULL; now_object= now_object->next)
	{
		now_object->i = p_cnt++;
	}
	mOptObjNum = p_cnt;	//�� ��Ƽ������ ������Ʈ��..

	//��ϻ� ó���ؾߵ� ������ ��������� ���� ����ߵǱⶫ�� ������Ʈ�� ��쿡���� ������.
	//���� �ȸ������...  �̰ű��� ����ϸ��� �峭�ƴ�...
	

}

int GetOneMeshFaceNum(_MESH *mesh,int obj_num,int f_id)
{
	int i;

	int nu_num=0;
	int old_num=0;
	for(i=0; i<obj_num;i++)
	{
		nu_num+=mesh->object[i].facenum;
	}
	return nu_num + f_id;
}

_MESH *R3BlockBsp::MakePMeshBlockBsp(_MESH *p_mesh,_MESH *r_p_mesh)
{
	int i;
	_MESH *mesh=(_MESH *)Dmalloc(sizeof(_MESH));

	mesh->object=(_OBJECT *)Dmalloc(sizeof(_OBJECT));	
	mesh->objectnum=1;	
	mesh->poly_type=TRUE;

	memcpy(mesh->object,p_mesh->object,sizeof(_OBJECT));
	mesh->object->facenum=p_mesh->object->facenum;

	mesh->object->p_face=(_POLYFACE *)Dmalloc(sizeof(_POLYFACE)*p_mesh->object->facenum);
	if( mesh->object->facenum > mPolys )
	{
		mPolyFace = (_POLYFACE **)ReAlloc(mPolyFace,sizeof(_POLYFACE *)*mPolys,sizeof(_POLYFACE *)*mesh->object->facenum);	//�ε��� ���� ����...
		mPolyWhatObj = (DWORD*)ReAlloc(mPolyWhatObj,sizeof(DWORD)*mPolys,sizeof(DWORD)* mesh->object->facenum);			//obj �ε��� ���� ����...
		mPolys = mesh->object->facenum;
	}
	for(i=0; i<mesh->object->facenum; i++)
	{
		memcpy(&mesh->object->p_face[i],mPolyFace[i],sizeof(_POLYFACE));
		
		int f_id = ((DWORD)(&mPolyFace[i]->vertex_num)-(DWORD)(r_p_mesh->object[mPolyWhatObj[i]].p_face))/sizeof(_POLYFACE);
		int r_f_id = GetOneMeshFaceNum(r_p_mesh,mPolyWhatObj[i],f_id);
		mesh->object->p_face[i].vertex_id=p_mesh->object->p_face[r_f_id].vertex_id;
		mesh->object->p_face[i].vertex_num=p_mesh->object->p_face[r_f_id].vertex_num;
	}
	return mesh;
}