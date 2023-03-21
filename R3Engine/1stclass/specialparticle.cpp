#include "R3Particle.h"
#include "core.h"
#include "R3math.h"
#include "R3util.h"
#include "R3D3dTex.h"
#include "R3render.h"
#include "CommonUtil.h"
#include "d3dutil.h"
#include "SpecialParticle.h"
#include "Procedural.h"
#include "RenderParticle.h"

static float GetDivU(int frame)
{
	float ret;
	if( 4 >= frame )
		ret=2;
	else
	if( 16 >= frame )
		ret=4;
	else
	if( 64 >= frame )
		ret=8;
	else
		ret=16;
	return ret;
}
static float GetDivV(int frame)
{
	float ret;
	if( 2 >= frame )
		ret=1;
	else
	if( 8 >= frame )
		ret=2;
	else
	if( 32 >= frame )
		ret=4;
	else
		ret=8;
	return ret;
}
static void GetTrackScaleAndARGB(_PARTICLE_ELEMENT *element,DWORD *dw_argb,float *scale)
{
	float f_a=max(element->mARGB[0],0);
	f_a=min(f_a,255);
	float f_r=max(element->mARGB[1],0);
	f_r=min(f_r,255);
	float f_g=max(element->mARGB[2],0);
	f_g=min(f_g,255);
	float f_b=max(element->mARGB[3],0);
	f_b=min(f_b,255);

	DWORD a = (DWORD)f_a;
	DWORD r = (DWORD)f_r;
	DWORD g = (DWORD)f_g;
	DWORD b = (DWORD)f_b;
	*scale = element->mScale;
	*dw_argb = (a<<24)|(r<<16)|(g<<8)|b;
}

LONG CParticle::SpecialLoop()	//��ƼŬ�� �ش� ����...
{
	LONG ret=0;
	Vector3f temp1,temp2,temp3;
	Vector3f dist;
	float diff;

	switch( mSpecialID )
	{
	case 2:
		ret = SpecialLoop2();
		break;
	case 1:
	case 3:	//���׷��̵�� ������ǥ 
		ret= -1;
		break;
	case 4:	//���׷��̵�� ������ǥ + y��ǥ ���� ����.
		Vector3fSub(mSpecialARGV[0],mSpecialARGV[1],dist);
		diff = Vector3fDist(dist);
		mStartPos[1][1] = mStartPos[0][1] + diff;
		ret= -1;
		break;
	case 5:
		Vector3fCopy(mGravity,temp1);
		Vector3fCopy(mStartPower[0],temp2);
		Vector3fCopy(mStartPower[1],temp3);
		Vector3fSub(mSpecialARGV[0],mSpecialARGV[1],dist);
		diff = Vector3fDist(dist)/10.0f;
		Vector3fMul(mGravity,diff,mGravity);
		Vector3fMul(mStartPower[0],diff,mStartPower[0]);
		Vector3fMul(mStartPower[1],diff,mStartPower[1]);
		ret= RealLoop();
		Vector3fCopy(temp1,mGravity);
		Vector3fCopy(temp2,mStartPower[0]);
		Vector3fCopy(temp3,mStartPower[1]);
		break;
	}
	return ret;

}
LONG CParticle::SpecialLoop2()	//��ƼŬ�� �ش� ����...
{	
	int i;
	int live_num;
	live_num=0;
	float loop_time = R3GetLoopTime();

	if( loop_time*mTimeSpeed > 1)		//��ƼŬ Ƣ�°� ����.
		loop_time=loop_time/mTimeSpeed;

	mParticleTimer += loop_time;
	if( mState != _PARTICLE_STATE_STOP )
		mNextCreatorTime -= loop_time;
	for(i=0 ;i<mNum; i++)
	{
		if( mElement[i].mIsLive )	//��� �ִ� �ѵ��� ������ش�.
		{			
			live_num++;
			mElement[i].mTime += loop_time;
			if( mElement[i].mTime > mLiveTime/mTimeSpeed )
			{
				live_num--;
				mElement[i].mIsLive =FALSE;
				while( mElement[i].mTime > mLiveTime/mTimeSpeed )
				{
					mElement[i].mTime -= mLiveTime/mTimeSpeed;
				}
			}
			else
			{
				GetPartcleStep(i,loop_time);//��ǥ�� ���ϰ�.
			}
		}
		if( mFlag & _PARTICLE_ATTR_END )
		{
			continue;
		}
		if( mFlag & _PARTICLE_ATTR_EMIT_TIME )
		{
			if( mParticleTimer > mEmitTime )
			{
				if( i==mNum-1 )
					 mFlag |= _PARTICLE_ATTR_END;
				continue;
			}
		}
		if( mNextCreatorTime + mOnePerTimeEpsilonTemp < 0 )	//element���� ũ������Ʈ �ϴ� �κ�..
		{
			if( mElement[i].mIsLive == FALSE )	//���� �����Ǿ� �ȴ�.
			{
				live_num++;
				if( mNextCreatorTime + loop_time < 0 ) //������ �Ѱ� ����ϴ� ��ƾ���� ���� �����ؾߵȴ�.
				{
					int temp_i = (int )(mNextCreatorTime*65536.0f);
					int temp_mod = (int)(loop_time*65536.0f);
					temp_mod = temp_i%temp_mod;
					mNextCreatorTime=(temp_mod/65536.0f);
				}
				InitElement(i,-(mNextCreatorTime+mOnePerTimeEpsilonTemp)); //������Ʈ �ʱ�ȭ
				float lastest_time = 100000;
				int temp_j,j;
				for( j=0; j<mNum; j++)	//���� �ֱٰŸ� ã�´�.
				{
					if( i == j )	//�ڱ��ڽ��� �����Ұ�.
						continue;
					if( !mElement[j].mIsLive )	//��� ���� �ʴ� �ѵ��� ����.
						continue;
					if( mElement[j].mTime < lastest_time )
					{
						lastest_time = mElement[j].mTime;
						temp_j = j;
					}
				}
				Vector3f t_pos;
				if( lastest_time != 100000 )	//�� ��������.
				{
					Vector3fSub(mElement[temp_j].mPos,mElement[i].mPos,t_pos);
					if( t_pos[0] !=0 || t_pos[1] !=0 || t_pos[2] !=0 )
					{
						float dist = Vector3fDist(t_pos);
						Normalize(t_pos);
						float new_time=-(mNextCreatorTime+mOnePerTimeEpsilonTemp)+(mOnePerTime*(1-mStartTimeRange));
						new_time = (-(mNextCreatorTime+mOnePerTimeEpsilonTemp)*dist)/new_time;
						Vector3fMul(t_pos,new_time,t_pos);
						Vector3fAdd(t_pos,mElement[i].mPos,mElement[i].mPos);
					}
				}
				mElement[i].mIsLive =TRUE;	//���� �����Ǿ� �ȴ�.
				mNextCreatorTime += mOnePerTime*(1-mStartTimeRange);
				mOnePerTimeEpsilonTemp = (Noise1(GetTickCount())-0.5f)*mOnePerTimeEpsilon*(1-mStartTimeRange);	//������ �Լ�.
				if( mFlag & _PARTICLE_ATTR_NO_LOOP )
				{
					if( i==mNum-1 )
						 mFlag |= _PARTICLE_ATTR_END;
				}
			}
		}
	}
	return live_num;
}
/*
LONG CParticle::SpecialLoop3()	//��ƼŬ�� �ش� ����...
{
	int i;
	int live_num;
	live_num=0;
	float loop_time = R3GetLoopTime();

	if( loop_time*mTimeSpeed > 1)		//��ƼŬ Ƣ�°� ����.
		loop_time=loop_time/mTimeSpeed;
		
	mParticleTimer += loop_time;
	if( mState != _PARTICLE_STATE_STOP )
		mNextCreatorTime -= loop_time;
	for(i=0 ;i<mNum; i++)
	{
		if( mElement[i].mIsLive )	//��� �ִ� �ѵ��� ������ش�.
		{			
			live_num++;
			mElement[i].mTime += loop_time;
			if( mElement[i].mTime > mLiveTime/mTimeSpeed )
			{
				live_num--;
				mElement[i].mIsLive =FALSE;
				while( mElement[i].mTime > mLiveTime/mTimeSpeed )
				{
					mElement[i].mTime -= mLiveTime/mTimeSpeed;
				}
			}
			else
			{
				GetPartcleStep(i,loop_time);//��ǥ�� ���ϰ�.
			}
//			GetPartcleStep(i,mElement[i].mTime);//��ǥ�� ���ϰ�.
		}
		if( mFlag & _PARTICLE_ATTR_END )
		{
			continue;
		}
		if( mFlag & _PARTICLE_ATTR_EMIT_TIME )
		{
			if( mParticleTimer > mEmitTime )
			{
				if( i==mNum-1 )
					 mFlag |= _PARTICLE_ATTR_END;
				continue;
			}
		}
		if( mNextCreatorTime + mOnePerTimeEpsilonTemp < 0 )	//element���� ũ������Ʈ �ϴ� �κ�..
		{
			if( mElement[i].mIsLive == FALSE )	//���� �����Ǿ� �ȴ�.
			{
				live_num++;
				if( mNextCreatorTime + loop_time < 0 ) //������ �Ѱ� ����ϴ� ��ƾ���� ���� �����ؾߵȴ�.
				{
					int temp_i = (int )(mNextCreatorTime*65536.0f);
					int temp_mod = (int)(loop_time*65536.0f);
					temp_mod = temp_i%temp_mod;
					mNextCreatorTime=(temp_mod/65536.0f);
				}
				InitElement(i,-(mNextCreatorTime+mOnePerTimeEpsilonTemp)); //������Ʈ �ʱ�ȭ
				mElement[i].mIsLive =TRUE;	//���� �����Ǿ� �ȴ�.
				mNextCreatorTime += mOnePerTime*(1-mStartTimeRange);
				mOnePerTimeEpsilonTemp = (Noise1(GetTickCount())-0.5f)*mOnePerTimeEpsilon*(1-mStartTimeRange);	//������ �Լ�.
				if( mFlag & _PARTICLE_ATTR_NO_LOOP )
				{
					if( i==mNum-1 )
						 mFlag |= _PARTICLE_ATTR_END;
				}
			}
		}
	}
	return live_num;
}

*/
//Ư��ó��.
//ID 1 : �ѹ��� �� ������ ó�� Z ������. mPowerTrack[0][0] = �ҽ�,mPowerTrack[0][1] = Ÿ��,scale = ������ �β� 
// Ư�� ��ɾ� tex_repeat �ؽ��� ���� ����Ʈ ��ġ  
void DrawSpectialParticle1(CParticle *particle,CEntity *entity,float world_mat[4][4],DWORD argb)
{
	_D3DR3VERTEX_TEX1 buf[2048];

//	if( particle->mSpecialID == 1 )
	if( !particle->mElement[0].mIsLive )
		return;

	LPDIRECT3DDEVICE8 pd3dDevice = GetD3dDevice();
	float live_time = particle->mLiveTime/particle->mTimeSpeed;
	int cnt=0;
	DWORD dw_argb;
	float scale;

	int mat_id=0;
	if( entity->mMatGroup[0].MtlId > 0)
		mat_id=entity->mMatGroup[0].MtlId;
	_R3MATERIAL* mat = &entity->mMat[mat_id];
	LPDIRECT3DTEXTURE8 pTex;
	if( entity->mFlag & _DO_NOT_MANAGE_TEXTURE )	//�³� Ȯ��..
	{
		LPDIRECT3DTEXTURE8 *ppTex=(LPDIRECT3DTEXTURE8 *)entity->mIndependencyTex;
		pTex = ppTex[mat->m_Layer[0].m_iSurface-entity->mStartTexID];
	}
	else
	{
		pTex = R3GetSurface(mat->m_Layer[0].m_iSurface);
	}
	pd3dDevice->SetTexture( 0, (LPDIRECT3DTEXTURE8)pTex );

	_D3DR3VERTEX_TEX1 *v_buf=(_D3DR3VERTEX_TEX1 *)buf;

	GetTrackScaleAndARGB(&particle->mElement[0],&dw_argb,&scale);
	if( (argb & 0x00ffffff) != 0x00ffffff )	//���� ���������̴�.
		dw_argb = (0xff000000&dw_argb)|(argb&0x00ffffff);
	if( (argb & 0xff000000) != 0xff000000 )	//���� ȥ�������̴�.
		dw_argb=(((dw_argb>>24)*(argb>>24)>>8)<<24)|(dw_argb&0x00ffffff);

	_ENTITY_M_GROUP *mat_group=entity->mMatGroup;
	float div_u=1;
	float div_v=1;
	float add_u=0;
	float add_v=0;

	//Ÿ�� �ؽ��ĸ� ���Ѱ�.
	if( mat_group->MtlId != -1 )
	{
		_R3MATERIAL *mat = &entity->mMat[mat_group->MtlId];
		if( _ANI_TEXTURE & mat->m_Layer[0].m_dwFlag )
		{
			int frame_num=(int)FixedShortToFloat(mat->m_Layer[0].m_sANITexFrame);
			float su=FixedShortToFloat(mat->m_Layer[0].m_sANITexSpeed);
			su*=particle->mElement[0].mTime;
			int frame = (int)(su)%frame_num;
			div_u=GetDivU(frame_num);
			div_v=GetDivV(frame_num);
			add_u=(frame%((int)div_u))/div_u;
			add_v=((frame+(int)div_u)/((int)div_u)-1)/div_v;
		}
	}
	Vector3f org_v[2],temp_v;
	Vector3fCopy(world_mat[3],org_v[0]);	//������.
	Vector3fCopy(particle->mPowerTrack[0][1],org_v[1]);	//����.
	DWORD v_cnt = CalcSnakeVertexList(buf,org_v,2,scale,dw_argb);

	scale = max(scale,0.01f);
	Vector3fSub(org_v[0],org_v[1],temp_v);
	float dist = Vector3fDot(temp_v,temp_v);
	if( dist == 0 )
		return;
	dist=sqrtf(dist);

	buf[0].u = 0;	buf[0].v = 0;
	buf[1].u = 1;	buf[1].v = 0;
	buf[2].u = 0;	buf[2].v = -particle->mTexRepeat*dist/(scale*2);
	buf[3].u = 1;	buf[3].v = -particle->mTexRepeat*dist/(scale*2);

	for(DWORD i =0; i<4 ;i++)
	{
		buf[i].u = 1/div_u*buf[i].u+add_u;
		buf[i].v = 1/div_v*buf[i].v+add_v;
	}

//	GetD3dDevice()->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
//	GetD3dDevice()->SetRenderState(D3DRS_ZWRITEENABLE ,FALSE);
	SetVPIPTex1IndexPrimitive(NULL,NULL);	//TL�鵵 ����� �ؾߵȴ�.
	if( v_cnt-2 > 0 )
	{
		GetD3dDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,v_cnt-2,buf,sizeof(_D3DR3VERTEX_TEX1));
	}
}

//ID 2 : �̵��ϴ� ��ƼŬ���� ��������°�...	�ݵ�� free�ɼ��̿��� ��.
extern void DrawParticlePolygon(CParticle *particle,CEntity *entity,float world_mat[4][4],DWORD argb=0xffffffff);	//�̰����� �ݵ�� ���� ��Ʈ���� ����ȭ�� �ؾߵȴ�.

static void GetChangeMatrix( float mat[4][4] )
{
	float imsi;
	for(int k=0;k<4;k++)
	{
		imsi=mat[k][1];
		mat[k][1]=mat[k][2];
		mat[k][2]=imsi;
	}
}
void GetMatrixFromAtoB2(float get_mat[4][4],Vector3f a,Vector3f b)	//a�� b�� ���� ������ ��Ʈ������ ���Ѵ�.
{
	float temp_mat[4][4];
	float yangle = GetYAngle( a,b );
	float xangle = GetXAngle( a,b );

	MatrixRotateY(get_mat,yangle);
	MatrixRotateX(temp_mat,xangle+90);
	MatrixMultiply(get_mat,get_mat,temp_mat);
}

void DrawSpectialParticle(CParticle *particle,CEntity *entity,float world_mat[4][4],DWORD argb)
{
	float mat[4][4];
	Vector3f normal,axis={0,1,0};

	switch(particle->mSpecialID)
	{
	case 1:
		DrawSpectialParticle1(particle,entity,world_mat,argb);
		break;
	case 3:	//���⼭ ȸ�� ��Ʈ������ ���� �־��ش�.
	case 4:	//���⼭ ȸ�� ��Ʈ������ ���� �־��ش�.
	case 5:	//���⼭ ȸ�� ��Ʈ������ ���� �־��ش�.
/*		Vector3fSub(particle->mSpecialARGV[1],particle->mSpecialARGV[0],normal);
		Normalize(normal);
		GetMatrixFromVector(particle->mRotMat,normal,axis);
*/
		GetMatrixFromAtoB2(particle->mRotMat,particle->mSpecialARGV[0],particle->mSpecialARGV[1]);
		MatrixCopy(mat,particle->mRotMat);
		Vector3fCopy(world_mat[3],mat[3]);
		DrawParticlePolygon(particle,entity,mat,argb);
		break;
	default:	//������ƾ�� ���� ���� ���.   ���� 2��
		DrawParticlePolygon(particle,entity,world_mat,argb);
		break;
	}
}


