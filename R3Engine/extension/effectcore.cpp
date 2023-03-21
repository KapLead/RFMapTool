#include "EffectUtil.h"
#include "EffectCore.h"
#include "CMagicScript.h"
#include "EffectEntityList.h"
#include "r3util.h"
#include "r3math.h"
#include "Commonutil.h"
#include "Core.h"
#include "Cjcache.h"
#include "renderparticle.h"
#include "../global.h"


//#define _NONE_CACHE	//ĳ���� �Ⱦ��� �ǽð� �ε��Ҷ�


//-------����Ʈ ���� �Ŵ�����.
//#define _MAX_EFFECT_RESOURCE 256 // ����Ʈ ���ҽ�����..
#define _MAX_EFFECT_CNT	256	// ���� ���� ����Ʈ�� 
#define _MAX_MAGIC_CACHE_SIZE	256		//�ִ� ���� ĳ�� ������


#define _MAX_SIMULTARIOUS_WEAPON 128	//���� ���� ����ǥ��.

#define _MAX_WEAPON 128	//�ִ� ����..

#define _MAX_MAGIC_EFFECT_CNT	256	// ���� ���� ���� ����Ʈ�� 

//BYTE EffectCache[_MAX_EFFECT_RESOURCE];

static CEffectList stEffectList[_MAX_EFFECT_CNT];
static DWORD stELcnt=0;

//���� ������ ���ѰŴ�. �̰��� ĳ���� ������ ����...
//static CProcessMagic stProcessMagic[10];	

static CMagicList stMagicList[_MAX_MAGIC_EFFECT_CNT];
static CMagicList *NowMagicListPtr;
static DWORD stMLcnt=0;
static CJcache stMagicCache;	//���� ĳ���� ���Ѱ�...
class CBsp;
static CBsp *stBsp=NULL;


static Vector3f stVectorARGV1,stVectorARGV2;

void SpecialARGV1(Vector3f pos)
{
	Vector3fCopy(pos,stVectorARGV1);
}
void GetSpecialARGV1(Vector3f pos)
{
	Vector3fCopy(stVectorARGV1,pos);
}
void SpecialARGV2(Vector3f pos)
{
	Vector3fCopy(pos,stVectorARGV2);
}
void GetSpecialARGV2(Vector3f pos)
{
	Vector3fCopy(stVectorARGV2,pos);
}

void SetBspPtr(void *bsp)
{
	stBsp=(CBsp *)bsp;
}

void ResetEffectManager()
{
	stELcnt=0;
	stMLcnt=0;
}

CEffectList* GetEffectList()
{
	return stEffectList;
}
DWORD GetEffectListCnt()
{
	return stELcnt;
}
CMagicList* GetMagicList()
{
	return stMagicList;
}
DWORD GetMagicListCnt()
{
	return stMLcnt;
}

DWORD GetEffectCacheSize()	//���� ���ǰ��մ� ����Ʈ ĳ�� ����� �˾Ƴ���.
{
	DWORD size = 0;
	size += _MAX_EFFECT_CNT*sizeof(CEffectList);
	size += _MAX_MAGIC_EFFECT_CNT*sizeof(CMagicList);
	size += _MAX_MAGIC_CACHE_SIZE*sizeof(CProcessMagic);

	return size;
}

DWORD GetEffectCacheTotalNum()	//���� ���ǰ��մ� ����Ʈ ĳ�� ������ �˾Ƴ���.
{
	return _MAX_MAGIC_CACHE_SIZE;
}

DWORD GetEffectCacheNum()	//���� ����Ʈ ĳ�� �� ������ �˾Ƴ���.
{
	return stMagicCache.GetCacheCnt();
}

static void DeleteEffectList(DWORD id)
{
	if( stELcnt <= 0 )
		return;
	if((stELcnt-id-1) > 0)
	{
		memcpy(&stEffectList[id],&stEffectList[id+1]
			,sizeof(CEffectList)*(stELcnt-id-1));
	}
	stELcnt--;
}

static DWORD stUniqueID=1;

DWORD GetNextUniqueID()
{
	if( stUniqueID > 0x00ffffff )	//Ȥ�� ��ĥ��츦 ����ؼ�..
		stUniqueID = 1;
	return stUniqueID++;
}


void StartEffect(float mat[4][4], DWORD effect_id,DWORD state
	,DWORD color,float scale,DWORD flag,float frame,DWORD unique_id)
{
	if( stELcnt > _MAX_EFFECT_CNT-1 )
	{
	//	stELcnt=_MAX_EFFECT_CNT-1;
		DeleteEffectList(0);	//�����ȰŸ� ������...
		stELcnt=_MAX_EFFECT_CNT-1;
	}
	stEffectList[stELcnt].mFrame=frame;
	stEffectList[stELcnt].mEffectID=effect_id;
	stEffectList[stELcnt].mSpecialID=0;

	stEffectList[stELcnt].mFlag = flag;
	//��ƼŬ�� ��ƼƼ�� �ε��Ѵ�.
	if( flag & _MT_FLAG_PARTICLE )
	{
		stEffectList[stELcnt].mParticle = GetParticleFromEffectEntityList(effect_id,mat);
		if( stEffectList[stELcnt].mParticle == NULL )
			return;
		if( flag & _MT_FLAG_Z_DISABLE )
			stEffectList[stELcnt].mParticle->mFlag |= _PARTICLE_ATTR_ZDISABLE;
	}
	else
	{
		stEffectList[stELcnt].mEntity = GetEntityFromEffectEntityList(effect_id);
		if( stEffectList[stELcnt].mEntity == NULL )
			return;
		if( flag & _MT_FLAG_Z_DISABLE )
			stEffectList[stELcnt].mEntity->mFlag |= _ENTITY_Z_DISABLE;

		if( flag & _MT_FLAG_BILLBOARD )	//�������� ���.
			stEffectList[stELcnt].mEntity->AddFlag(_EF_BILLBOARD);
		stEffectList[stELcnt].mEntity->SetStartTimer();			//�ؽ��� �ִϸ� ���Ѱ�.
	}

	MatrixCopy(stEffectList[stELcnt].mMat,mat);
//	Vector3fCopy(pos,stEffectList[stELcnt].mPos);
//	stEffectList[stELcnt].mYAngle = y_angle;
	if( !unique_id )	//���� ���̵� ���� �����ָ� �����´�.
		unique_id=GetNextUniqueID();
	stEffectList[stELcnt].mUniqueID=unique_id;
	stEffectList[stELcnt].mState = state;
	stEffectList[stELcnt].mAlphaDisapear=0;
	stEffectList[stELcnt].mAlphaApear=0;
	stEffectList[stELcnt].mColor = color;
	stEffectList[stELcnt].mScale = scale;

	stELcnt++;
}
void StartEffect(Vector3f pos,float y_angle, DWORD effect_id,DWORD state
	,DWORD color,float scale,DWORD flag,float frame,DWORD unique_id)
{
	float mat[4][4];

	if( _MT_FLAG_Y_RANDOM & flag )
	{
		union{
		WORD yrand[2];
		float goc;
		}_st;
		_st.goc=y_angle;
		y_angle=(WORD)rand()%(_st.yrand[1]-_st.yrand[0])+_st.yrand[0];
	}
	MatrixRotate(mat,0,y_angle,0); //������ 360��
	mat[3][0] = pos[0];
	mat[3][1] = pos[1];
	mat[3][2] = pos[2];
	StartEffect(mat,effect_id,state,color,scale,flag,frame,unique_id);
}

BOOL SetEffectState(DWORD unique_id,DWORD state)
{
	DWORD i;

	for( i=0; i<stELcnt; i++ )
	{
		if( stEffectList[i].mUniqueID == unique_id )
		{
			stEffectList[i].mState = state;
			return TRUE;
		}
	}
	return FALSE;
}

_MAGIC_TRACK *GetMagicTrack(DWORD unique_id)
{
	DWORD i;

	for( i=0; i<stMLcnt; i++ )
	{
		if( stMagicList[i].mUniqueID == unique_id )
		{
			return ((CProcessMagic *)stMagicList[i].mMagicPtr)->mMagicTrack;
		}
	}
	return NULL;
}

CProcessMagic *GetMagic(DWORD unique_id)
{
	DWORD i;

	for( i=0; i<stMLcnt; i++ )
	{
		if( stMagicList[i].mUniqueID == unique_id )
		{
			return (CProcessMagic *)stMagicList[i].mMagicPtr;
		}
	}
	return NULL;
}
CMagicList* GetMagicListFromID(DWORD unique_id)
{
	DWORD i;

	for( i=0; i<stMLcnt; i++ )
	{
		if( stMagicList[i].mUniqueID == unique_id )
		{
			return &stMagicList[i];
		}
	}
	return NULL;
}

void SetCurveMagicTrackRandom(_MAGIC_TRACK *mtrack)	//���� �������� ���� Ŀ�긦 �������� ����Ѵ�.
{
	DWORD i,id;
	float from,at;

	for(i=0; i<3; i++)
	{
		id = _MAX_MAGIC_TRACK-3;
		from = mtrack[id].Rand1[i];
		at = mtrack[id].Rand2[i];
		mtrack[id].Pos[i] = GetRandNum(from,at);

		id = _MAX_MAGIC_TRACK-2;
		from = mtrack[id].Rand1[i];
		at = mtrack[id].Rand2[i];
		mtrack[id].Pos[i] = GetRandNum(from,at);

		id = _MAX_MAGIC_TRACK-1;
		from = mtrack[id].Rand1[i];
		at = mtrack[id].Rand2[i];
		mtrack[id].Pos[i] = GetRandNum(from,at);
	}
}
/*
//�ش� �������� ���Ǵ� ����� id�� �ϳ��� ����.
DWORD GetSpecialIDFromMagic(DWORD unique_id)
{
	DWORD i;

	for( i=0; i<stMLcnt; i++ )
	{
		if( stMagicList[i].mUniqueID == unique_id )	//�ش�Ǵ� ��ƼƼ���� �˴�...
		{
			stMagicList[i].mState = state;
			for( int j=0; j<stMagicList[i].mEntityUniqueIDCnt; j++)
				SetEffectState(stMagicList[i].mEntityUniqueID[j],state);

			return TRUE;
		}
	}
	return 0;
}
*/

BOOL CopyEffectARGV(DWORD unique_id)
{
	DWORD i;

	for( i=0; i<stELcnt; i++ )
	{
		if( stEffectList[i].mUniqueID == unique_id )
		{
			Vector3fCopy(stVectorARGV1,stEffectList[i].mVectorArgv1);
			Vector3fCopy(stVectorARGV2,stEffectList[i].mVectorArgv2);
			return TRUE;
		}
	}
	return FALSE;
}

//�ش� �������� ���Ǵ� ����� id�� �ϳ��� ����.
BOOL SetSpecialIDFromMagic(DWORD unique_id,DWORD special_id)
{
	DWORD i;

	for( i=0; i<stMLcnt; i++ )
	{
		if( stMagicList[i].mUniqueID == unique_id )	//�ش�Ǵ� ��ƼƼ���� �˴�...
		{
			stMagicList[i].mSpecialID = special_id;
			Vector3fCopy(stVectorARGV1,stMagicList[i].mVectorArgv1);
			Vector3fCopy(stVectorARGV2,stMagicList[i].mVectorArgv2);
			return TRUE;
		}
	}
	return FALSE;
}

DWORD GetSpecialIDFromMagic(DWORD unique_id)
{
	DWORD i;

	for( i=0; i<stMLcnt; i++ )
	{
		if( stMagicList[i].mUniqueID == unique_id )	//�ش�Ǵ� ��ƼƼ���� �˴�...
		{
			return stMagicList[i].mSpecialID;
		}
	}
	return 0;
}

BOOL CopySpecialIDFromMagic(DWORD unique_id)	//������ ����Ʈ���� ���ڸ� ..	
{
	DWORD i;

	for( i=0; i<stMLcnt; i++ )
	{
		if( stMagicList[i].mUniqueID == unique_id )	//�ش�Ǵ� ��ƼƼ���� �˴�...
		{
			if( !stMagicList[i].mSpecialID )	//������Ұ� ����.
				return TRUE;

			for( int j=0; j<stMagicList[i].mEntityUniqueIDCnt; j++)
				CopyEffectARGV(stMagicList[i].mEntityUniqueID[j]);

			return TRUE;
		}
	}
	return FALSE;
}


BOOL SetMagicState(DWORD unique_id,DWORD state)
{
	DWORD i;

	for( i=0; i<stMLcnt; i++ )
	{
		if( stMagicList[i].mUniqueID == unique_id )	//�ش�Ǵ� ��ƼƼ���� �˴�...
		{
			if( state == stMagicList[i].mState )
				return TRUE;

			stMagicList[i].mState = state;
			for( int j=0; j<stMagicList[i].mEntityUniqueIDCnt; j++)
				SetEffectState(stMagicList[i].mEntityUniqueID[j],state);

			return TRUE;
		}
	}
	return FALSE;
}

BOOL SetDisapearTimeEffect(DWORD unique_id,float time)
{
	DWORD i;

	for( i=0; i<stELcnt; i++ )
	{
		if( stEffectList[i].mUniqueID == unique_id )
		{
			stEffectList[i].mState = _EF_STATE_START_DISAPEAR;	//�Ӽ� �ڵ� ����.
			stEffectList[i].mDisapearTime = time;
			stEffectList[i].mDisapearFrame = stEffectList[i].mFrame;
			if( time==0 )	//��� ���־� �Ǵ°�
			{
				if( stEffectList[i].mFlag & _MT_FLAG_PARTICLE )//��ƼŬ�ΰ��.
				{
					CParticle *particle=stEffectList[i].mParticle;
					DWORD flag = particle->GetParticleAttr();	//�����ȵ�.
					particle->SetParticleAttr(_PARTICLE_ATTR_NO_LOOP|flag);	//�����ȵ�.
				}
				else
				{
					DeleteEffectList(i);
					i--;	//��ĭ�� �и��Ƿ�...
				}
			}
			return TRUE;
		}
	}
	return FALSE;
}
//���� unique_id�� ����ִ°�?
BOOL IsExistUniqueID(DWORD unique_id)
{
	DWORD i;

	for( i=0; i<stELcnt; i++ )
	{
		if( stEffectList[i].mUniqueID == unique_id )
			return TRUE;
	}
	return FALSE;
}
//Ư�� ����Ʈ���� �����϶� ����Ѵ�.  ȭ���̳� ��Ÿ �׷��͵�...
BOOL MoveMatrixEffect(DWORD unique_id,float mat[4][4])	//������ ���о��̵��� ����Ʈ�� �����Ѵ�.
{
	DWORD i;

	for( i=0; i<stELcnt; i++ )
	{
		if( stEffectList[i].mUniqueID == unique_id )
		{
			if( mat !=0 )
			{
				if( NowMagicListPtr )
				{
					if( NowMagicListPtr->mSpecialID )
					{
						stEffectList[i].mSpecialID = stMagicList[i].mSpecialID;
						Vector3fCopy(NowMagicListPtr->mVectorArgv1,stEffectList[i].mVectorArgv1);
						Vector3fCopy(NowMagicListPtr->mVectorArgv2,stEffectList[i].mVectorArgv2);
					}
				}
				MatrixCopy(stEffectList[i].mMat,mat);
/*
				if( stEffectList[i].mFlag & _MT_FLAG_PARTICLE && stEffectList[i].mParticle )//��ƼŬ�ΰ��.	
				{
					if( stEffectList[i].mParticle->mFlag & _PARTICLE_ATTR_SPECIAL_ID )	//spcial ���ڸ� �־��ش�.
					{
						if( stEffectList[i].mParticle->mSpecialID == 1 )
						{
							Vector3fCopy(stEffectList[i].mVectorArgv1,stEffectList[i].mParticle->mPowerTrack[0][1]);	//Ÿ����ǥ �־�����.
						}
					}
				}
*/
			}
			return TRUE;
		}
	}
	return FALSE;
}


BOOL MovePosEffect(DWORD unique_id,Vector3f pos,float y_angle)	//������ ���о��̵��� ����Ʈ�� �����Ѵ�.
{
	float mat[4][4];

	MatrixRotate(mat,0,y_angle,0); //������ 360��
	mat[3][0] = pos[0];
	mat[3][1] = pos[1];
	mat[3][2] = pos[2];

	return MoveMatrixEffect(unique_id,mat);	//������ ���о��̵��� ����Ʈ�� �����Ѵ�.
}
/*
static void StartEffectTrack(Vector3f pos,float y_angle,_MAGIC_TRACK *magic_track,DWORD state,float frame=0,DWORD unique_id=0)
{
	StartEffect( pos, y_angle, magic_track->EntityID,state
		,magic_track->Color,magic_track->Scale,magic_track->Flag,frame,unique_id);
	stEffectList[stELcnt-1].mAlphaDisapear=magic_track->AlphaDisapear;
}
*/
static void StartEffectTrack(float mat[4][4],_MAGIC_TRACK *magic_track,DWORD state,float frame=0,DWORD unique_id=0)
{
	StartEffect( mat, magic_track->EntityID,state
		,magic_track->Color,magic_track->Scale,magic_track->Flag,frame,unique_id);
	if( stELcnt )
	{
		stEffectList[stELcnt-1].mAlphaDisapear=magic_track->AlphaDisapear;
		stEffectList[stELcnt-1].mAlphaApear=magic_track->AlphaApear;
	}
//	stEffectList[stELcnt-1].mDynamicLight=magic_track->DynamicLight;
}

static void FrameMoveEffectsList()
{
	DWORD i,j;

	// camera rendering �ӽ�.....
//	GetCamera()->Render();
//	if( g_pCamera->GetPosY() > 35 )
//		int iii=0;
	for( i=0; i<stELcnt; i++ )
	{
/*
		switch( stEffectList[i].mEffectID )
		{
		case _EF_BLOOD01:
			//stEffectList[i].stBlood
			break;
		default:;
		}
*/
		if( stEffectList[i].mState == _EF_STATE_DEATH )//��� �׿����´�.
			continue;
			
		if( stEffectList[i].mFlag & _MT_FLAG_PARTICLE )//��ƼŬ�ΰ��.
		{
			CParticle *particle = stEffectList[i].mParticle;
			particle->SetBspPtr(stBsp);
			if( stEffectList[i].mFlag &	_MT_FLAG_ENTITY_FADE )
				particle->SetCreatePos(GetR3State()->mInvMatView.m[3]);	//�浹 �Ҷ� �ʿ��ϴ�...
			else
				particle->SetCreatePos(stEffectList[i].mMat[3]);	//�浹 �Ҷ� �ʿ��ϴ�...

			stEffectList[i].mFrame += R3GetLoopTime()*30;
			if( stEffectList[i].mState == _EF_STATE_START_DISAPEAR )
			{
				if( stEffectList[i].mFrame-stEffectList[i].mDisapearFrame > stEffectList[i].mDisapearTime*30 ) //������� �ð����� ũ��...
				{
					DWORD flag = particle->GetParticleAttr();	//�����ȵ�.
					particle->SetParticleAttr(_PARTICLE_ATTR_NO_LOOP|flag);	//�����ȵ�.
				}
			}
			if( particle->mSpecialID )	//���伣�ϰ�� ó��.
			{
				if( particle->mSpecialID == 1 )
				{
					Vector3fCopy(stEffectList[i].mVectorArgv1,particle->mPowerTrack[0][1]);	//Ÿ����ǥ �־�����.
				}
				else
				if( particle->mSpecialID >= 3 )
				{
					Vector3fCopy(stEffectList[i].mVectorArgv1,particle->mSpecialARGV[0]);	//Ÿ����ǥ �־�����.
					Vector3fCopy(stEffectList[i].mVectorArgv2,particle->mSpecialARGV[1]);	//Ÿ����ǥ �־�����.
				}
			}
			if( particle->Loop() ==0 )
			{
				delete particle;	//��ƼŬ�� �Ź� �����ǹǷ� ��������Ѵ�.
				stEffectList[i].mParticle=NULL;
				DeleteEffectList(i);
				i--;	//��ĭ�� �и��Ƿ�...
			}
			continue;
		}

		if( stEffectList[i].mFlag & _MT_FLAG_DYNAMIC_LIGHT )//���̳��� ����Ʈ �Ӽ��ΰ��.
		{
			if( stEffectList[i].mEntity == NULL )
				continue;

			float f_alpha,temp_f;
			DWORD alpha;
			if( stEffectList[i].mEntity->mObject->frames !=0 )
			{
				f_alpha = 1-stEffectList[i].mFrame/(float)stEffectList[i].mEntity->mObject->frames;
				if( stEffectList[i].mAlphaDisapear != 1 )
					f_alpha=(f_alpha)/(1-stEffectList[i].mAlphaDisapear);
				else
					f_alpha=1;
				f_alpha=min(f_alpha,1);

				if( stEffectList[i].mAlphaApear != 0 )
				{
					temp_f = stEffectList[i].mFrame/(float)stEffectList[i].mEntity->mObject->frames;
					temp_f=(temp_f)/(stEffectList[i].mAlphaApear);
					f_alpha=min(f_alpha,temp_f);
				}
			}
			else
				f_alpha=1;
			if( stEffectList[i].mState == _EF_STATE_ALWAYS_LIVE )
				f_alpha=1;
			else
			if( stEffectList[i].mState == _EF_STATE_START_DISAPEAR ) //������� �����ϴ� �ð�.
			{
				f_alpha = 1-(stEffectList[i].mFrame-stEffectList[i].mDisapearFrame)/(stEffectList[i].mDisapearTime*30);
				f_alpha=min(f_alpha,1);
			}
			
			Vector3f pos;
			float scale;

			stEffectList[i].mEntity->PrepareAnimation();
			float w_matrix[4][4];
			for(j=0; j<stEffectList[i].mEntity->mMatGroupNum; j++)
			{
				GetObjectMatrix(w_matrix,stEffectList[i].mEntity->mMatGroup[j].ObjectId,
					stEffectList[i].mEntity->mObject,stEffectList[i].mFrame);	//�ִϸ��̼� ��Ʈ������ ������ �����´�.
			}

			if( stEffectList[i].mEntity->mObjectNum == 0 )
			{
				Vector3fSub(stEffectList[i].mEntity->mMatGroup[0].BBMax
					,stEffectList[i].mEntity->mMatGroup[0].BBMin,pos);
				scale = pos[0]/2;	//����..
				pos[0]=stEffectList[i].mMat[3][0];
				pos[1]=stEffectList[i].mMat[3][1];
				pos[2]=stEffectList[i].mMat[3][2];
			}
			else
			{
				Vector3fAdd(stEffectList[i].mMat[3],stEffectList[i].mEntity->mObject->f_matrix[3],pos);
				scale = stEffectList[i].mEntity->mObject->f_matrix[0][0];
			}
			f_alpha = f_alpha*((stEffectList[i].mColor>>24)/255.0f);	//������ ���ĸ� ���.
			alpha = ((DWORD)(f_alpha*255))<<24;
			scale *= stEffectList[i].mScale;

			void *tex;
			if( stEffectList[i].mEntity->mMatGroup )
			{
				if( stEffectList[i].mEntity->mMatGroup->MtlId == -1 )
					tex=NULL;
				else
					tex = (LPDIRECT3DTEXTURE8)R3GetSurface(stEffectList[i].mEntity->mMat->m_Layer[0].m_iSurface);
			}
			else
				tex=NULL;

			BOOL magic_light,alpha_type;
			
			if( _MT_FLAG_MAGIC_LIGHT & stEffectList[i].mFlag )
			{
				magic_light = TRUE;
				alpha_type = stEffectList[i].mEntity->mMat->m_Layer[0].m_dwAlphaType;
			}
			else
			{
				magic_light = FALSE;
				alpha_type = _BLEND_BRIGHT;
			}

			//ù��° ������Ʈ�� �����ϰ��� �����Ѵ�....
			scale=max(0.1f,scale);	//-�� ����.������.
			InsertDynamicLight(pos,scale
				,((stEffectList[i].mColor&0x00ffffff)|alpha),tex,magic_light,alpha_type);
//			InsertDynamicLight(pos,(stEffectList[i].mDynamicLight>>24)*4
//				,((stEffectList[i].mDynamicLight&0x00ffffff)|alpha));
		}
		
		if( !stEffectList[i].mEntity->IsLoadedEntity() )	//��ƼƼ�� �ε��� �ȉ��.���� �ʿ䰡 ����.
		{
			DeleteEffectList(i);
			i--;	//��ĭ�� �и��Ƿ�...
			continue;
		}

		stEffectList[i].mFrame += R3GetLoopTime()*30;


		if( stEffectList[i].mState == _EF_STATE_AUTO_DISAPEAR ||stEffectList[i].mState == _EF_STATE_MYSELF_LIVE)
		{
			if( stEffectList[i].mEntity->mObjectNum == 0 )	//�ִϰ� ������ �׻� ��� �ְ��Ѵ�.
				continue;
			if( stEffectList[i].mFrame > stEffectList[i].mEntity->mObject->frames )//�ִϰ� ������..
			{
				DeleteEffectList(i);
				i--;	//��ĭ�� �и��Ƿ�...
			}
		}
		else
		if( stEffectList[i].mState == _EF_STATE_START_DISAPEAR )
		{
			if( stEffectList[i].mFrame-stEffectList[i].mDisapearFrame > stEffectList[i].mDisapearTime*30 ) //������� �ð����� ũ��...
			{
				DeleteEffectList(i);
				i--;	//��ĭ�� �и��Ƿ�...
			}
		}
	}
}
/*
BOOL SetMagicEffectState(DWORD magic_id,DWORD state)	//��������Ʈ ���� �����Ѵ�.
{
	CProcessMagic *get_magic;
	if( !stMagicCache.GetCache(magic_id,(void**)&get_magic) )
		return FALSE;
	get_magic->mState = state;
	for(DWORD i=0; i<get_magic->mTrackNum; i++)
	{
		get_magic->mMagicTrack[i].EntityID;
	}
}

DWORD GetMagicEffectState(DWORD magic_id)	//��������Ʈ ���¸� ���´�.
{
	CProcessMagic *get_magic;
	if( !stMagicCache.GetCache(magic_id,(void**)&get_magic) )
		return 0;
	return get_magic->mState;
}
*/
void DeleteMagicList(DWORD id)
{
	if( stMLcnt <= 0 )
		return;
	memcpy(&stMagicList[id],&stMagicList[id+1]
		,sizeof(CMagicList)*(stMLcnt-id-1));
	stMLcnt--;
}

BOOL DelMagicEffect(DWORD magic_id)//ĳ���� �ִ� ������ �����.
{
	CProcessMagic *get_magic;
	if( !stMagicCache.GetCache(magic_id,(void**)&get_magic) )
		return FALSE;
	get_magic->DestoryMagicTrack();
	return stMagicCache.DelCache(magic_id);
}


static DWORD stMagicID[_MAX_MAGIC_ID_STACK];

void PushMagicIDList( DWORD magic_id )
{
	memcpy(&stMagicID[0],&stMagicID[1],sizeof(DWORD)*(_MAX_MAGIC_ID_STACK-1));
	stMagicID[_MAX_MAGIC_ID_STACK-1]=magic_id;
}
DWORD *GetMagicIDList()
{
	return stMagicID;
}


//���������� ĳ����.../
DWORD StartMagicEffect(float mat[4][4], DWORD magic_id,void *v_einfo,DWORD state)
{
	PushMagicIDList(magic_id);
	EffectInfo *e_Info = (EffectInfo *)v_einfo;
	if( stMLcnt > _MAX_MAGIC_EFFECT_CNT-1 )	//���� ���������� ���ƴ�. 
	{
		stMLcnt=_MAX_MAGIC_EFFECT_CNT-1;
		DeleteMagicList(0);	//�����ȰŸ� ������...
		stMLcnt=_MAX_MAGIC_EFFECT_CNT-1;
	}
	DWORD real_magic_id = GetRealIDFromMagicSpt(magic_id);		//���� �ε����� �ٲ���.
	if( real_magic_id == -1 )
		return 0;		//���� ���̵� ����....

	stMagicList[stMLcnt].mState = state;
	stMagicList[stMLcnt].mCharTypeID = -1;// ĳ���� Ÿ��
	stMagicList[stMLcnt].mCharIndex = -1;// ���� ������ ���̺��� ���ڵ� �ε���
	stMagicList[stMLcnt].mFrame=0;
	stMagicList[stMLcnt].mMagicID=magic_id;
	stMagicList[stMLcnt].mSpecialID=0;
	if( e_Info )
		memcpy( &stMagicList[stMLcnt].mEInfo,e_Info,sizeof(EffectInfo));

	MatrixCopy(stMagicList[stMLcnt].mMat,mat);	//�ѹ濡.
//	Vector3fCopy(pos,stMagicList[stMLcnt].mPos);
//	stMagicList[stMLcnt].mYAngle = y_angle;

	//ĳ������ �ܾ� �´�.������ ���θ��� ����...
	CProcessMagic *get_magic;
	LONG result = stMagicCache.HitCache(magic_id,(void**)&get_magic);
#ifdef _NONE_CACHE
	ReleaseEffectEntityList();
	LoadEffectEntityList(".\\Effect\\EffectEntityList.spt");
	result = _HITCACHE_RET_DEL_AND_NEW;	//������ �ٽþ���.
#endif


	{
		char path[256];
		char *r_name = GetFilenameFromMagicSpt(magic_id);

		_R3ENGINE_STATE *state = GetR3State();
		strncpy(path,state->EffectPath,250);	//����Ʈ �н� ������...
		if( r_name )
			strcat(path,r_name);
		if( !IsExistFile(path))
		{
			Warning(path,"�����Ͼ���.������~");
			return 0;
		}
	}

	if( result == _HITCACHE_RET_NEW )
	{
		char *name = GetFilenameFromMagicSpt(magic_id);
		get_magic->CreateMagicTrack(name);
	}
	else
	if( result == _HITCACHE_RET_DEL_AND_NEW )	//������ �ٽþ���.
	{
		char *name = GetFilenameFromMagicSpt(magic_id);
		get_magic->DestoryMagicTrack();
		get_magic->CreateMagicTrack(name);
	}
	else// _HITCACHE_RET_OLD ������ �����Ƿ� �ٷ� ����.
	{
	}
	stMagicList[stMLcnt].mMagicPtr=get_magic;
	//------------------- ������ ���Ŀ� �ٷ�����ؼ�...��ġ��ǥ�� �̵��Ѵٵ���.. �� �׷���..
	stMagicList[stMLcnt].mEntityUniqueIDCnt=get_magic->mTrackNum;
	for( DWORD i=0; i<get_magic->mTrackNum; i++)	//����ũ ���̵� �Ҵ� �����ش�.
		stMagicList[stMLcnt].mEntityUniqueID[i] = GetNextUniqueID();

	stMagicList[stMLcnt].mUniqueID = stUniqueID+1;	//���� ���о��̵� ����.

	stMLcnt++;
	stUniqueID++;
	return stUniqueID;
}

//ĳ���� ���̵� ����Ѵ�. Ư���� ��Ȳ�̸� ���߿� ���ֱ� ����.
BOOL SetAttachCharMagicEffect(DWORD unique_id,DWORD char_type,DWORD char_index)
{
	DWORD i;

	for( i=0; i<stMLcnt; i++ )
	{
		if( stMagicList[i].mUniqueID == unique_id )
		{
			stMagicList[i].mCharTypeID = char_type;
			stMagicList[i].mCharIndex = char_index;			
			return TRUE;
		}
	}
	return FALSE;
}

//���������� ĳ����.../
DWORD StartMagicEffect(Vector3f pos,float y_angle, DWORD magic_id,void *v_einfo,DWORD state)
{
	EffectInfo *e_Info = (EffectInfo *)v_einfo;
	float mat[4][4];
	
	MatrixRotate(mat,0,y_angle,0); //������ 360��
	mat[3][0] = pos[0];
	mat[3][1] = pos[1];
	mat[3][2] = pos[2];
	//--�װŶ����� 
	stMagicList[stMLcnt].mYAngle = y_angle;
	stMagicList[stMLcnt].mYOldAngle = y_angle;
	stMagicList[stMLcnt].mYAngleCnt = 0;
	return StartMagicEffect(mat,magic_id,e_Info,state);
}

BOOL MovePosMagicEffect(DWORD unique_id,Vector3f pos,float y_angle)	//������ ���о��̵��� ����Ʈ�� �����Ѵ�.
{
	for(int i=0; i<stMLcnt; i++ )
	{
		if( stMagicList[i].mUniqueID == unique_id )	//�ش�Ǵ� ��ƼƼ���� �˴� �����δ�.
		{
			DWORD ret=FALSE;
			stMagicList[i].mYOldAngle = stMagicList[i].mYAngle;
			stMagicList[i].mYAngle = y_angle;
			for( int j=0; j<stMagicList[i].mEntityUniqueIDCnt; j++)
			{
				ret|=MovePosEffect(stMagicList[i].mEntityUniqueID[j],pos,y_angle);
			}
			return TRUE;
			//return ret;	//��ƼƼ�� ���λ���� ���� ���ϰ��� FALSE��.
		}
	}
	return FALSE;
}

BOOL MoveMatrixMagicEffect(DWORD unique_id,float matrix[4][4])	//������ ���о��̵��� ����Ʈ�� �����Ѵ�.
{
	Vector3f temp;

	for(int i=0; i<stMLcnt; i++ )
	{
		if( stMagicList[i].mUniqueID == unique_id )	//�ش�Ǵ� ��ƼƼ���� �˴� �����δ�.
		{
			DWORD ret=FALSE;
			for( int j=0; j<stMagicList[i].mEntityUniqueIDCnt; j++)
			{
				_MAGIC_TRACK *magic_track = &((CProcessMagic *)stMagicList[i].mMagicPtr)->mMagicTrack[j];
				
				temp[0] = matrix[0][0]*magic_track->Pos[0] + matrix[1][0]*magic_track->Pos[1] + matrix[2][0]*magic_track->Pos[2];
				temp[1] = matrix[0][1]*magic_track->Pos[0] + matrix[1][1]*magic_track->Pos[1] + matrix[2][1]*magic_track->Pos[2];
				temp[2] = matrix[0][2]*magic_track->Pos[0] + matrix[1][2]*magic_track->Pos[1] + matrix[2][2]*magic_track->Pos[2];
				
				NowMagicListPtr = &stMagicList[i];
				Vector3fAdd(matrix[3],temp,matrix[3]);
				ret|=MoveMatrixEffect(stMagicList[i].mEntityUniqueID[j],matrix);
				Vector3fSub(matrix[3],temp,matrix[3]);
				NowMagicListPtr=NULL;
/*

				Vector3fAdd(matrix[3],magic_track->Pos,matrix[3]);
				ret|=MoveMatrixEffect(stMagicList[i].mEntityUniqueID[j],matrix);
				Vector3fSub(matrix[3],magic_track->Pos,matrix[3]);
*/
				ret=1;
			}
			return ret;	//��ƼƼ�� ���λ���� ���� ���ϰ��� FALSE��.
		}
	}
	return FALSE;
}

BOOL GetMagicCTRLPoint012(DWORD unique_id,Vector3f *get_pos)	//���̾� Ŀ�� 4���� ������(Ÿ��) ���� �����ϰ� �־��ش�. 
{
	DWORD i;

	for( i=0; i<stMLcnt; i++ )
	{
		if( stMagicList[i].mUniqueID == unique_id )	//�ش�Ǵ� ��ƼƼ���� �˴� �����δ�.
		{
			CProcessMagic *magic = (CProcessMagic*)stMagicList[i].mMagicPtr;
			Vector3fCopy(magic->mMagicTrack[_MAX_MAGIC_TRACK-3].Pos,get_pos[0]);	//������.
			Vector3fCopy(magic->mMagicTrack[_MAX_MAGIC_TRACK-2].Pos,get_pos[1]);	//ctrl 1.
			Vector3fCopy(magic->mMagicTrack[_MAX_MAGIC_TRACK-1].Pos,get_pos[2]);	//ctrl 2.
			return TRUE;
		}
	}
	return FALSE;
}


//
BOOL SetDisapearTimeMagicEffect(DWORD unique_id,float time)
{
	for(int i=0; i<stMLcnt; i++ )
	{
		if( stMagicList[i].mUniqueID == unique_id )	//�ش�Ǵ� ��ƼƼ���� �˴�...
		{
			stMagicList[i].mState = _EF_STATE_START_DISAPEAR;
			for( int j=0; j<stMagicList[i].mEntityUniqueIDCnt; j++)
			{
//				SetEffectState(stMagicList[i].mEntityUniqueID[j],_EF_STATE_START_DISAPEAR);
				SetDisapearTimeEffect(stMagicList[i].mEntityUniqueID[j],time);
			}
			return TRUE;
		}
	}
	return FALSE;
}


//���������� �ٷ�� ����Ʈ���� ��ü�� �������.
void CreateEffectsList(char *filename)		//����Ʈ�� �ʱ�ȭ.
{
	stMagicCache.CreateJcache(_MAX_MAGIC_CACHE_SIZE,sizeof(CProcessMagic));
	LoadEffectEntityList(filename);	//��ũ��Ʈ���� ��ƼƼ����Ʈ�� �д´�.
}

void DestroyEffectsList()		//����Ʈ�� ������
{
	stMagicCache.DestroyJcache();

	//��ƼŬ�� �������ؾ� �Ѵ�.... ������ ��ü�� �����Ƿ�...
	for( int i=0; i<stELcnt; i++ )
	{
		if( stEffectList[i].mFlag & _MT_FLAG_PARTICLE )//��ƼŬ�ΰ��.
		{
			CParticle *particle = stEffectList[i].mParticle;
			delete particle;	//��ƼŬ�� �Ź� �����ǹǷ� ��������Ѵ�.
			stEffectList[i].mParticle=NULL;
			DeleteEffectList(i);
			i--;	//��ĭ�� �и��Ƿ�...
		}
	}
	stELcnt=0;	//��� ������...�ٸ� ��ƼƼ�� ������.

	ReleaseEffectEntityList();
}
BOOL IsAllDisapear(CMagicList *magic_list)	//���δ� ��������� TRUE
{
	for( int j=0; j<magic_list->mEntityUniqueIDCnt; j++)
	{
		DWORD id = magic_list->mEntityUniqueID[j];
		if( IsExistUniqueID(id) )
			return FALSE;
	}
	return TRUE;
}

static void FrameMoveMagicEffects()
{
	DWORD i;
	LONG ret,j,start_j;
	float start_frame;
	_MAGIC_TRACK *magic_track=NULL;

	for( i=0; i<stMLcnt; i++ )
	{
		stMagicList[i].mOldFrame = stMagicList[i].mFrame;
		stMagicList[i].mFrame += R3GetLoopTime()*30;
		//���� Ʈ���� �о Ÿ�ֿ̹� �´� ��ƼƼ�� �Ѹ���.
//		ret=((CProcessMagic *)stMagicList[i].mMagicPtr)->GetEntityIDs(stMagicList[i].mOldFrame,stMagicList[i].mFrame,start_frame,&magic_track);
		ret=((CProcessMagic *)stMagicList[i].mMagicPtr)->GetEntityIDs(stMagicList[i].mOldFrame,stMagicList[i].mFrame,&start_j);
		if( ret >= 0 )	//��ƼƼ�� �ִ�. �Ѹ���.
		{
			magic_track = ((CProcessMagic *)stMagicList[i].mMagicPtr)->mMagicTrack;

			for(j=start_j; j<ret+start_j; j++)
			{
				if( magic_track[j].Flag & _MT_FLAG_QUAKE )	//�� �����̴�.
				{
					float density = GetDensityFromPos(stMagicList[i].mMat[3]);
					if( density > 0 )
						R3EnvironmentQuake(magic_track[j].QuakeTime,magic_track[j].QuakeDensity*density);
					continue;
				}
				if( magic_track[j].Flag & _MT_FLAG_SKY_FADE )	//�� �ϴ� ���̵����̴�.
				{
					SetFadeSky(magic_track[j].Color,magic_track[j].SkyFadeTime);
					continue;
				}
				if( magic_track[j].Flag & _MT_FLAG_TEX_FADE )	//�� �ؽ��� ���̵����̴�.
				{
					SetFadeTex(magic_track[j].Color,magic_track[j].TexFadeTime,GetFirstTexureFromEffectEntityList(magic_track[j].EntityID));
					continue;
				}
				if( magic_track[j].Flag & _MT_FLAG_CHECK_DAMAGE )	//�� ������ Ÿ�̹��̴�.
				{
					PushEffectQueueDamage(&stMagicList[i].mEInfo);	//��ȯ�� ����Ʈ ť�� �ִ´�.
				}

				start_frame = stMagicList[i].mFrame-magic_track[j].Time*30.0f;
				float mat[4][4];
				MatrixRotate(mat,0,magic_track[j].Yangle,0); //������ 360��
				mat[3][0] = magic_track[j].Pos[0];
				mat[3][1] = magic_track[j].Pos[1];
				mat[3][2] = magic_track[j].Pos[2];
				MatrixMultiply(mat,stMagicList[i].mMat,mat);

				if( magic_track[j].Flag & _MT_FLAG_ENTITY_FADE )	//�� ��ƼƼ ���̵����� �׻� �ڱ�ȭ�鿡�� �Ĵٺ���.
				{
					//SetFadeTex(magic_track[j].Color,magic_track[j].TexFadeTime,EFUtil_GetFirstTexure(magic_track[j].EntityID));
					MatrixIdentity(mat);
					_R3ENGINE_STATE *state= GetR3State();

					mat[3][0] = state->mMatView.m[3][0];
					mat[3][1] = state->mMatView.m[3][1];
					mat[3][1] = state->mMatView.m[3][2];
					MatrixCopy(mat,state->mInvMatView.m);
				}

				StartEffectTrack(mat,&magic_track[j],stMagicList[i].mState,start_frame
					,stMagicList[i].mEntityUniqueID[j]);
				if( stMagicList[i].mSpecialID )
				{
					stEffectList[stELcnt-1].mSpecialID = stMagicList[i].mSpecialID;
					Vector3fCopy(stMagicList[i].mVectorArgv1,stEffectList[stELcnt-1].mVectorArgv1);
					Vector3fCopy(stMagicList[i].mVectorArgv2,stEffectList[stELcnt-1].mVectorArgv2);
				}
			}
		}
		else
		if( ret == -2)
		{
			if( stMagicList[i].mState == _EF_STATE_AUTO_DISAPEAR) //�ڵ����� ������� ��츸.
			{
				if( IsAllDisapear(&stMagicList[i]) )	//���δ� ��������� 
				{
					//����Ʈ���� ������ ������.
					//�̰��� ������ ������ ������ ����Ǵ� ����Ʈ�� �ڱ��ڽ��� �������� �����Ƿ� �������.
					DeleteMagicList(i);
					i--;	//��ĭ�� �и��Ƿ�...
				}
			}
			else
			if( stMagicList[i].mState == _EF_STATE_START_DISAPEAR) //�������� ��������մ°Ŵ�.
			{
				if( IsAllDisapear(&stMagicList[i]) )	//���δ� ��������� 
				{
					//���� ����Ʈ���� ������ ������.
					DeleteMagicList(i);
					i--;	//��ĭ�� �и��Ƿ�...
				}
			}
		}
	}
}

void DestroyEffectsCache()		//ĳ������ ��� ����Ʈ���� ������ ��Ų��. ���ο� �����ε��� �ʿ�.
{
	DWORD i;

	for( i=0; i<stELcnt; i++ )
	{
		DeleteEffectList(i);
		i--;	//��ĭ�� �и��Ƿ�...
	}
	for( i=0; i<stMLcnt; i++ )
	{
		DeleteMagicList(i);
		i--;	//��ĭ�� �и��Ƿ�...
	}
	stELcnt=0;
	stMLcnt=0;
}

void FrameMoveAllEffectsList()
{
	FrameMoveMagicEffects();
	FrameMoveEffectsList();
}


void RenderCommonEffect(DWORD i)
{
	D3DXMATRIX matWorld,matRot;
	float f_alpha,temp_f;
	DWORD alpha,color;

	D3DXMatrixIdentity( &matWorld );
	matWorld.m[0][0]*=stEffectList[i].mScale;
	matWorld.m[1][1]*=stEffectList[i].mScale;
	matWorld.m[2][2]*=stEffectList[i].mScale;
/*
	MatrixRotate(matRot.m,0,stEffectList[i].mYAngle,0); //������ 360��
	matWorld=matWorld*matRot;
	matWorld.m[3][0]=stEffectList[i].mPos[0];
	matWorld.m[3][1]=stEffectList[i].mPos[1];
	matWorld.m[3][2]=stEffectList[i].mPos[2];
*/		

	if( stEffectList[i].mFlag &	_MT_FLAG_ENTITY_FADE )
	{
		MatrixCopy(matRot.m,GetR3State()->mInvMatView.m);
		matRot.m[3][0]=0;
		matRot.m[3][1]=0;
		matRot.m[3][2]=0;
		matWorld=matWorld*GetR3State()->mInvMatView;
	}
	else
	{
		MatrixCopy(matRot.m,stEffectList[i].mMat);
		matRot.m[3][0]=0;
		matRot.m[3][1]=0;
		matRot.m[3][2]=0;

		if( !(stEffectList[i].mFlag & _MT_FLAG_BILLBOARD) )	//�������� ���.
			matWorld=matWorld*(*(D3DXMATRIX*)stEffectList[i].mMat);
		else
		{
			matWorld.m[3][0]=stEffectList[i].mMat[3][0];
			matWorld.m[3][1]=stEffectList[i].mMat[3][1];
			matWorld.m[3][2]=stEffectList[i].mMat[3][2];
		}
	}
/*
	//-----------����� �ڵ� 
	if( stEffectList[i].mFlag == 0 && stEffectList[i].mEntity->mObjectNum > 256)
	{			
		stELcnt--;
		continue;
	}
*/
	//---------------
	if( stEffectList[i].mFlag & _MT_FLAG_PARTICLE )//��ƼŬ�ΰ��.
	{
 		CParticle *particle = stEffectList[i].mParticle;
		color = 0xff000000|(stEffectList[i].mColor&0x00ffffff);
		MatrixCopy(particle->mRotMat,matRot.m);
		DrawCParticle(particle,particle->mEntity,matWorld.m,color);	//��ƼŬ�ϰ�� ��ƼŬ�� �ѷ�����.			
	}
	else	//��ƼƼ�� ���.
	{
		if( stEffectList[i].mEntity->mObject->frames !=0 )
		{
			f_alpha = 1-stEffectList[i].mFrame/(float)stEffectList[i].mEntity->mObject->frames;
			if( stEffectList[i].mAlphaDisapear != 1 )
				f_alpha=(f_alpha)/(1-stEffectList[i].mAlphaDisapear);
			else
				f_alpha=1;
			f_alpha=min(f_alpha,1);

			if( stEffectList[i].mAlphaApear != 0 )
			{
				temp_f = stEffectList[i].mFrame/(float)stEffectList[i].mEntity->mObject->frames;
				temp_f=(temp_f)/(stEffectList[i].mAlphaApear);
				f_alpha=min(f_alpha,temp_f);
			}
		}
		else
			f_alpha=1;

		if( stEffectList[i].mState == _EF_STATE_ALWAYS_LIVE )
			f_alpha=1;
		else
		if( stEffectList[i].mState == _EF_STATE_START_DISAPEAR ) //������� �����ϴ� �ð�.
		{
			f_alpha = 1-(stEffectList[i].mFrame-stEffectList[i].mDisapearFrame)/(stEffectList[i].mDisapearTime*30);
			f_alpha=min(f_alpha,1);
		}
		f_alpha = f_alpha*((stEffectList[i].mColor>>24)/255.0f);	//������ ���ĸ� ���.
		alpha = ((DWORD)(f_alpha*255))<<24;
		color = alpha|(stEffectList[i].mColor&0x00ffffff);
		if( stEffectList[i].mEntity->DrawEntity(matWorld.m,color,stEffectList[i].mFrame) == FALSE )
		{	//��ƼŬ�� ������ ���.
			DeleteEffectList(i);
			i--;	//��ĭ�� �и��Ƿ�...
		}
	}
}

void RenderAllEffectsList()
{
	DWORD i;

	_R3ENGINE_STATE *r3_state=GetR3State();

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity( &matWorld );
	GetD3dDevice()->SetTransform( D3DTS_WORLD, &matWorld );


	if( r3_state->mIsFog )
		GetD3dDevice()->SetRenderState(D3DRS_FOGENABLE,FALSE);

//---����Ʈ ����Ʈ.....
	for( i=0; i<stELcnt; i++ )//���� �ƴѰź��� �Ѹ���.
	{
		if( stEffectList[i].mState == _EF_STATE_DEATH )//��� �׿����´�.
			continue;
		if( stEffectList[i].mFlag &  _MT_FLAG_PARTICLE)	//��ƼŬ�� ���߿�
			continue;
		if( stEffectList[i].mEntity->IsFirstAlpha() )	//���Ĵ� ���߿�
			continue;
		if( stEffectList[i].mFlag &	_MT_FLAG_DYNAMIC_LIGHT )
			continue;
		if( stEffectList[i].mFlag &	_MT_FLAG_ENTITY_FADE )
			continue;
		RenderCommonEffect(i);	//�������ɱ�.
	}

	for( i=0; i<stELcnt; i++ )	//������ ���ĸ� �Ѹ���.
	{
		if( stEffectList[i].mState == _EF_STATE_DEATH )//��� �׿����´�.
			continue;
		if( !(stEffectList[i].mFlag &  _MT_FLAG_PARTICLE) )	//��ƼƼ �ΰ���.
		{
			if( !stEffectList[i].mEntity->IsFirstAlpha() )	//���� �ƴѰŴ� ������ ������ �Ѹ��ʿ䰡����.
				continue;
		}
		if( stEffectList[i].mFlag &	_MT_FLAG_DYNAMIC_LIGHT )
			continue;
		if( stEffectList[i].mFlag &	_MT_FLAG_ENTITY_FADE )
			continue;
		RenderCommonEffect(i);	//�������ɱ�.
	}

	//��ƼƼ ���̵������Ѹ���.
	for( i=0; i<stELcnt; i++ )	//������ ���ĸ� �Ѹ���.
	{
		if( stEffectList[i].mFlag &	_MT_FLAG_ENTITY_FADE )
		{
			RenderCommonEffect(i);	//�������ɱ�.
		}
	}

/*
		D3DXMatrixIdentity( &matWorld );
		matWorld.m[0][0]*=stEffectList[i].mScale;
		matWorld.m[1][1]*=stEffectList[i].mScale;
		matWorld.m[2][2]*=stEffectList[i].mScale;

		MatrixCopy(matRot.m,stEffectList[i].mMat);
		matRot.m[3][0]=0;
		matRot.m[3][1]=0;
		matRot.m[3][2]=0;
		matWorld=matWorld*(*(D3DXMATRIX*)stEffectList[i].mMat);

		if( stEffectList[i].mFlag & _MT_FLAG_PARTICLE )//��ƼŬ�ΰ��.
		{
 			CParticle *particle = stEffectList[i].mParticle;
			color = 0xff000000|(stEffectList[i].mColor&0x00ffffff);
			MatrixCopy(particle->mRotMat,matRot.m);
			DrawCParticle(particle,particle->mEntity,matWorld.m,color);	//��ƼŬ�ϰ�� ��ƼŬ�� �ѷ�����.			
		}
		else	//��ƼƼ�� ���.
		{
			if( stEffectList[i].mEntity->mObject->frames !=0 )
			{
				f_alpha = 1-stEffectList[i].mFrame/(float)stEffectList[i].mEntity->mObject->frames;
				f_alpha=(f_alpha)/(1-stEffectList[i].mAlphaDisapear);
				f_alpha=min(f_alpha,1);
			}
			else
				f_alpha=1;

			if( stEffectList[i].mState == _EF_STATE_ALWAYS_LIVE )
				f_alpha=1;
			else
			if( stEffectList[i].mState == _EF_STATE_START_DISAPEAR ) //������� �����ϴ� �ð�.
			{
				f_alpha = 1-(stEffectList[i].mFrame-stEffectList[i].mDisapearFrame)/(stEffectList[i].mDisapearTime*30);
				f_alpha=min(f_alpha,1);
			}
			f_alpha = f_alpha*((stEffectList[i].mColor>>24)/255.0f);	//������ ���ĸ� ���.
			alpha = ((DWORD)(f_alpha*255))<<24;
			color = alpha|(stEffectList[i].mColor&0x00ffffff);
			if( stEffectList[i].mEntity->DrawEntity(matWorld.m,color,stEffectList[i].mFrame) == FALSE )
			{	//��ƼŬ�� ������ ���.
				DeleteEffectList(i);
				i--;	//��ĭ�� �и��Ƿ�...
			}
		}
*/
	if( r3_state->mIsFog )
		GetD3dDevice()->SetRenderState( D3DRS_FOGENABLE, TRUE );
}

