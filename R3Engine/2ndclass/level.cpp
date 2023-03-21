#include "level.h"
#include "R3bsp.h"
#include "R3d3dtex.h"
#include "core.h"
#include "Skybox.h"
#include "R3input.h"

#include "Jmalloc.h"
#include "JError.h"
#include "Collision.h"

#include "CommonUtil.h"
#include "R3LightMap.h"

#include "R3move.h"
#include "AniCamera.h"
#include "R3render.h"
#include "R3util.h"
#include "R3Sound.h"
#include "MultiLayer.h"
#include "R3engineKernel.h"
#include "RTMovie.h"
#include <stdio.h>

//#define Y_ABLE_RATE			5.67// 80�� ��� 3.27f=73��	  ; 2.74f=70��
#define Y_ABLE_RATE			1.73f	// 80�� ��� 3.27f=73��	  ; 2.74f=70��
#define _ABLE_HEIGHT  23

CLevel::CLevel()
{
	mBsp = new CBsp;
	mSkyBox = new CSkyBox;
	mIsLoadedBsp=FALSE;
	mMapName[0]=NULL;

	//----���� ���� �ؽ��� �޸𸮻�����...
	mLightTexMemSize=0;
	mMapTexMemSize=0;
	mSkyTexMemSize=0;
	mEntityTexMemSize=0;
}

CLevel::~CLevel()
{
	delete mSkyBox;
	delete mBsp;
}

void CLevel::ReleaseLevel()
{
	if(mBsp)
		mBsp->ReleaseBsp();
	mIsLoadedBsp=FALSE;
	if( IsServerMode()== FALSE )	//������尡 �ƴҶ�..
	{
//		ReleaseConsole();
	    R3ReleaseAllTextures();	//��,��������Ʈ,�ý��� �ؽ��� �� ������.

		ReleaseMainMaterial();
		ReleaseLightMap();
		R3ReleasePreTextures();
		if(mSkyBox)
			mSkyBox->ReleaseSkyBox();
//		ReleaseAniCamera();
		_R3ENGINE_STATE *state = GetR3State();
		state->mLensFlareTex = -1;

		mLightTexMemSize=0;
		mMapTexMemSize=0;
		mSkyTexMemSize=0;
		mEntityTexMemSize=0;
		
		RTMovieRelease();	//����Ÿ�ӹ��� ������..
		mDummy.ReleaseExtDummy();
	}
	else
		ReleaseMainMaterial();
}
//#include "afterscript.h"

void CLevel::LoadLevel(char *name)
{
	char buf[256];
	char f_buf[256];
	char real_bsp_name[256];
	char tempname[256];

	_R3ENGINE_STATE *state = GetR3State();

	if( name==NULL )
	{
		if( mMapName[0]==NULL )	//�ʱ�ȭ...
			return;
		//strcpy(f_buf,mMapName);
		sprintf(f_buf,"%s\\%s\\%s.bsp",state->MapPath,mMapName,mMapName);
		name = f_buf;
	}

	FILE *fp;
	fp = fopen(name,"rb");
	if(fp == NULL )
	{
		mBsp->SetIsLoaded(FALSE);
		if( name[0] == NULL )
			Error("������������ ���� �ε��Ϸ� �ϰ��ֽ��ϴ�.","");
		else
			Error(name,"<-�� �������� �����ϴ�.");
		return;
	}
	fclose(fp);

	strcpy(buf,name);
	StripEXT(buf);
	StripPath(buf);
	strcpy(real_bsp_name,state->MapPath);
	strcat(real_bsp_name,buf);
	strcat(real_bsp_name,"\\");
	strcat(real_bsp_name,buf);
	strcat(real_bsp_name,".bsp");

	ResetTexMemSize();
	if( IsServerMode()== FALSE )	//���͸����� �����˾ƾ� ���ؽ����۸� �����Ҽ��ִ�.������尡 �ƴҶ�..
	{
//		InitConsole();

		char load_name[256];

		strcpy(load_name,real_bsp_name);
		StripEXT(load_name);
		strcat(load_name,".r3m");

		strcpy(buf,name);
		StripEXT(buf);
		StripPath(buf);
		strcpy(mMapName,buf);
		if( IsExistFile(load_name) )	//r3m������ �����ϸ�..
		{
			LoadMainR3M(load_name);

			strcpy(load_name,real_bsp_name);
			StripEXT(load_name);
			strcat(load_name,".r3t");
			R3Texture *t_tex = R3GetTexInfoR3T(load_name,0);	//R3T�ؽ��� ������ �����Ѵ�.
			SetNoLodTextere();	//lod�ؽ��� �ȸԾ�ߵǴ°�..
			LoadR3T(t_tex);
		}
		else
		{
			strcpy(load_name,state->MapPath);
			strcat(load_name,"\\");
			strcat(load_name,buf);
			strcat(load_name,"\\Tex\\");
			LoadMainMaterial(load_name);
			SetNoLodTextere();	//lod�ؽ��� �ȸԾ�ߵǴ°�..
		}
		mMapTexMemSize += GetNowTexMemSize();
	}
	else	//��������϶� 
	{
		char load_name[256];

		strcpy(load_name,real_bsp_name);
		StripEXT(load_name);
		strcat(load_name,".r3m");

		if( IsExistFile(load_name) )	//r3m������ �����ϸ�..
			LoadMainR3M(load_name);
	}

	ResetTexMemSize();

	mBsp->LoadBsp(real_bsp_name);
	
	//ext bsp �ε�.
	strcpy(tempname,real_bsp_name);	
	StripEXT(tempname);
	strcat(tempname,".ebp");
	mBsp->LoadExtBsp(tempname);
	mEntityTexMemSize+= GetNowTexMemSize();

	if( IsServerMode()== FALSE )	//������尡 �ƴҶ�..
	{
		strcpy(tempname,real_bsp_name);
		StripEXT(tempname);
		strcat(tempname,"Lgt.R3T");

		ResetTexMemSize();
		LoadLightMap(tempname);
		mLightTexMemSize+= GetNowTexMemSize();
		mBsp->CalcEntitiesMainColor();	//��ƼƼ�� �ٴ��÷����.
		
		strcpy(tempname,state->MapPath);
		strcat(tempname,buf);
		strcat(tempname,"\\sky\\");
		strcat(tempname,mMapName);
		strcat(tempname,"sky.r3e");

		ResetTexMemSize();
		if(IsExistFile(tempname))
			mSkyBox->LoadSkyBox(tempname);
		mSkyTexMemSize+= GetNowTexMemSize();

		ResetTexMemSize();
	    R3RestoreAllTextures();	//��,��������Ʈ,�ý��� �ؽ��� �� �ø���.
		mMapTexMemSize += GetNowTexMemSize();

		mIsLoadedBsp=TRUE;
		strcpy(tempname,real_bsp_name);
		StripEXT(tempname);
		strcat(tempname,".rvp");
		RTMovieCreate(tempname,this);	//����Ÿ�� �������� ������ �ε��Ѵ�.
		RTMovieSetState(RTM_STATE_NO_LOGO|RTM_STATE_NO_WIDE|RTM_STATE_NO_CAMERAMOVE);	//����Ʈ ����.

		strcpy(tempname,real_bsp_name);
		StripEXT(tempname);
		strcat(tempname,"EXT.spt");
		//���� ������ �ε��Ѵ�.
		mDummy.LoadExtDummy(tempname);
	}
	mIsLoadedBsp=TRUE;
	ClearDynamicLight();
	//Ű������� Ŭ���.
	memset(gAsci,0,256);
	mBsp->SetIsLoaded(TRUE);
}


void CLevel::ReLoadAllMaterial()
{
	char material_name[256];

	SetReLoadState(TRUE);
	strcpy(material_name,mMapName);
	ReLoadMaterial(material_name,GetMainMaterial());

//	strcat(material_name,"sky");
//	ReLoadMaterial(material_name,GetSkyMaterial());
	SetReLoadState(FALSE);
}

void CLevel::SetViewMatrix(D3DXMATRIX *view_mat)
{
	Vector3f pos={0,0,0};
	memcpy(&mMatView,view_mat,sizeof(D3DXMATRIX));	
	GetR3State()->mMatView = mMatView;
	R3GetQuakeVector(pos);
	mMatView.m[3][0] +=pos[0];
	mMatView.m[3][1] +=pos[1];
	mMatView.m[3][2] +=pos[2];
	GetR3State()->mMatView.m[3][0] +=pos[0];
	GetR3State()->mMatView.m[3][1] +=pos[1];
	GetR3State()->mMatView.m[3][2] +=pos[2];
}
void CLevel::FrameMove()	//�� �������ϱ����� ȣ���ϴ°�..
{
//	R3QuakeMatrix(mMatView.m);
	//�����غ�.

	RTMovieFrameMove();	//����Ÿ�ӷ����� �����ӹ��� 

    D3DXMATRIX matWorld;
    D3DXMatrixIdentity( &matWorld );
    GetD3dDevice()->SetTransform( D3DTS_WORLD, &matWorld );
	
	GetD3dDevice()->SetTransform( D3DTS_VIEW, &mMatView );
	SetDefaultFlags();
	SetFrustumNormalPlane();	//core�� view frustum ����� 
	SetMultiLayerTime(R3GetTime());


//	ClearShadows();	//������ Ŭ���� ..
	ClearDynamicLight();	//���̳��� ����Ʈ Ŭ���� �ؾ���...

	//-----BSP�� �����غ�.
	ResetVertexBufferCache();	//�������� ���� �Ҷ� �ݵ�� ȣ������.. 

	mBsp->ReadyBspRender(mCamPos);	//Bsp�������غ�

	mBsp->CalcObjectLoop();		//������Ʈ���� ������ ������ ��ƾ�� �ִ� ��.

	UpdateStreamMP3();		//mp3�� ���Ѱ�...

	CalcR3Fog();	//���� �̸� ���.
}
void CLevel::DrawSkyBoxRender()
{
	mSkyBox->DrawSkyBox((void *)&mMatView);
	DrawFadeSKy();	//�ϴ� ���̵� �κκ�..
}
void CLevel::DrawMapEntitiesRender()	//�� ��ƼƼ�� ������.
{
	mBsp->DrawMapEntitiesRender();		//�ʾ��� ��ƼƼ���� �������Ѵ�.
}

void CLevel::PrepareShadowRender(Vector3f ch_pos,void *tex,float intensity,DWORD blur,float scale,float add_pos)	// 
{
	mBsp->PrepareShadowRender(ch_pos,tex,intensity,blur,scale,add_pos);
}
void CLevel::DrawShadowRender(Vector3f ch_pos)
{
	mBsp->DrawShadowRender(ch_pos);
}

void CLevel::SetCameraPos(Vector3f camera)
{
	mCamPos[0] = camera[0];
	mCamPos[1] = camera[1];
	mCamPos[2] = camera[2];
}

void CLevel::DrawMapRender()
{
	D3DXMATRIX final;
	final=mMatView;	//����� ���δ�..

//	mBsp->ReadyBspRender(mCamPos);	//Bsp�������غ�
	mBsp->DrawBspRender();			//��濡 �ش�Ǵ� �κз�����.
	mBsp->DrawDynamicLights();
//	DrawFogTest();
//	extern void DrawEtcPrint();
//	DrawEtcPrint();	//������ �ڽ��� ��ġ ������ ����Ʈ�� ȭ�鿡 ���
	RTMovieRender();	//����Ÿ�� ������.
}
void CLevel::DrawMapAlphaRender(Vector3f pos)
{	
	if( !IsLoadedBsp())
		return;

	ResetVertexBufferCache();	//������ �������� ���� �Ҷ� �ݵ�� ȣ������.. 
	_R3ENGINE_STATE *state = GetR3State();
	pos[0] = state->mInvMatView.m[3][0];
	pos[1] = state->mInvMatView.m[3][1];
	pos[2] = state->mInvMatView.m[3][2];
	mBsp->DrawAlphaRender(pos);	//��� ������������� ������. ī�޶� ��ġ�� �־�� �ȴ�.
}

void CLevel::ScreenShot()
{
	char name[256];

	_R3ENGINE_STATE *state = GetR3State();
	for(int i=0; i<9999; i++)
	{
/*
		sprintf(name,"%s%s%04d.bmp",state->ScreenShotsPath,mMapName,i);
		if( IsExistFile(name) == FALSE )
		{
			FramebufferToBMP(name);
			break;
		}
*/
		sprintf(name,"%s%s%04d.jpg",state->ScreenShotsPath,mMapName,i);
		if( IsExistFile(name) == FALSE )
		{
			FramebufferToJPG(name);
			break;
		}
	}
}
void CLevel::DrawTestBox(Vector3f box_size,Vector3f pos,DWORD color)		//����׽�Ʈ�� �ڽ���ƿ...
{
    D3DXMATRIX matWorld;
    D3DXMatrixIdentity( &matWorld );
	matWorld.m[3][0] = pos[0];
	matWorld.m[3][1] = pos[1];
	matWorld.m[3][2] = pos[2];
    GetD3dDevice()->SetTransform( D3DTS_WORLD, &matWorld );

	::DrawTestBox(box_size[0],box_size[1],box_size[2],color);

}
BOOL CLevel::GetPointFromScreenRay(LONG x,LONG y,Vector3f *get_point)
{
	Vector3f src;
	_R3ENGINE_STATE *state = GetR3State();
	x -= state->mViewSx;
	y -= state->mViewSy;
	return mBsp->GetPointFromScreenRay((float)x,(float)y,*get_point,src);
}
BOOL CLevel::GetPointFromScreenRayFar(LONG x,LONG y,Vector3f *get_point)	//���� �հ����� ���´�.
{
	Vector3f src;
	_R3ENGINE_STATE *state = GetR3State();
	x -= state->mViewSx;
	y -= state->mViewSy;
	return mBsp->GetPointFromScreenRayFar((float)x,(float)y,*get_point,src);
}
/*
BOOL CLevel::GetPointFromScreenRay(LONG x,LONG y,Vector3f *tar_point,Vector3f *src_point)
{
	return mBsp->GetPointFromScreenRay((float)x,(float)y,*tar_point,*src_point);
}
*/
float CLevel::GetFirstYpos(Vector3f pos,int option)
{
	if( !IsLoadedBsp() )	//�� �ε��� �ȉ��.
		return FALSE;
	return mBsp->GetFirstYpos(pos,option);
}
float CLevel::GetFirstYpos(Vector3f pos,Vector3f min,Vector3f max)
{
	if( !IsLoadedBsp() )	//�� �ε��� �ȉ��.
		return FALSE;
	return mBsp->GetFirstYpos(pos,min,max);
}
float CLevel::GetFirstYpos(Vector3f pos,Vector3s min,Vector3s max)	//�ٿ�� �ڽ� �����ȿ��� ���Ѵ�.���Ҽ� ���°�� 65535�� ����
{
	if( !IsLoadedBsp() )	//�� �ε��� �ȉ��.
		return FALSE;
	return mBsp->GetFirstYpos(pos,min,max);
}
void CLevel::DrawLeafBBox()
{
	mBsp->DrawLeafBBox();
}
void CLevel::DrawMatBBox()
{
	mBsp->DrawMatBBox();
}

BOOL CLevel::GetNextYpos(Vector3f tpos,float *ypos)
{
	BOOL is_can=TRUE;
	int leaf_num=0,i;
	short leaf_id[256];
	float temp_ypos=-100000;


	if( !IsLoadedBsp() )	//�� �ε��� �ȉ��.
		return FALSE;

	tpos[1]-=_ABLE_HEIGHT;	//�������� �����ؼ�...
	leaf_id[0] = mBsp->GetLeafNum(tpos);
	tpos[1]+=_ABLE_HEIGHT*2;	//���� ���̸� �ö󰥼������� �������� �ִ��� ýũ�غ���.
	leaf_id[1] = mBsp->GetLeafNum(tpos);
	tpos[1]-=_ABLE_HEIGHT;	//���� ���ڸ�

	if( leaf_id[0] == leaf_id[1] )
		leaf_num=1;
	else
		leaf_num=2;

	Vector3f b_camera,n_camera;
	Vector3fCopy(tpos,b_camera);
	Vector3fCopy(tpos,n_camera);

	b_camera[1] += 30000;
	n_camera[1] -= 30000;

	is_can = FALSE;
	for( i=0; i<leaf_num; i++)
	{
		float y_height = mBsp->GetYposInLeaf(b_camera,n_camera,_ABLE_HEIGHT,tpos[1],leaf_id[i] );

		if( y_height != -32000 )
		{
			if( y_height > temp_ypos )
			{
				is_can = TRUE;
				temp_ypos = y_height;
			}
		}
	}
	if( is_can )
		*ypos = temp_ypos;
	return is_can;
}


BOOL CLevel::GetNextYposNoAttr(Vector3f tpos,float *ypos)
{
	BOOL is_can=TRUE;
	int leaf_num=0,i;
	short leaf_id[256];
	float temp_ypos=-100000;

	if( !IsLoadedBsp() )	//�� �ε��� �ȉ��.
		return FALSE;

	tpos[1]-=_ABLE_HEIGHT;	//�������� �����ؼ�...
	leaf_id[0] = mBsp->GetLeafNum(tpos);
	tpos[1]+=_ABLE_HEIGHT*2;	//���� ���̸� �ö󰥼������� �������� �ִ��� ýũ�غ���.
	leaf_id[1] = mBsp->GetLeafNum(tpos);
	tpos[1]-=_ABLE_HEIGHT;	//���� ���ڸ�

	if( leaf_id[0] == leaf_id[1] )
		leaf_num=1;
	else
		leaf_num=2;

	Vector3f b_camera,n_camera;
	Vector3fCopy(tpos,b_camera);
	Vector3fCopy(tpos,n_camera);

	b_camera[1] += 9999;
	n_camera[1] -= 9999;

	is_can = FALSE;
	for( i=0; i<leaf_num; i++)
	{
		float y_height = mBsp->GetYposInLeafNoAttr(b_camera,n_camera,_ABLE_HEIGHT,tpos[1],leaf_id[i] );

		if( y_height != -32000 )
		{
			if( y_height > temp_ypos )
			{
				is_can = TRUE;
				temp_ypos = y_height;
			}
		}/*
		else
		{
			b_camera[0] += _ABLE_HEIGHT/2;
			n_camera[1] += _ABLE_HEIGHT/2;
			float y_height = mBsp->GetYposInLeafNoAttr( b_camera,n_camera,_ABLE_HEIGHT,tpos[1],leaf_id[i] );
			if( y_height != -32000 )
			{
				is_can = TRUE;
				temp_ypos = y_height;
			}
			else	//������ ����.
			{
			}			
		}*/
	}
	if( is_can )
		*ypos = temp_ypos;
	return is_can;
}

BOOL CLevel::GetNextYposForServer(Vector3f tpos,float *ypos)	//������ġ�� y��ǥ��..	���������� ����. ��������� ������ �����Ƿ� �ӵ� ����.
{
	BOOL is_can=TRUE;
	int leaf_num=0,i;
	short leaf_id[256];
	float temp_ypos=-100000;

	tpos[1]-=_ABLE_HEIGHT;	//�������� �����ؼ�...
	leaf_id[0] = mBsp->GetLeafNum(tpos);
	tpos[1]+=_ABLE_HEIGHT*2;	//���� ���̸� �ö󰥼������� �������� �ִ��� ýũ�غ���.
	leaf_id[1] = mBsp->GetLeafNum(tpos);
	tpos[1]-=_ABLE_HEIGHT;	//���� ���ڸ�

	if( leaf_id[0] == leaf_id[1] )
		leaf_num=1;
	else
		leaf_num=2;

	Vector3f b_camera,n_camera;
	Vector3fCopy(tpos,b_camera);
	Vector3fCopy(tpos,n_camera);

	b_camera[1] += 9999;
	n_camera[1] -= 9999;

	temp_ypos = -100000;
	is_can = FALSE;
	for( i=0; i<leaf_num; i++)
	{
		float y_height = mBsp->GetYposInLeafUseEdgeNormal(b_camera,n_camera,_ABLE_HEIGHT,tpos[1],leaf_id[i] );

		if( y_height != -32000 )
		{
			if( temp_ypos < y_height )
			{
				if( y_height > temp_ypos )
				{
					is_can = TRUE;
					temp_ypos = y_height;
				}
			}
		}/*
		else
		{
			b_camera[0] += _ABLE_HEIGHT/2;
			n_camera[1] += _ABLE_HEIGHT/2;
			float y_height = mBsp->GetYposInLeafUseEdgeNormal( b_camera,n_camera,_ABLE_HEIGHT,tpos[1],leaf_id[i] );
			if( y_height != -32000 )
			{
				is_can = TRUE;
				temp_ypos = y_height;
			}
			else	//������ ����.
			{
			}			
		}*/
	}
	if( is_can )
		*ypos = temp_ypos;
	return is_can;
}
BOOL CLevel::GetNextYposFar(Vector3f src_pos,Vector3f tar_pos,float *ypos)
{
	BOOL is_can=TRUE;
	LONG leaf_num=0,i;
	SHORT leaf_id[32000];
	Vector3f temp_pos;
	float temp_ypos;

	if( !IsLoadedBsp() )	//�� �ε��� �ȉ��.
		return FALSE;
	if( src_pos[0]== tar_pos[0] && src_pos[1]== tar_pos[1] && src_pos[2]== tar_pos[2])
	{
		*ypos=src_pos[1];
		return TRUE;
	}

#define _CHECK_MIN_DIST	25.0f
	Vector3f t_src_pos,t_tar_pos;

	Vector3fCopy(src_pos,t_src_pos);
	Vector3fCopy(tar_pos,t_tar_pos);


	while( 1 )
	{
		temp_ypos=-100000;
		float dist = GetDist(t_src_pos,t_tar_pos);
		if( dist > _CHECK_MIN_DIST )
		{
			Vector3fSub(t_tar_pos,t_src_pos,temp_pos);
			Normalize(temp_pos);
			Vector3fMul(temp_pos,_CHECK_MIN_DIST,temp_pos);
			Vector3fAdd(t_src_pos,temp_pos,t_tar_pos);
			dist = _CHECK_MIN_DIST;
		}
		float able_height=dist*Y_ABLE_RATE;	//Y_ABLE_RATE��� ��������.
		if( able_height < _ABLE_HEIGHT )
			able_height = _ABLE_HEIGHT;

		Vector3fCopy(t_tar_pos,temp_pos);

		//���� �ִ� ���Ʒ� ���̸� ����� ��ߵȴ�.
		temp_pos[1] = t_tar_pos[1]+able_height;
		t_tar_pos[1]-=able_height;
		mBsp->GetLeafList(temp_pos,t_tar_pos,&leaf_num,leaf_id,32000);
		t_tar_pos[1]+=able_height;

		Vector3f b_camera,n_camera;
		Vector3fCopy(t_tar_pos,b_camera);
		Vector3fCopy(t_tar_pos,n_camera);

		b_camera[1] += 9999;
		n_camera[1] -= 9999;

		is_can = FALSE;
		for( i=0; i<leaf_num; i++)
		{
			float y_height = mBsp->GetYposInLeaf(b_camera,n_camera,able_height,t_tar_pos[1],leaf_id[i] );

			if( y_height != -32000 )
			{
				if( temp_ypos < y_height )
				{
					is_can = TRUE;
					temp_ypos = y_height;
				}
			}
		}
		if( !is_can )
			return FALSE;
		if( t_tar_pos[0] == tar_pos[0] && t_tar_pos[2] == tar_pos[2] )	//ýũ ���޵�.
			break;
		Vector3fCopy(t_tar_pos,t_src_pos);
		Vector3fCopy(tar_pos,t_tar_pos);
		t_src_pos[1]=temp_ypos;
		t_tar_pos[1]=temp_ypos;
	}
	*ypos = temp_ypos;
	return TRUE;
}

static DWORD CollisionLevel(float org,float best_add_height,float col_height)	//0�� �������� ���� ���� �ܰ��.
{
	float best_height = org + best_add_height;
	if( org <= col_height )
	{
		return (DWORD)((col_height-org)/(best_height - org))*2;
	}
	else
	{
		return (DWORD)((org-col_height)/(best_height - org))*2+1;
	}
}


BOOL CLevel::GetNextYposFarProgress(Vector3f src_pos,Vector3f tar_pos,float *ypos)
{
	if( !IsLoadedBsp() )	//�� �ε��� �ȉ��.
		return FALSE;
	if( src_pos[0]== tar_pos[0] && src_pos[1]== tar_pos[1] && src_pos[2]== tar_pos[2])
	{
		*ypos=src_pos[1];
		return TRUE;
	}
	tar_pos[1] = src_pos[1];

	BOOL is_can=TRUE;
	LONG leaf_num=0,i,j;
	SHORT leaf_id[10512];
	Vector3f temp_pos;
	float temp_ypos;

#define _CHECK_MIN_DIST	25.0f
	Vector3f t_src_pos,t_tar_pos;

	Vector3fCopy(src_pos,t_src_pos);
	Vector3fCopy(tar_pos,t_tar_pos);


	while( 1 )
	{
		temp_ypos=-100000;
		float dist = GetDist(t_src_pos,t_tar_pos);
		if( dist > _CHECK_MIN_DIST )
		{
			Vector3fSub(t_tar_pos,t_src_pos,temp_pos);
			Normalize(temp_pos);
			Vector3fMul(temp_pos,_CHECK_MIN_DIST,temp_pos);
			Vector3fAdd(t_src_pos,temp_pos,t_tar_pos);
			dist = _CHECK_MIN_DIST;
		}
		float able_height=dist*Y_ABLE_RATE;	//Y_ABLE_RATE��� ��������.
		if( able_height < _ABLE_HEIGHT )
			able_height = _ABLE_HEIGHT;

		Vector3fCopy(t_tar_pos,temp_pos);

		//���� �ִ� ���Ʒ� ���̸� ����� ��ߵȴ�.
		temp_pos[1] = t_tar_pos[1]+30000;
		t_tar_pos[1]-=30000;
		mBsp->GetLeafList(temp_pos,t_tar_pos,&leaf_num,leaf_id,10000);
		t_tar_pos[1]+=30000;

		Vector3f b_camera,n_camera;
		Vector3fCopy(t_tar_pos,b_camera);
		Vector3fCopy(t_tar_pos,n_camera);

		b_camera[1] += 30000;
		n_camera[1] -= 30000;

		is_can = FALSE;
		DWORD best_level=10000;
		for( i=0; i<leaf_num; i++)
		{
			for(j=0; j<2; j++ )		//������ ���� 2���� ������.
			{
				float y_height;
				y_height = mBsp->GetBestYposInLeaf(b_camera,n_camera,able_height,t_tar_pos[1],leaf_id[i] );
				
				if( y_height != -32000 )
				{
					DWORD h_level = CollisionLevel(t_tar_pos[1],able_height,y_height);
					if( best_level == h_level ) 
					{
						if( temp_ypos < y_height )
						{
							is_can = TRUE;
							temp_ypos = y_height;
						}
					}
					else
					if( best_level > h_level ) 
					{
						best_level=h_level;
						is_can = TRUE;
						temp_ypos = y_height;
					}
				}
				b_camera[0]+=0.5f;
				n_camera[0]+=0.5f;
			}
			b_camera[0]-=1;
			n_camera[0]-=1;
		}
		if( !is_can )
			return FALSE;
		if( t_tar_pos[0] == tar_pos[0] && t_tar_pos[2] == tar_pos[2] )	//ýũ ���޵�.
			break;
		Vector3fCopy(t_tar_pos,t_src_pos);
		Vector3fCopy(tar_pos,t_tar_pos);
		t_src_pos[1]=temp_ypos;
		t_tar_pos[1]=temp_ypos;
	}
	*ypos = temp_ypos;
	return TRUE;

}

/*
���������� Ypos�� �ӵ����������� �ŷ������ٴ� ĳ���� Ű(����18)�� ����� ���� �Ÿ����� ���Լ��� 
����ϴ� ���� ������. 
���� ��� ���Ͱ� 1�ʿ� 40���� �����δٸ� 18�̶� �Ÿ��� 0.45�ʸ��� ���Ƿ� 0.45�� ���� ypos�� �����ϸ� 
�ǰڴ�. ������ ���� ��Ȯ�ؾߵ� �ʿ䰡 �����Ƿ� ������ ��0.2�������� ��������..
*/
BOOL CLevel::GetNextYposForServerFar(Vector3f src_pos,Vector3f tar_pos,float *ypos)
{
	BOOL is_can=TRUE;
	LONG leaf_num=0,i;
	SHORT leaf_id[32000];
	Vector3f temp_pos;
	float temp_ypos=-100000;

	if( src_pos[0]== tar_pos[0] && src_pos[1]== tar_pos[1] && src_pos[2]== tar_pos[2])
	{
		*ypos=src_pos[1];
		return TRUE;
	}

#define _CHECK_MIN_DIST_FOR_SERVER	30.0f
	Vector3f t_src_pos,t_tar_pos;

	Vector3fCopy(src_pos,t_src_pos);
	Vector3fCopy(tar_pos,t_tar_pos);


	while( 1 )
	{
		temp_ypos=-100000;
		float dist = GetDist(t_src_pos,t_tar_pos);
		if( dist > _CHECK_MIN_DIST_FOR_SERVER )
		{
			Vector3fSub(t_tar_pos,t_src_pos,temp_pos);
			Normalize(temp_pos);
			Vector3fMul(temp_pos,_CHECK_MIN_DIST,temp_pos);
			Vector3fAdd(t_src_pos,temp_pos,t_tar_pos);
			dist = _CHECK_MIN_DIST_FOR_SERVER;
		}
		float able_height=dist*Y_ABLE_RATE;	//Y_ABLE_RATE��� ��������.
		if( able_height < _ABLE_HEIGHT )
			able_height = _ABLE_HEIGHT;

		Vector3fCopy(t_tar_pos,temp_pos);

		//���� �ִ� ���Ʒ� ���̸� ����� ��ߵȴ�.
		temp_pos[1] = t_tar_pos[1]+able_height;
		t_tar_pos[1]-=able_height;
		mBsp->GetLeafList(temp_pos,t_tar_pos,&leaf_num,leaf_id,32000);
		t_tar_pos[1]+=able_height;

		Vector3f b_camera,n_camera;
		Vector3fCopy(t_tar_pos,b_camera);
		Vector3fCopy(t_tar_pos,n_camera);

		b_camera[1] += 9999;
		n_camera[1] -= 9999;

		is_can = FALSE;
		for( i=0; i<leaf_num; i++)
		{
			float y_height = mBsp->GetYposInLeaf(b_camera,n_camera,able_height,t_tar_pos[1],leaf_id[i] );

			if( y_height != -32000 )
			{
				if( temp_ypos < y_height )
				{
					is_can = TRUE;
					temp_ypos = y_height;
				}
			}
		}
		if( !is_can )
			return FALSE;
		if( t_tar_pos[0] == tar_pos[0] && t_tar_pos[2] == tar_pos[2] )	//ýũ ���޵�.
			break;
		Vector3fCopy(t_tar_pos,t_src_pos);
		Vector3fCopy(tar_pos,t_tar_pos);
		t_src_pos[1]=temp_ypos;
		t_tar_pos[1]=temp_ypos;
	}
	*ypos = temp_ypos;
	return TRUE;
}

BOOL CLevel::IsCollisionRayAABB(LONG x,LONG y,Vector3f BBmin,Vector3f BBmax, Vector3f *get_cross)
{
	_R3ENGINE_STATE *state = GetR3State();
	BOOL is_col=FALSE;
	Vector3f from,at;
	float v[3],n[3];

	x -= state->mViewSx;
	y -= state->mViewSy;

    n[0] =  ( ( ( 2.0f * x ) / (state->mViewXL) ) - 1.0f ) / state->mMatProj._11;
    n[1] = -( ( ( 2.0f * y ) / (state->mViewYL) ) - 1.0f) / state->mMatProj._22;
    n[2] =  1.0f;	//���� �޼� ��ǥ���...

	v[0] = n[0]*3660000;		//���ڰ� Ŀ�� ������ ��������????
	v[1] = n[1]*3660000;
	v[2] = n[2]*3660000;

    // Get the inverse view matrix
    D3DXMATRIX m;
    D3DXMatrixInverse( &m, NULL, &state->mMatView );

    // Transform the screen space pick ray into 3D space
    at[0]  = v[0]*m._11 + v[1]*m._21 + v[2]*m._31;
    at[1]  = v[0]*m._12 + v[1]*m._22 + v[2]*m._32;
    at[2]  = v[0]*m._13 + v[1]*m._23 + v[2]*m._33;
    from[0] = m._41;
    from[1] = m._42;
    from[2] = m._43;

	if( IsRayAABB(BBmin,BBmax,from,at,get_cross) )
		return TRUE;
	else
		return FALSE;
}

DWORD CLevel::GetPath(Vector3f from,Vector3f at,Vector3f get[16],DWORD *cnt,int depth)	//�н� ���δ�.
{
	return mBsp->GetPathFind(from,at,get,cnt,depth);	//�н� ���δ�.
}

BOOL CLevel::GetPathFromDepth(Vector3f from,Vector3f at,int depth,Vector3f get[16],DWORD *cnt)	//���� ������ �н� ���δ�.
{
	return mBsp->GetPathFind( from, at, get,cnt, depth );
}

void CLevel::DrawBBox(Vector3f bb_min,Vector3f bb_max,DWORD color)	//�ٿ�� �ڽ��� �������� �׸��� �Լ�.
{
	int cnt=0;
	_D3DR3VERTEX_TEX0 vertex[1000*3];
	DWORD tc;


    D3DXMATRIX matWorld;
    D3DXMatrixIdentity( &matWorld );
    GetD3dDevice()->SetTransform( D3DTS_WORLD, &matWorld );
    GetD3dDevice()->SetTexture( 0,NULL);

#define LineSet(xx,yy,zz,cc) {	vertex[cnt].x = xx;	vertex[cnt].y = yy;	vertex[cnt].z = zz;		vertex[cnt].color = cc; cnt++;};


	tc=(color|0xff000000);
	LineSet(bb_min[0],bb_min[1],bb_min[2],tc);
	LineSet(bb_max[0],bb_min[1],bb_min[2],tc);
	LineSet(bb_min[0],bb_min[1],bb_min[2],tc);
	LineSet(bb_min[0],bb_min[1],bb_max[2],tc);
	LineSet(bb_max[0],bb_min[1],bb_min[2],tc);
	LineSet(bb_max[0],bb_min[1],bb_max[2],tc);
	LineSet(bb_min[0],bb_min[1],bb_max[2],tc);
	LineSet(bb_max[0],bb_min[1],bb_max[2],tc);

	LineSet(bb_min[0],bb_max[1],bb_min[2],tc);
	LineSet(bb_max[0],bb_max[1],bb_min[2],tc);
	LineSet(bb_min[0],bb_max[1],bb_min[2],tc);
	LineSet(bb_min[0],bb_max[1],bb_max[2],tc);
	LineSet(bb_max[0],bb_max[1],bb_min[2],tc);
	LineSet(bb_max[0],bb_max[1],bb_max[2],tc);
	LineSet(bb_min[0],bb_max[1],bb_max[2],tc);
	LineSet(bb_max[0],bb_max[1],bb_max[2],tc);

	LineSet(bb_min[0],bb_min[1],bb_min[2],tc);
	LineSet(bb_min[0],bb_max[1],bb_min[2],tc);
	LineSet(bb_max[0],bb_min[1],bb_min[2],tc);
	LineSet(bb_max[0],bb_max[1],bb_min[2],tc);
	LineSet(bb_min[0],bb_min[1],bb_max[2],tc);
	LineSet(bb_min[0],bb_max[1],bb_max[2],tc);
	LineSet(bb_max[0],bb_min[1],bb_max[2],tc);
	LineSet(bb_max[0],bb_max[1],bb_max[2],tc);
	DrawPrimitiveUPLine(cnt/2,vertex);
}
void CLevel::DrawBBox(Vector3s bb_min,Vector3s bb_max,DWORD color)	//�ٿ�� �ڽ��� �������� �׸��� �Լ�.
{
	Vector3f bb_min_f,bb_max_f;

	bb_min_f[0] = bb_min[0];
	bb_min_f[1] = bb_min[1];
	bb_min_f[2] = bb_min[2];

	DrawBBox(bb_min_f,bb_max_f,color);	//�ٿ�� �ڽ��� �������� �׸��� �Լ�.
}

/*
void CLevel::LoadMemoryBSP(char *name)		//bsp�� �״�� �޸𸮿� �д´�.
{
	FILE *fp;
	char filename[256]=".\\map\\";

	strcat(filename,mMapName);
	strcat(filename,".bsp");

	if( (fp=fopen(filename,"rb")) == NULL)
	{
		Warning(filename,"<- ������ ������.");
		return;
	}
	fread(&mBspMemoryHeader,sizeof(_BSP_FILE_HEADER),1,fp);

	if(mBspMemoryHeader.version != _R3BSP_VERSION)
	{
		Warning("BSP���� ������ �����ʽ���.","�����");
		fclose(fp);
		return;
	}

	int size=0;
	size+=mBspMemoryHeader.CPlanes.size;
	size+=mBspMemoryHeader.CFaceId.size;
	size+=mBspMemoryHeader.Node.size;
	size+=mBspMemoryHeader.Leaf.size;
	size+=mBspMemoryHeader.MatListInLeaf.size;
	size+=mBspMemoryHeader.Object.size;
	size+=mBspMemoryHeader.Track.size;
	size+=mBspMemoryHeader.CFVertex.size;
	size+=mBspMemoryHeader.CFLine.size;
	size+=mBspMemoryHeader.CFLineId.size;
	size+=mBspMemoryHeader.CFLeaf.size;
	size+=mBspMemoryHeader.BVertex.size;
	size+=mBspMemoryHeader.WVertex.size;
	size+=mBspMemoryHeader.FVertex.size;
	size+=mBspMemoryHeader.VertexColor.size;
	size+=mBspMemoryHeader.UV.size;
	size+=mBspMemoryHeader.LgtUV.size;
	size+=mBspMemoryHeader.Face.size;
	size+=mBspMemoryHeader.FaceId.size;
	size+=mBspMemoryHeader.VertexId.size;
	size+=mBspMemoryHeader.ReadMatGroup.size;

	mBspMemory=(BYTE *)Dmalloc(size);
	fread(mBspMemory,size,1,fp);
	fclose(fp);
}
*/
char *CLevel::GetMapName()		
{
	return mMapName;
}
/*
void CLevel::ReleaseMemoryBSP()		//bsp�� �״�� �޸𸮿� �д´�.
{
	if( mBspMemory ) 
		Dfree(mBspMemory);
	mBspMemory=NULL;
}
*/
void CLevel::GetFrustumNormalPlane(Vector4f n_plane[6])	//core�� view frustum ����� 
{
	GetFrustumNormalPlane(n_plane);	//core�� view frustum ����� 
}

void CLevel::HearMapSound()	//�� ���带 ����.(�������� �ƴϹǷ� endscene()���Ŀ� ����..)
{
	mBsp->HearMapSound();
}

#define _INTER_SPEED 1.0f
void CLevel::CalcR3Fog()	//���׸� ����Ѵ�.
{
	DWORD cnt;
	DWORD list[10000];
	mDummy.GetDummyList(_DUMMY_FLAG_FOG, &cnt, list);	
	if( !cnt )	//���װ���Ұ� ����.
		return;
	DWORD i,is_in=-1,now_fog;
	float now_start,now_end;
	_R3ENGINE_STATE *state=GetR3State();

	Vector3f pos;
	Vector3fCopy( state->mInvMatView.m[3],pos);
	now_fog = state->mMainFogColor;
	now_start = state->mMainFogStart;
	now_end = state->mMainFogEnd;

	for(i=0; i<cnt; i++)
	{
		if( mDummy.IsInBBox(list[i],pos) )
		{
			_EXT_DUMMY *dummy=mDummy.GetDummy(list[i]);
			now_fog = dummy->mdwArgv[0];
			now_start = dummy->mfArgv[1];
			now_end = dummy->mfArgv[2];
			break;
		}
	}
	
	if( state->mFogColor != now_fog )
	{
		float nr,ng,nb;
		float fr,fg,fb;
		DWORD r,g,b;

		fr = (float)((state->mFogColor>>16)&0xff);
		fg = (float)((state->mFogColor>>8)&0xff);
		fb = (float)((state->mFogColor>>0)&0xff);
		nr = (float)((now_fog>>16)&0xff);
		ng = (float)((now_fog>>8)&0xff);
		nb = (float)((now_fog>>0)&0xff);
		r = (DWORD)(( nr - fr )*_INTER_SPEED*R3GetLoopTime() + fr);
		g = (DWORD)(( ng - fg )*_INTER_SPEED*R3GetLoopTime() + fg);
		b = (DWORD)(( nb - fb )*_INTER_SPEED*R3GetLoopTime() + fb);

		state->mFogColor = 0xff000000|(r<<16)|(g<<8)|(b);
	}

	if( state->mFogStart != now_start )
	{
		state->mFogStart = (now_start - state->mFogStart)*_INTER_SPEED*R3GetLoopTime() + state->mFogStart;
		if( state->mFogStart - now_start < 2 )
			state->mFogStart = now_start;
	}

	if( state->mFogEnd != now_end )
	{
		state->mFogEnd = ( now_end - state->mFogEnd )*_INTER_SPEED*R3GetLoopTime() + state->mFogEnd;
		if( state->mFogEnd - now_end < 2 )
			state->mFogEnd = now_end;
		state->mFar = state->mFogEnd;
		D3DXMatrixPerspectiveFovLH( &state->mMatProj, state->mFov
			, state->mAsfectRatio, state->mNear, state->mFar );
		GetD3dDevice()->SetTransform( D3DTS_PROJECTION, &state->mMatProj );
	}
}




CTimer::CTimer()
{
	memset(this,0,sizeof(CTimer));
	mMinFPS=1.0f;
}

UINT64 CTimer::GetTicks()
{
	UINT64 qw;

	if(QueryPerformanceCounter((LARGE_INTEGER *) &qw))
	{
		while(!qw)
			QueryPerformanceCounter((LARGE_INTEGER *) &qw);
	} 
	else
	{
		qw = (UINT64) timeGetTime();
	}
	return qw;
}
float CTimer::GetDuration()
{
	INT64 qwDuration = (INT64)(GetTicks() - m_qwStartTicks);
	float fDuration = (float)qwDuration / (float)(INT64)m_qwTicksPerSec;
	return fDuration;
}

//#include "r3input.h"

void CTimer::CalculateTime()	//�������� ����ߵȴ�.
{
	float fFramesPerSec =1000;

//	if (gScan[VK_L])
  //      m_bTimerInitialized = FALSE;

    if( FALSE == m_bTimerInitialized )
    {
        m_bTimerInitialized = TRUE;
		if(!QueryPerformanceFrequency((LARGE_INTEGER *) &m_qwTicksPerSec))
			m_qwTicksPerSec = 1000;
		m_fTicksPerSec  = (float) (__int64) m_qwTicksPerSec;
		m_fSecsPerFrame = 1.0f / fFramesPerSec;
		m_qwTicksPerFrame = m_qwTicksPerSec / (__int64) fFramesPerSec;
		m_qwStartTicks = m_qwTicks = GetTicks();
 	}

	if( 0 )
	{
		UINT64 qw;
		qw = GetTicks();
		if(qw != m_qwTicks)
		{
			m_qwTicksPerFrame = qw - m_qwTicks;
			m_qwTicks = qw;
	//		m_fFramesPerSec = 0.75f * m_fFramesPerSec 
	//			+ 0.25f * (m_fTicksPerSec / (float) (__int64) m_qwTicksPerFrame);
			m_fFramesPerSec = 0.00f * m_fFramesPerSec 
				+ 1.00f * (m_fTicksPerSec / (float) (__int64) m_qwTicksPerFrame);
			if(m_fFramesPerSec < 1.0f)
				m_fFramesPerSec = 1.0f;
			m_fSecsPerFrame = 1.0f / m_fFramesPerSec;
			mLoopTime=m_fSecsPerFrame;
		}
	}
	else
	{
		DWORD gop=timeGetTime();
		DWORD sub=gop-mOldTime;
		
		mLoopCnt++;
		mLoopHop+=sub;
	//	if( mLoopHop > 200 )
		if( mLoopHop > 10 )
		{
			mLoopTime = (mLoopHop/(float)mLoopCnt)/1000.0f;
			mLoopCnt=0;
			mLoopHop=0;
		}
		mOldTime = gop;

		if( mLoopTime > mMinFPS || mLoopTime < 0 )
		{
			mLoopTime=mMinFPS;
		}

//		SetMultiLayerTime(mRealTime);
	}


	mRealTime+=mLoopTime;
	mFPSTime+=mLoopTime;
	mLoopFPSCnt++;
	if( mFPSTime > 1 )
	{
		mFPS = mLoopFPSCnt/mFPSTime;
		mLoopFPSCnt=0;
		mFPSTime=0;
	}
}
float CTimer::GetLoopTime()	//�ѷ����� ���� �ð��� ������.
{
	return mLoopTime;
}

float CTimer::GetTime()				//���� �ð��� �˾Ƴ���.
{
	return mRealTime;
}
void CTimer::SetTime(float time)	//Ÿ���� �� ����...
{
	mRealTime=time;
}
void CTimer::SetMinFPS(float min_fps)	//�ּ� ������ ����Ʈ�� �����Ѵ�.
{
	mMinFPS=1.0f/min_fps;
}


