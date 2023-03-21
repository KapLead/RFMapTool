#include <windows.h>
#include "stdio.h"
#include "entity.h"
#include "jerror.h"
#include "jmalloc.h"
#include "commonutil.h"
#include "R3material.h"
#include "R3render.h"
#include "R3math.h"
#include "R3d3dtex.h"
#include "Core.h"
#include "R3Particle.h"

CEntity::CEntity()
{
	mVetexBufferSize=0;
	mIsEntityLoad=FALSE;
	mTextureSize=0;
	mMat=NULL;
	mFrame=0;	//���� ������.
	mFlag=0;
	mIndependencyTex=0;
}
CEntity::~CEntity()
{
}

int CEntity::GetUsedVertexBufferSize()
{
	return mVetexBufferSize;
}

void CEntity::GetAnimationMatrix(float get_ani_mat[4][4],_ANI_OBJECT *obj,float now_frame)
{
	::GetAniMatrix(get_ani_mat,obj,now_frame);
}
void ExtractVertex(WORD type,int num,Vector3f *vertex,void *comp_vertex,Vector3f pos,float scale)
{
	int i;
	Vector3c *b_buffer;
	Vector3s *s_buffer;
//	Vector3f *f_buffer;

	if( type & _VERTEX_ARC_TYPE_CHAR )
	{
		b_buffer = (Vector3c *)comp_vertex;

		for(i=0; i<num; i++)
		{
			vertex[i][0]=(b_buffer[i][0]/127.0f)*scale+pos[0];
			vertex[i][1]=(b_buffer[i][1]/127.0f)*scale+pos[1];
			vertex[i][2]=(b_buffer[i][2]/127.0f)*scale+pos[2];
		}
	}
	else
	if( type & _VERTEX_ARC_TYPE_SHORT )
	{
		s_buffer = (Vector3s *)comp_vertex;

		for(i=0; i<num; i++)
		{
			vertex[i][0]=(s_buffer[i][0]/32767.0f)*scale+pos[0];
			vertex[i][1]=(s_buffer[i][1]/32767.0f)*scale+pos[1];
			vertex[i][2]=(s_buffer[i][2]/32767.0f)*scale+pos[2];
		}
	}
	else
	{
		memcpy(vertex,comp_vertex,sizeof(Vector3f)*num);
	}
}

void CEntity::AddFlag(DWORD flag)	//���� �÷��׸� �߰��Ѵ�.
{
	mFlag |= flag;
}


BOOL CEntity::LoadEntity(char *filename,DWORD option)
{
	float version=_ENTITY_VERSION;
	int i,j;
	WORD k;
	int buf_size;
	BYTE *load_buf;
	Vector3f *vertex;
	Vector2s *uv;
	void *comp_vertex;
	DWORD *color;
	WORD *f_id,*v_id;
	_READ_ENTITY_FACE *entity_face;
	_READ_ENTITY_M_GROUP *read_entity_m_group;
	_READ_ANI_OBJECT *read_object;		//���̳��� ���� ������Ʈ.
	BYTE *track;						//�ִϸ��̼� Ʈ��.
	_ENTITY_COMP_HEADER *comp_header;
	struct{
		DWORD version;
		DWORD Identity;
	}MiniHeader;

	mMapColor = 0xff808080;
	mStartTime=0;
	mFlag=option;
	FILE *fp = fopen(filename,"rb");
	if( fp == NULL )
	{
		Warning(filename,"<-�� ������ �Ⱥ��δ�.");
		return FALSE;
	}
	fread(&MiniHeader,sizeof(DWORD)*2,1,fp);
/*
	if( MiniHeader.Identity == _IDENTITY_PARTICLE)	//�� ��ƼƼ�� ��ƼŬ�̴�.
	{
		_R3ENGINE_STATE *state = GetR3State();
		char r_name[256];
		_SAVE_PARTICLE s_particle;

		strcpy(r_name,state->EntityPath);
		fread(&s_particle,sizeof(_SAVE_PARTICLE),1,fp);	//��ƼŬ ������ �� �����´�.
		mParticle = new CParticle;
		mParticle->CopySaveParticleToParticle(&s_particle);
		mParticle->InitParticle();
		mParticle->SetParticleState(_PARTICLE_STATE_START);
		
		fclose(fp);
		strcat(r_name,s_particle.mEntityName);
		LoadEntity(r_name,_LOAD_POLY_IN_MEMORY);	//��ƼŬ�̹Ƿ� �޸𸮿� �ϴ� ��´�.
		return TRUE;
	}
*/	
	if( version != MiniHeader.version )
	{
		Warning(filename,"���� ������ ���������Դϴ�.");
		fclose(fp);
		return FALSE;
	}
	fread(&mHeader,sizeof(_ENTITY_FILE_HEADER),1,fp);

	DWORD load_option;
	if( option & _LOAD_POLY_IN_MEMORY )	//�޸𸮿� ���ߵɰ��..
	{
		load_option=_USE_MEMORY_VP;
		mIsUseMemory =TRUE;
	}
	else
		load_option=_USE_STATIC_VP;

	mStartTexID = GetNowR3D3DTexCnt();	//���� �ؽ��� ī��Ʈ�� ����
	int org_r3tex_num = GetNowR3TexCnt();			//���� r3�ؽ��� ī��Ʈ

	if( mFlag & _DO_NOT_MANAGE_TEXTURE )	//�ؽ��� �޴����� �Ⱦ��� ���.
	{
		SetR3TexManageFlag(_R3TEX_MANAGE_FLAG_NO_SAME_CHECK);	//�����̸��ؽ��ĸ� ���°�츦 �����ϰ� ���� �ø���.
	}

	DWORD old_tex_size=GetNowTexMemSize();
	
	char r3m_path[256];
	strcpy(r3m_path,filename);
	StripEXT(r3m_path);
	strcat(r3m_path,".r3m");
	if( IsExistFile(r3m_path) )
	{
		mMat = LoadSubR3M(r3m_path);

		StripEXT(r3m_path);
		strcat(r3m_path,".r3t");
		R3Texture *r3tex = R3GetTexInfoR3T(r3m_path,0);	//R3T�ؽ��� ������ �����Ѵ�.
		if( r3tex != NULL )
		{
			//r3tex = R3GetTexInfoR3T(r3m_path,0);	//R3T�ؽ��� ������ �����Ѵ�.

			if( r3tex->mSameCnt == 0 )	//�����ؽ��İ� ������ �ε��Ϲ� �ȵȴ�.
			{
				LoadR3T(r3tex);
				mTexNum = GetNowR3D3DTexCnt()-mStartTexID;	//���� �ؽ��� ������ �˾Ƴ���.
			}
			else
			{
				AdjustIndependenceR3M(mMat,mStartTexID,r3tex->mStartID);
				mStartTexID = r3tex->mStartID;
				mTexNum = r3tex->mTexNum;	//
			}
		}
	}
	else
	{
		//---���͸����� ���� �ε��Ǿ�...
		char pmat_path[256];
		strcpy(pmat_path,filename);
		StripName(pmat_path);

		mMat = LoadSubMaterial(pmat_path);
		if( mMat == NULL )
		{
			fclose(fp);
			return FALSE;
		}
		mTexNum = GetNowR3D3DTexCnt()-mStartTexID;	//���� �ؽ��� ������ �˾Ƴ���.
	}

	if( mFlag & _DO_NOT_MANAGE_TEXTURE )	//�ϴ� �ؽ��� �޴����� ���� Ȯ��..
	{
		//R3 �ؽ��� �Ŵ����� �Ⱦ��� ����� �ϴ� �ε����ѳ��� �ؽ��������͸� �����´�..
		//�װ����� �ٷ��.. ���߿� ���� �������ϴ� ����̴�...
		//����� ����������... �㿡 ���� �ٲٰų� ���ο� ������ �ݵ�� �����ҰŴ�....
		for(i=0; i<mTexNum; i++)	//�ϴ� �ؽ��ĸ� �ø���.
		{
			R3LoadTextureMem(mStartTexID+i);	//Ư�� �ؽ��ĸ� �ؽ��ĸ޸𸮿� ��´�.
		}
		LPDIRECT3DTEXTURE8 *ppTex;
		ppTex=(LPDIRECT3DTEXTURE8 *)Dmalloc(mTexNum*sizeof(LPDIRECT3DTEXTURE8 *));
		for( i=0; i<mTexNum; i++)
		{
			ppTex[i]=R3GetSurface(mStartTexID+i);
		}
		mIndependencyTex = ppTex;
		SetNowR3D3DTexCnt(mStartTexID); //����� ����% �𸣰� ���� å�Ӹ���...
		SetNowR3TexCnt(org_r3tex_num);	//r3�ؽ��� ����, �𸣰� ���� å�Ӹ���...
		//mStartTexID=0;
		SetR3TexManageFlag(0);	//�����̸��ؽ��ĸ� ���°�츦 �����ϰ� ���� �ø���.
	}
	mTextureSize += (GetNowTexMemSize()-old_tex_size);

	//---�׸��� �ε�...
	mIsEntityLoad=TRUE;

	buf_size=0;
	buf_size+=mHeader.CompHeader.size;
	buf_size+=mHeader.Vertex.size;
	buf_size+=mHeader.VColor.size;
	buf_size+=mHeader.UV.size;
	buf_size+=mHeader.Face.size;
	buf_size+=mHeader.FaceId.size;
	buf_size+=mHeader.VertexId.size;
	buf_size+=mHeader.MatGroup.size;
	buf_size+=mHeader.Object.size;
	buf_size+=mHeader.Track.size;
	
	load_buf=(BYTE *)Dmalloc(buf_size);

	buf_size=0;

	comp_header=(_ENTITY_COMP_HEADER *)&load_buf[buf_size];
	buf_size+=mHeader.CompHeader.size;

	comp_vertex = (void *)&load_buf[buf_size];
	buf_size+=mHeader.Vertex.size;

	color = (DWORD *)&load_buf[buf_size];
	buf_size+=mHeader.VColor.size;

	uv = (Vector2s *)&load_buf[buf_size];
	buf_size+=mHeader.UV.size;

	entity_face = (_READ_ENTITY_FACE *)&load_buf[buf_size];
	buf_size+=mHeader.Face.size;

	f_id = (WORD *)&load_buf[buf_size];
	buf_size+=mHeader.FaceId.size;

	v_id = (WORD *)&load_buf[buf_size];
	buf_size+=mHeader.VertexId.size;

	read_entity_m_group = (_READ_ENTITY_M_GROUP *)&load_buf[buf_size];
	buf_size+=mHeader.MatGroup.size;
	
	read_object = (_READ_ANI_OBJECT *)&load_buf[buf_size];
	buf_size+=mHeader.Object.size;

	track = (BYTE *)&load_buf[buf_size];
	buf_size+=mHeader.Track.size;

	fread(comp_header,mHeader.CompHeader.size,1,fp);
	fread(comp_vertex,mHeader.Vertex.size,1,fp);
	fread(color,mHeader.VColor.size,1,fp);
	fread(uv,mHeader.UV.size,1,fp);
	fread(entity_face,mHeader.Face.size,1,fp);
	fread(f_id,mHeader.FaceId.size,1,fp);
	fread(v_id,mHeader.VertexId.size,1,fp);
	fread(read_entity_m_group,mHeader.MatGroup.size,1,fp);
	fread(read_object,mHeader.Object.size,1,fp);
	fread(track,mHeader.Track.size,1,fp);
	fclose(fp);

	int divide,vertex_num;
	if( comp_header->type == _VERTEX_ARC_TYPE_CHAR )
	{
		divide = 4;
		vertex_num = mHeader.Vertex.size/sizeof(Vector3c);
	}
	else
	if( comp_header->type == _VERTEX_ARC_TYPE_SHORT )
	{
		divide = 2;
		vertex_num = mHeader.Vertex.size/sizeof(Vector3s);
	}
	else
	{
		divide = 1;
		vertex_num = mHeader.Vertex.size/sizeof(Vector3f);
	}

	vertex = (Vector3f *)Dmalloc(vertex_num*sizeof(Vector3f));//����Ǯ ���ؽ� ����.

	ExtractVertex(comp_header->type,vertex_num,vertex,comp_vertex
		,comp_header->pos,comp_header->scale);

	int static_trinum=0;
	int static_vertexnum=0;
	// ���ĳ� ������ ����� ������ �ﰢ���� ���Ѵ�.
	for(i=0; i< (int)(mHeader.MatGroup.size/sizeof(_READ_ENTITY_M_GROUP)); i++)
	{
		for(j=0; j<read_entity_m_group[i].face_num; j++)
		{
			DWORD face_id = f_id[read_entity_m_group[i].face_start_id+j];
			static_trinum+=(entity_face[face_id].v_num-2);
			static_vertexnum += entity_face[face_id].v_num;
		}
	}
	mVetexBufferSize=static_vertexnum*sizeof(_D3DR3VERTEX_TEX1);
	mVetexBufferSize+=static_trinum*3*sizeof(WORD);

	mMatGroupNum=(int)(mHeader.MatGroup.size/sizeof(_READ_ENTITY_M_GROUP));
	mObjectNum = mHeader.Object.size/sizeof(_READ_ANI_OBJECT);
	//----------���� ���͵鿡 ���� �޸� �Ҵ�.(��ϼ��� ������ �ٿ�����.)

//	int fsdfsdf = ((mMatGroupNum)*sizeof(_ENTITY_M_GROUP)
//		+mObjectNum*sizeof(_ANI_OBJECT)+mHeader.Track.size+static_vertexnum*sizeof(Vector2f));
	mMatGroup = (_ENTITY_M_GROUP*)Dmalloc((mMatGroupNum)*sizeof(_ENTITY_M_GROUP)
		+mObjectNum*sizeof(_ANI_OBJECT)+mHeader.Track.size+static_vertexnum*sizeof(Vector2f));
	mObject=(_ANI_OBJECT *)((BYTE*)mMatGroup+(mMatGroupNum)*sizeof(_ENTITY_M_GROUP));
	mTrack=(BYTE *)mObject+mObjectNum*sizeof(_ANI_OBJECT);
	mOrgUV=(Vector2f*)((BYTE*)mTrack+mHeader.Track.size);

	memcpy(mTrack,track,mHeader.Track.size);	//Ʈ���� �״�� ����
	ConvAniObject(mObjectNum,mTrack,read_object,mObject);		//Ʈ���ε����� �����ͷ� �ٲ�����. ���������Ʈ �����ϴ°�����..

	if( option & _USE_VERTEX_SHADER )	//���ؽ� ���̴��� ����ϸ�.
		mStaticVertexBuffer.InitVertexBuffer( static_vertexnum*sizeof(_D3DR3VERTEX_TEX1),load_option,0 );
	else
		mStaticVertexBuffer.InitVertexBuffer( static_vertexnum*sizeof(_D3DR3VERTEX_TEX1),load_option,D3DFVF_R3VERTEX_TEX1 );
	mStaticIndexedBuffer.InitIndexBuffer(static_trinum*3*sizeof(WORD),load_option);

	_D3DR3VERTEX_TEX1 *static_vertexbp;
	WORD *static_indexbp;

	static_vertexbp=(_D3DR3VERTEX_TEX1 *)mStaticVertexBuffer.VPLock();
	static_indexbp =(WORD *)mStaticIndexedBuffer.VPLock();

	if( static_vertexbp==NULL || static_indexbp==NULL )	//���ؽ� ���ۿ� �ε��� ���۸� �������ߴ�.
	{
		ReleaseEntity();
		Dfree(vertex);
		Dfree(load_buf);
		mIsEntityLoad = FALSE;
		return TRUE;
	}

	//���ؽ� ���ۿ� ����.....  ���͸���׷�� UV ���ۿ� ����.
	int static_v_index_cnt=0;
	WORD static_v_cnt=0;

	float uv_scale = (comp_header->uv_max-comp_header->uv_min)/2.0f;
	float uv_pos = comp_header->uv_min+uv_scale;

	mBBMin[0] = 67000	,mBBMin[1] = 67000	,mBBMin[2] = 67000;
	mBBMax[0] = -67000	,mBBMax[1] = -67000	,mBBMax[2] = -67000;

	mIsAlpha=FALSE;
	BOOL IsLightEnable=0;
	for(i=0; i< mMatGroupNum; i++)
	{
		mMatGroup[i].BBMin[0] = read_entity_m_group[i].bb_min[0];
		mMatGroup[i].BBMin[1] = read_entity_m_group[i].bb_min[1];
		mMatGroup[i].BBMin[2] = read_entity_m_group[i].bb_min[2];
		mMatGroup[i].BBMax[0] = read_entity_m_group[i].bb_max[0];
		mMatGroup[i].BBMax[1] = read_entity_m_group[i].bb_max[1];
		mMatGroup[i].BBMax[2] = read_entity_m_group[i].bb_max[2];
		mMatGroup[i].MtlId = read_entity_m_group[i].mtl_id;
		mMatGroup[i].MultiSourceUV = &mOrgUV[static_v_cnt];
		mMatGroup[i].ObjectId = read_entity_m_group[i].object_id;
		mBBMin[0]=min(mBBMin[0],mMatGroup[i].BBMin[0]);
		mBBMin[1]=min(mBBMin[1],mMatGroup[i].BBMin[1]);
		mBBMin[2]=min(mBBMin[2],mMatGroup[i].BBMin[2]);
		mBBMax[0]=max(mBBMax[0],mMatGroup[i].BBMax[0]);
		mBBMax[1]=max(mBBMax[1],mMatGroup[i].BBMax[1]);
		mBBMax[2]=max(mBBMax[2],mMatGroup[i].BBMax[2]);

		int temp_f_num;
		int temp_v_num;
		mMatGroup[i].Type = _MAT_GROUP_TYPE_STANDARD;	//����� ���Ĵٵ常...
		if( mMat[read_entity_m_group[i].mtl_id].m_Layer[0].m_dwAlphaType != _BLEND_NONE )
		{
			mMatGroup[i].Type |= _MAT_GROUP_TYPE_OPPA;
			mIsAlpha=TRUE;
		}


		DWORD alpha;
		if( read_entity_m_group[i].mtl_id == -1 )
		{
			alpha = 0xff000000;
			IsLightEnable=0;
		}
		else
		{
			alpha=mMat[mMatGroup[i].MtlId].m_Layer[0].m_ARGB&0xff000000;
			if( mMat[mMatGroup[i].MtlId].m_dwFlag & _MAT_FLAG_LIGHT_MAP )
				IsLightEnable=TRUE;
			else
				IsLightEnable=FALSE;
			if( alpha&0xff000000 != 0xff000000 )
				mIsAlpha=TRUE;
		}
		mMatGroup[i].VBMinIndex = static_v_cnt;
		mMatGroup[i].IBMinIndex = static_v_index_cnt;

		temp_f_num = static_v_index_cnt/3;
		temp_v_num = static_v_cnt;
		for(j=0; j<read_entity_m_group[i].face_num; j++)
		{
			WORD face_id = f_id[read_entity_m_group[i].face_start_id+j];
			for(k=0; k<entity_face[face_id].v_num-2; k++)	//�ε��� �����͸� ä���.
			{
				static_indexbp[static_v_index_cnt++]=static_v_cnt;
				static_indexbp[static_v_index_cnt++]=static_v_cnt+k+1;
				static_indexbp[static_v_index_cnt++]=static_v_cnt+k+2;
			}

			for(k=0; k<entity_face[face_id].v_num; k++)
			{
				DWORD vr_id = entity_face[face_id].v_start_id+k;

				static_vertexbp[static_v_cnt].x = vertex[v_id[vr_id]][0];
				static_vertexbp[static_v_cnt].y = vertex[v_id[vr_id]][1];
				static_vertexbp[static_v_cnt].z = vertex[v_id[vr_id]][2];

				mOrgUV[static_v_cnt][0]=((uv[vr_id][0]/32767.0f)*uv_scale)+uv_pos;
				mOrgUV[static_v_cnt][1]=((uv[vr_id][1]/32767.0f)*uv_scale)+uv_pos;
				static_vertexbp[static_v_cnt].u = mOrgUV[static_v_cnt][0];	//�ؽ���uv
				static_vertexbp[static_v_cnt].v = mOrgUV[static_v_cnt][1];
//				static_vertexbp[static_v_cnt].color =0x00ffffff;
				if( IsLightEnable )
					static_vertexbp[static_v_cnt].color =0xff808080;
				else
					static_vertexbp[static_v_cnt].color =color[v_id[vr_id]];
				static_vertexbp[static_v_cnt].color |= alpha;
				static_v_cnt++;
			}
			mMatGroup[i].VCnt = static_v_cnt - temp_v_num;	//�� ���͸���׷��� ���ؽ� ����..
			mMatGroup[i].TriNum = static_v_index_cnt/3-temp_f_num;	//����͸����� tri��
		}
	}

	mStaticVertexBuffer.VPUnLock();
	mStaticIndexedBuffer.VPUnLock();
	Dfree(vertex);
	Dfree(load_buf);

//	RestoreTexMem();	//�ؽ��� �޸𸮿� �ø���.
	return TRUE;
}



void CEntity::ReleaseEntity()
{
	if( mIsEntityLoad == FALSE )
		return;
/*
	if( mParticle )	//�� ��ƼƼ�� ��ƼŬ�̴�.
	{
		mParticle->ReleaseParticle();
		delete mParticle;
		mParticle=NULL;
	}*/
	mIsEntityLoad=FALSE;
	mVetexBufferSize=0;
	mTextureSize=0;
	mObjectNum=0;

	mStaticVertexBuffer.ReleaseVertexBuffer();
	mStaticIndexedBuffer.ReleaseIndexBuffer();
//	ReleaseTexMem();	//�ؽ��� �޸𸮿� ������.
	if( mMatGroup )
	{
		Dfree(mMatGroup);
		mMatGroup=0;
	}
	if( mMat )
	{
		ReleaseSubMaterial(mMat);
		mMat=NULL;
	}

	if( mIndependencyTex )	//�ؽ��İ� �����ֳ�..
	{
		if( mFlag & _DO_NOT_MANAGE_TEXTURE )	//�ϴ� �³� Ȯ��..
		{
			ReleaseTexMem();	//�ؽ��� �޸𸮿� ������.
		}
		mIndependencyTex=NULL;
	}
}

void CEntity::PrepareAnimation()	//�ִϸ��̼� �ϱ����� �غ��Ұ͵�..
{
	int i;
	if( !mObject )
		return;
	for(i=0; i<mObjectNum; i++)
	{
		mObject[i].AniFrameCache = 0;
	}
}
void CEntity::RestoreTexMem()	//�ؽ��� �޸𸮿� �ø���.
{
	if( mFlag & _DO_NOT_MANAGE_TEXTURE )	//�ϴ� �ؽ��� �޴����� ���� Ȯ��..
		return;
	DWORD old_tex_size=GetNowTexMemSize();
	for(int i=0; i<mTexNum; i++)
	{
		R3LoadTextureMem(mStartTexID+i);	//Ư�� �ؽ��ĸ� �ؽ��ĸ޸𸮿� ��´�.
	}
	mTextureSize += (GetNowTexMemSize()-old_tex_size);
}

void CEntity::ReleaseTexMem()	//�ؽ��� �޸𸮿� ������.
{
	int i;

	if( mFlag & _DO_NOT_MANAGE_TEXTURE )	//�ϴ� �ؽ��� �޴����� ���� Ȯ��..
	{
		LPDIRECT3DTEXTURE8 *ppTex;
		ppTex=(LPDIRECT3DTEXTURE8 *)mIndependencyTex;
		for( i=0; i<mTexNum; i++)
		{
			if( ppTex[i] )
				ppTex[i]->Release();
		}
		Dfree(mIndependencyTex);
		mIndependencyTex=NULL;
		return;
	}
	for( i=0; i<mTexNum; i++)
	{
		R3ReleaseTextureMem(mStartTexID+i);	//Ư�� �ؽ��ĸ� �ؽ��ĸ޸𸮿� ��´�.
	}
}

BOOL CEntity::IsFirstAlpha()  //ù��°���͸���׷��� �����ΰ�? ��ƼƼ ������ �ʿ�.
{
	if ( mMatGroupNum <= 0 || mMat == NULL )
		return FALSE;
	if( mMat[0].m_Layer[0].m_dwAlphaType != _BLEND_NONE )
		return TRUE;
	return FALSE;
}
