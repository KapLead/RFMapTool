#include "jerror.h"
#include "jmalloc.h"
#include "R3Material.h"
#include "SkyBox.h"
#include "R3d3dtex.h"
#include "core.h"
#include "commonutil.h"
#include <stdio.h>
#include <assert.h>

static _R3MATERIAL *R3material=NULL;
static DWORD stdwMaterialNum=0;
static char stTexturePath[256]=".\\Texture\\";

static void MatInit(_R3MATERIAL *mat)
{
	if(mat)
	{
		memset(mat,0,sizeof(_R3MATERIAL));//�Ѹ��͸����� �����о����
		for(int i=0; i<_MAX_LAYER_NUM; i++)
		{
			mat->m_Layer[i].m_iSurface	= 0;
			mat->m_Layer[i].m_dwAlphaType	= 0;
			mat->m_Layer[i].m_ARGB = 0xffffffff;
			mat->m_Layer[i].m_dwFlag		= 0;
		}
	}
	else
	{
		assert(!"mat_init error");
	}
}


_R3MATERIAL *GetMainMaterial()
{
	return R3material;
}

static BOOL stIsReLoad=FALSE;
void SetReLoadState(BOOL state)
{
	stIsReLoad=state;
}

static void LoadOneLayer(FILE *fp,_R3MATERIAL *material,int l_id)
{
	char name[256];
	char hole[256];

	material->m_Layer[l_id].m_ARGB=0xffffffff;

//	material->m_dwFlag=0;
//	material->m_sGradientAlpha=0;

    while(1)
    {
        fscanf(fp,"%s",&hole[0]);
        if (!strcmp(hole,"}"))
			break;

        if (!strcmp(hole,"map_name") && stIsReLoad == FALSE)	//���̸�.
		{
	        fscanf(fp,"%s",&hole[0]);
			//strcpy(material->m_tex_name,hole);
			strcpy(name,stTexturePath);
			strcat(name,hole);
			if( hole[0] != NULL && strcmp(hole,"NULL"))
			{
				if( IsAniTex(hole) == FALSE )	//�ִϸ��̼� �ؽ��İ� �ƴϴ�.
				{
					material->m_Layer[l_id].m_iSurface = R3GetPreTextureId(name);
				}
				else	//Ÿ�� �ִϸ��̼� �ؽ��Ĵ�.
				{		//material->m_iSurface�� ���� ���̵� ������ �ִ´�.
					material->m_Layer[l_id].m_dwFlag|=_ANI_TILE_TEXTURE;			
					LONG get_tex_num;
					R3GetPreAniTextureId(stTexturePath,hole,&material->m_Layer[l_id].m_iSurface,&get_tex_num);
					material->m_Layer[l_id].m_iTileAniTexNum=(SHORT)get_tex_num;			//Ÿ�� �ִ� �� ����
				}
			}
			else
			{
				material->m_Layer[l_id].m_iSurface = 0;
			}
		}
		
        if (!strcmp(hole,"uv_scroll_u"))	
		{
	        fscanf(fp,"%s",&hole[0]);
			material->m_Layer[l_id].m_sUVScrollU= FixedFloatToShort((float)atof(hole));
			material->m_Layer[l_id].m_dwFlag|=_UV_SCROLL_U;			
		}
        if (!strcmp(hole,"uv_scroll_v"))	
		{
			fscanf(fp,"%s",&hole[0]);
			material->m_Layer[l_id].m_sUVScrollV=FixedFloatToShort((float)atof(hole));
			material->m_Layer[l_id].m_dwFlag|=_UV_SCROLL_V;			
		}
        if (!strcmp(hole,"uv_scale"))	
		{
			//un�������� ���۰� ���� �̸� �����Ѵ�.
	        fscanf(fp,"%s",&hole[0]);
			material->m_Layer[l_id].m_sUVScaleStart=FixedFloatToShort((float)atof(hole));
			material->m_Layer[l_id].m_sUVScaleEnd =material->m_Layer[l_id].m_sUVScaleStart; 
			material->m_Layer[l_id].m_sUVScaleSpeed=256;
			material->m_Layer[l_id].m_dwFlag|=_UV_SCALE;
		}
        if (!strcmp(hole,"uv_scale_speed"))	
		{
	        fscanf(fp,"%s",&hole[0]);
			material->m_Layer[l_id].m_sUVScaleSpeed=FixedFloatToShort((float)atof(hole));
		}
        if (!strcmp(hole,"uv_scale_end"))	
		{
	        fscanf(fp,"%s",&hole[0]);
			material->m_Layer[l_id].m_sUVScaleEnd=FixedFloatToShort((float)atof(hole));
		}
        if (!strcmp(hole,"uv_rotate"))	
		{
	        fscanf(fp,"%s",&hole[0]);
			material->m_Layer[l_id].m_sUVRotate=FixedFloatToShort((float)atof(hole));
			material->m_Layer[l_id].m_dwFlag|=_UV_ROTATE;
		}
        if (!strcmp(hole,"uv_lava"))	
		{
	        fscanf(fp,"%s",&hole[0]);
			if( material->m_Layer[l_id].m_sUVLavaWave==0)
				material->m_Layer[l_id].m_sUVLavaWave=FixedFloatToShort(1.0f);
			material->m_Layer[l_id].m_sUVLavaSpeed=FixedFloatToShort((float)atof(hole));
			material->m_Layer[l_id].m_dwFlag|=_UV_LAVA;
		}
        if (!strcmp(hole,"uv_lava_wave"))	
		{
	        fscanf(fp,"%s",&hole[0]);
			material->m_Layer[l_id].m_sUVLavaWave=FixedFloatToShort((float)atof(hole));
		}
        if (!strcmp(hole,"uv_gradient_alpha_u"))	
		{
	        fscanf(fp,"%s",&hole[0]);
			material->m_Layer[l_id].m_sGradientAlpha|=((WORD)((float)atof(hole)*25+100)<<8);
			material->m_Layer[l_id].m_dwFlag|=_UV_GRADIENT_ALPHA_U;
		}
        if (!strcmp(hole,"uv_gradient_alpha_v"))	
		{
	        fscanf(fp,"%s",&hole[0]);
			material->m_Layer[l_id].m_sGradientAlpha|=((WORD)((float)atof(hole)*25+100));
			material->m_Layer[l_id].m_dwFlag|=_UV_GRADIENT_ALPHA_V;
		}
        if (!strcmp(hole,"uv_metal_floor"))	
		{
	        fscanf(fp,"%s",&hole[0]);
			material->m_Layer[l_id].m_sUVMetal=FixedFloatToShort((float)atof(hole));
			material->m_Layer[l_id].m_dwFlag|=_UV_METAL_FLOOR;
		}
        if (!strcmp(hole,"uv_metal_wall"))	
		{
	        fscanf(fp,"%s",&hole[0]);
			material->m_Layer[l_id].m_sUVMetal=FixedFloatToShort((float)atof(hole));
			material->m_Layer[l_id].m_dwFlag|=_UV_METAL_WALL;
		}
        if (!strcmp(hole,"ani_alpha_flicker"))
		{
	        fscanf(fp,"%s",&hole[0]);
			material->m_Layer[l_id].m_sANIAlphaFlicker = FixedFloatToShort((float)atof(hole));
			material->m_Layer[l_id].m_dwFlag|=_ANI_ALPHA_FLICKER;
		}
        if (!strcmp(hole,"ani_alpha_flicker_start"))
		{
	        fscanf(fp,"%s",&hole[0]);
			material->m_Layer[l_id].m_sANIAlphaFlickerAni |= ((WORD)(255*(float)atof(hole))<<8);
		}
        if (!strcmp(hole,"ani_alpha_flicker_end"))
		{
	        fscanf(fp,"%s",&hole[0]);
			material->m_Layer[l_id].m_sANIAlphaFlickerAni |= (WORD)(255*(float)atof(hole));
		}
        if (!strcmp(hole,"ani_tex_speed"))
		{
	        fscanf(fp,"%s",&hole[0]);
			material->m_Layer[l_id].m_sANITexSpeed = FixedFloatToShort((float)atof(hole));
		}
        if (!strcmp(hole,"ani_tex_frame"))
		{
	        fscanf(fp,"%s",&hole[0]);
			material->m_Layer[l_id].m_dwFlag|=_ANI_TEXTURE;
			material->m_Layer[l_id].m_sANITexFrame = FixedFloatToShort((float)atof(hole));
			if( material->m_Layer[l_id].m_sANITexSpeed == 0)
				material->m_Layer[l_id].m_sANITexSpeed = FixedFloatToShort(1.0f);
		}
		
        if (!strcmp(hole,"uv_env"))
		{
	        fscanf(fp,"%s",&hole[0]);
//			material->m_sUVEnv=FixedFloatToShort((float)atof(hole));
			material->m_Layer[l_id].m_dwFlag|=_UV_ENV;
		}
		
        if (!strcmp(hole,"alpha_sort"))
		{
	        fscanf(fp,"%s",&hole[0]);
			char *boolgop = strupr(hole);
			if( !strcmp(boolgop,"TRUE"))
			{
				if( l_id == 0 )
			 		material->m_Layer[l_id].m_dwAlphaType |= _ALPHA_SORT_ON;
				else
					Warning("alpha_sort TRUE�� 0�����̾��..","");
			}
		}
        if (!strcmp(hole,"tex_clamp"))
		{
			material->m_Layer[l_id].m_dwFlag |= _MAT_TEX_CLAMP;
		}

        if (!strcmp(hole,"zwrite"))
		{
			//if( l_id == 0 )
			 	material->m_Layer[l_id].m_dwAlphaType |= _MAT_ZWRITE;
			//else
			//	Warning("zwrite�� 0�����̾��..","");
		}
        if (!strcmp(hole,"bump_env"))
		{
			material->m_Layer[l_id].m_dwFlag |= _MAT_ENV_BUMP;
		}

        if (!strcmp(hole,"type"))
		{
	        fscanf(fp,"%s",&hole[0]);
			material->m_Layer[l_id].m_dwAlphaType = atoi(hole);
		}
		
        if (!strcmp(hole,"alpha"))
		{
	        fscanf(fp,"%s",&hole[0]);
			material->m_Layer[l_id].m_ARGB &= 0x00ffffff;
			material->m_Layer[l_id].m_ARGB |= ((DWORD)(atoi(hole)))<<24;
		}
        if (!strcmp(hole,"color"))
		{
			DWORD r,g,b;
	        fscanf(fp,"%s",&hole[0]);
			r = (DWORD)(atoi(hole));
	        fscanf(fp,"%s",&hole[0]);
			g = (DWORD)(atoi(hole));
	        fscanf(fp,"%s",&hole[0]);
			b = (DWORD)(atoi(hole));
			material->m_Layer[l_id].m_ARGB &= 0xff000000;
			material->m_Layer[l_id].m_ARGB |= (r<<16)|(g<<8)|b;
		}
	}
	if( material->m_Layer[l_id].m_sANIAlphaFlickerAni == 0 )
		material->m_Layer[l_id].m_sANIAlphaFlickerAni=0x00ff;
}

static void LoadOneMaterial(char *name,_R3MATERIAL *material)
{
	FILE *fp;
	char hole[256];
	int l_id;

	bool sub = false;

	if( (fp = fopen(name,"rt")) == NULL )	
	{
		Warning(name,"<-�������� ����.");
		return;
	}

	if( stIsReLoad ==FALSE )
	{
		for(int q = 0;q < _MAX_LAYER_NUM;q++)
		{
			material->m_Layer[q].m_iSurface		= -1;
		}
		material->m_iDetailSurface=-1;
		material->m_dwFlag=0;
	}

	DWORD temp_texture_detail = GetR3State()->mSceneTextureDetail;
//	material->m_dwFlag |= _MAT_FLAG_NO_LOD_TEXTURE;

    while(1)
    {

		if( fscanf(fp,"%s",&hole[0])==EOF)	break;
        if (!strcmp(hole,"}"))
		{
			if(sub) sub = false;
			else	break;
		}
        if (!strcmp(hole,"light_map"))
		{
	        fscanf(fp,"%s",&hole[0]);
			char *boolgop = strupr(hole);
			if( !strcmp(boolgop,"TRUE"))
		 		material->m_dwFlag |= _MAT_FLAG_LIGHT_MAP;
		}
        if (!strcmp(hole,"layer_num"))
		{
	        fscanf(fp,"%s",&hole[0]);
		 	material->m_dwLayerNum = atoi(hole);
		}
        if (!strcmp(hole,"no_lod_texture"))
		{
	 		material->m_dwFlag |= _MAT_FLAG_NO_LOD_TEXTURE;
			GetR3State()->mSceneTextureDetail = 0;	//�̷���� �ɼ��� �����ϰ� �ؽ��ķε��� �ְ�� �Ѵ�.
		}
        if (!strcmp(hole,"detail_texture"))
		{
	        fscanf(fp,"%s",&hole[0]);
			strcpy(name,stTexturePath);
			strcat(name,hole);
	 		material->m_dwFlag |= _MAT_FLAG_NO_LOD_TEXTURE;
			GetR3State()->mSceneTextureDetail = 0;	//����� �����ϰ�� �ɼ��� �����ϰ� �ؽ��ķε��� �ְ�� �Ѵ�.(���߿��� ���ٰ���.)
			if( hole[0] != NULL && strcmp(hole,"NULL"))
			{
				material->m_iDetailSurface = R3GetPreTextureId(name);
			}
			else
			{
				material->m_iDetailSurface = 0;
			}
			material->m_fDetailScale=1.0;
		}
        if (!strcmp(hole,"no_collision"))	//�� ���͸����� ���� ��������� ��� �浹�˻縦 ���Ѵ�.
		{
			material->m_Layer[0].m_dwFlag |= _MAT_NO_COLLISON;	//0�� ���̾� �÷��׸� ����.
		}
        if (!strcmp(hole,"water"))	//�� ���͸����� ���� ��������� ��� �浹�˻縦 ���Ѵ�.
		{
			material->m_Layer[0].m_dwFlag |= _MAT_NO_COLLISON;	
			material->m_Layer[0].m_dwFlag |= _MAT_WATER;		//��ȿ����.
		}
        if (!strcmp(hole,"detail_texture_scale"))
		{
	        fscanf(fp,"%s",&hole[0]);
			material->m_fDetailScale=(float)atof(hole);
		}
        if (!strcmp(hole,"layer"))
		{
	        fscanf(fp,"%s",&hole[0]);
			l_id = atoi(hole);
//			if(l_id != 1) 
			LoadOneLayer(fp,material,l_id);
			sub = true;
		}		
	}
	GetR3State()->mSceneTextureDetail = temp_texture_detail;

	fclose(fp);
}

BOOL IsLoadedMaterial()	//���� ���͸����� �ε�Ǿ��°�?
{
	if( R3material )
		return TRUE;
	return FALSE;
}


//������ ���͸��� �ε�..
static _R3MATERIAL* LoadIndependenceMaterial(char *matpath)	//��ī�̹ڽ��� ��ƼƼ�� �Ʒ� addmaterial���� �߰��ؼ� ����...
{
	FILE *fp;
	char hole[256],name[256];
	int m_num,i,m_id;

	_R3MATERIAL *mat;

	strcpy(stTexturePath,matpath);
	strcpy(name,matpath);
	strcat(name,"MainMaterial.mst");
	if( (fp = fopen(name,"rt")) == NULL )	
	{
		Warning(name,"<-�������� ����.");
		return NULL;
	}

	_R3ENGINE_STATE *r3_state=GetR3State();
    while(1)
    {
		if( fscanf(fp,"%s",&hole[0])==EOF)		
			break;
//		if (!strcmp(hole,"}"))					break;
		strupr(hole);
        if (!strcmp(hole,"*MATERIAL_NUM"))
		{
            fscanf(fp,"%s",&hole[0]);
			m_num=atoi(hole);

			if( m_num >= _MAX_MATERIALS	)	
				Error("�ִ� ���͸��� �ʰ�","");

			mat = (_R3MATERIAL *)Dmalloc(m_num*sizeof(_R3MATERIAL));
			memset(mat,0,m_num*sizeof(_R3MATERIAL));
            fscanf(fp,"%s",&hole[0]);

			for(i=0; i<m_num; i++)
				MatInit(&mat[i]);	//���͸��� �ʱ�ȭ.
			for(i=0; i<m_num; i++)
			{
				strcpy(name,matpath);
	            fscanf(fp,"%s",&hole[0]);
				strcat(name,hole);
				strcpy(mat[i].m_name,hole);// mtrl name set
				strcat(name,".mst");
	            fscanf(fp,"%s",&hole[0]);
				m_id=atoi(hole);
				LoadOneMaterial(name,&mat[m_id]);
			}
		}

        if (!strcmp(hole,"*FOG_START"))
		{
			r3_state->mIsFog=TRUE;
            fscanf(fp,"%s",&hole[0]);
			r3_state->mMainFogStart=(float)atof(hole);
			r3_state->mFogStart=(float)atof(hole);
		}
        if (!strcmp(hole,"*FOG_END"))
		{
			r3_state->mIsFog=TRUE;
            fscanf(fp,"%s",&hole[0]);
			r3_state->mMainFogEnd=(float)atof(hole);
			r3_state->mFogEnd=(float)atof(hole);
			r3_state->mFar=r3_state->mFogEnd;
		}
        if (!strcmp(hole,"*FOG_COLOR"))
		{
			r3_state->mIsFog=TRUE;
			if( r3_state->mFogEnd == 0 )
			{
				r3_state->mFogEnd=5000;
				r3_state->mMainFogEnd=5000;
			}
			if( r3_state->mFogStart == 0 )
			{
				r3_state->mFogStart=5;
				r3_state->mMainFogStart=5;
			}
            fscanf(fp,"%s",&hole[0]);
			DWORD r=(DWORD)(atoi(hole));
            fscanf(fp,"%s",&hole[0]);
			DWORD g=(DWORD)(atoi(hole));
            fscanf(fp,"%s",&hole[0]);
			DWORD b=(DWORD)(atoi(hole));
			r3_state->mFogColor = 0xff000000|(r<<16)|(g<<8)|(b<<0);
			r3_state->mMainFogColor = 0xff000000|(r<<16)|(g<<8)|(b<<0);
		}
        if (!strcmp(hole,"*NO_FOG_SKY"))
		{
			r3_state->mIsNoFogSky=TRUE;
		}
		
        if (!strcmp(hole,"*FOG_RANGE"))
		{
            fscanf(fp,"%s",&hole[0]);
	        if (!strcmp(hole,"TRUE"))
				r3_state->mFogRangeEnable =TRUE;
		}

		//-----�����÷�� ���Ѱ���...
        if (!strcmp(hole,"*LENS_FLARE_SCALE"))
		{
			for(i=0; i<16; i++)
			{
	            fscanf(fp,"%s",hole);
				r3_state->mLensFlareScale[i]=(float)atof(hole);
			}
		}
        if (!strcmp(hole,"*LENS_FLARE"))
		{
            fscanf(fp,"%s",hole);
			strcpy(name,matpath);
			strcat(name,hole);
			r3_state->mLensFlareTex = R3GetPreTextureId(name);
			strcpy(r3_state->mLensTexName,name);
		}
        if (!strcmp(hole,"*LENS_FLARE_POS"))
		{
            fscanf(fp,"%s",hole);
			r3_state->mLensFlarePos[0] = (float)atof(hole);
            fscanf(fp,"%s",hole);
			r3_state->mLensFlarePos[2] = (float)atof(hole);
            fscanf(fp,"%s",hole);
			r3_state->mLensFlarePos[1] = (float)atof(hole);
		}
        if (!strcmp(hole,"*ENVIRONMENT"))//
		{	//---Environment ���� ��ƼƼ�̸� ����.  // ��,��,��Ÿ ��ǳ��...
            fscanf(fp,"%s",hole);
			strcpy(r3_state->mEnvEntityName,hole);// ȯ���� ��ƼƼ�� ���ٵȴ�.
		}
        if (!strcmp(hole,"*ENVIRONMENT_ID"))//
		{
            fscanf(fp,"%s",hole);
			r3_state->mEnvID = (DWORD)atoi(hole);
		}
	}

	mat->m_iMatNum = (WORD)m_num;//�ش� ��Ʈ�� ���͸��󰹼���..

	fclose(fp);
	return mat;
}


void LoadR3X(char *name)
{
	int i;
	float version;
	_EXT_MAT ext_mat;
	FILE *fp;
	_R3ENGINE_STATE *state = GetR3State();
	memset(&ext_mat,0,sizeof(_EXT_MAT));

	//--------�ش� ���͸��� ��ü ���....
	if( (fp = fopen(name,"rb")) == NULL) 
		return;
	fread(&version,sizeof(DWORD),1,fp);
	if( version != _R3X_VERSION )
	{
		Warning(name,"<-�� ������ ������ ���� �����Դϴ�.");
		fclose(fp);
		return;
	}
	fread(&ext_mat,sizeof(_EXT_MAT),1,fp);


	if( ext_mat.mFlag & EXT_MAT_EXIST_1ST_FOG )
	{
		state->mIsFog=TRUE;
		state->mFogColor=ext_mat.mFogColor;
		state->mFogStart=ext_mat.mFogStart;
		state->mFogEnd=ext_mat.mFogEnd ;
		state->mFar=state->mFogEnd;
		state->mMainFogStart=ext_mat.mFogStart;
		state->mMainFogEnd=ext_mat.mFogEnd;
		state->mMainFogColor=ext_mat.mFogColor;
	}
	/*
	if( ext_mat.mFlag & EXT_MAT_EXIST_2ND_FOG )
	{
		state->mIsFog2 = TRUE;
		state->mFogColor2=ext_mat.mFogColor2;
		state->mFogStart2=ext_mat.mFogStart2 ;
		state->mFogEnd2=ext_mat.mFogEnd2 ;
		Vector3fCopy(ext_mat.BBMin2,state->FogBBMin2);
		Vector3fCopy(ext_mat.BBMax2,state->FogBBMax2);
	}*/
	if( ext_mat.mFlag & EXT_MAT_EXIST_LENS_FLARE )	//���� �÷�� �ֵ�..
	{
		//state->mLensFlareTex;
		strcpy(state->mLensTexName,ext_mat.mLensTexName);	//Ǯ �н���...
		
		state->mLensFlareTex = R3GetPreTextureId(state->mLensTexName);

		Vector3fCopy(ext_mat.mLensFlarePos,state->mLensFlarePos);
		for(i=0; i<16; i++)
			state->mLensFlareScale[i]=ext_mat.mLensFlareScale[i];
	}
	if( ext_mat.mFlag & EXT_MAT_EXIST_ENV )
	{
		state->mEnvID = ext_mat.mEnvID;
		strcpy(state->mEnvEntityName,ext_mat.mEnvEntityName);
	}
	if( ext_mat.mFlag & EXT_MAT_NO_FOG_SKY )
		state->mIsNoFogSky = TRUE;
	if( ext_mat.mFlag & EXT_MAT_FOG_RANGE )
		state->mFogRangeEnable = TRUE;

	fclose(fp);
}

//���͸��� ���̵� �߸��Ȱ� ����.
void AdjustIndependenceR3M(_R3MATERIAL *mat,LONG old_base_tex_cnt,LONG new_base_tex_cnt)
{	//���� �Ȱ���..
	int i,j;
	
	for(i=0; i<mat->m_iMatNum; i++)
	{
		if( mat[i].m_iDetailSurface != -1 )
			mat[i].m_iDetailSurface = mat[i].m_iDetailSurface - old_base_tex_cnt + new_base_tex_cnt;
		for(j=0; j<(int)mat[i].m_dwLayerNum; j++)	//�� ���� �ȵ����� �뷮������ �̹����...
		{
			mat[i].m_Layer[j].m_iSurface = mat[i].m_Layer[j].m_iSurface - old_base_tex_cnt + new_base_tex_cnt;	//���� ���̽��� �� 
		}
	}
}

_R3MATERIAL* LoadIndependenceR3M(char *name)	//���� �Ȱ���..
{
	FILE *fp;
	int i,j;
	DWORD m_num;
	float version;
	
	if( (fp = fopen(name,"rb")) == NULL) 
	{
		Warning(name,"<-�� ������ �����ϴ�.");
		return NULL;
	}

	//------���� ����.
//	tex_id=(BYTE *)Dmalloc(sizeof(BYTE)*texcnt);
//	memset(tex_id,0,sizeof(BYTE)*texcnt);

	//���͸��� �κ� ..
	fread(&version,sizeof(float),1,fp);

	if( version != _R3M_VERSION )
	{
		Warning(name,"<-�� ������ ����...");
		return NULL;
	}

	fread(&m_num,sizeof(DWORD),1,fp);

	_R3MATERIAL *mat = (_R3MATERIAL *)Dmalloc(m_num*sizeof(_R3MATERIAL));
	memset(mat,0,m_num*sizeof(_R3MATERIAL));
	
	int base_tex_cnt = GetNowR3D3DTexCnt()-1;
	mat->m_iMatNum = m_num;
	for(i=0; i<mat->m_iMatNum; i++)
	{
		fread(&mat[i].m_dwLayerNum,sizeof(DWORD),1,fp);		//���̾��.
		fread(&mat[i].m_dwFlag,sizeof(BOOL),1,fp);			//�÷���.
		fread(&mat[i].m_iDetailSurface,sizeof(LONG),1,fp);		//����Ʈ�� ����.
		fread(&mat[i].m_fDetailScale,sizeof(float),1,fp);	//������ ������.
		fread(mat[i].m_name,sizeof(char)*_TEX_NAME_SIZE,1,fp);	//���͸��� �̸� 

		if( mat[i].m_iDetailSurface != -1 )
			mat[i].m_iDetailSurface += base_tex_cnt;
		for(j=0; j<(int)mat[i].m_dwLayerNum; j++)	//�� ���� �ȵ����� �뷮������ �̹����...
		{
			fread(&mat[i].m_Layer[j],sizeof(_ONE_LAYER),1,fp);	//���͸��� �̸� 
			mat[i].m_Layer[j].m_iSurface += base_tex_cnt;	//���� ���̽��� �� 
		}
//		if( mat[i].m_dwFlag )	//����� �������ؽ��ĸ� ���ִ�.
		if( mat[i].m_iDetailSurface != -1 )	//����� �������ؽ��ĸ� ���ִ�.
			mat[i].m_dwFlag |= _MAT_FLAG_NO_LOD_TEXTURE;
	}

	fclose(fp);
	return mat;
}

//�ؽ�Ʈ�� �� ���� ���͸���ε�.
_R3MATERIAL* LoadMainMaterial(char *matpath)	//��ī�̹ڽ��� ��ƼƼ�� �Ʒ� addmaterial���� �߰��ؼ� ����...
{
	_R3ENGINE_STATE *state=GetR3State();
	state->mFogColor=0xff000000;
	state->mIsFog=FALSE;			//�����ϴ� ����.
	state->mIsNoFogSky=FALSE;		//��ī�̹ڽ� ���װ� ���°�?
	state->mFogRangeEnable=FALSE;	//���� ����ų��?
	state->mEnvID=0;				//env �ʱ�ȭ.
	state->mLensFlareTex=-1;	//�����÷��� �ʱ�ȭ.

	if( R3material )
		ReleaseMainMaterial();
	R3material = LoadIndependenceMaterial(matpath);	//���� �Ȱ���..
	stdwMaterialNum = R3material->m_iMatNum;	//���͸��� ���� �÷�����..
	return R3material;	//��Ʈ�� 
}

_R3MATERIAL* LoadSubR3M(char *mat_name)	//���θ��͸��� �ܴ̿� �̰Ÿ�.
{
	_R3MATERIAL*ret = LoadIndependenceR3M(mat_name);	//���� �Ȱ���..
	return ret;	
}

//R3M�� ���͸��� ���̳ʸ� �����̴�. 
_R3MATERIAL* LoadMainR3M(char *name)	//��ī�̹ڽ��� ��ƼƼ�� �Ʒ� addmaterial���� �߰��ؼ� ����...
{
	_R3ENGINE_STATE *state=GetR3State();
	state->mFogColor=0xff000000;
	state->mIsFog=FALSE;			//�����ϴ� ����.
	state->mIsNoFogSky=FALSE;		//��ī�̹ڽ� ���װ� ���°�?
	state->mFogRangeEnable=FALSE;	//���� ����ų��?
	state->mEnvID=0;				//env �ʱ�ȭ.
	state->mLensFlareTex=-1;	//�����÷��� �ʱ�ȭ.

	char buf[256];

	strcpy(buf,name);
	StripEXT(buf);
	strcat(buf,".r3x");

	LoadR3X( buf );		//Ȯ�� ���͸����� �켱�̴�.

	if( R3material )
		ReleaseMainMaterial();
	R3material = LoadIndependenceR3M(name);	//���� �Ȱ���..
	stdwMaterialNum = R3material->m_iMatNum;	//���͸��� ���� �÷�����..
	return R3material;	//��Ʈ�� 
}

_R3MATERIAL* LoadSubMaterial(char *mat_path)	//���θ��͸��� �ܴ̿� �̰Ÿ�.
{
	_R3MATERIAL*ret = LoadIndependenceMaterial(mat_path);	//���� �Ȱ���..
	return ret;	
}

//�ܼ� ��ġ���� �ε��Ҷ�
void ReLoadMaterial(char *matpath,_R3MATERIAL *now_material)
{
	FILE *fp;
	char hole[256],name[256];
	int m_num,i,m_id;

	strcpy(stTexturePath,matpath);
	strcpy(name,matpath);
	strcat(name,"MainMaterial.mst");
	if( (fp = fopen(name,"rt")) == NULL )
		return;

    while(1)
    {
		if( fscanf(fp,"%s",&hole[0])==EOF)		break;

        if (!strcmp(hole,"*MATERIAL_NUM"))
		{
            fscanf(fp,"%s",&hole[0]);
			m_num=atoi(hole);
            fscanf(fp,"%s",&hole[0]);

			if( m_num >= _MAX_MATERIALS	)	
				Error("�ִ� ���͸��� �ʰ�","");

			for(i=0; i<m_num; i++)
			{
				strcpy(name,stTexturePath);
	            fscanf(fp,"%s",&hole[0]);
				strcat(name,hole);
				strcpy(now_material[i].m_name,hole);// mtrl name set
				strcat(name,".mst");
	            fscanf(fp,"%s",&hole[0]);
				m_id=atoi(hole);
				LoadOneMaterial(name,&now_material[m_id]);
			}
		}
	}

	fclose(fp);
}

void ReleaseSubMaterial(_R3MATERIAL *mat)
{
	if( mat )
	{
		Dfree(mat);
	}
}

void ReleaseMainMaterial()
{
	if( R3material )
	{
		Dfree(R3material);
	}
	R3material=NULL;
	stdwMaterialNum=0;
}

DWORD GetMainMaterialNum()
{
	return stdwMaterialNum;
}

void SetNoLodTextere()	//lod�ؽ��İ� �ƴѰ��.
{
	DWORD i,j;

	for(i=0; i<stdwMaterialNum; i++)
	{
		if( R3material[i].m_dwFlag & _MAT_FLAG_NO_LOD_TEXTURE )
		{
			SetR3D3DTexture( R3material[i].m_iDetailSurface,_R3D3DTEXTURE_FLAG_NO_LOD );
			for(j=0; j<R3material[i].m_dwLayerNum; j++)
			{
				SetR3D3DTexture( R3material[i].m_Layer[j].m_iSurface,_R3D3DTEXTURE_FLAG_NO_LOD );				
			}
		}
	}
}

