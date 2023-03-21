#include "2dSprite.h"
#include "jerror.h"
#include "jmalloc.h"
#include "R3d3dtex.h"
#include "R3render.h"
#include "CommonUtil.h"
#include <stdio.h>

#define _VERSION 1.1f


CSprite::CSprite()
{
	memset( this, 0, sizeof( CSprite ) );
}

CSprite::~CSprite()
{
}

void CSprite::LoadSprite(char *name)
{
	strcpy(mFileName,name);

//	RestoreSpriteManager(this);		//�޴��� �Ƚ�..����..
	RestoreTexMemSprite();//	 ������ �ε����ڸ��� �޸𸮿� �ø���.
}

void CSprite::InitPageSprite(WORD xl,WORD yl)	//�ϳ��� �ؽ��ķ� �������� ��������Ʈ�� �����Ұ��.��ҿ�� ����� �����Ѵ�.
{
	mPageElementXL = xl;
	mPageElementYL = yl;
	mPageSPrite=TRUE;
}

void CSprite::SetPage(DWORD page)	//�ϳ��� �ؽ��ķ� �������� ��������Ʈ�� �����Ұ��.
{
	if( mFrames[0] <= page )
	{
		Warning("���� �ε��� �������������� ū page�� �����߽��ϴ�.","");
		return;
	}
	mPage = page;
}


extern void SetDDSTexSizeDebug(DWORD size);

void CSprite::RestoreTexMemSprite()
{
	DWORD get,i,j;
	float version;
	BYTE *buf;

	if( mIsLoadedTexMem )	//�ؽ��� �޸𸮿� ������
		return;
	FILE *fp=fopen(mFileName,"rb");

	if( fp == NULL )
	{
		Warning(mFileName,"<-�� ���������ϴ�. or ���� LoadSprite�� �ϼ���.");
		return;
	}
	fread(&version, 4, 1,fp);
	if( version != _VERSION )
		Error(mFileName,"<-������ ���� �����Դϴ�.");

	fread(&mActions, 4, 1,fp);
	if( _MAX_ACTIONS <= mActions)
		Error(mFileName,"<-���� ������ �ִ� �׼Ǽ��� �Ѿ����ϴ٤�,.��.");
	for(i=0; i<mActions; i++)
	{
		fread(&mFrames[i], 4, 1,fp);
		mFrameInfo[i]=(_FRAME_INFO *)Dmalloc(sizeof(_FRAME_INFO)*mFrames[i]);
		memset( mFrameInfo[i], 0, mFrames[i] * sizeof( _FRAME_INFO ) );

		for(j=0; j<mFrames[i]; j++)
		{
			fread(&mFrameInfo[i][j].Format, 4, 1,fp);
			fread(&get, 4, 1,fp);
			mFrameInfo[i][j].OrgXL=(WORD)get;
			fread(&get, 4, 1,fp);
			mFrameInfo[i][j].OrgYL=(WORD)get;
			fread(&get, 4, 1,fp);
			mFrameInfo[i][j].XL=(WORD)get;
			fread(&get, 4, 1,fp);
			mFrameInfo[i][j].YL=(WORD)get;
			fread(&mFrameInfo[i][j].Size, 4, 1,fp);
			
			buf=(BYTE *)Dmalloc(mFrameInfo[i][j].Size);
			fread(buf,mFrameInfo[i][j].Size,1,fp);

			SetDDSTexSizeDebug(mFrameInfo[i][j].Size);	//����׿�...
			mFrameInfo[i][j].Tex=GetDDSTexFromBuffer(mFrameInfo[i][j].XL,mFrameInfo[i][j].YL
				,mFrameInfo[i][j].Format,buf);
			Dfree(buf);
		}
	}

	fclose(fp);
	mIsLoadedTexMem = TRUE;
}
#define SAFE_RELEASE(p)      { if(p) { (p)->Release();  } }

void CSprite::ReleaseTexMemSprite()
{
	DWORD i,j;
	if( mIsLoadedTexMem ==FALSE )
		return;

	for(i=0; i<mActions; i++)
	{
		for(j=0; j<mFrames[i]; j++)
		{
			//((LPDIRECT3DTEXTURE8)mFrameInfo[i][j].Tex)->Release();
			SAFE_RELEASE((LPDIRECT3DTEXTURE8)mFrameInfo[i][j].Tex);
		}
		if( mFrameInfo[i] )
		{
			Dfree(mFrameInfo[i]);
			mFrameInfo[ i ] = NULL;
		}
	}
	mActions=0;
	mIsLoadedTexMem=FALSE;
}

char *CSprite::GetFileName()
{
	return mFileName;
}

void CSprite::ReleaseSprite()
{
//	DWORD i;

	ReleaseTexMemSprite();
/*
	for(i=0; i<mActions; i++)
	{
		if( mFrameInfo[i] )
			Dfree(mFrameInfo[i]);
	}
	*/
//	ReleaseSpriteManager(this);
	mActions=0;
}

void CSprite::SetAction(DWORD action)		//�׼��� �ٲ۴�.
{
	mAniFrame=0;
	mNowAction = action;

	if( mPageSPrite )	//������ ��������Ʈ�� ���� ýũ���ʿ����.
		return;
	if( !mActions )	//0�ϰ��.
		return;
	if( mActions <= mNowAction )
	{
		mNowAction=mActions-1;
	}
}
DWORD CSprite::GetNowActions()				//���� �������ִ� �׼��� �˾Ƴ���.
{
	return mNowAction;
}
DWORD CSprite::GetMaxActions()				//�� ��������Ʈ�� �� �׼��� �˾Ƴ���.
{
	return mActions;
}
void CSprite::SetFrame(DWORD frame)
{
	mNowFrame = frame;
	if( mPageSPrite )	//������ ��������Ʈ�� ���� ýũ���ʿ����.
		return;
	if( mFrames[mNowAction] <=mNowFrame )
		mNowFrame=mFrames[mNowAction]-1;
}
DWORD CSprite::GetNowFrames(DWORD action)	// ���� ���õ��ִ� �������� �˾Ƴ���.
{
	return mNowFrame; 
}
DWORD CSprite::GetMaxFrames(DWORD action)	//���� �׼��� �� �����Ӽ��� �˾Ƴ���.
{
	return mFrames[action];
}
void CSprite::SetAniAnimationSpeed(DWORD speed)		//�ʴ� �����Ӽ��� �־��ش�. 1�� 30frame �϶� 30
{
	mFPS = (float)speed;
}
void CSprite::RunAnimation()						//�ִϸ��̼��� �� ���.
{
	mAniFrame+=R3GetLoopTime()*mFPS;
	if( mAniFrame > 1000)
		mAniFrame = 0;
	while(mAniFrame >= mFrames[mNowAction])
	{
		mAniFrame-=(float)mFrames[mNowAction];
	}
	mNowFrame = (DWORD)mAniFrame;
	if(mNowFrame==1 )
		int uu=0;
}

void CSprite::DrawSprite(LONG x,LONG y,LONG ex,LONG ey,DWORD color)//�Ѹ���.
{
	LONG add_x,add_y;
	LONG xl,yl,org_xl,org_yl;

	if( mPageSPrite )
	{
		xl = mPageElementXL;
		yl = mPageElementYL;
		org_xl = 0;
		org_yl = 0;
	}
	else
	{
		xl = mFrameInfo[mNowAction][mNowFrame].XL;
		yl = mFrameInfo[mNowAction][mNowFrame].YL;
		org_xl = mFrameInfo[mNowAction][mNowFrame].OrgXL;
		org_yl = mFrameInfo[mNowAction][mNowFrame].OrgYL;
	}
	if( ex == _SELF_SIZE )
	{
		ex = xl+x;
		add_x=0;
	}
	else
	{
		float diff_x=(float)xl-org_xl;
		float diff_xx=xl-diff_x;
		if( diff_xx == 0)
			add_x=0;
		else
			add_x=(LONG)(((float)(ex-x)*diff_x )/diff_xx);
	}

	if( ey == _SELF_SIZE )
	{
		ey = yl+y;
		add_y=0;
	}
	else
	{
		float diff_y=(float)yl-org_yl;
		float diff_yy = yl-diff_y;
		if( diff_yy == 0)
			add_y=0;
		else
			add_y=(LONG)(((float)(ey-y)*diff_y )/diff_yy);
//		add_y=mFrameInfo[mNowAction][mNowFrame].YL-mFrameInfo[mNowAction][mNowFrame].OrgYL;
	}
	if( mPageSPrite )
	{
		float u[2],v[2];
		float u_frag,v_frag;
		u_frag = (float)mPageElementXL/(float)mFrameInfo[0][mPage].XL;
		v_frag = (float)mPageElementYL/(float)mFrameInfo[0][mPage].YL;
		u[0]=mNowAction*u_frag;

		u[1]=u[0]+u_frag;
		v[0]=mNowFrame*v_frag;
		v[1]=v[0]+v_frag;
		Draw2DSprite((float)x,(float)y,(float)ex-(float)x+add_x,(float)ey-(float)y+add_y
			,u ,v ,mFrameInfo[0][mPage].Tex,color);
	}
	else
	{
		Draw2DSprite((float)x,(float)y,(float)ex-(float)x+add_x,(float)ey-(float)y+add_y
			,mFrameInfo[mNowAction][mNowFrame].Tex,color);
	}
}

/*
void CSprite::DrawSpriteUV(LONG x,LONG y,float uv[2][2],LONG ex,LONG ey,DWORD color)//�Ѹ���.
{
	LONG add_x,add_y;
	LONG xl,yl,org_xl,org_yl;

	xl = mFrameInfo[mNowAction][mNowFrame].XL;
	yl = mFrameInfo[mNowAction][mNowFrame].YL;
	org_xl = mFrameInfo[mNowAction][mNowFrame].OrgXL;
	org_yl = mFrameInfo[mNowAction][mNowFrame].OrgYL;

	if( ex == _SELF_SIZE )
	{
		ex = xl+x;
		add_x=0;
	}
	else
	{
		float diff_x=(float)xl-org_xl;
		float diff_xx=xl-diff_x;
		if( diff_xx == 0)
			add_x=0;
		else
			add_x=(LONG)(((float)(ex-x)*diff_x )/diff_xx);
	}

	if( ey == _SELF_SIZE )
	{
		ey = yl+y;
		add_y=0;
	}
	else
	{
		float diff_y=(float)yl-org_yl;
		float diff_yy = yl-diff_y;
		if( diff_yy == 0)
			add_y=0;
		else
			add_y=(LONG)(((float)(ey-y)*diff_y )/diff_yy);
	}
	float u[2],v[2];
	u[0]=uv[0][0];
	u[1]=uv[0][1];
	v[0]=uv[1][0];
	v[1]=uv[1][1];

	Draw2DSprite((float)x,(float)y,(float)ex-(float)x+add_x,(float)ey-(float)y+add_y
		,u ,v ,mFrameInfo[mNowAction][mNowFrame].Tex,color);
}
*/

void CSprite::DrawSpriteUV(LONG x,LONG y,float uv[2][2],LONG ex,LONG ey,DWORD color)//�Ѹ���.
{	
	float u[2],v[2];
	u[0]=uv[0][0];
	u[1]=uv[0][1];
	v[0]=uv[1][0];
	v[1]=uv[1][1];

	if( mPageSPrite )
		Draw2DSprite((float)x,(float)y,(float)ex-(float)x,(float)ey-(float)y
			,u ,v , mFrameInfo[0][mPage].Tex,color);
	else
		Draw2DSprite((float)x,(float)y,(float)ex-(float)x,(float)ey-(float)y
			,u ,v ,mFrameInfo[mNowAction][mNowFrame].Tex,color);
}

void CSprite::DrawSpriteRot(LONG x,LONG y,float angle,LONG ex,LONG ey,DWORD color)//�Ѹ���.
{
	LONG add_x,add_y;
	LONG xl,yl,org_xl,org_yl;

	if( mPageSPrite )
	{
		xl = mPageElementXL;
		yl = mPageElementYL;
		org_xl = 0;
		org_yl = 0;
	}
	else
	{
		xl = mFrameInfo[mNowAction][mNowFrame].XL;
		yl = mFrameInfo[mNowAction][mNowFrame].YL;
		org_xl = mFrameInfo[mNowAction][mNowFrame].OrgXL;
		org_yl = mFrameInfo[mNowAction][mNowFrame].OrgYL;
	}
	if( ex == _SELF_SIZE )
	{
		ex = xl+x;
		add_x=0;
	}
	else
	{
		float diff_x=(float)xl-org_xl;
		float diff_xx=xl-diff_x;
		if( diff_xx == 0)
			add_x=0;
		else
			add_x=(LONG)(((float)(ex-x)*diff_x )/diff_xx);
	}

	if( ey == _SELF_SIZE )
	{
		ey = yl+y;
		add_y=0;
	}
	else
	{
		float diff_y=(float)yl-org_yl;
		float diff_yy = yl-diff_y;
		if( diff_yy == 0)
			add_y=0;
		else
			add_y=(LONG)(((float)(ey-y)*diff_y )/diff_yy);
//		add_y=mFrameInfo[mNowAction][mNowFrame].YL-mFrameInfo[mNowAction][mNowFrame].OrgYL;
	}
	if( mPageSPrite )
	{
		float u[2],v[2];
		float u_frag,v_frag;
		u_frag = (float)mPageElementXL/(float)mFrameInfo[0][mPage].XL;
		v_frag = (float)mPageElementYL/(float)mFrameInfo[0][mPage].YL;
		u[0]=mNowAction*u_frag;

		u[1]=u[0]+u_frag;
		v[0]=mNowFrame*v_frag;
		v[1]=v[0]+v_frag;
		Draw2DSpriteRot((float)x,(float)y,(float)ex-(float)x+add_x,(float)ey-(float)y+add_y
			,angle,u ,v ,mFrameInfo[0][mPage].Tex,color);
	}
	else
	{
		Draw2DSpriteRot((float)x,(float)y,(float)ex-(float)x+add_x,(float)ey-(float)y+add_y
			,angle,mFrameInfo[mNowAction][mNowFrame].Tex,color);
	}
}

void CSprite::DrawSpriteRotUV( LONG x, LONG y, float angle, float uv[2][2], LONG ex, LONG ey, DWORD color )
{
	float u[2],v[2];	
	u[0] = uv[0][0];
	u[1] = uv[0][1];
	v[0] = uv[1][0];
	v[1] = uv[1][1];

	Draw2DSpriteRot( (float)x,(float)y, (float)ex-(float)x,(float)ey-(float)y, 
				angle, u, v, mFrameInfo[mNowAction][mNowFrame].Tex,color);
}

void CSprite::DrawSpriteVertex( _D3DR3TLVERTEX_TEX1 * pVertex, int nPrimitiveNum )
{	
	Draw2DSprite( pVertex, nPrimitiveNum, mFrameInfo[mNowAction][mNowFrame].Tex );
}

WORD CSprite::GetSpriteOrgXL(DWORD action,DWORD frame) //���ڰ� ������ ���� �׼ǰ� ���� �������� ����� ���ϵȴ�.
{
	if( mPageSPrite )	//������ ��������Ʈ�� ���� ýũ���ʿ����.
	{
		return mFrameInfo[0][mPage].OrgXL;
	}
	if( action == _NOW_STATE)
	{
		action = mNowAction;
	}
	if( frame == _NOW_STATE)
	{
		frame = mNowFrame;
	}
	return mFrameInfo[action][frame].OrgXL;
}
WORD CSprite::GetSpriteOrgYL(DWORD action,DWORD frame) //���ڰ� ������ ���� �׼ǰ� ���� �������� ����� ���ϵȴ�.
{
	if( mPageSPrite )	//������ ��������Ʈ�� ���� ýũ���ʿ����.
	{
		return mFrameInfo[0][mPage].OrgYL;
	}
	if( action == _NOW_STATE)
	{
		action = mNowAction;
	}
	if( frame == _NOW_STATE)
	{
		frame = mNowFrame;
	}
	return mFrameInfo[action][frame].OrgYL;
}

WORD CSprite::GetSpriteXL( DWORD action, DWORD frame )
{
	if( mPageSPrite )	//������ ��������Ʈ�� ���� ýũ���ʿ����.
	{
		return mFrameInfo[0][mPage].XL;
	}
	if( action == _NOW_STATE)
	{
		action = mNowAction;
	}
	if( frame == _NOW_STATE)
	{
		frame = mNowFrame;
	}
	return mFrameInfo[action][frame].XL;

}

WORD CSprite::GetSpriteYL( DWORD action, DWORD frame )
{
	if( mPageSPrite )	//������ ��������Ʈ�� ���� ýũ���ʿ����.
	{
		return mFrameInfo[0][mPage].YL;
	}
	if( action == _NOW_STATE)
	{
		action = mNowAction;
	}
	if( frame == _NOW_STATE)
	{
		frame = mNowFrame;
	}
	return mFrameInfo[action][frame].YL;
}

void Draw2DRectangle(LONG x,LONG y,LONG ex,LONG ey,DWORD color)//�Ѹ���.
{
	Draw2DSprite((float)x,(float)y,(float)ex-(float)x,(float)ey-(float)y,(void*)NULL,color);
}

void Draw2DTexture(LONG x,LONG y,LONG ex,LONG ey,DWORD color,void *tex)//�Ѹ���.
{
	Draw2DSprite((float)x,(float)y,(float)ex-(float)x,(float)ey-(float)y,(void*)tex,color);
}


void Draw2DLineList(Vector2f *line,DWORD line_num,DWORD color)//���ε� �Ѳ����� �Ѹ���.
{
	R3Draw2DLineList(line,line_num,color);//���ε� �Ѳ����� �Ѹ���.
}

void Draw2DLine(Vector2f start,Vector2f end,DWORD color)//���� �ϳ� �Ѹ���.
{
	R3Draw2DLine(start,end,color);
}

void *CSprite::GetD3D8TexPtr(DWORD action,DWORD frame)//���ڰ� ������ ���� �׼ǰ� ���� �������� ����� ���ϵȴ�.
{
	if( action == _NOW_STATE)
	{
		action = mNowAction;
	}
	if( frame == _NOW_STATE)
	{
		frame = mNowFrame;
	}
	if( mActions <= action )
		return NULL;
	if( mFrames[action] <= frame )
		return NULL;

	return mFrameInfo[action][frame].Tex;
}
