#include "R3Bsp.h"
#include "R3math.h"
#include "R3Particle.h"
#include "JError.h"
#include "Jmalloc.h"
#include "Procedural.h"
#include "CommonUtil.h"
#include "Core.h"
#include <math.h>
#include <stdio.h>



#define _TIME_EPSILON 0.3

static _PRE_PARTICLE_LIST stPreParticleList[]={
	"����� ����",0,0,0,0		,1 ,2,2, 0,0,0, 1

	,255,203,152,101,50,0	,255,255,255,255,255,255	,255,255,255,255,255,255	,255,255,255,255,255,255	,1,1,1,1,1,1

	,"�ܼ� �߶�"	 ,0,0,0,0		,4 ,2,2, 0,0,0, 1
	,255,203,152,101,50,0	,255,255,255,255,255,255	,255,255,255,255,255,255	,255,255,255,255,255,255	,1,1,1,1,1,1
 
	,"����"		 ,0,0,0,0		,4 ,-2,-2, 0,0,0,1		
	,255,203,152,101,50,0	,255,255,255,255,255,255	,255,255,255,255,255,255	,255,255,255,255,255,255	,1,1,1,1,1,1

	,"����"		 ,0,0,0,0		,1 ,2,2, 0,0,0,	1
	,255,203,152,101,50,0	,255,255,255,255,255,255	,255,255,255,255,255,255	,255,255,255,255,255,255	,1,1,1,1,1,1

	,"���� ��"	 ,0,0,0,0		,4 ,2,-0.7f, 0,-5,-10, 2
	,255,203,152,101,50,0	,255,255,255,255,255,255	,255,255,255,255,255,255	,255,255,255,255,255,255	,0.5f,1,1,1,1,2

	,"������� ����"	,0,100,0,100		,4 ,-2,-2, 0,0,0,1		
	,255,203,152,101,50,0	,255,255,255,255,255,255	,255,255,255,255,255,255	,255,255,255,255,255,255	,0.2f,1,1,1,1,4
};

int GetPreParticleListNum()//�̸� ��ġ�� ����ִ� ��ƼŬ ����Ʈ ���� 
{
	return sizeof(stPreParticleList)/sizeof(_PRE_PARTICLE_LIST);
}

_PRE_PARTICLE_LIST *GetPreParticleList(int id)//�ش� ���̵��� ������ ����
{
	return &stPreParticleList[id];
}


#define _AFC			0.0f // �������װ��

#define _GTIME ((80*250.0f)/4900.0)		// �ð�*�ð� = �� 1/70 ��*1/70

float GetGravity( float num )     // 0�� �� ����	-�� �ö󰣴� +�� ��������.
{
	if(num<0)
	    return((float)(_GTIME)*num*num);    // ���⼭ 250 �� �������� �ӵ�
	else
		return((float)-(_GTIME)*num*num);    // ���⼭ 250 �� �������� �ӵ�
}

float PowInterpolate(float a,float b, float alpha)
{
	float start = 15;
	float max_i=(float)pow(start,2);

	float p_pow= (float)pow(start, 2-alpha);
	alpha = (((p_pow-start)/(max_i-start)));

	return  a*(1-alpha) + b*alpha;
}


void InitR3Particle(void)
{
	float start = 15;
	float max_i=(float)pow(start,2);
	float div_f = (1)/256.0f;

	for(int i=0; i<256; i++)
	{
		float p_pow= (float)pow(start, 2-(float)i*div_f);
		BYTE gop=255-(BYTE)(((p_pow-start)/(max_i-start))*255);  
	}
}
void ReleaseR3Particle(void)
{
}
//�����϶� 2���� Ÿ���� �ִ�. 
//�ڱⰡ �������� ���ϴ� ��Ÿ��
//������ ���������� ��ƼŬ�� ���ڸ��� ���Ƽ� ó���Ǵ� ��Ÿ��.	<- ���� ������ƼŬpos�� �̵��Ǵ°���.

CParticle::CParticle()
{
	memset(this,0,sizeof(CParticle));
	mFlag=0;
	mEntity=NULL;
/*
	memset(mATrack,0xff,_PATICLE_MAX_TRACK);
	memset(mRTrack,0xff,_PATICLE_MAX_TRACK*2);
	memset(mGTrack,0xff,_PATICLE_MAX_TRACK*2);
	memset(mBTrack,0xff,_PATICLE_MAX_TRACK*2);
*/
/*
	mATrack[0]=255;
	mATrack[5]=0;
	mRTrack[0]=255;
	mRTrack[5]=255;
	mGTrack[0]=255;
	mGTrack[5]=255;
	mBTrack[0]=255;
	mBTrack[5]=255;
	mScaleTrack[0][0]=1.0f;
	mScaleTrack[0][1]=1.0f;
	mScaleTrack[5][0]=1.0f;
	mScaleTrack[5][1]=1.0f;
*/
	mGravity[0]=0;
	mGravity[1]=-15;
	mGravity[2]=0;
	mStartPower[0][0]=0;
	mStartPower[0][1]=0;
	mStartPower[0][2]=0;
	mStartPower[1][0]=0;
	mStartPower[1][1]=0;
	mStartPower[1][2]=0;
	mCreatePos[0]=0;
	mCreatePos[1]=0;
	mCreatePos[2]=0;
}

CParticle::~CParticle()
{
	if( mElement )
		Dfree(mElement);
}

void CParticle::ReleaseEntity()	//Ư�� ���� : Ŭ�� ����  ���������� �ʴ´�.
{
	if( !mEntity )
		return;
	mEntity->ReleaseEntity();
	delete mEntity;
}

static float GetRandNum(float a,float b)
{
	LONG d_a= (LONG)(a*32767);
	LONG d_b= (LONG)(b*32767);
	if( d_b-d_a == 0 )
		return a;
	DWORD count=GetTickCount();
	LONG gop = (rand()*count)%(d_b-d_a)+d_a;
	return gop/32767.0f;
}

//save��ƼŬ���� �̵�.
void CParticle::CopyParticleToSaveParticle(_SAVE_PARTICLE *s_particle)
{
	memset(s_particle,0,sizeof(_SAVE_PARTICLE));
	strcpy(s_particle->mEntityName,mEntityName);
	s_particle->mNum=mNum;
	s_particle->mAlphaType=mAlphaType;
	s_particle->mFlag = mFlag;	
	s_particle->mLiveTime = mLiveTime;	
	Vector3fCopy(mGravity,s_particle->mGravity);	
	s_particle->mTimeSpeed = mTimeSpeed;	
//	Vector3fCopy(mPowerNormal,s_particle->mPowerNormal);	
	//������ Ʈ�����θ� ī��.
	memcpy(s_particle->mATrack,mATrack,_PATICLE_MAX_TRACK*4*sizeof(BYTE)+sizeof(float)*_PATICLE_MAX_TRACK);
}

void CParticle::CopySaveParticleToParticle(_SAVE_PARTICLE *s_particle)
{
	memset(this,0,sizeof(CParticle));
	strcpy(mEntityName,s_particle->mEntityName);
//	Vector3lCopy(s_particle->mCreateArea,mCreateArea);
	mFlag = s_particle->mFlag;
	mNum = s_particle->mNum;
	mAlphaType=s_particle->mAlphaType;
	mLiveTime = s_particle->mLiveTime;	
	mTimeSpeed = s_particle->mTimeSpeed;	
//	Vector3fCopy(s_particle->mPowerNormal,mPowerNormal);
	//������ Ʈ�����θ� ī��.
	memcpy(mATrack,s_particle->mATrack,_PATICLE_MAX_TRACK*4*sizeof(BYTE)+sizeof(float)*_PATICLE_MAX_TRACK);
}

void CParticle::SetPreCalcParticle(DWORD type)	//��ƼŬ ���� �ʱ�ȭ ������ �ݵ�� ���º������� �����ؾߵȴ�.
{
	mLiveTime=stPreParticleList[type].mLiveTime;
//	Vector3fCopy(stPreParticleList[type].mPowerNormal,mPowerNormal);
	mTimeSpeed=stPreParticleList[type].mTimeSpeed;
	mFlag=stPreParticleList[type].mFlag;
//	Vector3fCopy(stPreParticleList[type].mCreateArea,mCreateArea);	//	�� ������ ���..[0]�� �迭�� ��������...�ִ�. �ڽ��� x,y,z 
	//������ Ʈ�����θ� ī��.
	memcpy(mATrack,stPreParticleList[type].mATrack,_PATICLE_MAX_TRACK*4*sizeof(BYTE)+sizeof(float)*_PATICLE_MAX_TRACK);
//	mAlphaGradient=stPreParticleList[type].mAlphaGradient;
//	mOnePerTimeEpsilon=stPreParticleList[type].mOnePerTimeEpsilon;
//	mStartPos[0][0] = 0;	mStartPos[0][1] = 0;	mStartPos[0][2] = 0;
}

void CParticle::GetBBox(Vector3f get_min,Vector3f get_max)			//�ش���ƼŬ �׷��� �ٿ�� �ڽ��� ����Ѵ�.
{//���������� �������� �ƿ� ���Ѵ�.

	get_min[0] = 65000;
	get_min[1] = 65000;
	get_min[2] = 65000;
	get_max[0] = -65000;
	get_max[1] = -65000;
	get_max[2] = -65000;

	float i;

	_PARTICLE_ELEMENT temp;
	memcpy(&temp,mElement,sizeof(_PARTICLE_ELEMENT));	//�ϴ� ���.

	_ENTITY_M_GROUP *mat_group=&mEntity->mMatGroup[0];
	float bb_max;

	Vector3f temp_v[3];
	Vector3fCopy(mStartPos[0],temp_v[0]);
	Vector3fCopy(mStartPos[1],temp_v[1]);
	Vector3fCopy(mCreatePos,temp_v[2]);
	mCreatePos[0]=0;
	mCreatePos[1]=0;
	mCreatePos[2]=0;
	mStartPos[0][0]=0;
	mStartPos[0][1]=0;
	mStartPos[0][2]=0;
	mStartPos[1][0]=0;
	mStartPos[1][1]=0;
	mStartPos[1][2]=0;
	bb_max = -65000;
	for(int k=0; k<(int)mat_group->VCnt ;k++)
	{
		_D3DR3VERTEX_TEX1 *tex1=&mEntity->mStaticVertexBuffer.m_VertexBufferTex1[k+mat_group->VBMinIndex];
		if( fabs(tex1->x) > bb_max )
			bb_max=(float)fabs(tex1->x);
		if( fabs(tex1->y) > bb_max )
			bb_max=(float)fabs(tex1->y);
		if( fabs(tex1->z) > bb_max )
			bb_max=(float)fabs(tex1->z);
	}

	InitElement(0,0); //������Ʈ �ʱ�ȭ
	for(i=0; i<mLiveTime/mTimeSpeed; i+=0.1f)	//0.1������ �� �������.
	{
		mElement[0].mTime = i;
		GetPartcleStep(0,0.1f);//��ǥ�� ���ϰ�.
		if( mElement->mPos[0] < get_min[0] )
			get_min[0]=mElement->mPos[0];
		if( mElement->mPos[1] < get_min[1] )
			get_min[1]=mElement->mPos[1];
		if( mElement->mPos[2] < get_min[2] )
			get_min[2]=mElement->mPos[2];

		if( mElement->mPos[0] > get_max[0] )
			get_max[0]=mElement->mPos[0];
		if( mElement->mPos[1] > get_max[1] )
			get_max[1]=mElement->mPos[1];
		if( mElement->mPos[2] > get_max[2] )
			get_max[2]=mElement->mPos[2];
	}
	get_min[0]-=bb_max;
	get_min[1]-=bb_max;
	get_min[2]-=bb_max;
	get_max[0]+=bb_max;
	get_max[1]+=bb_max;
	get_max[2]+=bb_max;

	Vector3fCopy(temp_v[0],mStartPos[0]);
	Vector3fCopy(temp_v[1],mStartPos[1]);
	Vector3fCopy(temp_v[2],mCreatePos);

	get_min[0]+=mStartPos[0][0];
	get_min[1]+=mStartPos[0][1];
	get_min[2]+=mStartPos[0][2];
	get_max[0]+=mStartPos[1][0];
	get_max[1]+=mStartPos[1][1];
	get_max[2]+=mStartPos[1][2];

	memcpy(mElement,&temp,sizeof(_PARTICLE_ELEMENT));	//����.
}

//��ƼŬ �ʱ�ȭ �Լ����� ������....
void CParticle::InitParticle()	//��ƼŬ ���� �ʱ�ȭ ������ �ݵ�� ���º������� �����ؾߵȴ�.
{
	if(  mNum < 1 )
	{
		Warning("��ƼŬ ������Ʈ�� 1���� �Ǿ�...","");
		return;
	}
	if(  mLiveTime == 0 )
	{
		Warning("��ƼŬ ���º������� �̸� �����ϰų� ���Լ� ���� SetParticleAttr()�� ���� ȣ���ϼ���","");
		return;
	}
	mElement = (_PARTICLE_ELEMENT *)Dmalloc(sizeof(_PARTICLE_ELEMENT)*mNum );
	for(int i=0; i<mNum; i++)
		InitElement(i,0);
	mOnePerTime = (mLiveTime/mTimeSpeed)/mNum;
//	mNextCreatorTime=0;
}
void CParticle::ResetOnePerTime()
{
	mOnePerTime = (mLiveTime/mTimeSpeed)/mNum;
}

void CParticle::ReInitParticle(int num)	//��ƼŬ ������ ���Ҵ� ��Ų��.  �ŷ������ٴ� ����...(malloc�� �ִ�.)
{
	int i;

	if( mNum == num )
		return;		//������ ���ʿ� ���� ����.
	mElement = (_PARTICLE_ELEMENT *)ReAlloc(mElement,sizeof(_PARTICLE_ELEMENT)*mNum
		,sizeof(_PARTICLE_ELEMENT)*num );
	if( num > mNum )
	{
		for(i=mNum; i<num; i++)
			InitElement(i,0);
	}
	mNum = num;
	mOnePerTime = (mLiveTime/mTimeSpeed)/mNum;
}
void CParticle::ReleaseParticle()	//��ƼŬ ������ 
{
	if( mElement )
		Dfree(mElement);
	mElement=NULL;
}

/*
void CParticle::GetPartcleAbsoluteTime(int id,float time)	//��ƼŬ�� ����Ÿ�ӿ� ������ǥ ���Ѵ�.
{
	
	float AirFriction=0.1f;
	float fr[3]; //���� ���׹� �߷� ���װ��� ���� ��갪. 

	time*=mTimeSpeed;
	time=min(mLiveTime,time);
	//������ �߷� Ʈ���� ��.	�̰��� �� �����ؾ� Ƣ���� ź��ü�� �����Ѵ�.
	float g_tweening=(mEndGravity-mStartGravity)*(time/mLiveTime)+mStartGravity;

	mElement[id].mGravityCnt=time*g_tweening;

	float attn=(1-time/mLiveTime-.0f);	//�߷� ���跮.
	attn = max(attn,0);

//	AirFriction=(1-mElement[id].mTime/mLiveTime);	//�������װ�.
	AirFriction=(time);	//�������װ�.

	fr[0] = mPowerNormal[0]*AirFriction;	//���� ���� ���� ����?
	fr[1] = mPowerNormal[1]*attn;			//y��ǥ�� �߷� ����.
	fr[2] = mPowerNormal[2]*AirFriction;
	
	float length=GetGravity(mElement[id].mGravityCnt);//g_tweening);	//���ӵ��� ���Ѵ�. �߷»�Ȳ�� ���� 1.0f�� �ٲٰ�.
	//��ƼŬ�� ���� ���⿡ �����Ѵ�.
	mElement[id].mPos[0]=fr[0]+mStartPos[0][0];
	mElement[id].mPos[1]=length+fr[1]+mStartPos[0][1]-mPowerNormal[1];	
	mElement[id].mPos[2]=fr[2]+mStartPos[0][2];

}
*/


//���� ����� �߷��� ���� �������...
//��� = - ��� * �浹��� (��� = ���� * �ӵ�)

void _PARTICLE_ELEMENT::UpDate(float time)//,float mTimeSpeed)
{
	mPos[0]+=mDir[0]*time;
	mPos[1]+=mDir[1]*time;
	mPos[2]+=mDir[2]*time;
	mDir[0]+=mDirStep[0]*time;		//�� ��ƼŬ ����.
	mDir[1]+=mDirStep[1]*time;		//�� ��ƼŬ ����.
	mDir[2]+=mDirStep[2]*time;		//�� ��ƼŬ ����.
	mARGB[0]+=mARGBStep[0]*time;
	mARGB[1]+=mARGBStep[1]*time;
	mARGB[2]+=mARGBStep[2]*time;
	mARGB[3]+=mARGBStep[3]*time;
	mScale+=mScaleStep*time;
	mNowFrame+=time;
	mZRot += mZRotStep*time;
	mYRot += mYRotStep*time;
}

void CParticle::CheckCollision(int id,float time)	//��ƼŬ�� ���� ��ǥ�� �ش��ϴ� ����..
{
	if( (mFlag & _PARTICLE_ATTR_COLLISION ) && mBsp )	//������ ��ƼŬ �浹ýũ...
	{
		Vector3f next_pos,c_pos,now_pos;
		Vector4f c_normal;

		CBsp *bsp = (CBsp *)mBsp;
		//mElement[id].UpDate(time);
		//���� ��ǥ�� �̸� ���غ���.
		next_pos[0] = mElement[id].mPos[0]+mElement[id].mDir[0]*time;
		next_pos[1] = mElement[id].mPos[1]+mElement[id].mDir[1]*time;
		next_pos[2] = mElement[id].mPos[2]+mElement[id].mDir[2]*time;

		if( mFlag & _PARTICLE_ATTR_FREE )
		{
			Vector3fCopy(mElement[id].mPos,now_pos);
		}
		else
		{
			Vector3fAdd(mCreatePos,next_pos,next_pos);
			Vector3fAdd(mCreatePos,mElement[id].mPos,now_pos);
		}
		
		if( bsp->IsCollisionFace(now_pos,next_pos,&c_pos,&c_normal) )
		{
			//�浹�� �����...
			Vector3f r,t;
			CrossVector(c_normal,mElement[id].mDir,r);
			if( r[0] == 0 || r[1] == 0 || r[2] == 0 )
			{
				t[0] = 1;
				t[1] = 0;
				t[2] = 0;
			}
			else
			{
				CrossVector(c_normal,r,t);
				Normalize(r);
				Normalize(t);
			}
			D3DXMATRIX matMirror;
			D3DXMatrixIdentity( &matMirror );
			matMirror._11 -= 2*t[0]*t[0]; matMirror._12 -= 2*t[0]*t[1]; matMirror._13 -= 2*t[0]*t[2];
			matMirror._21 -= 2*t[1]*t[0]; matMirror._22 -= 2*t[1]*t[1]; matMirror._23 -= 2*t[1]*t[2];
			matMirror._31 -= 2*t[2]*t[0]; matMirror._32 -= 2*t[2]*t[1]; matMirror._33 -= 2*t[2]*t[2];
			matMirror._41 -= 2*t[3]*t[0];   matMirror._42 -= 2*t[3]*t[1];   matMirror._43 -= 2*t[3]*t[2];
//			Vector3fSub(mElement[id].mDir,c_pos,r);
			Vector3fCopy(mElement[id].mDir,r);
			Vector3fTransform(t,r,matMirror.m);

			mElement[id].mDir[0] = -t[0]*mElasticity;
			mElement[id].mDir[1] = -t[1]*mElasticity;
			mElement[id].mDir[2] = -t[2]*mElasticity;
			mElement[id].mDirStep[0] = 0;
			mElement[id].mDirStep[1] = 0;
			mElement[id].mDirStep[2] = 0;//15;

			if( !(mFlag & _PARTICLE_ATTR_FREE) )
			{
				Vector3fSub(c_pos,mCreatePos,c_pos);
//				Vector3fAdd(c_pos,mElement[id].mPos,mElement[id].mPos);
				Vector3fCopy(c_pos,mElement[id].mPos);
			}
			else
			{
				Vector3fCopy(c_pos,mElement[id].mPos);
			}
		}
/*
		//----��鿡 �ݻ簢 ���ϱ�...  a�� �ε����� �������� 
		Vector3f a,b,r,t;
		a[0]=0;		a[1]=1;		a[2]=0;
		b[0]=1;		b[1]=1;		b[2]=0;
		CrossVector(a,b,r);
		CrossVector(b,r,t);
		*/
	}

	mElement[id].UpDate(time);

	mElement[id].mDir[0]+=mGravity[0]*time;		// ��ƼŬ �߷� ����.
	mElement[id].mDir[1]+=mGravity[1]*time;	
	mElement[id].mDir[2]+=mGravity[2]*time;	
/*
	Vector3f temp;	//�߷��� �ٲ�� �ȵɵ�.
	if( mFlag & _PARTICLE_ATTR_FREE )
		Vector3fTransform( temp,mGravity,mRotMat);
	else
		Vector3fCopy( mGravity,temp);
	mElement[id].mDir[0]+=temp[0]*time;		// ��ƼŬ �߷� ����.
	mElement[id].mDir[1]+=temp[1]*time;	
	mElement[id].mDir[2]+=temp[2]*time;	
*/
}
void CParticle::GetPartcleStep(int id,float time)	//��ƼŬ�� ���� ��ǥ�� �ش��ϴ� ����..
{
	time*=mTimeSpeed;
	time=min(mLiveTime,time);

	CheckCollision(id,time);
//	if( mElement[id].mScale > 0.2)
//		int dfdf=0;

	if( mTrackCnt > mElement[id].mNowTrack )
	{
		BOOL is_calc=FALSE;
/*
		if( mElement[id].mTime > mLiveTime/mTimeSpeed )
		{
			mElement[id].mTime=0;
			mElement[id].mIsLive=FALSE;
			return;
		}
*/
		//���� ��ƼŬ �ð��� Ʈ���ð����� ũ�� 
		while( mTimeTrack[mElement[id].mNowTrack] <=  mElement[id].mTime*mTimeSpeed )
		{
			int next_track = mElement[id].mNowTrack+1;
			if( mTrackCnt-1 < next_track )
			{
				next_track=mTrackCnt-1;
				if( is_calc==FALSE )
				{
					mElement[id].mDirStep[0]=0;
					mElement[id].mDirStep[1]=0;
					mElement[id].mDirStep[2]=0;
					mElement[id].mARGBStep[0] = 0;
					mElement[id].mARGBStep[1] = 0;
					mElement[id].mARGBStep[2] = 0;
					mElement[id].mARGBStep[3] = 0;
					mElement[id].mScaleStep=0;
				}
				break;
			}
			float div=mTimeTrack[next_track]-mElement[id].mTime*mTimeSpeed;
			if( div < 0 ) //_TIME_EPSILON )
				goto _NEXT_TRACK;
			is_calc=TRUE;

			if(fabs(div) < _TIME_EPSILON)
				div=1;
			if( mTrackFlag[mElement[id].mNowTrack] & _TRACK_FLAG_POWER )
			{/*
				mElement[id].mDirStep[0] = GetRandNum(mPowerTrack[mElement[id].mNowTrack][0][0]
					,mPowerTrack[mElement[id].mNowTrack][1][0]);
				mElement[id].mDirStep[1] = GetRandNum(mPowerTrack[mElement[id].mNowTrack][0][1]
					,mPowerTrack[mElement[id].mNowTrack][1][1]);
				mElement[id].mDirStep[2] = GetRandNum(mPowerTrack[mElement[id].mNowTrack][0][2]
					,mPowerTrack[mElement[id].mNowTrack][1][2]);*/
				Vector3f temp;
				temp[0] = GetRandNum(mPowerTrack[mElement[id].mNowTrack][0][0]
					,mPowerTrack[mElement[id].mNowTrack][1][0]);
				temp[1] = GetRandNum(mPowerTrack[mElement[id].mNowTrack][0][1]
					,mPowerTrack[mElement[id].mNowTrack][1][1]);
				temp[2] = GetRandNum(mPowerTrack[mElement[id].mNowTrack][0][2]
					,mPowerTrack[mElement[id].mNowTrack][1][2]);
				if( mFlag & _PARTICLE_ATTR_FREE )
					Vector3fTransform( mElement[id].mDirStep,temp,mRotMat);
				else
					Vector3fCopy( temp,mElement[id].mDirStep);

				mElement[id].mDirStep[0]= mElement[id].mDirStep[0] - mElement[id].mDir[0]/div;
				mElement[id].mDirStep[1]= mElement[id].mDirStep[1] - mElement[id].mDir[1]/div;
				mElement[id].mDirStep[2]= mElement[id].mDirStep[2] - mElement[id].mDir[2]/div;
//				mElement[id].mDir[0]=0;
//				mElement[id].mDir[1]=0;
//				mElement[id].mDir[2]=0;
			}
			else
			{//�׷��� ������� ���� �Ŀ��� ����Ѵ�.
//				mElement[id].mDirStep[0]=0;	
//				mElement[id].mDirStep[1]=0;
//				mElement[id].mDirStep[2]=0;
			}

			if( mTrackFlag[mElement[id].mNowTrack] & _TRACK_FLAG_FLICKER )
			{				
				if( mElement[id].mFlag & _ELEMENT_ATTR_FLICKER )
					mElement[id].mFlag = ~_ELEMENT_ATTR_FLICKER & mElement[id].mFlag;
				else
					mElement[id].mFlag |= _ELEMENT_ATTR_FLICKER;
			}

			if( mTrackFlag[next_track] & _TRACK_FLAG_ALPHA )
				mElement[id].mARGBStep[0] = ((float)mATrack[next_track]-mElement[id].mARGB[0])/div;
			else
				mElement[id].mARGBStep[0] = 0;
			if( mTrackFlag[next_track] & _TRACK_FLAG_COLOR )
			{
				mElement[id].mARGBStep[1] = ((float)GetRandNum(mRTrack[next_track][0],mRTrack[next_track][1])-mElement[id].mARGB[1])/div;
				mElement[id].mARGBStep[2] = ((float)GetRandNum(mGTrack[next_track][0],mGTrack[next_track][1])-mElement[id].mARGB[2])/div;
				mElement[id].mARGBStep[3] = ((float)GetRandNum(mBTrack[next_track][0],mBTrack[next_track][1])-mElement[id].mARGB[3])/div;
			}
			else
			{
				mElement[id].mARGBStep[1] = 0;
				mElement[id].mARGBStep[2] = 0;
				mElement[id].mARGBStep[3] = 0;
			}
			if( mTrackFlag[next_track] & _TRACK_FLAG_SCALE )
			{
				float temp = GetRandNum(mScaleTrack[next_track][0],mScaleTrack[next_track][1]);
				mElement[id].mScaleStep = (temp-mElement[id].mScale)/div;
			}
			else
				mElement[id].mScaleStep = 0;
			if( mTrackFlag[next_track] & _TRACK_FLAG_ZROT )
			{
				float temp = GetRandNum(mZRotTrack[next_track][0],mZRotTrack[next_track][1]);
				mElement[id].mZRotStep = (temp)/div;
			}
			else
				mElement[id].mZRotStep = 0;
			if( mTrackFlag[next_track] & _TRACK_FLAG_YROT )
			{
				float temp = GetRandNum(mYRotTrack[next_track][0],mYRotTrack[next_track][1]);
				mElement[id].mYRotStep = (temp)/div;
			}
			else
				mElement[id].mYRotStep = 0;

//			CheckCollision(id,time);
//			mElement[id].UpDate(time);
_NEXT_TRACK:
			mElement[id].mNowTrack++;
			if( mTrackCnt <= mElement[id].mNowTrack )
				break;
		}
		if( is_calc )
			return;
	}
}

DWORD CParticle::GetParticleState()			// ����,��,���� ���¸� �˾ƺ�
{
	return mState;
}
void CParticle::SetParticleState(DWORD state)	//��ƼŬ ���۳��� ����.
{
	int i,temp_i;
	float oldest_element;

	mState = state;
	switch(state)
	{
	case _PARTICLE_STATE_START:	//������ �Ѱ� ����ϴ� ��ƾ
		for( i=0; i<mNum; i++)
		{
			InitElement(i,0);
			mElement[i].mIsLive =FALSE;
		}
		break;
	case _PARTICLE_STATE_START_ONE_ELEMENT:	//������ �Ѱ� ����ϴ� ��ƾ
		oldest_element = -1;
		temp_i=-1;	//Ȥ�� ����..
		for(i=0 ;i<mNum; i++)
		{
			if( !mElement[i].mIsLive )	//�׾� �ִ� �ѵ��� �ϳ��� Ű���.
			{
				//mElement[i].mTime=0;
				//mElement[i].mIsLive=TRUE;
				InitElement(i,0); //������Ʈ �ʱ�ȭ
				break;
			}
			else	//������ ��� ��� �ִ� �ֵ����� ��������� �Ÿ� ��´�.
			{
				if( oldest_element < mElement[i].mTime )
				{
					oldest_element = mElement[i].mTime;
					temp_i=i;
				}
			}
		}
		if( temp_i >= 0 )
		{
			InitElement(temp_i,0); //������Ʈ �ʱ�ȭ
		}
		break;
	case _PARTICLE_STATE_ALWAYS_LIVE:
		for( i=0; i<mNum; i++)
		{			
			mOnePerTimeEpsilonTemp = (Noise1(GetTickCount())-0.5f)*mOnePerTimeEpsilon;	//������ �Լ�.
			InitElement(i,i*(mOnePerTime+mOnePerTimeEpsilonTemp));
		}
		break;
	default:
		break;
	}
}

void CParticle::InitElement(int id,float time) //������Ʈ �ʱ�ȭ
{
	Vector3f temp;
	//�ѹ� ����� ����� �Ŀ����� ���� �ʱ���ġ�� �����Ҽ��� �ִ�.
	mElement[id].mTime = 0;
	mElement[id].mFlag = 0;

	if( mFlag & _PARTICLE_ATTR_FLICKER )	//���ۺ��� �ø�Ŀ��.
		mElement[id].mFlag |= _ELEMENT_ATTR_FLICKER;
//	mElement[id].mDir[0] = GetRandNum(mStartPower[0][0],mStartPower[1][0]);
//	mElement[id].mDir[1] = GetRandNum(mStartPower[0][1],mStartPower[1][1]);
//	mElement[id].mDir[2] = GetRandNum(mStartPower[0][2],mStartPower[1][2]);

	temp[0] = GetRandNum(mStartPower[0][0],mStartPower[1][0]);
	temp[1] = GetRandNum(mStartPower[0][1],mStartPower[1][1]);
	temp[2] = GetRandNum(mStartPower[0][2],mStartPower[1][2]);

	if( mFlag & _PARTICLE_ATTR_FREE )
		Vector3fTransform( mElement[id].mDir,temp,mRotMat);
	else
		Vector3fCopy( temp,mElement[id].mDir);


	
	mElement[id].mDirStep[0] = 0;
	mElement[id].mDirStep[1] = 0;
	mElement[id].mDirStep[2] = 0;//15;

	Vector3f start_pos;

	start_pos[0] = GetRandNum(mStartPos[0][0],mStartPos[1][0]);
	start_pos[1] = GetRandNum(mStartPos[0][1],mStartPos[1][1]);
	start_pos[2] = GetRandNum(mStartPos[0][2],mStartPos[1][2]);

	mElement[id].mZRot=GetRandNum(mStartZRot[0],mStartZRot[1]);
	mElement[id].mYRot=GetRandNum(mStartYRot[0],mStartYRot[1]);

	if( _PARTICLE_ATTR_FREE & mFlag )	//������ƼŬ.
	{
		mElement[id].mPos[0]=start_pos[0] + mCreatePos[0];	//ù������ ����.
		mElement[id].mPos[1]=start_pos[1] + mCreatePos[1];
		mElement[id].mPos[2]=start_pos[2] + mCreatePos[2];
	}
	else
	{
		mElement[id].mPos[0]=start_pos[0];// + mCreatePos[0];	//ù������ ����.
		mElement[id].mPos[1]=start_pos[1];// + mCreatePos[1];
		mElement[id].mPos[2]=start_pos[2];// + mCreatePos[2];
	}

	mElement[id].mNowFrame=0;

	mElement[id].mNowTrack = 0;
	mElement[id].mIsLive = FALSE;	//�츮�°Ŵ� ���� �ؾ�..
//	mElement[id].mIsLive = TRUE;	//�츮��..
	mElement[id].mTime = time;

	if( mElement[id].mTime > mLiveTime/mTimeSpeed )
		mElement[id].mTime=0;

	GetPartcleStep(id,mElement[id].mTime);	//¥���� �ð� �������...

	if( mTrackCnt )
	{
		DWORD next_track = 0;
		float div=mTimeTrack[next_track]-mElement[id].mTime*mTimeSpeed;
		if(fabs(div) < _TIME_EPSILON)
			div=1;

		if( mTrackFlag[next_track] & _TRACK_FLAG_ALPHA )
			mElement[id].mARGBStep[0] = ((float)mATrack[next_track]-GetRandNum(mStartARGB[0][0],mStartARGB[0][1]))/div;
		else
			mElement[id].mARGBStep[0] = 0;

		if( mTrackFlag[next_track] & _TRACK_FLAG_COLOR )
		{
			mElement[id].mARGBStep[1] = ((float)GetRandNum(mRTrack[next_track][0],mRTrack[next_track][1])
				-GetRandNum(mStartARGB[1][0],mStartARGB[1][1]))/div;
			mElement[id].mARGBStep[2] = ((float)GetRandNum(mGTrack[next_track][0],mGTrack[next_track][1])
				-GetRandNum(mStartARGB[2][0],mStartARGB[2][1]))/div;
			mElement[id].mARGBStep[3] = ((float)GetRandNum(mBTrack[next_track][0],mBTrack[next_track][1])
				-GetRandNum(mStartARGB[3][0],mStartARGB[3][1]))/div;
		}
		else
		{
			mElement[id].mARGBStep[1] = 0;
			mElement[id].mARGBStep[2] = 0;
			mElement[id].mARGBStep[3] = 0;
		}
		if( mTrackFlag[next_track] & _TRACK_FLAG_SCALE )
		{
			float temp = GetRandNum(mScaleTrack[next_track][0],mScaleTrack[next_track][1]);
			float temp_s = GetRandNum(mStartScale[0],mStartScale[1]);
			mElement[id].mScaleStep = (temp-temp_s)/div;
		}
		else
			mElement[id].mScaleStep = 0;

		if( mTrackFlag[next_track] & _TRACK_FLAG_ZROT )
		{
			float temp = GetRandNum(mZRotTrack[next_track][0],mZRotTrack[next_track][1]);
			mElement[id].mZRotStep = (temp)/div;
		}
		else
			mElement[id].mZRotStep = 0;

		if( mTrackFlag[next_track] & _TRACK_FLAG_YROT )
		{
			float temp = GetRandNum(mYRotTrack[next_track][0],mYRotTrack[next_track][1]);
			mElement[id].mYRotStep = (temp)/div;
		}
		else
			mElement[id].mYRotStep = 0;
	}
	else
	{
		mElement[id].mARGBStep[0] = 0;
		mElement[id].mARGBStep[1] = 0;
		mElement[id].mARGBStep[2] = 0;
		mElement[id].mARGBStep[3] = 0;
		mElement[id].mScaleStep = 0;
	}
	mElement[id].mARGB[0]=GetRandNum(mStartARGB[0][0],mStartARGB[0][1]);
	mElement[id].mARGB[1]=GetRandNum(mStartARGB[1][0],mStartARGB[1][1]);
	mElement[id].mARGB[2]=GetRandNum(mStartARGB[2][0],mStartARGB[2][1]);
	mElement[id].mARGB[3]=GetRandNum(mStartARGB[3][0],mStartARGB[3][1]);
	mElement[id].mScale = GetRandNum(mStartScale[0],mStartScale[1]);
	mElement[id].mZRot = GetRandNum(mStartZRot[0],mStartZRot[1]);
	mElement[id].mYRot = GetRandNum(mStartYRot[0],mStartYRot[1]);

//	GetPartcleStep(id,0);//

}

#include "r3input.h"

/*
	if( gScan[VK_1] )
	{
		SetParticleState(_PARTICLE_STATE_START);
	}
	if( gScan[VK_2] )
	{
//		SetParticleState(_PARTICLE_STATE_RESET);
		SetParticleState(_PARTICLE_STATE_STOP);
	}
	if( gAsci[VK_3] )
	{
		gAsci[VK_3]=0;
		SetParticleState(_PARTICLE_STATE_START_ONE_ELEMENT);
	}
*/
LONG CParticle::RealLoop()	//��ƼŬ�� �ش� ����...
{
	int i;
	int live_num=0;
	float loop_time = R3GetLoopTime();

	if( loop_time*mTimeSpeed > 1)		//��ƼŬ Ƣ�°� ����.
		loop_time=loop_time/mTimeSpeed;
		
	mParticleTimer += loop_time;
	if( mState != _PARTICLE_STATE_STOP )
		mNextCreatorTime -= loop_time;
	{
		for(i=0 ;i<mNum; i++)
		{
			if( mElement[i].mIsLive )	//��� �ִ� �ѵ��� ������ش�.
			{			
				/*	//float�� ��ⷯ�� ������ �ϱ����ؼ�...
				if( mElement[i].mTime > mLiveTime*2000 )	//�ʹ� Ŀ���� ������ ������ִ�.
				{
					mElement[i].mTime -=mLiveTime*2000;
				}
				DWORD dwTime = (DWORD)(mElement[i].mTime*512);
				DWORD dwLiveTime = (DWORD)(mLiveTime*512);
				float f_rtime = (dwTime%dwLiveTime)/512.0f;
				GetPartcleStep(i,f_rtime);//��ǥ�� ���ϰ�.
				*/
				live_num++;
				mElement[i].mTime += loop_time;
				if( mElement[i].mTime > mLiveTime/mTimeSpeed )
				{
					live_num--;
					mElement[i].mIsLive =FALSE;
					if( mState == _PARTICLE_STATE_STOP )
					{
					//	mElement[i].mTime=0;
					//	memset(&mElement[i],0,sizeof(_PARTICLE_ELEMENT));
					}
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
//			if( mFlag & _PARTICLE_ATTR_NO_LOOP )
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
	}
	return live_num;
}

LONG CParticle::Loop()	//��ƼŬ�� �ش� ����...
{
	int live_num;
	if( mFlag & _PARTICLE_ATTR_SPECIAL_ID )
	{
		live_num = SpecialLoop();		//Ư���Ѱ͵� ���� ��Ų��.
		if( live_num != -1 )			//Ư�� ������ ���°͸� ���Ͻ�Ű�� �Ⱦ��°��� �׳� loop�Լ��� ����Ѵ�.
			return live_num;
	}
	return RealLoop();	//��ƼŬ�� �ش� ����...
}

DWORD GetTokenFloat(char *ch,float *get_f)	//
{
	int i;
	int cnt = strlen(ch);
	char buf[256];

	int start_cnt=0;
	for(i=0; i<cnt; i++)
	{
		if( ch[i] == '(' )
		{
			start_cnt=0;
			continue;
		}
		if( ch[i] == ',' ||  ch[i] == ')')
		{
			buf[start_cnt] = NULL;
			*get_f=(float)atof(buf);
			return i+1;
		}
		buf[start_cnt] = ch[i];
		start_cnt++;
	}
	*get_f=0;
	return cnt;
}

BOOL GetRandOrNum( FILE *fp,float *from,float *at )	//TRUE�� ����.
{
	char hole[256];
	char temp[256];

	fscanf(fp,"%s",hole);
	if( hole[0] == 'r' && hole[1] == 'a' && hole[2] == 'n'&& hole[3] == 'd' ) //���������ϰ��.
	{
		int i,j;
		j=strlen(hole);
		BOOL is_end=FALSE;
		while(1)	//�պκ� ��ŵ.
		{
			for(i=0; i<j ;i++)
			{
				if( hole[i]== ')' )
				{
					is_end=TRUE;
				}
			}
			if( is_end )
				break;
			if( fscanf(fp,"%s",temp)==EOF)	
				Error("��ũ��Ʈ read ����.",")������.");
			strcat(hole,temp);
			j=strlen(hole);
		}
		DWORD cnt = GetTokenFloat(hole,from);	// ��������.
		cnt = GetTokenFloat(&hole[cnt],at);	//���� ��.
		return TRUE;
	}

	*from = (float)atof(hole);
	*at = *from;
	return FALSE;
}

BOOL CParticle::LoadParticleSPT(char *name,DWORD option)
{
	FILE *fp;
	float temp_f;
//	int i;

	if(( fp =fopen(name,"rt"))==NULL) 
	{
		Warning(name,"<-�� �����̾����ϴ�.");
		return FALSE;
	}

	char hole[256];

	BOOL is_par=FALSE;
	mFlag=0;

    while(1)	//�պκ� ��ŵ.
    {
		if( fscanf(fp,"%s",hole)==EOF)	
			break;
        if (!strcmp(hole,"[Particle]"))
		{
			is_par=TRUE;
			break;
		}
	}
	if( !is_par )
	{
		Warning(name,"<-�������� ��ƼŬ��ũ��Ʈ�� �ƴմϴ�.");
		fclose(fp);
		return FALSE;
	}
	int is_track_ok=-1,track_id;
	memset(this,0,sizeof(CParticle));
	mStartScale[0]=1;	//����Ʈ �� ����.
	mStartScale[1]=1;	//����Ʈ �� ����.
	mStartARGB[0][0]=255;
	mStartARGB[1][0]=255;
	mStartARGB[2][0]=255;
	mStartARGB[3][0]=255;
	mStartARGB[0][1]=255;
	mStartARGB[1][1]=255;
	mStartARGB[2][1]=255;
	mStartARGB[3][1]=255;
	mTimeSpeed=1;
	mAlphaType=3;
	float old_time=0;
	track_id=0;
    while(1)
    {
		if( fscanf(fp,"%s",hole)==EOF)	
			break;
		strlwr(hole);
		if( !strcmp(hole,"entity_file"))
		{
			fscanf(fp,"%s",hole);
			strcpy(mEntityName,hole);
			//���⿡�� ��ƼƼ�� �ε� 
			mEntity = new CEntity;
			if( !mEntity->LoadEntity(mEntityName,option|_LOAD_POLY_IN_MEMORY) )
			{
				fclose(fp);
				delete mEntity;
				mEntity=NULL;
				return FALSE;
			}
			mEntity->RestoreTexMem();
		}
		if( !strcmp(hole,"num"))
		{
			fscanf(fp,"%s",hole);
			mNum=atoi(hole);	//��ƼŬ �� ����.
		}
		if( !strcmp(hole,"start_time_range"))
		{
			fscanf(fp,"%s",hole);
			mStartTimeRange = (float)atof(hole);
		}
		if( !strcmp(hole,"always_live"))
		{
//			SetParticleState(_PARTICLE_STATE_ALWAYS_LIVE);
//			mNum=atoi(hole);	//��ƼŬ �� ����.
		}
		if( !strcmp(hole,"pos"))
		{
			fscanf(fp,"%s",hole);
			if( !strcmp(hole,"box"))
			{
			}
			else
			if( !strcmp(hole,"sphere"))
			{
				mFlag |= _PARTICLE_ATTR_SPHERE;
			}
			else
			if( !strcmp(hole,"sphere_edge"))
			{
				mFlag |= _PARTICLE_ATTR_SPHERE_EDGE;
			}
			else
			{
				Warning(hole,"������ġ ���� ��ɾ �ƴմϴ�.");
				continue;
			}
			if( !GetRandOrNum(fp,&mStartPos[0][0],&mStartPos[1][0]) )	//������ �ƴϸ�.
				mStartPos[1][0] = mStartPos[0][0];
			if( !GetRandOrNum(fp,&mStartPos[0][1],&mStartPos[1][1]) )	//������ �ƴϸ�.
				mStartPos[1][1] = mStartPos[0][1];
			if( !GetRandOrNum(fp,&mStartPos[0][2],&mStartPos[1][2]) )	//������ �ƴϸ�.
				mStartPos[1][2] = mStartPos[0][2];
		}
		if( !strcmp(hole,"no_billboard"))
			mFlag |= _PARTICLE_ATTR_NO_BILLBOARD;
		if( !strcmp(hole,"z_disable"))
			mFlag |= _PARTICLE_ATTR_ZDISABLE;
		if( !strcmp(hole,"live_time"))
		{
			GetRandOrNum(fp,&mLiveTime,&temp_f);	// ���� �������� ������..
		}
		if( !strcmp(hole,"alpha_type"))
		{
			fscanf(fp,"%s",hole);
			mAlphaType = atoi(hole);
		}
		if( !strcmp(hole,"always_live"))
			mFlag |= _PARTICLE_ATTR_ALWAYS_LIVE;
		if( !strcmp(hole,"time_speed"))
		{
			GetRandOrNum(fp,&mTimeSpeed,&temp_f);	// ���� �������� ������..
		}
		if( !strcmp(hole,"gravity"))
		{
			GetRandOrNum(fp,&mGravity[0],&temp_f);	// ���� �������� ������..
			GetRandOrNum(fp,&mGravity[1],&temp_f);	// ���� �������� ������..
			GetRandOrNum(fp,&mGravity[2],&temp_f);	// ���� �������� ������..
		}
		if( !strcmp(hole,"start_power"))
		{
			if( !GetRandOrNum(fp,&mStartPower[0][0],&mStartPower[1][0]) )	//������ �ƴϸ�.
				mStartPower[1][0] = mStartPower[0][0];
			if( !GetRandOrNum(fp,&mStartPower[0][1],&mStartPower[1][1]) )	//������ �ƴϸ�.
				mStartPower[1][1] = mStartPower[0][1];
			if( !GetRandOrNum(fp,&mStartPower[0][2],&mStartPower[1][2]) )	//������ �ƴϸ�.
				mStartPower[1][2] = mStartPower[0][2];
		}
		if( !strcmp(hole,"start_scale"))
		{
			if( !GetRandOrNum(fp,&mStartScale[0],&mStartScale[1]) )	//������ �ƴϸ�.
				mStartScale[1] = mStartScale[0];
		}
		if( !strcmp(hole,"start_zrot"))
		{
			mFlag |= _PARTICLE_ATTR_Z_ROT;
			if( !GetRandOrNum(fp,&mStartZRot[0],&mStartZRot[1]) )	//������ �ƴϸ�.
				mStartZRot[1] = mStartZRot[0];
		}
		if( !strcmp(hole,"start_yrot"))
		{
			mFlag |= _PARTICLE_ATTR_Y_ROT;
			if( !GetRandOrNum(fp,&mStartYRot[0],&mStartYRot[1]) )	//������ �ƴϸ�.
				mStartYRot[1] = mStartYRot[0];
		}

		if( !strcmp(hole,"y_billboard"))
			mFlag |= _PARTICLE_ATTR_Y_BILLBOARD;
		if( !strcmp(hole,"z_billboard"))
			mFlag |= _PARTICLE_ATTR_Z_BILLBOARD;
		if( !strcmp(hole,"free"))
			mFlag |= _PARTICLE_ATTR_FREE;
		if( !strcmp(hole,"check_collision"))
			mFlag |= _PARTICLE_ATTR_COLLISION;

		if( !strcmp(hole,"z_front"))
		{
			mFlag |= _PARTICLE_ATTR_ZFRONT;
			GetRandOrNum(fp,&mZFront,&temp_f);	// ���� �������� ������..
		}
		if( !strcmp(hole,"emit_time"))
		{
			mFlag |= _PARTICLE_ATTR_EMIT_TIME;
			GetRandOrNum(fp,&mEmitTime,&temp_f);	// ���� �������� ������..
		}

		if( !strcmp(hole,"start_alpha"))
			GetRandOrNum(fp,&mStartARGB[0][0],&mStartARGB[0][1]);
		if( !strcmp(hole,"start_color"))
		{
			GetRandOrNum(fp,&mStartARGB[1][0],&mStartARGB[1][1]);	// ���� �������� ������..
			GetRandOrNum(fp,&mStartARGB[2][0],&mStartARGB[2][1]);	// ���� �������� ������..
			GetRandOrNum(fp,&mStartARGB[3][0],&mStartARGB[3][1]);	// ���� �������� ������..
		}
		if( !strcmp(hole,"flicker_alpha"))
		{		
			fscanf(fp,"%s",hole);
			mFlickerAlpha = (BYTE)atoi(hole);
		}
		if( !strcmp(hole,"flicker_time"))
		{		
			fscanf(fp,"%s",hole);
			mFlickerTime = (float)atof(hole);
			if( mTimeSpeed != 0 )
				mFlickerTime /= mTimeSpeed; 
		}
		if( !strcmp(hole,"flicker"))
		{		
			mFlag |= _PARTICLE_ATTR_FLICKER;
		}
		if( !strcmp(hole,"create_time_epsilon"))
		{
			GetRandOrNum(fp,&mOnePerTimeEpsilon,&temp_f);	// ���� �������� ������..
		}
		if( !strcmp(hole,"elasticity"))
		{
			GetRandOrNum(fp,&mElasticity,&temp_f);	// ���� �������� ������..
		}

		if( !strcmp(hole,"special_id"))
		{
			fscanf(fp,"%s",hole);
			mSpecialID = atoi(hole);
			mFlag |= _PARTICLE_ATTR_SPECIAL_ID;
		}
		if( !strcmp(hole,"tex_repeat"))
		{
			fscanf(fp,"%s",hole);
			mTexRepeat = (float)atof(hole);
		}
		
		if( !strcmp(hole,"time"))
		{
			is_track_ok=0;
			fscanf(fp,"%s",hole);
			mTimeTrack[mTrackCnt] = (float)atof(hole);
			/*
			if( mTimeTrack[mTrackCnt]-old_time <= _TIME_EPSILON )
			{
				Warning("�����ð����� 0.3���̻� ���̰� ���ߵ˴ϴ�.","<-��ũ��Ʈ read ����");
				//continue;
			}*/
			old_time = mTimeTrack[mTrackCnt];
			track_id = mTrackCnt;
			mTrackCnt++;
			if( mTrackCnt >= _PATICLE_MAX_TRACK )
				Error("�ִ� Ʈ�������� �Ѿ����ϴ�.","");
		}

		if( !strcmp(hole,"power"))
		{
			if( is_track_ok == -1 )
			{
				Warning(hole,"�� ��ɾ�� time�� ���� �����ѵ� ���ּ���.");
				continue;
			}
			if( !GetRandOrNum(fp,&mPowerTrack[track_id][0][0],&mPowerTrack[track_id][1][0]) )	//������ �ƴϸ�.
				mPowerTrack[track_id][1][0] = mPowerTrack[track_id][0][0];
			if( !GetRandOrNum(fp,&mPowerTrack[track_id][0][1],&mPowerTrack[track_id][1][1]) )	//������ �ƴϸ�.
				mPowerTrack[track_id][1][1] = mPowerTrack[track_id][0][1];
			if( !GetRandOrNum(fp,&mPowerTrack[track_id][0][2],&mPowerTrack[track_id][1][2]) )	//������ �ƴϸ�.
				mPowerTrack[track_id][1][2] = mPowerTrack[track_id][0][2];
			mTrackFlag[track_id]|=_TRACK_FLAG_POWER;
		}
		if( !strcmp(hole,"alpha"))
		{
			if( is_track_ok == -1 )
			{
				Warning(hole,"�� ��ɾ�� time�� ���� �����ѵ� ���ּ���.");
				continue;
			}
			GetRandOrNum(fp,&temp_f,&temp_f);	// ���� �������� ������..
			mATrack[track_id]=(BYTE)temp_f;
			mTrackFlag[track_id]|=_TRACK_FLAG_ALPHA;
		}
		if( !strcmp(hole,"zrot"))
		{
			mTrackFlag[track_id]|=_TRACK_FLAG_ZROT;
			mFlag |= _PARTICLE_ATTR_Z_ROT;
			if( is_track_ok == -1 )
			{
				Warning(hole,"�� ��ɾ�� time�� ���� �����ѵ� ���ּ���.");
				continue;
			}
			if( !GetRandOrNum(fp,&mZRotTrack[track_id][0],&mZRotTrack[track_id][1]) )	//������ �ƴϸ�.
				mZRotTrack[track_id][1] = mZRotTrack[track_id][0];
		}
		if( !strcmp(hole,"yrot"))
		{
			mTrackFlag[track_id]|=_TRACK_FLAG_YROT;
			mFlag |= _PARTICLE_ATTR_Y_ROT;
			if( is_track_ok == -1 )
			{
				Warning(hole,"�� ��ɾ�� time�� ���� �����ѵ� ���ּ���.");
				continue;
			}
			if( !GetRandOrNum(fp,&mYRotTrack[track_id][0],&mYRotTrack[track_id][1]) )	//������ �ƴϸ�.
				mYRotTrack[track_id][1] = mYRotTrack[track_id][0];
		}
		if( !strcmp(hole,"flicker"))
			mTrackFlag[track_id]|=_TRACK_FLAG_FLICKER;
		
		if( !strcmp(hole,"color"))
		{
			if( is_track_ok == -1 )
			{
				Warning(hole,"�� ��ɾ�� time�� ���� �����ѵ� ���ּ���.");
				continue;
			}
			/*
			GetRandOrNum(fp,&temp_f,&temp_f);	// ���� �������� ������..
			mRTrack[track_id]=(BYTE)temp_f;
			GetRandOrNum(fp,&temp_f,&temp_f);	// ���� �������� ������..
			mGTrack[track_id]=(BYTE)temp_f;
			GetRandOrNum(fp,&temp_f,&temp_f);	// ���� �������� ������..
			mBTrack[track_id]=(BYTE)temp_f;
			*/
			float temp_f2;
			GetRandOrNum(fp,&temp_f,&temp_f2);
			mRTrack[track_id][0]=(BYTE)temp_f;
			mRTrack[track_id][1]=(BYTE)temp_f2;
			GetRandOrNum(fp,&temp_f,&temp_f2);
			mGTrack[track_id][0]=(BYTE)temp_f;
			mGTrack[track_id][1]=(BYTE)temp_f2;
			GetRandOrNum(fp,&temp_f,&temp_f2);
			mBTrack[track_id][0]=(BYTE)temp_f;
			mBTrack[track_id][1]=(BYTE)temp_f2;
			mTrackFlag[track_id]|=_TRACK_FLAG_COLOR;
		}
		if( !strcmp(hole,"scale"))
		{
			if( is_track_ok == -1 )
			{
				Warning(hole,"�� ��ɾ�� time�� ���� �����ѵ� ���ּ���.");
				continue;
			}
			if( !GetRandOrNum(fp,&mScaleTrack[track_id][0],&mScaleTrack[track_id][1]) )	//������ �ƴϸ�.
				mScaleTrack[track_id][1] = mScaleTrack[track_id][0];
			mTrackFlag[track_id]|=_TRACK_FLAG_SCALE;
		}
//		if( !strcmp(hole,"time"))
//			fscanf(fp,"%s",hole);
	}
	fclose(fp);

	return TRUE;
}
void CParticle::GetFlickerARGB(int i,DWORD *dw_argb)	//�ø�Ŀ�� �ִٸ� �÷����� ���Ѵ�.
{
	if( ! (mElement[i].mFlag & _ELEMENT_ATTR_FLICKER) )	//�ø�Ŀ�� �ƴѴ�.
		return;
	DWORD dw_frame = (DWORD)(mElement[i].mNowFrame*32768);
	DWORD dw_flicker = (DWORD)(mFlickerTime*65536);
	if( dw_flicker )
	{
		if( dw_frame % dw_flicker < dw_flicker/2 )	//�ø�Ŀ �����ؾߵȴ�.
		{
			if( (*dw_argb>>24) >  mFlickerAlpha )
				*dw_argb = (*dw_argb&0x00ffffff)|(mFlickerAlpha<<24);
		}
	}
}


void CParticle::SetStartBoxArea()	//���۹ڽ��ȿ��� ��� ��ƼŬ�� ������ �ű��. //�� ��ó�� ȯ�濡�� ��������
{
	Vector3f pos;
	int i;


	for(i=0 ;i<mNum; i++)
	{
		if( !mElement[i].mIsLive )	//��� �ִ� �ѵ鸸 ���.
			continue;
		
		float x_length = mStartPos[1][0]-mStartPos[0][0];
		float z_length = mStartPos[1][2]-mStartPos[0][2];

		pos[0] = mElement[i].mPos[0] - mCreatePos[0];
		if(pos[0] < mStartPos[0][0])
			pos[0]+=x_length;
		if(pos[0] > mStartPos[1][0])
			pos[0]-=x_length;
		mElement[i].mPos[0] = pos[0] + mCreatePos[0];

		pos[2] = mElement[i].mPos[2] - mCreatePos[2];
		if(pos[2] < mStartPos[0][2])
			pos[2]+=z_length;
		if(pos[2] > mStartPos[1][2])
			pos[2]-=z_length;
		mElement[i].mPos[2] = pos[2] + mCreatePos[2];
	}
}

static void CrossVector( Vector3f a, Vector3f b, Vector3f r )
{
    r[0] = (a[1] * b[2]) - (a[2] * b[1]);
    r[1] = (a[2] * b[0]) - (a[0] * b[2]);
    r[2] = (a[0] * b[1]) - (a[1] * b[0]);
}






void GetViewVector(Vector3f v)
{
	Vector3f org = {0,0,1};

	Vector3fTransform(v,org,GetR3State()->mMatView.m);
	v[0] -= stState.mMatView.m[3][0];
	v[1] -= stState.mMatView.m[3][1];
	v[2] -= stState.mMatView.m[3][2];
	Normalize(v);
}


//Get Snake Vertex list	������� ���� strip ���ؽ� ����Ʈ �̴�.
DWORD CalcSnakeVertexList(_D3DR3VERTEX_TEX1 *get_v,Vector3f *org_v,DWORD cnt,float f_length,DWORD color)
{
	if( cnt < 2 || cnt > 99)	//2���� �־����..
		return 0;

	DWORD v_cnt=0,i;
	Vector4f plane;	//�⺻�� �Ǵ� ����� ������.	ax+by+cz=d
	Vector3f v_temp,cross_v;
	Vector3f l_snake[100][2];	//���� ����.
	Vector3f r_snake[100][2];	//������ ����.
	float f_temp;

	Vector3f org = {0,0,1};
/*
	GetR3State()->mViewVector[0]=-GetR3State()->mViewVector[0];
	GetR3State()->mViewVector[1]=-GetR3State()->mViewVector[1];
	Vector3fCopy( GetR3State()->mViewVector,plane);
*/
//	plane[1]=0;
//	Normalize(plane);

	Vector3fCopy(GetR3State()->mInvMatView.m[3],org);

	Vector3f old_cross_v={0,1,0};
	for(i=0; i<cnt-1; i++)
	{
		org_v[i];
		org_v[i+1];
		Vector3fSub(org_v[i],org_v[i+1],v_temp);
		f_temp = Vector3fDot(v_temp,v_temp);
		if( f_temp < 0.01f )	//���� ���� ��ġ��.. �׷��� �ȻѸ���.
			continue;
		f_temp = sqrtf(f_temp);
		v_temp[0] /= f_temp;
		v_temp[1] /= f_temp;
		v_temp[2] /= f_temp;
		Vector3fSub(org_v[i],org_v[i+1],v_temp);
		
		Vector3fSub(org_v[i],org,plane);
		Normalize(plane);


//		Vector3fCross(v_temp,plane,cross_v);  //��� ���� �ϼ�.
//		Vector3fCross(plane,v_temp,cross_v);  //��� ���� �ϼ�.
		CrossVector(plane,v_temp,cross_v);  //��� ���� �ϼ�.

		f_temp = Vector3fDot(cross_v,cross_v);
/*		if( f_temp < 1.1f )	//�� ���Ϳ� ���� ���� 
		{
			cross_v[0] = 0;
			cross_v[1] = 1;
			cross_v[2] = 0;
//			Vector3fCopy(old_cross_v,cross_v);
		}
		else*/
		{
			f_temp = sqrtf(f_temp);
			cross_v[0] /= f_temp;
			cross_v[1] /= f_temp;
			cross_v[2] /= f_temp;
			Vector3fMul(cross_v,f_length,cross_v);// �������߰��ؼ� ���Դ�. �̰ɷ�..������ ���ؽ��� ������.
		}
		Vector3fAdd(org_v[i],cross_v,l_snake[v_cnt][0]);
		Vector3fSub(org_v[i],cross_v,r_snake[v_cnt][0]);
		Vector3fAdd(org_v[i+1],cross_v,l_snake[v_cnt][1]);
		Vector3fSub(org_v[i+1],cross_v,r_snake[v_cnt][1]);
		Vector3fCopy(cross_v,old_cross_v);
		v_cnt++;
	}
	if( v_cnt < 1 )
		return 0;

#define SetF(aa,bb,cc)	{	get_v[f_cnt].u=aa;	get_v[f_cnt].v=bb;	get_v[f_cnt].color=cc; };
	//���� ���ε��� ���� �ؼ� ���� ������....
	DWORD f_cnt=0;
	Vector3fCopy( l_snake[0][0],get_v[f_cnt].pos );
	SetF(0,0,color);
	f_cnt++;
	Vector3fCopy( r_snake[0][0],get_v[f_cnt].pos );
	SetF(1,0,color);
	f_cnt++;
	float fv_cnt = (float)v_cnt;
	for(i=0; i<v_cnt-1; i++)
	{
		float v = (i+1)/(fv_cnt);
		Vector3fAdd(l_snake[i][1],l_snake[i+1][0],v_temp);
		Vector3fMul(v_temp,0.5f,get_v[f_cnt].pos);
		SetF(0.0f,v,color);
		f_cnt++;
		Vector3fAdd(r_snake[i][1],r_snake[i+1][0],v_temp);
		Vector3fMul(v_temp,0.5f,get_v[f_cnt].pos);
		SetF(1,v,color);
		f_cnt++;
	}
	Vector3fCopy( l_snake[i][1],get_v[f_cnt].pos );
	SetF(0,1,color);
	f_cnt++;
	Vector3fCopy( r_snake[i][1],get_v[f_cnt].pos );
	SetF(1,1,color);
	f_cnt++;
	return f_cnt;
}
