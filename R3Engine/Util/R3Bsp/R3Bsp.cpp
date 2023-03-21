#include "world.h"
#include "R3Bsp.h"
#include "R3BspTool.h"
#include "BSPRenderPart.h"
#include "R3BspUtil.h"
#include "JError.h"
#include "entity.h"
#include "jmalloc.h"
#include <stdio.h>
#include <gl\gl.h>

#define _FRONT	0x2
#define _BACK	0x1

//#define _MAX_LEAF	65536
//#define _MAX_NODE	65536

#define _SPLIT_FACE_FLAG		0x00000001

#define _MIN_FACE		48	//Leaf �ϳ��� ������ �ּ� face�ִ��.
#define _MIN_BBOX_EDGE	50//196	//�ϳ��� Leaf�� �ڽ��� �ּ� �𼭸� ����. ���Ű�� 18�̴ϱ�..

int Max_node,Max_leaf;
static _TOOL_LEAF *Leaf;
static _TOOL_NODE *Node;
static Leaf_num=0,Node_num=0;

//---------------------�ڸ� �༮���� ����...
#define	_MAX_BSP_FACE			65536*32	//�ִ� 200�� �������̻��� �ȵȴ�.
//#define	_MAX_BSP_VERTEX			65536*8
int Max_bsp_face,Max_bsp_vertex;

static _VERTEX *Bsp_vertex;
static LONG Bsp_vertex_num;		//bsp�� �߷��� ���ؽ� ��.
static LONG Bsp_face_num;		//bsp�� �߷��� ���̽���.
static _TOOL_BSP_FACE *Bsp_face[_MAX_BSP_FACE];


static int Org_vertex_num;
static int Org_face_num;

DWORD GetEventDObjectID(_MESH *m_mesh,int id)
{
	int d_obj_num=0,i;
	for(i=0; i<m_mesh->objectnum; i++)
	{
		if( id == d_obj_num ) 
		{
			if( !(m_mesh->object[i].type & _DYNAMIC_OBJECT) )
				continue;
			if( m_mesh->object[i].type & _EVENT_ANI )
				return m_mesh->object[i].event_id;
			else
				return 0;
		}
		if( m_mesh->object[i].type & _DYNAMIC_OBJECT )
			d_obj_num++;
	}
	return d_obj_num;
}

void InitBspMalloc(_MESH *mesh)
{
	Max_node=500;	//ó���� 
	Max_leaf=500;	//ó����
	Max_bsp_face = mesh->object->facenum+1;
	Max_bsp_vertex = mesh->object->vertexnum;
	Leaf=(_TOOL_LEAF *)Dmalloc(Max_leaf*sizeof(_TOOL_LEAF));
	memset(Leaf,0,Max_leaf*sizeof(_TOOL_LEAF));
	Node=(_TOOL_NODE *)Dmalloc(Max_node*sizeof(_TOOL_NODE));
	memset(Node,0,Max_node*sizeof(_TOOL_NODE));
	Bsp_vertex=(_VERTEX *)Dmalloc(Max_bsp_vertex*sizeof(_VERTEX));
//	Bsp_face=(_TOOL_BSP_FACE *)Dmalloc(Max_bsp_face*sizeof(_TOOL_BSP_FACE));
//	memset(Bsp_face,0,Max_bsp_face*sizeof(_TOOL_BSP_FACE));
}

void ReleaseBspMalloc()//���߿� ���������� ���������� free�� ���Ѵ�..
{
}

void AddLeaf(int leaf_num)
{

//	if( leaf_num == 3574 )
//		int sdfdf=0;

	if( leaf_num >= Max_leaf )	//�ִ�ġ�� �ѱ�� �ٽø޸𸮸� �Ҵ��Ѵ�.
	{
		int old_temp = Max_node;

		Max_leaf=(int)((float)(Max_leaf+1)*1.2f);	//20%�� ����...
		Leaf=(_TOOL_LEAF *)ReAlloc(Leaf,old_temp*sizeof(_TOOL_LEAF),Max_leaf*sizeof(_TOOL_LEAF));
//		_TOOL_LEAF *Temp=(_TOOL_LEAF *)Dmalloc(Max_leaf*sizeof(_TOOL_LEAF));
//		memcpy(Temp,Leaf,sizeof(_TOOL_LEAF)*(leaf_num));
//		Dfree(Leaf);
//		Leaf = Temp;
	} 
	memset( &Leaf[leaf_num],0,sizeof(_TOOL_LEAF));
}
void AddNode(int node_num)
{
	if( node_num >= Max_node )	//�ִ�ġ�� �ѱ�� �ٽø޸𸮸� �Ҵ��Ѵ�.
	{
		int old_temp = Max_node;

		Max_node=(int)((float)(Max_node+1)*1.2f);	//20%�� ����...
		Node=(_TOOL_NODE *)ReAlloc(Node,old_temp*sizeof(_TOOL_NODE),Max_node*sizeof(_TOOL_NODE));
		
//		_TOOL_NODE *Temp=(_TOOL_NODE *)Dmalloc(Max_node*sizeof(_TOOL_NODE));
//		memcpy(Temp,Node,sizeof(_TOOL_NODE)*(node_num));
//		Dfree(Node);
//		Node = Temp;
	}
	memset( &Node[Node_num],0,sizeof(_TOOL_NODE));
}
DWORD AddBspVertex(Vector3f cross)
{
//	oldd=Bsp_vertex_num;
	if( Bsp_vertex_num >= Max_bsp_vertex )
	{
		/*
		Max_bsp_vertex=(int)((float)(Max_bsp_vertex+1)*1.3f);	//30%�� ����...
		_VERTEX *Temp=(_VERTEX *)Dmalloc(Max_bsp_vertex*sizeof(_VERTEX));
		memcpy(Temp,Bsp_vertex,sizeof(_VERTEX)*(Bsp_vertex_num-1));
		Dfree(Bsp_vertex);
		Bsp_vertex = Temp;
		*/
		int new_Max_bsp_vertex=(int)((float)(Max_bsp_vertex+1)*1.3f);	//30%�� ����...
		Bsp_vertex=(_VERTEX *)ReAlloc(Bsp_vertex,Max_bsp_vertex*sizeof(_VERTEX),new_Max_bsp_vertex*sizeof(_VERTEX));
		Max_bsp_vertex=new_Max_bsp_vertex;
//		_VERTEX *Temp=(_VERTEX *)Dmalloc(Max_bsp_vertex*sizeof(_VERTEX));
//		memcpy(Temp,Bsp_vertex,sizeof(_VERTEX)*(Bsp_vertex_num-1));
//		Dfree(Bsp_vertex);
//		Bsp_vertex = Temp;
	}
	Vector3fCopy(cross,Bsp_vertex[Bsp_vertex_num].local);
	Bsp_vertex_num++;
	return Bsp_vertex_num-1;
}
_TOOL_BSP_FACE *AddBspFace(void)
{
	Bsp_face[Bsp_face_num]=(_TOOL_BSP_FACE *)Dmalloc(sizeof(_TOOL_BSP_FACE));
	memset(Bsp_face[Bsp_face_num],0,sizeof(_TOOL_BSP_FACE));
	Bsp_face_num++;
	if( Bsp_face_num >= _MAX_BSP_FACE )
		Error("_MAX_BSP_FACE overflow","");
	return Bsp_face[Bsp_face_num-1];
}


float GetPlaneDist(Vector4f plane,Vector3f a)
{
	return plane[0]*a[0]+plane[1]*a[1]+plane[2]*a[2]-plane[3];
}
int GetPlaneSide(Vector4f plane,Vector3f a)		//	1�� ����  -1�� ����
{
	if( GetPlaneDist( plane,a) >=0 )
		return 1;
	else
		return -1;
}

void GetBestSplitPlane(_TOOL_BSP_FACE *node_bsp_face,Vector3f bb_min,Vector3f bb_max,Vector4f split)
//������ ����� ��󳽴�.
{
	Vector3f box;

//--------------- ������ ����� ��󳽴�.
	box[0] = (bb_max[0]-bb_min[0]);	//x
	box[1] = (bb_max[1]-bb_min[1]);	//y
	box[2] = (bb_max[2]-bb_min[2]);	//z

	if( box[0] > box[1] )
	{
		if( box[0] > box[2] )
		{
			split[0]=1;
			split[1]=0;
			split[2]=0;
			split[3]=bb_min[0] + box[0]/2.0f;
		}
		else
		{
			split[0]=0;
			split[1]=0;
			split[2]=1;
			split[3]=bb_min[2] + box[2]/2.0f;
		}
	}
	else
	{
		if( box[1] > box[2] )
		{
			split[0]=0;
			split[1]=1;
			split[2]=0;
			split[3]=bb_min[1] + box[1]/2.0f;
		}
		else
		{
			split[0]=0;
			split[1]=0;
			split[2]=1;
			split[3]=bb_min[2] + box[2]/2.0f;
		}
	}
//----------------------------------
}

// �� a�� b�� ���split ���� ������ ���Ѵ�.
BOOL GetCrossPoint(Vector4f split,Vector3f a,Vector3f b,Vector3f dest)
{
	float front = split[0]*a[0]+split[1]*a[1]+split[2]*a[2]-split[3];
	float back = split[0]*b[0]+split[1]*b[1]+split[2]*b[2]-split[3];

	if(front > 0 && back > 0 )
		return FALSE;
	if(front < 0 && back < 0 )
		return FALSE;

	float ratio = front/(front-back);
	dest[0] = a[0] + ratio*(b[0]-a[0]);
	dest[1] = a[1] + ratio*(b[1]-a[1]);
	dest[2] = a[2] + ratio*(b[2]-a[2]);

	return TRUE;
}


//��带 �ѹ� �߶󺻴�. �߶��� ���ũ�Ⱑ �� �۾����°�?
BOOL IsSplitBBox( Vector4f split ,Vector3f bb_min,Vector3f bb_max,Vector3f front_bb_min
	,Vector3f front_bb_max,Vector3f back_bb_min,Vector3f back_bb_max )
{
	int i,cnt,a,b;
	Vector3f bb[8],cross[4];

	Vector3fCopy( bb_min,bb[0] );	//0

	bb[1][0] = bb_max[0];			//1
	bb[1][1] = bb_min[1];
	bb[1][2] = bb_min[2];

	bb[2][0] = bb_max[0];			//2
	bb[2][1] = bb_min[1];
	bb[2][2] = bb_max[2];

	bb[3][0] = bb_min[0];			//3
	bb[3][1] = bb_min[1];
	bb[3][2] = bb_max[2];

	bb[4][0] = bb_min[0];			//4
	bb[4][1] = bb_max[1];
	bb[4][2] = bb_min[2];

	bb[5][0] = bb_max[0];			//5
	bb[5][1] = bb_max[1];
	bb[5][2] = bb_min[2];

	Vector3fCopy( bb_max,bb[6] );	//6

	bb[7][0] = bb_min[0];			//7
	bb[7][1] = bb_max[1];
	bb[7][2] = bb_max[2];


	// �ٿ�� �ڽ��� ������ ���� 
	cnt=0;
	for(i=0; i<4; i++)
	{
		a=GetPlaneSide(split,bb[i]);
		b=GetPlaneSide(split,bb[(i+1)%4]);
		if(a != b )
		{
			GetCrossPoint(split,bb[i],bb[(i+1)%4],cross[cnt]);
			cnt++;
		}
		a=GetPlaneSide(split,bb[i+4]);
		b=GetPlaneSide(split,bb[(i+1+4)%4]);
		if(a != b )
		{
			GetCrossPoint(split,bb[i+4],bb[(i+1+4)%4],cross[cnt]);
			cnt++;
		}

		a=GetPlaneSide(split,bb[i]);
		b=GetPlaneSide(split,bb[(i+4)%4]);
		if(a != b )
		{
			GetCrossPoint(split,bb[i],bb[(i+4)%4],cross[cnt]);
			cnt++;
		}
	}
	if( cnt != 3 && cnt != 4 )
	{
		Error("���� �̷���찡 ����ٴ�!","");
	}

	front_bb_min[0]=165000;		front_bb_min[1]=165000;		front_bb_min[2]=165000;
	front_bb_max[0]=-165000;	front_bb_max[1]=-165000;	front_bb_max[2]=-165000;
	back_bb_min[0]=165000;		back_bb_min[1]=165000;		back_bb_min[2]=165000;
	back_bb_max[0]=-165000;		back_bb_max[1]=-165000;		back_bb_max[2]=-165000;
	for(i=0; i<8; i++)
	{
		a=GetPlaneSide(split,bb[i]);
		if( a==1 )
		{
			front_bb_min[0] =min(bb[i][0],front_bb_min[0]);
			front_bb_min[1] =min(bb[i][1],front_bb_min[1]);
			front_bb_min[2] =min(bb[i][2],front_bb_min[2]);
			front_bb_max[0] =max(bb[i][0],front_bb_max[0]);
			front_bb_max[1] =max(bb[i][1],front_bb_max[1]);
			front_bb_max[2] =max(bb[i][2],front_bb_max[2]);
		}
		if( a==-1 )
		{
			back_bb_min[0] =min(bb[i][0],back_bb_min[0]);
			back_bb_min[1] =min(bb[i][1],back_bb_min[1]);
			back_bb_min[2] =min(bb[i][2],back_bb_min[2]);
			back_bb_max[0] =max(bb[i][0],back_bb_max[0]);
			back_bb_max[1] =max(bb[i][1],back_bb_max[1]);
			back_bb_max[2] =max(bb[i][2],back_bb_max[2]);
		}
	}
	for(i=0; i<cnt; i++)
	{
		front_bb_min[0] =min(cross[i][0],front_bb_min[0]);
		front_bb_min[1] =min(cross[i][1],front_bb_min[1]);
		front_bb_min[2] =min(cross[i][2],front_bb_min[2]);

		front_bb_max[0] =max(cross[i][0],front_bb_max[0]);
		front_bb_max[1] =max(cross[i][1],front_bb_max[1]);
		front_bb_max[2] =max(cross[i][2],front_bb_max[2]);

		back_bb_min[0] =min(cross[i][0],back_bb_min[0]);
		back_bb_min[1] =min(cross[i][1],back_bb_min[1]);
		back_bb_min[2] =min(cross[i][2],back_bb_min[2]);

		back_bb_max[0] =max(cross[i][0],back_bb_max[0]);
		back_bb_max[1] =max(cross[i][1],back_bb_max[1]);
		back_bb_max[2] =max(cross[i][2],back_bb_max[2]);
	}
	for(i=0; i<3; i++)	//������ �ٿ�� �ڽ� �𼭸��� �ּұ��̺��� ������ ������ ����.
	{
		if( front_bb_max[i] - front_bb_min[i] < _MIN_BBOX_EDGE )
			return FALSE;
		if( back_bb_max[i] - back_bb_min[i] < _MIN_BBOX_EDGE )
			return FALSE;
	}
	return TRUE;
}


_TOOL_BSP_FACE *Front_face = NULL;
_TOOL_BSP_FACE *Back_face = NULL;
_TOOL_BSP_FACE *Start_front_face = NULL;
_TOOL_BSP_FACE *Start_back_face = NULL;
int	Front_face_num,Back_face_num;

static void AddFaceSplitNode(_TOOL_BSP_FACE *face,Vector4f split)	//front_face�� back_face�� �����͸� ä���� �������ش�.
{
	int j;
	Front_face = NULL;
	Back_face = NULL;

	Front_face_num=0;
	Back_face_num=0;
	while( face != NULL )
	{
		_TOOL_BSP_FACE *n_face=face->next;	//�̸� �ؽ�Ʈ�� ����.
		DWORD f_vertex[64];	//���� �� �ε��� 
		DWORD b_vertex[64];	//���� �� �ε���

		DWORD f_v_num=0,b_v_num=0;
		int front_style=0;
		if( GetPlaneSide(split,Bsp_vertex[face->v_list[0]].local)==1 )
			front_style=1;
		else
			front_style=0;

		//---------------�߷��� ���� ����.
		BOOL is_split=FALSE;
		for(j=0; j<face->v_num; j++)
		{
			_VERTEX *v1 = &Bsp_vertex[face->v_list[j]];
			_VERTEX *v2 = &Bsp_vertex[face->v_list[(j+1)%face->v_num]];
			int side1 = GetPlaneSide(split,v1->local);
			int side2 = GetPlaneSide(split,v2->local);
			_VERTEX cross;
			DWORD nv_num;	//���� ���� ���ؽ� ��ȣ
	
			if( side1 + side2 == 0)	//��鿡 ���� �߷�����...
			{
				GetCrossPoint(split,v1->local,v2->local,cross.local);
				nv_num=AddBspVertex(cross.local);
				is_split=TRUE;
			}

			if(side1 == 1)	//front
			{
				f_vertex[f_v_num]=face->v_list[j];
				f_v_num++;
				if( side1+side2 == 0 && front_style == 0)	//��鿡 ���� �߷�����...
				{
					f_vertex[f_v_num]=nv_num;
					f_v_num++;
					b_vertex[b_v_num]=nv_num;
					b_v_num++;
				}
				if( side1+side2 == 0 && front_style == 1)	//��鿡 ���� �߷�����...
				{
					f_vertex[f_v_num]=nv_num;
					f_v_num++;
					b_vertex[b_v_num]=nv_num;
					b_v_num++;
				}
			}
			else		//back
			{
				b_vertex[b_v_num]=face->v_list[j];			
				b_v_num++;
				if( side1+side2 == 0 && front_style == 1)	//��鿡 ���� �߷�����...
				{
					b_vertex[b_v_num]=nv_num;
					b_v_num++;
					f_vertex[f_v_num]=nv_num;
					f_v_num++;
				}
				if( side1+side2 == 0 && front_style == 0)	//��鿡 ���� �߷�����... ����
				{
					b_vertex[b_v_num]=nv_num;
					b_v_num++;
					f_vertex[f_v_num]=nv_num;
					f_v_num++;
				}
			}
			if( f_v_num >= 32 ||  b_v_num >= 32 )
			{
				Error("�ִ� ���� �� 32�� �Ѿ���","");
			}
			//----------------------------------------------
		}
		if( is_split )	//��鿡 ���� �߷��� ���̸�.
		{
			Front_face_num++;
			Back_face_num++;
			//---------------���� �߷��� ������ �����.
			if(Front_face==NULL)
			{
				Start_front_face = face;
				face->before=NULL;
				face->next=NULL;
				Front_face=face;
			}
			else
			{
				Front_face->next = face;
				face->before=Front_face;
				face->next=NULL;
				Front_face=face;
			}
			if(Back_face==NULL)
			{
				Back_face=AddBspFace();
				Start_back_face = Back_face;
				Back_face->before=NULL;
				Back_face->next=NULL;
			}
			else
			{
				Back_face->next = AddBspFace();
				Back_face->next->before=Back_face;
				Back_face->next->next=NULL;
				Back_face=Back_face->next;
			}
			Back_face->org_facenum = face->org_facenum;	//���� ����ſ� ���̽����� ī��
			Front_face->v_num = f_v_num;
			memcpy(Front_face->v_list,f_vertex,sizeof(LONG)*f_v_num);
			Back_face->v_num = b_v_num;
			memcpy(Back_face->v_list,b_vertex,sizeof(LONG)*b_v_num);
		}
		else
		if( front_style == 1)	//��鿡 front ���̸�.
		{
			Front_face_num++;
			if(Front_face==NULL)
			{
				Start_front_face = face;
				face->before=NULL;
				face->next=NULL;
				Front_face=face;
			}
			else
			{
				Front_face->next = face;
				face->before=Front_face;
				face->next=NULL;
				Front_face=face;
			}
		}
		else		//��鿡 back ���̸�.
		{
			Back_face_num++;
			if(Back_face==NULL)
			{
				Start_back_face = face;
				face->before=NULL;
				face->next=NULL;
				Back_face=face;
			}
			else
			{
				Back_face->next = face;
				face->before=Back_face;
				face->next=NULL;
				Back_face=face;
			}
		}
		face = n_face;
	}
}



LONG DivideNode(_TOOL_BSP_FACE *node_bsp_face,DWORD facenum,Vector3f bb_min,Vector3f bb_max)	//��带 �и��ϴ� ��Ȱ
{
	Vector3f front_bb_max,front_bb_min;
	Vector3f back_bb_max,back_bb_min;
	BOOL is_true;
	Vector4f split;

	//----���̻� ���� �������ʰ� ������ �Ǿ���� ��Ȳ�̸�
	if( Node_num != 1 )
	{
		if( facenum <= 0 )	//��峻�� ���� ����.
		{
			return 0;	//0�� ����(�ƹ��͵� ���� ����)�� �����Ѵ�.
		}
		if( facenum < _MIN_FACE ) //��峻�� ���� �ּ�ġ���� ����.
		{
			return -1;	//���ο� ������ �ɰ��̴�.
		}
	}
	GetBestSplitPlane( node_bsp_face,bb_min,bb_max,split); //�ڸ��� ������ ����� �������.
		//--front,back �ٿ�� �ڽ� ���۹� �ڸ���� �ٿ�� �ڽ��� �𼭸��� �ʹ�������...
	is_true=IsSplitBBox( split ,bb_min, bb_max,front_bb_min,front_bb_max,back_bb_min,back_bb_max );
	if( is_true == FALSE && Node_num != 1)	//�ڸ��� �ȵǴ� ��Ȳ�̶� LEAF�� ...
	{
		return -1;	//���ο� ������ �ɰ��̴�.
	}

	//--------------------------------------------
	AddNode(Node_num);
	Vector4fCopy( split, Node[Node_num].plane );		//��带 ������ ��� ������
	Vector3fCopy( bb_min,Node[Node_num].bb_min );		//�ٿ�� �ڽ�
	Vector3fCopy( bb_max,Node[Node_num].bb_max );		//�ٿ�� �ڽ�
	Node[Node_num].face_num = facenum;

	int stack_node_num=Node_num;
	Node_num++;		//��带 ���� ��Ų��.

	//----------------��������� split ��鿡���� ���� ������.
	AddFaceSplitNode(node_bsp_face,split);	//start_front_face�� start_back_face�� �����͸� ä���� �������ش�.

	//���������� ���ȣ��� ������.
	_TOOL_BSP_FACE *start_front_face = Start_front_face;
	_TOOL_BSP_FACE *start_back_face = Start_back_face;
	int	front_face_num=Front_face_num;
	int	back_face_num=Back_face_num;

	LONG ret;
	ret = DivideNode(start_front_face,front_face_num,front_bb_min,front_bb_max);
	if( ret == 0 )	//�������� 0�� ����(�ϰ͵� ���� ����)�� �����µ� �������� ������ �ڽ��� ������ �־�ߵȴ�.
	{
//		Node[stack_node_num].front = -1;
		Node[stack_node_num].front = -Leaf_num-1;
		AddLeaf(Leaf_num);
		Leaf[Leaf_num].face_num = 0;
		Leaf[Leaf_num].face = start_front_face;
		Vector3fCopy(front_bb_min,Leaf[Leaf_num].bb_min);
		Vector3fCopy(front_bb_max,Leaf[Leaf_num].bb_max);
		Leaf_num++;
	}
	else
	if( ret < 0 )	//���̻� �������� ������ ������.
	{
		Node[stack_node_num].front = -Leaf_num-1;
		AddLeaf(Leaf_num);
		Leaf[Leaf_num].face_num = front_face_num;
		Leaf[Leaf_num].face = start_front_face;
		Vector3fCopy(front_bb_min,Leaf[Leaf_num].bb_min);
		Vector3fCopy(front_bb_max,Leaf[Leaf_num].bb_max);
		Leaf_num++;
	}
	else
		Node[stack_node_num].front = ret;

	ret = DivideNode(start_back_face,back_face_num,back_bb_min,back_bb_max);

	if( ret == 0 )	//�������� 0�� ����(�ϰ͵� ���� ����)�� �����µ� �������� ������ �ڽ��� ������ �־�ߵȴ�.
	{
//		Node[stack_node_num]->back = -1;
		Node[stack_node_num].back = -Leaf_num-1;
		AddLeaf(Leaf_num);
		Leaf[Leaf_num].face_num = 0;
		Leaf[Leaf_num].face = start_back_face;
		Vector3fCopy(back_bb_min,Leaf[Leaf_num].bb_min);
		Vector3fCopy(back_bb_max,Leaf[Leaf_num].bb_max);
		Leaf_num++;
	}
	else
	if( ret < 0 )	//���̻� �������� ������ ������.
	{
		Node[stack_node_num].back = -Leaf_num-1;
		AddLeaf(Leaf_num);
		Leaf[Leaf_num].face_num = back_face_num;
		Leaf[Leaf_num].face = start_back_face;
		Vector3fCopy(back_bb_min,Leaf[Leaf_num].bb_min);
		Vector3fCopy(back_bb_max,Leaf[Leaf_num].bb_max);
		Leaf_num++;
	}
	else
		Node[stack_node_num].back = ret;

	return stack_node_num;
	//�ٽ� ȣ��
}

void MakeBSPTree(_MESH *mesh)	//���� ��¥�޽���.
{
	int i,j;
	_OBJECT *obj=mesh->object;

	Org_vertex_num = mesh->object->vertexnum;
	Org_face_num = mesh->object->facenum;

	if( mesh->poly_type != TRUE )
		Error("convert ������ �޽�","");

	InitBspMalloc(mesh);

	//---node�� ó�� 0���� ������. 
	AddNode(0);
	Node_num=1;

	//---leaf�� ó�� 0���� ������. 
	AddLeaf(0);
	Leaf_num=1;

	//------------------ù �ٿ�� �ڽ��� ���Ѵ�.
	Vector3f bb_min={65000,65000,65000};
	Vector3f bb_max={-65000,-65000,-65000};

	for(i=0; i<obj->vertexnum;i++)
	{
		for(j=0; j<3; j++)
		{
			if( obj->vertex[i].local[j] < bb_min[j] )
				bb_min[j]=obj->vertex[i].local[j];
			if( obj->vertex[i].local[j] > bb_max[j] )
				bb_max[j]=obj->vertex[i].local[j];
		}
	}
	//-----------------------------------------
	for(i=0; i<obj->vertexnum; i++)
	{
		AddBspVertex(obj->vertex[i].local);
	}
	//Bsp_vertex_num = obj->vertexnum;
	//������ BSP FACE����Ʈ�� �����.
	_TOOL_BSP_FACE *temp = (_TOOL_BSP_FACE *)malloc(sizeof(_TOOL_BSP_FACE)*obj->facenum);
	memset(temp,0,sizeof(_TOOL_BSP_FACE)*obj->facenum);
	for(i=0; i<obj->facenum; i++)
	{
		Bsp_face[i] = temp+i;
		memset(Bsp_face[i],0,sizeof(_TOOL_BSP_FACE));
	}
	//Bsp_face_num = obj->facenum;
 	for(i=0; i<obj->facenum; i++)	
	{

		Bsp_face[i]->org_facenum=i;
		Bsp_face[i]->v_num = obj->p_face[i].vertex_num;
		for(j=0; j<obj->p_face[i].vertex_num; j++)
		{
			Bsp_face[i]->v_list[j] = obj->p_face[i].vertex_id[j];
		}
		if( i!=0 )
			Bsp_face[i]->before = Bsp_face[i-1];
		if( i!=obj->facenum-1 )
			Bsp_face[i]->next = Bsp_face[i+1];
	}

	DivideNode(Bsp_face[0],obj->facenum,bb_min,bb_max);

    PutR3BspInfoText("Bsp Compile Complete!\n");
}

//-------------------------------- bsp ��Ÿ ��ƿ
void BspPrint(void)
{
	int i;
	char buf[512];

	sprintf(buf,"Dmalloc size = %4.2fMByte\n",GetDmallocSize()/1000000.0f);
//	sprintf(buf,"Dmalloc size = %8dk\n",GetDmallocSize()/1000);
	PutR3BspInfoText(buf);
	R3BspInfoTextState(1);
    PutR3BspInfoText("--------------- Bsp print ------------\n");
	sprintf(buf,"Node num = %8d\n",Node_num);
	PutR3BspInfoText(buf);
	sprintf(buf,"Leaf num = %8d\n",Leaf_num);
	PutR3BspInfoText(buf);
	sprintf(buf,"Org vertex num = %8d,Bsp vertex num = %8d\n",Org_vertex_num,Bsp_vertex_num);
	PutR3BspInfoText(buf);
	sprintf(buf,"Org face num = %8d,Bsp face num = %8d\n",Org_face_num,Bsp_face_num);
	PutR3BspInfoText(buf);

    PutR3BspInfoText("--- Node print ---\n");
	for( i=0; i<Node_num; i++)
	{
		sprintf(buf,"Node %6d = %6d faces",i,Node[i].face_num);
	    PutR3BspInfoText(buf);
		if(Node[i].front < 0)
		{
			sprintf(buf,",front leaf index = %6d",-Node[i].front-1);
			PutR3BspInfoText(buf);
		}
		else
		{
			sprintf(buf,",front node index = %6d",Node[i].front);
			PutR3BspInfoText(buf);
		}
		if(Node[i].back < 0)
		{
			sprintf(buf,",back leaf index = %6d",-Node[i].back-1);
			PutR3BspInfoText(buf);
		}
		else
		{
			sprintf(buf,",back node index = %6d",Node[i].back);
			PutR3BspInfoText(buf);
		}
	    PutR3BspInfoText("\n");
	}
    PutR3BspInfoText("--- Leaf print ---\n");
	for( i=0; i<Leaf_num; i++)
	{
		sprintf(buf,"Leaf %6d = %6d,bbmin = %5d,%5d,%5d ,bbmax = %5d,%5d,%5d\n",i,Leaf[i].face_num
			,(int)Leaf[i].bb_min[0],(int)Leaf[i].bb_min[1],(int)Leaf[i].bb_min[2]
			,(int)Leaf[i].bb_max[0],(int)Leaf[i].bb_max[1],(int)Leaf[i].bb_max[2]);
	    PutR3BspInfoText(buf);
	}
	R3BspInfoTextState(0);
}

void DrawBspSplitPolygon(void)		//bsp leaf ������ ǥ�� 
{
	int i,j;
	Vector3f c;

	glBegin(GL_TRIANGLES);
/*
	for(i=0; i<Bsp_face_num; i++)
	{
		if( Bsp_face[i]==NULL)
			continue;
		for(j=0; j<Bsp_face[i].v_num-2; j++)
		{
		c[0]=(rand()%128)/256.0+.5;	c[1]=(rand()%128)/256.0+.5;	c[2]=(rand()%128)/256.0+.5;
		glColor3f(c[0], c[1], c[2]);
			int v0=Bsp_face[i].v_list[0];
			int v1=Bsp_face[i].v_list[j+1];
			int v2=Bsp_face[i].v_list[j+2];
			glVertex3f(Bsp_vertex[v0].local[0],Bsp_vertex[v0].local[1],Bsp_vertex[v0].local[2]);
			glVertex3f(Bsp_vertex[v1].local[0],Bsp_vertex[v1].local[1],Bsp_vertex[v1].local[2]);
			glVertex3f(Bsp_vertex[v2].local[0],Bsp_vertex[v2].local[1],Bsp_vertex[v2].local[2]);
		}
	}
*/
	int cnt=0;
	int mv_num=0;
	for( i=0; i<Leaf_num; i++)
	{
		_TOOL_BSP_FACE *face=Leaf[i].face;
		while(face)
		{
			cnt+=63;
			c[0]=(cnt%128)/256.0f+.5f;	
			cnt+=13;
			c[1]=(cnt%128)/256.0f+.5f;
			cnt+=71;
			c[2]=(cnt%128)/256.0f+.5f;
			glColor3f(c[0], c[1], c[2]);
			mv_num=max(face->v_num,mv_num);
			for(j=0; j<face->v_num-2; j++)
			{
				int v0=face->v_list[0];
				int v1=face->v_list[j+1];
				int v2=face->v_list[j+2];
				glVertex3f(Bsp_vertex[v0].local[0],Bsp_vertex[v0].local[1],Bsp_vertex[v0].local[2]);
				glVertex3f(Bsp_vertex[v1].local[0],Bsp_vertex[v1].local[1],Bsp_vertex[v1].local[2]);
				glVertex3f(Bsp_vertex[v2].local[0],Bsp_vertex[v2].local[1],Bsp_vertex[v2].local[2]);
			}
			face=face->next;
		}
	}
	glEnd();
}

void DrawBspLeafBBox(void)		//bsp leaf �ٿ�� �ڽ� ǥ�� 
{
	int i;

	glColor3f(1,1,1);
	glBegin(GL_LINES);
	for( i=0; i<Leaf_num; i++)
	{		
		glVertex3f(Leaf[i].bb_min[0],Leaf[i].bb_min[1],Leaf[i].bb_min[2]);
		glVertex3f(Leaf[i].bb_max[0],Leaf[i].bb_min[1],Leaf[i].bb_min[2]);

		glVertex3f(Leaf[i].bb_max[0],Leaf[i].bb_min[1],Leaf[i].bb_min[2]);
		glVertex3f(Leaf[i].bb_max[0],Leaf[i].bb_max[1],Leaf[i].bb_min[2]);

		glVertex3f(Leaf[i].bb_max[0],Leaf[i].bb_max[1],Leaf[i].bb_min[2]);
		glVertex3f(Leaf[i].bb_min[0],Leaf[i].bb_max[1],Leaf[i].bb_min[2]);

		glVertex3f(Leaf[i].bb_min[0],Leaf[i].bb_max[1],Leaf[i].bb_min[2]);
		glVertex3f(Leaf[i].bb_min[0],Leaf[i].bb_min[1],Leaf[i].bb_min[2]);


		glVertex3f(Leaf[i].bb_min[0],Leaf[i].bb_min[1],Leaf[i].bb_min[2]);
		glVertex3f(Leaf[i].bb_min[0],Leaf[i].bb_min[1],Leaf[i].bb_max[2]);

		glVertex3f(Leaf[i].bb_max[0],Leaf[i].bb_min[1],Leaf[i].bb_min[2]);
		glVertex3f(Leaf[i].bb_max[0],Leaf[i].bb_min[1],Leaf[i].bb_max[2]);

		glVertex3f(Leaf[i].bb_max[0],Leaf[i].bb_max[1],Leaf[i].bb_min[2]);
		glVertex3f(Leaf[i].bb_max[0],Leaf[i].bb_max[1],Leaf[i].bb_max[2]);

		glVertex3f(Leaf[i].bb_min[0],Leaf[i].bb_max[1],Leaf[i].bb_min[2]);
		glVertex3f(Leaf[i].bb_min[0],Leaf[i].bb_max[1],Leaf[i].bb_max[2]);

		
		glVertex3f(Leaf[i].bb_min[0],Leaf[i].bb_min[1],Leaf[i].bb_max[2]);
		glVertex3f(Leaf[i].bb_max[0],Leaf[i].bb_min[1],Leaf[i].bb_max[2]);

		glVertex3f(Leaf[i].bb_max[0],Leaf[i].bb_min[1],Leaf[i].bb_max[2]);
		glVertex3f(Leaf[i].bb_max[0],Leaf[i].bb_max[1],Leaf[i].bb_max[2]);

		glVertex3f(Leaf[i].bb_max[0],Leaf[i].bb_max[1],Leaf[i].bb_max[2]);
		glVertex3f(Leaf[i].bb_min[0],Leaf[i].bb_max[1],Leaf[i].bb_max[2]);

		glVertex3f(Leaf[i].bb_min[0],Leaf[i].bb_max[1],Leaf[i].bb_max[2]);
		glVertex3f(Leaf[i].bb_min[0],Leaf[i].bb_min[1],Leaf[i].bb_max[2]);

	}
	glEnd();
}


void CalculateSplitFaceFlag(_MESH *mesh)
{
	int i,j;

	for(i=0; i<mesh->object->facenum; i++)
	{
		mesh->object->p_face[i].temp=0;
	}
	for(i=0; i<Leaf_num; i++)
	{
		_TOOL_BSP_FACE *t_face = Leaf[i].face;

		for(j=0; j<Leaf[i].face_num; j++)
		{
			mesh->object->p_face[t_face->org_facenum].temp++;
			t_face = t_face->next;
		}
	}
	int cnt=0;
	for(i=0; i<mesh->object->facenum; i++)
	{
		if( mesh->object->p_face[i].temp == 0 )
			Error("��� �̷� �� ���� ��찡 ","");
		else
		if( mesh->object->p_face[i].temp > 1 )
			mesh->object->p_face[i].temp |= _SPLIT_FACE_FLAG;
		else
			cnt++;
	}
}

WORD *MakeMBlockListInLeaf(_BSP_LEAF *b_leaf,R3BlockBsp *block_bsp,DWORD *cnt)
{

#define _MALLOC_BLOCK 10000 //����� ������...

	int i,j;
	int start_id=0,n_size;
	//������ ����Ʈ�� �����.
	WORD *leaf_list = (WORD *)Dmalloc(_MALLOC_BLOCK*sizeof(WORD));
	n_size = _MALLOC_BLOCK;

/*
	_OptObject *op_bsp = block_bsp->mOptObject;
	for(j=0; j<block_bsp->mOptObjNum; j++)	//������ ��Ȯ�� �ִ�.
	{
		if( j==534 )
			break;
		for(i=0; i<Leaf_num; i++)
		{
			if( op_bsp->bb_max[0] >= Leaf[i].bb_min[0] && op_bsp->bb_min[0] <= Leaf[i].bb_max[0] 
			&& op_bsp->bb_max[1] >= Leaf[i].bb_min[1] && op_bsp->bb_min[1] <= Leaf[i].bb_max[1] 
			&& op_bsp->bb_max[2] >= Leaf[i].bb_min[2] && op_bsp->bb_min[2] <= Leaf[i].bb_max[2] )
			{
				int dfsdfsdf=0;
			}
		}
		op_bsp = op_bsp->next;
	}
	for(i=1; i<Leaf_num; i++)
	{
		if( op_bsp->bb_max[0] >= Leaf[i].bb_min[0] && op_bsp->bb_min[0] <= Leaf[i].bb_max[0] 
		&& op_bsp->bb_max[1] >= Leaf[i].bb_min[1] && op_bsp->bb_min[1] <= Leaf[i].bb_max[1] 
		&& op_bsp->bb_max[2] >= Leaf[i].bb_min[2] && op_bsp->bb_min[2] <= Leaf[i].bb_max[2] )
		{
			int dfsdfsdf=0;
		}
	}

*/	
	for(i=0; i<Leaf_num; i++)
	{
		_OptObject *op_bsp = block_bsp->mOptObject;
		b_leaf[i].m_group_start_id = start_id;//���� ���͸���׷� 
		b_leaf[i].m_group_num=0;
		for(j=0; j<block_bsp->mOptObjNum; j++)	//������ ��Ȯ�� �ִ�.
		{
			if( op_bsp->bb_max[0] >= Leaf[i].bb_min[0] && op_bsp->bb_min[0] <= Leaf[i].bb_max[0] 
			&& op_bsp->bb_max[1] >= Leaf[i].bb_min[1] && op_bsp->bb_min[1] <= Leaf[i].bb_max[1] 
			&& op_bsp->bb_max[2] >= Leaf[i].bb_min[2] && op_bsp->bb_min[2] <= Leaf[i].bb_max[2] )
			{
				leaf_list[start_id] = (WORD)j;
				b_leaf[i].m_group_num++;
				start_id++;
				if( start_id >= n_size )	//��ġ�� ���Ҵ�� ī��..
				{
					 leaf_list = (WORD *)ReAlloc(leaf_list,n_size*sizeof(WORD)
						 ,(n_size+_MALLOC_BLOCK)*sizeof(WORD));
		 			 n_size += _MALLOC_BLOCK;
				}
			}
			op_bsp = op_bsp->next;
		}
	}
	*cnt=(DWORD)start_id;
	return leaf_list;
}

#define MinWhatAddSu(a)		( -0.999f )
#define MaxWhatAddSu(a)		( 0.999f )

static _BSP_FILE_HEADER BSP_Header;
void BspExport(_MESH *c_mesh,_MESH *m_mesh,char *name,R3BlockBsp *block_bsp)
{
	int i,j,k,cnt,is_exist,f_normal_cnt,offset;

	if( c_mesh->poly_type == FALSE )
		Error("����Ʈ�ϰ� ��!","");
	
	//---------face��� �غ�
	cnt=0;
	Vector3f *f_normal=(Vector3f*)Dmalloc(sizeof(Vector3f)*(c_mesh->object->facenum+8));
/*
	for(i=0; i<c_mesh->object->facenum; i++)
	{
		is_exist = -1;

		for(j=0; j<cnt; j++)
		{
			if(c_mesh->object->p_face[i].f_normal[0] == f_normal[j][0]
			&& c_mesh->object->p_face[i].f_normal[1] == f_normal[j][1]
			&& c_mesh->object->p_face[i].f_normal[2] == f_normal[j][2])
			{
				is_exist = j;
				break;
			}
		}
		if( is_exist == -1)
		{
			f_normal[cnt][0] = c_mesh->object->p_face[i].f_normal[0];
			f_normal[cnt][1] = c_mesh->object->p_face[i].f_normal[1];
			f_normal[cnt][2] = c_mesh->object->p_face[i].f_normal[2];
			c_mesh->object->p_face[i].f_normal_id = cnt;
			cnt++;
			if( cnt>65535)
				Error("���̽���� 65536 �����÷ο�","");
		}
		else
		{
			c_mesh->object->p_face[i].f_normal_id = (DWORD)is_exist;
		}
	}*/
	for(i=0; i<Node_num; i++)	//��带 ������ ����� �������� ����Ѵ�.
	{		
		is_exist = -1;

		for(j=0; j<cnt; j++)
		{
			if(Node[i].plane[0] == f_normal[j][0]
			&& Node[i].plane[1] == f_normal[j][1]
			&& Node[i].plane[2] == f_normal[j][2])
			{
				is_exist = j;
				break;
			}
		}
		if( is_exist == -1)
		{
			f_normal[cnt][0] = Node[i].plane[0];
			f_normal[cnt][1] = Node[i].plane[1];
			f_normal[cnt][2] = Node[i].plane[2];
			Node[i].f_normal_id = cnt;
			cnt++;
			if(cnt >= c_mesh->object->facenum+8)
				Error("��ֺ��� ����Ʈ�� overflow!","");
		}
		else
		{
			Node[i].f_normal_id = (DWORD)is_exist;
		}
	}
	f_normal_cnt = cnt;

	//---------- edge�� edge list �غ� face�� �غ�
	int v_id_num=0;		//���ؽ� ���̵� ����
	for(i=0; i<c_mesh->object->facenum; i++)	//�ϴ� �ִ� ���ؽ� �ε��� ������ ���Ѵ�.
	{
		v_id_num += c_mesh->object->p_face[i].vertex_num;
	}
//	WORD *vertex_id=(WORD*)Dmalloc(sizeof(WORD)*(v_id_num));
	_BSP_C_FACE *b_face = (_BSP_C_FACE *)Dmalloc(sizeof(_BSP_C_FACE)*c_mesh->object->facenum);	//�� 

	v_id_num=0;
	for(i=0; i<c_mesh->object->facenum; i++)
	{
		b_face[i].d = c_mesh->object->p_face[i].f_normal[3];
		b_face[i].f_normal_id = (DWORD)c_mesh->object->p_face[i].f_normal_id;
		b_face[i].vertex_num = (WORD)c_mesh->object->p_face[i].vertex_num;
		b_face[i].vertex_start_id = v_id_num;
//		b_face[i].color = c_mesh->object->p_face[i].argb;		//���÷�..

		for(k=0; k<c_mesh->object->p_face[i].vertex_num; k++)
		{
//			if( c_mesh->object->p_face[i].vertex_id[k] > 65536)
//				int dfdjf=0;
//			vertex_id[v_id_num] = (WORD)c_mesh->object->p_face[i].vertex_id[k];
			v_id_num++;
		}

	}
	
	//-----uv��ǥ �غ�
	Vector2f *uv = (Vector2f *)Dmalloc(sizeof(Vector2f)*v_id_num);
	Vector2f *lgt_map_uv=(Vector2f *)Dmalloc(sizeof(Vector2f)*(v_id_num));
	cnt=0;
	for(i=0; i<c_mesh->object->facenum; i++)
	{
		for(k=0; k<c_mesh->object->p_face[i].vertex_num; k++)
		{
			uv[cnt][0] = c_mesh->object->p_face[i].fu[k];
			uv[cnt][1] = c_mesh->object->p_face[i].fv[k];
			lgt_map_uv[cnt][0] =c_mesh->object->p_face[i].lu[k];//����Ʈ�� ����.
			lgt_map_uv[cnt][1] =c_mesh->object->p_face[i].lv[k];
			cnt++;
		}
	}

	//-------------- ���̽� �׷� ����
	CalculateSplitFaceFlag(c_mesh );	//p_face->temp �� ���ø��Ǵ� �÷��׸� Ų��.

	//------ �̰� face index
	cnt=0;
	for(i=0; i<Leaf_num; i++)	//b_face_id ������ ������
		cnt+=Leaf[i].face_num;
	DWORD *b_face_id = (DWORD *)Dmalloc(sizeof(DWORD)*cnt);

/*
		// �ִ��ǰ�츦 �����ϱ�����
	_BSP_F_GROUP *f_group = (_BSP_F_GROUP *)Dmalloc(sizeof(_BSP_F_GROUP)*cnt);
	memset(f_group,0,sizeof(_BSP_F_GROUP)*cnt);
	LONG *f_group_id = (LONG *)Dmalloc(sizeof(LONG)*cnt);
	int f_group_id_num = cnt;


	//----ó���� �׽�Ʈ���̶� ���̽��Ѱ��� �Ѱ��� �׷��̴�.
	f_group_num = c_mesh->object->facenum;
	for(i=0; i<f_group_num; i++)
	{
		f_group[i].face_num=1;
		f_group[i].face_start_id=i;
		//f_group[i].lgt_id=-1;
		//f_group[i].lgt_id=c_mesh->object->p_face[i].l_mtlid;
		b_face_id[i] = i;
		//f_group[i].attr =; ���� �Ӽ��� �ʿ����.
		f_group[i].mtl_id = c_mesh->object->p_face[i].mtlid;
	}
*/

	//------------��� ��Ʈ��
	_BSP_NODE *b_node = (_BSP_NODE *)Dmalloc(sizeof(_BSP_NODE)*Node_num);
	for(i=0; i<Node_num; i++)
	{
		if( Node[i].front <-32767 || Node[i].front > 32767 )
			Error("�ִ��带 �����ߴ�.","");
		if( Node[i].back <-32767 || Node[i].back > 32767 )
			Error("�ִ��带 �����ߴ�.","");

		b_node[i].front = (SHORT)Node[i].front;
		b_node[i].back = (SHORT)Node[i].back;
		b_node[i].bb_min[0] = (short)MinFixFloatToInt(Node[i].bb_min[0]);	//�� ũ��
		b_node[i].bb_min[1] = (short)MinFixFloatToInt(Node[i].bb_min[1]);	//�� ũ��
		b_node[i].bb_min[2] = (short)MinFixFloatToInt(Node[i].bb_min[2]);	//�� ũ��

		b_node[i].bb_max[0] = (short)MaxFixFloatToInt(Node[i].bb_max[0]);	//�� ũ��
		b_node[i].bb_max[1] = (short)MaxFixFloatToInt(Node[i].bb_max[1]);	//�� ũ��
		b_node[i].bb_max[2] = (short)MaxFixFloatToInt(Node[i].bb_max[2]);	//�� ũ��
		b_node[i].d = Node[i].plane[3];
		b_node[i].f_normal_id =(DWORD)Node[i].f_normal_id; 
	}

	//------------���� ���
	_BSP_LEAF *b_leaf = (_BSP_LEAF *)Dmalloc(sizeof(_BSP_LEAF)*Leaf_num);

	cnt=0;
	for(i=0; i<Leaf_num; i++)
	{
		b_leaf[i].bb_min[0] = (short)MinFixFloatToInt(Leaf[i].bb_min[0]);	//�� ũ��
		b_leaf[i].bb_min[1] = (short)MinFixFloatToInt(Leaf[i].bb_min[1]);	//�� ũ��
		b_leaf[i].bb_min[2] = (short)MinFixFloatToInt(Leaf[i].bb_min[2]);	//�� ũ��
		b_leaf[i].bb_max[0] = (short)MaxFixFloatToInt(Leaf[i].bb_max[0]);	//�� ũ��
		b_leaf[i].bb_max[1] = (short)MaxFixFloatToInt(Leaf[i].bb_max[1]);	//�� ũ��
		b_leaf[i].bb_max[2] = (short)MaxFixFloatToInt(Leaf[i].bb_max[2]);	//�� ũ��
		//------------ �浹 ���̽���..
		_TOOL_BSP_FACE *t_face = Leaf[i].face;
		b_leaf[i].face_start_id = cnt;

//		if( i < 1700)
		for(j=0; j<Leaf[i].face_num; j++)
		{
			b_face_id[cnt] = (DWORD)t_face->org_facenum;
			t_face = t_face->next;
			cnt++;
		}
		b_leaf[i].face_num = Leaf[i].face_num;
		b_leaf[i].type = Leaf[i].type;
	}

//--------�� �ִϸ��̼� ����.

	int d_obj_num,track_size;

	_D_OBJECT *object = AllocDObject(m_mesh);	//���̳��� ������Ʈ ���� ����.
	BYTE *ani_track = AllocTrack(m_mesh);			// �ִϸ��̼� Ʈ������.
	WORD *event_ani_id=NULL;
	d_obj_num=GetDObjectNum(m_mesh);
	track_size=GetTrackSize(m_mesh);

	if( d_obj_num )
	{
		event_ani_id=(WORD *)Dmalloc(sizeof(WORD)*d_obj_num);
		for(i=0; i<d_obj_num; i++)
		{
			event_ani_id[i]=(WORD)GetEventDObjectID(m_mesh,i);
		}
	}
	StoreDObjectAndTrack(object,ani_track,m_mesh);

//------------------------------------- ��Ʈ������ -------------------------------
	BSP_Header.version = _R3BSP_VERSION;
	offset=sizeof(_BSP_FILE_HEADER);

	//---------�� 
//	BSP_Header.Cvertex.size = sizeof(Vector3f)*c_mesh->object->vertexnum;
//	BSP_Header.Cvertex.offset = offset;
//	offset += BSP_Header.Cvertex.size;

	//-------- ����� ������
	BSP_Header.Cplanes.size = sizeof(Vector3f)*f_normal_cnt;
	BSP_Header.Cplanes.offset = offset;
	offset += BSP_Header.Cplanes.size;

	//------------���ؽ� id
//	BSP_Header.Cvertex_id.size = sizeof(WORD)*v_id_num;
//	BSP_Header.Cvertex_id.offset=offset;
//	offset+=BSP_Header.Cvertex_id.size;

	//------------�� ��Ʈ��
//	BSP_Header.Cface.size=sizeof(_BSP_C_FACE)*c_mesh->object->facenum;
//	BSP_Header.Cface.offset=offset;
//	offset += BSP_Header.Cface.size;

	//------------- ���̽� ���̵� ��Ʈ��
	BSP_Header.Cface_id.size = sizeof(DWORD)*cnt;
	BSP_Header.Cface_id.offset = offset;
	offset += BSP_Header.Cface_id.size;

	//------------ ��� ��Ʈ��
	BSP_Header.node.size = sizeof(_BSP_NODE)*Node_num;
	BSP_Header.node.offset = offset;
	offset += BSP_Header.node.size;

	//------------- ���� ��Ʈ��
	BSP_Header.leaf.size = sizeof(_BSP_LEAF)*Leaf_num;
	BSP_Header.leaf.offset = offset;
	offset += BSP_Header.leaf.size;


//------------------------------��bsp ����...-------------------------------------------
	_BSP_M_GROUP * bsp_m_group = (_BSP_M_GROUP *)Dmalloc(sizeof(_BSP_M_GROUP)*block_bsp->mOptObjNum);
	memset(bsp_m_group,0,sizeof(_BSP_M_GROUP)*block_bsp->mOptObjNum);

	CreateBSPRenderPart(bsp_m_group,m_mesh,block_bsp);	//render part�� ä��� ������ ������Ʈ���� �����.

	DWORD b_list_in_leaf_cnt;
	WORD *b_list_in_leaf= MakeMBlockListInLeaf(b_leaf,block_bsp,&b_list_in_leaf_cnt);

	//--------���͸��� �� ���� ���
	BSP_Header.MatListInLeaf.size = sizeof(WORD)*b_list_in_leaf_cnt;
	BSP_Header.MatListInLeaf.offset = offset;
	offset += BSP_Header.MatListInLeaf.size;

	//------------���̳��� ������Ʈ ��Ʈ�� 
	BSP_Header.Object.size = sizeof(_D_OBJECT)*d_obj_num;
	BSP_Header.Object.offset=offset;
	offset += BSP_Header.Object.size;

	//------------Ʈ�� ��Ʈ�� 
	BSP_Header.Track.size = track_size;
	BSP_Header.Track.offset=offset;
	offset += BSP_Header.Track.size;

	//-----------�̺�Ʈ ���꺤Ʈ...
	BSP_Header.EventObjectID.size = d_obj_num*sizeof(WORD);
	BSP_Header.EventObjectID.offset= offset;
	offset += BSP_Header.EventObjectID.size;
	
	//--Ȯ���..
	BSP_Header.CFVertex.offset=offset;
	BSP_Header.CFVertex.size=0;
	BSP_Header.CFLine.offset=offset;
	BSP_Header.CFLine.size=0;
	BSP_Header.CFLineId.offset=offset;
	BSP_Header.CFLineId.size=0;
	BSP_Header.CFLeaf.offset=offset;
	BSP_Header.CFLeaf.size=0;

	//----������ ��Ʈ�κ� �а��� free�ϴ°�..
	BSPRenderPart *render_p=GetBSPRenderPart();

	//-------- byte ��
	BSP_Header.BVertex.size = sizeof(Vector3c)*render_p->BVertexNum;
	BSP_Header.BVertex.offset = offset;
	offset += BSP_Header.BVertex.size;

	BSP_Header.WVertex.size = sizeof(Vector3s)*render_p->WVertexNum;
	BSP_Header.WVertex.offset = offset;
	offset += BSP_Header.WVertex.size;

	BSP_Header.FVertex.size = sizeof(Vector3f)*render_p->FVertexNum;
	BSP_Header.FVertex.offset = offset;
	offset += BSP_Header.FVertex.size;

	//----------- ���ؽ� �÷�.
	BSP_Header.VertexColor.size = sizeof(DWORD)*render_p->FVertexNum;
	BSP_Header.VertexColor.offset = offset;
	offset += BSP_Header.VertexColor.size;

	//----------- uv ��ǥ
	BSP_Header.UV.size = sizeof(Vector2f)*render_p->VertexIdNum;
	BSP_Header.UV.offset=offset;
	offset += BSP_Header.UV.size;

	//------------����Ʈ�� uv ��Ʈ�� 
	BSP_Header.LgtUV.size = sizeof(Vector2s)*render_p->VertexIdNum;
	BSP_Header.LgtUV.offset=offset;
	offset += BSP_Header.LgtUV.size;

	//------------�� ��Ʈ�� 
	BSP_Header.Face.size = sizeof(_BSP_FACE)*render_p->FaceNum;
	BSP_Header.Face.offset=offset;
	offset += BSP_Header.Face.size;

	//------------���ε���  ��Ʈ�� 
	BSP_Header.FaceId.size = sizeof(DWORD)*render_p->FaceNum;
	BSP_Header.FaceId.offset=offset;
	offset += BSP_Header.FaceId.size;

	//------------�� �ε��� ��Ʈ�� 
	BSP_Header.VertexId.size = sizeof(DWORD)*render_p->VertexIdNum;
	BSP_Header.VertexId.offset=offset;
	offset += BSP_Header.VertexId.size;

	//------------���͸��� ������Ʈ ��Ʈ�� 
	BSP_Header.MatGroup.size = sizeof(_BSP_M_GROUP)*render_p->MatObjNum;
	BSP_Header.MatGroup.offset=offset;
	offset += BSP_Header.MatGroup.size;

	BSP_Header.EntityList.size = 0;
	BSP_Header.EntityList.offset= offset;
	BSP_Header.EntityID.size = 0;
	BSP_Header.EntityID.offset= offset;
	BSP_Header.LeafEntityList.size = 0;
	BSP_Header.LeafEntityList.offset= offset;
	BSP_Header.SoundEntityID.size = 0;
	BSP_Header.SoundEntityID.offset= offset;
	BSP_Header.LeafSoundEntityList.size = 0;
	BSP_Header.LeafSoundEntityList.offset= offset;


	//---����� ��Ʈ��.
	for(i=0; i<_READ_SPARE_CNT; i++ )
	{
		BSP_Header.ReadSpare[i].offset=0;
		BSP_Header.ReadSpare[i].size=0;
	}
	for(i=0; i<_FREE_SPARE_CNT; i++ )
	{
		BSP_Header.FreeSpare[i].offset=0;
		BSP_Header.FreeSpare[i].size=0;
	}

//--------------------------- ���̺� ����.....-------------------------------
	FILE *fp;
	if( (fp=fopen(name,"wb")) == NULL)
	{
		Error(name,"<- ���̺긦 �Ҽ� ����. CD�Ǵ� �ϵ尡 ��������?");
		return;
	}
	fwrite(&BSP_Header,sizeof(_BSP_FILE_HEADER),1,fp);
//	fwrite(c_mesh->object->vertex,BSP_Header.Cvertex.size,1,fp);
	fwrite(f_normal,BSP_Header.Cplanes.size,1,fp);
//	fwrite(vertex_id,BSP_Header.Cvertex_id.size,1,fp);
//	fwrite(b_face,BSP_Header.Cface.size,1,fp);
	fwrite(b_face_id,BSP_Header.Cface_id.size,1,fp);
	fwrite(b_node,BSP_Header.node.size,1,fp);
	fwrite(b_leaf,BSP_Header.leaf.size,1,fp);

	fwrite(b_list_in_leaf,BSP_Header.MatListInLeaf.size,1,fp);

	//-�� �ִϸ��̼�.
	fwrite(object,BSP_Header.Object.size,1,fp);
	fwrite(ani_track,BSP_Header.Track.size,1,fp);

	// �̺�Ʈ �ִϸ��̼� ���̺�
	fwrite(event_ani_id,BSP_Header.EventObjectID.size,1,fp);


	
	//------------------��������Ʈ ���̺�...

	fwrite(render_p->BVertex,BSP_Header.BVertex.size,1,fp);
	fwrite(render_p->WVertex,BSP_Header.WVertex.size,1,fp);
	fwrite(render_p->FVertex,BSP_Header.FVertex.size,1,fp);
	fwrite(render_p->VertexColor,BSP_Header.VertexColor.size,1,fp);
	fwrite(render_p->UV,BSP_Header.UV.size,1,fp);
	fwrite(render_p->LGTUV,BSP_Header.LgtUV.size,1,fp);
	fwrite(render_p->Face,BSP_Header.Face.size,1,fp);
	fwrite(render_p->FaceId,BSP_Header.FaceId.size,1,fp);
	fwrite(render_p->VertexId,BSP_Header.VertexId.size,1,fp);
	fwrite(bsp_m_group,BSP_Header.MatGroup.size,1,fp);

	
	fclose(fp);

	ReleaseBSPRenderPart();	//�׷� ������

	Dfree( b_list_in_leaf );
	Dfree( bsp_m_group );

	//���̳��� ������Ʈ�� Ʈ�� ������
	FreeDObject( object );
	FreeTrack( ani_track );

	if( d_obj_num )
		Dfree(event_ani_id);
	Dfree( f_normal );
//	Dfree( vertex_id );
	Dfree( uv );
	Dfree( lgt_map_uv );
	Dfree( b_face );
	Dfree( b_face_id );
	Dfree( b_node );
	Dfree( b_leaf );

}

