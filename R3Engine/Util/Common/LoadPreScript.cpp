#include <stdio.h>
#include "world.h"
#include "d3dmath.h"
#include "r3bspUtil.h"
#include "jerror.h"

static int GetObjectId(char *name,_MESH *mesh)
{
	int i;

	for(i=0; i<mesh->objectnum; i++)
	{
		if( !strcmp( mesh->object[i].objectname,name))
			return i;
	}
	return -1;
}

//Ʈ���� ��ũ��Ʈ�� �����ȴ�� �߶� ����.
void FixTrack(_OBJECT *obj)
{
	int i,fr_before;
	float alpha,x,y,z,w;

	for(i=0; i<obj->Pos_cnt; i++)
	{
		if( obj->Pos[i].frame > obj->end_frame)
		{
			fr_before=i-1;
			if(fr_before < 0)
			{
				alpha=0;
			}
			else
			{
				alpha = ( (obj->end_frame - obj->Pos[fr_before].frame)
					/(obj->Pos[i].frame-obj->Pos[fr_before].frame) );
			}
			x=(obj->Pos[i].x-obj->Pos[fr_before].x)*alpha
				+obj->Pos[fr_before].x;
			y=(obj->Pos[i].y-obj->Pos[fr_before].y)*alpha
				+obj->Pos[fr_before].y;
			z=(obj->Pos[i].z-obj->Pos[fr_before].z)*alpha
				+obj->Pos[fr_before].z;
			obj->Pos_cnt=i+1;
			obj->Pos[i].x = x;
			obj->Pos[i].y = y;
			obj->Pos[i].z = z;
			obj->Pos[i].frame = obj->end_frame;
			break;
		}
	}
	for(i=0; i<obj->Rot_cnt; i++)
	{
		if( obj->Rot[i].frame > obj->end_frame)
		{
			fr_before=i-1;
			if(fr_before < 0)
			{
				alpha=0;
			}
			else
			{
				alpha = ( (obj->end_frame - obj->Rot[fr_before].frame)
					/(obj->Rot[i].frame-obj->Rot[fr_before].frame) );
			}
			D3DMath_QuaternionSlerp( x,y,z,w,obj->Rot[fr_before].x
						,obj->Rot[fr_before].y,obj->Rot[fr_before].z
						,obj->Rot[fr_before].w,obj->Rot[i].x
						,obj->Rot[i].y,obj->Rot[i].z
						,obj->Rot[i].w,alpha);
			obj->Rot_cnt=i+1;
			obj->Rot[i].x = x;
			obj->Rot[i].y = y;
			obj->Rot[i].z = z;
			obj->Rot[i].w = w;
			obj->Rot[i].frame = obj->end_frame;
			break;
		}
	}
	if( obj->Rot_cnt || obj->Pos_cnt || obj->Scale_cnt )
		return;
	//�ִϰ� ���µ� �������
//	obj->type &= (~_DYNAMIC_OBJECT);	//�ִϸ��̼� ������Ʈ
	obj->end_frame = 0;
//	Warning(obj->objectname,"<-�ִϸ��̼� �����Ͱ� �����ϴ�.");
}

void GetPreScriptAtt(char *mapname,_MESH *mesh)
{
	char hole[256],obj_name[256],temp[256],name[256]=".\\���߰���\\script\\map\\";

	strcpy(temp,mapname);
	StripEXT(temp);
	StripPath(temp);
	strcat(name,temp);
	strcat(name,".pst");
	FILE *fp = fopen(name,"rt");

	if( fp == NULL )
		return;

	int obj_num=-1;
	while( 1 )
	{
		if( fscanf(fp,"%s",hole)==EOF)
			break;
		if( !strcmp(hole,"script_begin"))
			break;
	}
	while( 1 )
	{
		if( fscanf(fp,"%s",hole)==EOF)
			break;
		if( hole[0] == '*')
		{
			strcpy(obj_name,&hole[1]);
			obj_num = GetObjectId(obj_name,mesh);
		}
		if( hole[0] == '-')
		{
			if( !strcmp(hole,"-no_merge"))
			{
				obj_num = GetObjectId(obj_name,mesh);
				if( obj_num == -1 )
					continue;
				mesh->object[obj_num].type|=_NO_MERGE_OBJECT;
			}
			if( obj_num == -1 )
				continue;
			if( !strcmp(hole,"-corona"))
			{
				mesh->object[obj_num].type |= _CORONA;	//�ڷγ�
				mesh->object[obj_num].type |= _NO_MERGE_OBJECT;
				mesh->object[obj_num].type |= _SHADOW_CAST_OFF;//ĳ��Ʈ �ɼ��� ���� ����.
				obj_num = -1;
			}
			else
			if( !strcmp(hole,"-y_billboard"))
			{
				mesh->object[obj_num].type|=_Y_BILLBOARD;	//y�� ������
				mesh->object[obj_num].type|=_NO_MERGE_OBJECT;
				obj_num = -1;
			}
			else
			if( !strcmp(hole,"-ani"))
			{
 				if( fscanf(fp,"%s",hole)==EOF)
					break;
				int end_frame=atoi(hole);
				mesh->object[obj_num].start_frame = 0;
				mesh->object[obj_num].end_frame = end_frame;
				mesh->object[obj_num].type|=_DYNAMIC_OBJECT;	//�ִϸ��̼� ������Ʈ
				mesh->object[obj_num].type|=_NO_MERGE_OBJECT;	//�ִϸ��̼� ������Ʈ�� �����ϸ� �ȵȴ�.
				FixTrack(&mesh->object[obj_num]);
			}
			else
			if( !strcmp(hole,"-event_ani"))
			{
 				if( fscanf(fp,"%s",hole)==EOF)
					break;
				int end_frame=atoi(hole);
				mesh->object[obj_num].start_frame = 0;
				mesh->object[obj_num].end_frame = end_frame;
				mesh->object[obj_num].type|=_DYNAMIC_OBJECT;	//�ִϸ��̼� ������Ʈ
				mesh->object[obj_num].type|=_EVENT_ANI;			//�ִϸ��̼� ������Ʈ
				mesh->object[obj_num].type|=_NO_MERGE_OBJECT;	//�ִϸ��̼� ������Ʈ�� �����ϸ� �ȵȴ�.
				FixTrack(&mesh->object[obj_num]);
 				if( fscanf(fp,"%s",hole)==EOF)
					break;
				end_frame=atoi(hole);
				mesh->object[obj_num].event_id = (WORD)end_frame;
			}
			else
			if( !strcmp(hole,"-vfog"))	//���� ������ �ϳ��� ���͸��� ����� ������.
			{
				if( obj_num==-1)
					continue;
				mesh->object[obj_num].type|=_V_FOG;		//���� ����...
				mesh->object[obj_num].type|=_SHADOW_RECEIVE_OFF;		//�� ����Ʈ��
				mesh->object[obj_num].type|=_NO_MERGE_OBJECT;	//�ִϸ��̼� ������Ʈ�� �����ϸ� �ȵȴ�.
				obj_num = -1;
			}
			/*
			if( !strcmp(hole,"-no_light_map"))	//����Ʈ���� �����Ѵ�. 
			{
				mesh->object[obj_num].type|=_NO_LIGHT_MAP;		//�� ����Ʈ��
			}*/
		}
	}
	fclose(fp);
}