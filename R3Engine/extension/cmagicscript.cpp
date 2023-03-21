#include "CMagicScript.h"
#include "Core.h"
#include "EffectEntityList.h"

float GetRandNum(float a,float b)
{
	LONG d_a= (LONG)(a*32767);
	LONG d_b= (LONG)(b*32767);
	if( d_b-d_a == 0 )
		return a;
	DWORD count=GetTickCount();
	LONG gop = (rand()*count)%(d_b-d_a)+d_a;
	return gop/32767.0f;
}
static DWORD GetTokenFloat(char *ch,float *get_f)	//
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

static BOOL GetRandOrNum( FILE *fp,float *from,float *at )	//TRUE�� ����.
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
	return FALSE;
}

//EffectEntityList.spt
void CProcessMagic::CreateMagicTrack(char *name)
{
	FILE *fp;
	char r_name[256];
	_R3ENGINE_STATE *state = GetR3State();
	strcpy(r_name,state->EffectPath);	//����Ʈ �н� ������...
	strcat(r_name,name);

	if(( fp =fopen(r_name,"rt"))==NULL) 
	{
		Warning(r_name,"<-�� �����̾����ϴ�.");
		return;
	}

	char hole[256];

    while(1)	//�պκ� ��ŵ.
    {
		if( fscanf(fp,"%s",hole)==EOF)	
		{
			fclose(fp);
			return;
		}
        if (!strcmp(hole,"[MagicTrack]"))
		{
			break;
		}
	}  
	mIsLoaded=TRUE;
	mTrackNum=0;
	mSpeed = 100;
	DWORD now_track_id=0,i;
	mCreateType=0;				// �����߻� ����.
	mCreatePos[0]=0;
	mCreatePos[1]=0;
	mCreatePos[2]=0;
	float from,at;
    while(1)
    {
		if( fscanf(fp,"%s",hole)==EOF)	
			break;
		strlwr(hole);

		if( !strcmp(hole,"ctrl_0") || !strcmp(hole,"ctrl_1")|| !strcmp(hole,"ctrl_2"))
		{
			DWORD t_id=0;
			if( !strcmp(hole,"ctrl_0"))
				t_id = _MAX_MAGIC_TRACK-3;
			else
			if( !strcmp(hole,"ctrl_1"))
				t_id = _MAX_MAGIC_TRACK-2;
			else
				t_id = _MAX_MAGIC_TRACK-1;

			for(i=0; i<3; i++)
			{
				if( !GetRandOrNum(fp,&from,&at) )	//������ �ƴϸ�.
				{
					mMagicTrack[t_id].Rand1[i] = from;
					mMagicTrack[t_id].Rand2[i] = from;
				}
				else
				{
					mMagicTrack[t_id].Rand1[i] = from;
					mMagicTrack[t_id].Rand2[i] = at;
//					mMagicTrack[t_id].Pos[i] = GetRandNum(from,at);
				}
			}
		}

		if( !strcmp(hole,"time"))
		{
			fscanf(fp,"%s",hole);
			memset(&mMagicTrack[mTrackNum],0,sizeof(_MAGIC_TRACK));
			mMagicTrack[mTrackNum].Time=(float)atof(hole);
			now_track_id=mTrackNum;
			mMagicTrack[now_track_id].Scale=1;
			mMagicTrack[now_track_id].Color=0xffffffff;

			mTrackNum++;
			if( mTrackNum>= _MAX_MAGIC_TRACK)
			{
				Warning(name,"<- �ִ� Ʈ�������� �Ѿ����ϴ�.");
			}
		}
		if( !strcmp(hole,"billboard"))
		{
			mMagicTrack[now_track_id].Flag|=_MT_FLAG_BILLBOARD;
		}
		if( !strcmp(hole,"quake_time"))
		{
			fscanf(fp,"%s",hole);
			mMagicTrack[now_track_id].Flag|=_MT_FLAG_QUAKE;
			if( mMagicTrack[now_track_id].QuakeDensity == 0 )//���� ����
				mMagicTrack[now_track_id].QuakeDensity = 1;	//����Ʈ��
			mMagicTrack[now_track_id].QuakeTime=(float)atof(hole);
		}
		if( !strcmp(hole,"speed"))
		{
			fscanf(fp,"%s",hole);
			mSpeed = (float)atof(hole);
		}
		if( !strcmp(hole,"quake_density"))
		{
			fscanf(fp,"%s",hole);
			mMagicTrack[now_track_id].Flag|=_MT_FLAG_QUAKE;
			if( mMagicTrack[now_track_id].QuakeTime == 0 )	//���� �ð�.
				mMagicTrack[now_track_id].QuakeTime=0.3f;	//����Ʈ��
			mMagicTrack[now_track_id].QuakeDensity=(float)atof(hole);
		}
		
		if( !strcmp(hole,"sky_fade_color"))
		{
			mMagicTrack[now_track_id].Flag|=_MT_FLAG_SKY_FADE;
			DWORD r,g,b;
			fscanf(fp,"%s",hole);
			r=(DWORD)atoi(hole);
			fscanf(fp,"%s",hole);
			g=(DWORD)atoi(hole);
			fscanf(fp,"%s",hole);
			b=(DWORD)atoi(hole);
			mMagicTrack[now_track_id].Color=((0xff000000 & mMagicTrack[now_track_id].Color)|(r<<16)|(g<<8)|(b));
		}
		if( !strcmp(hole,"sky_fade_time"))
		{
			fscanf(fp,"%s",hole);
			mMagicTrack[now_track_id].Flag|=_MT_FLAG_SKY_FADE;
			mMagicTrack[now_track_id].SkyFadeTime=(float)atof(hole);
		}
		if( !strcmp(hole,"tex_fade_color"))
		{
			mMagicTrack[now_track_id].Flag|=_MT_FLAG_TEX_FADE;
			DWORD r,g,b;
			fscanf(fp,"%s",hole);
			r=(DWORD)atoi(hole);
			fscanf(fp,"%s",hole);
			g=(DWORD)atoi(hole);
			fscanf(fp,"%s",hole);
			b=(DWORD)atoi(hole);
			mMagicTrack[now_track_id].Color=((0xff000000 & mMagicTrack[now_track_id].Color)|(r<<16)|(g<<8)|(b));
		}
		if( !strcmp(hole,"tex_fade_time"))
		{
			fscanf(fp,"%s",hole);
			mMagicTrack[now_track_id].Flag|=_MT_FLAG_TEX_FADE;
			mMagicTrack[now_track_id].SkyFadeTime=(float)atof(hole);
		}
		if( !strcmp(hole,"entity_fade"))
		{
//			fscanf(fp,"%s",hole);
			mMagicTrack[now_track_id].Flag|=_MT_FLAG_ENTITY_FADE;
		}
		if( !strcmp(hole,"z_disable"))
			mMagicTrack[now_track_id].Flag|=_MT_FLAG_Z_DISABLE;
		
		if( !strcmp(hole,"check_damage"))
		{
//			fscanf(fp,"%s",hole);
			mMagicTrack[now_track_id].Flag|=_MT_FLAG_CHECK_DAMAGE;
		}
		if( !strcmp(hole,"create_type"))
		{
			fscanf(fp,"%s",hole);
			mCreateType=(BYTE)atoi(hole);
		}		
		if( !strcmp(hole,"create_pos"))
		{
			fscanf(fp,"%s",hole);
			mCreatePos[0]=(float)atof(hole);
			fscanf(fp,"%s",hole);
			mCreatePos[1]=(float)atof(hole);
			fscanf(fp,"%s",hole);
			mCreatePos[2]=(float)atof(hole);
		}

		//--------���̳��� ����Ʈ 
		if( !strcmp(hole,"dynamic_light"))
		{
			mMagicTrack[now_track_id].Flag|=_MT_FLAG_DYNAMIC_LIGHT;

//			if( (mMagicTrack[now_track_id].DynamicLight&0xff000000) == 0 )
//				mMagicTrack[now_track_id].DynamicLight|= (20<<24);	//����Ʈ�� ���� 40��..
/*
			DWORD r,g,b;
			fscanf(fp,"%s",hole);
			r=(DWORD)atoi(hole);
			fscanf(fp,"%s",hole);
			g=(DWORD)atoi(hole);
			fscanf(fp,"%s",hole);
			b=(DWORD)atoi(hole);
			mMagicTrack[now_track_id].Color=0xff000000|(r<<16)|(g<<8)|(b);
*/
		}
		if( !strcmp(hole,"magic_light"))
		{
			mMagicTrack[now_track_id].Flag|=_MT_FLAG_MAGIC_LIGHT;
			mMagicTrack[now_track_id].Flag|=_MT_FLAG_DYNAMIC_LIGHT;
		}
/*
		if( !strcmp(hole,"dynamic_light_radious"))
		{
			if( (mMagicTrack[now_track_id].DynamicLight&0x00ffffff) == 0 )
				mMagicTrack[now_track_id].DynamicLight|=0x00ffffff;	//����Ʈ�� �����...
 
			mMagicTrack[now_track_id].Flag|=_MT_FLAG_DYNAMIC_LIGHT;
			fscanf(fp,"%s",hole);
			LONG length=(DWORD)atof(hole)/2;	//���� 255*4�� �����ϱ����� �������̴ϱ�..
			length=min(255,length);
			mMagicTrack[now_track_id].DynamicLight&=0x00ffffff;
			mMagicTrack[now_track_id].DynamicLight|=(length<<24);
		}*/

		if( !strcmp(hole,"scale"))
		{
			fscanf(fp,"%s",hole);
			mMagicTrack[now_track_id].Scale=(float)atof(hole);
		}
		if( !strcmp(hole,"color"))
		{
			DWORD r,g,b;
			fscanf(fp,"%s",hole);
			r=(DWORD)atoi(hole);
			fscanf(fp,"%s",hole);
			g=(DWORD)atoi(hole);
			fscanf(fp,"%s",hole);
			b=(DWORD)atoi(hole);
			mMagicTrack[now_track_id].Color=((0xff000000 & mMagicTrack[now_track_id].Color)|(r<<16)|(g<<8)|(b));
		}
		if( !strcmp(hole,"alpha"))
		{
			DWORD a;
			fscanf(fp,"%s",hole);
			a=(DWORD)atoi(hole);
			mMagicTrack[now_track_id].Color=((0x00ffffff & mMagicTrack[now_track_id].Color)|(a<<24));
		}

		if( !strcmp(hole,"entity_id"))
		{
			fscanf(fp,"%s",hole);
			mMagicTrack[now_track_id].EntityID=(DWORD)atoi(hole);
			//���� ���̵�� �ٲ۴�.
			mMagicTrack[now_track_id].EntityID=GetRealID(mMagicTrack[now_track_id].EntityID);
			if( IsParticle(mMagicTrack[now_track_id].EntityID) )
				mMagicTrack[now_track_id].Flag|=_MT_FLAG_PARTICLE;

		}
		if( !strcmp(hole,"alpha_disapear"))
		{
			fscanf(fp,"%s",hole);
			mMagicTrack[now_track_id].AlphaDisapear=(float)atof(hole);
		}
		if( !strcmp(hole,"alpha_apear"))
		{
			fscanf(fp,"%s",hole);
			mMagicTrack[now_track_id].AlphaApear=(float)atof(hole);
		}
		if( !strcmp(hole,"y_angle"))
		{
//			fscanf(fp,"%s",hole);
//			mMagicTrack[now_track_id].Yangle=(float)atof(hole);
			float from,at;
			if( !GetRandOrNum(fp,&from,&at) )	//������ �ƴϸ�.
			{
				mMagicTrack[now_track_id].Yangle=from;
			}
			else
			{
				mMagicTrack[now_track_id].Flag|=_MT_FLAG_Y_RANDOM;
				mMagicTrack[now_track_id].Yrand[0]=(WORD)from;
				mMagicTrack[now_track_id].Yrand[1]=(WORD)at;
			}
		}
		if( !strcmp(hole,"pos"))
		{
			fscanf(fp,"%s",hole);
			if( !strcmp(hole,"rand_xz"))
			{
				fscanf(fp,"%s",hole);
				DWORD range=(DWORD)atof(hole);
				mMagicTrack[now_track_id].Pos[0]=(rand()%range)-range/2.0f;
				mMagicTrack[now_track_id].Pos[2]=(rand()%range)-range/2.0f;
			}
			else
			if( !strcmp(hole,"track"))
			{
				fscanf(fp,"%s",hole);
				DWORD tr_id=(DWORD)atoi(hole);
				mMagicTrack[now_track_id].Pos[0]=mMagicTrack[tr_id].Pos[0];
				mMagicTrack[now_track_id].Pos[1]=mMagicTrack[tr_id].Pos[1];
				mMagicTrack[now_track_id].Pos[2]=mMagicTrack[tr_id].Pos[2];
			}
			else
			if( !strcmp(hole,"xyz"))
			{
				fscanf(fp,"%s",hole);
				float gop=(float)atof(hole);
				mMagicTrack[now_track_id].Pos[0]=gop;
				fscanf(fp,"%s",hole);
				gop=(float)atof(hole);
				mMagicTrack[now_track_id].Pos[1]=gop;
				fscanf(fp,"%s",hole);
				gop=(float)atof(hole);
				mMagicTrack[now_track_id].Pos[2]=gop;
			}
		}
	}
	fclose(fp);
}
void CProcessMagic::DestoryMagicTrack()
{
	//��ƼŬ�� ���⼭ �����.
}

/*
LONG CProcessMagic::GetEntityIDs(float old_frame,float now_frame,float *get_start_frame,_MAGIC_TRACK **get_magic_track,)
{
	DWORD i;
	LONG cnt=0;

	now_frame /=30.0f;	//�������� Ÿ������ �ٲ۴�.
	old_frame /=30.0f;	//�������� Ÿ������ �ٲ۴�.

	for(i=0; i<mTrackNum; i++)
	{
		if( mMagicTrack[i].Time < now_frame
		&& mMagicTrack[i].Time >= old_frame )
		{
			if( cnt==0 )
				*get_magic_track = &mMagicTrack[i];
			get_start_frame[cnt] = (mMagicTrack[i].Time-now_frame)*30.0f;
			cnt++;
		}
	}
	if( cnt )	//������ �����ϸ�...
		return cnt;
	if( now_frame > mMagicTrack[mTrackNum-1].Time )
		return -2;

	return -1;
}
*/

//���ϰ��� -1�ΰ�� ������ ��ƼƼ���̵� ����. 
//-2�ΰ�� �������� Ʈ���� ��� ������ �����.�׿ܿ� ���ϰ��� �����Ѵ�. 
LONG CProcessMagic::GetEntityIDs(float old_frame,float now_frame,LONG *start_loop)
{
	DWORD i;
	LONG cnt=0;

	now_frame /=30.0f;	//�������� Ÿ������ �ٲ۴�.
	old_frame /=30.0f;	//�������� Ÿ������ �ٲ۴�.

	for(i=0; i<mTrackNum; i++)
	{
		if( mMagicTrack[i].Time < now_frame
		&& mMagicTrack[i].Time >= old_frame )
		{
			if( cnt==0 )
				*start_loop = (LONG)i;
			cnt++;
		}
	}
	if( cnt )	//������ �����ϸ�...
		return cnt;
	if( now_frame > mMagicTrack[mTrackNum-1].Time )
		return -2;

	return -1;
}

//------------------------------------- ��������Ʈ spt ����.

typedef struct{
	char name[90];
	DWORD id;
}_MAGIC_SPT_LIST;
static _MAGIC_SPT_LIST *stMagicSptList=NULL;
static int stMaxMagicSptNum;	//�ִ� ��ƼƼ ����.. ���� �ε��� ���� 
static int stMagicSptNum;	//��ƼƼ ���� 


DWORD GetMagicSptNum()
{
	return stMagicSptNum;
}

DWORD GetAllMemorySizeMagic()	//���� �뷫 ���Ǵ� ���� �޸� ��. 
{
	DWORD size=0;
	
	size += stMaxMagicSptNum*sizeof(_MAGIC_SPT_LIST);
	return size;
}

DWORD GetRealIDFromMagicSpt(DWORD id)	//�����Ⱦ��̵��� ���� �ε����� ����.
{
	for(int i=0; i<stMagicSptNum; i++ )
	{
		if( stMagicSptList[i].id == id )
			return i;
	}
	char buf[256];
	sprintf(buf,"0x%x",id);
	Warning(buf,"<- MagicEntityList�� �����ϴ�.");
	return -1;
}
BOOL IsExistFromMagicSpt(DWORD id)	//�����Ⱦ��̵��� ���� �ε����� ����.
{
	if( id == -1 )
		return FALSE;

	for(int i=0; i<stMagicSptNum; i++ )
	{
		if( stMagicSptList[i].id == id )
			return TRUE;
	}
	return FALSE;
}

DWORD GetRealIDFromMagicSptSkipWarning(DWORD id)	//�����Ⱦ��̵��� ���� �ε����� ����.
{
	for(int i=0; i<stMagicSptNum; i++ )
	{
		if( stMagicSptList[i].id == id )
			return i;
	}
	return 0;
}
DWORD GetIDFromRealID(DWORD id)		//���� �ε����� �����Ⱦ��̵� ����.
{
	if( (DWORD)stMagicSptNum <= id )
		return -1;
	return stMagicSptList[id].id;
}

DWORD GetIDFromMagicSpt(char *name)	//������ �����̸����� ���̵� ����.
{
	/*
	char r_name[256];
	_R3ENGINE_STATE *state = GetR3State();
	strcpy(r_name,state->EffectPath);
	strcat(r_name,name);
*/
	for(int i=0; i<stMagicSptNum; i++ )
	{
		if( !strcmp(stMagicSptList[i].name,name) )
			return stMagicSptList[i].id;
	}
	char buf[256];
	sprintf(buf,"%s",name);
	Warning(buf,"<-�� �̸��� MagicEntityList.spt�� �����ϴ�.");
	return 0;
}

char *GetFilenameFromMagicSpt(DWORD id)	//�����Ⱦ��̵��� ���� �̸��� ����.
{
	for(int i=0; i<stMagicSptNum; i++ )
	{
		if( stMagicSptList[i].id == id )
			return stMagicSptList[i].name;
	}
	char buf[256];
	sprintf(buf,"%d",id);
	Warning(buf,"<-�� ���̵� MagicEntityList.spt�� �����ϴ�.");
	return 0;
}

void ForToolUseSetMagicSptName(char *name)
{
	if( !stMagicSptList )
	{
		stMaxMagicSptNum=1;
		stMagicSptNum=1;
		stMagicSptList=(_MAGIC_SPT_LIST * )Dmalloc( sizeof(_MAGIC_SPT_LIST)*stMaxMagicSptNum );
	}
	stMagicSptList[0].id=0;
	strncpy(stMagicSptList[0].name,name,89);
}

static DWORD ConvertHexa(char *hole)
{
	int i;

	strlwr(hole);
	if( hole[0] != '0' || hole[1] != 'x' )
		return 0;
	DWORD su=0,temp,cnt=0;
	for(i=strlen(hole)-1; i>1; i--)
	{
		if( '0' <= hole[i] && hole[i] <= '9')
		{
			temp = hole[i]-'0';
		}
		else
		if( 'a' <= hole[i] && hole[i] <= 'f')
		{
			temp = hole[i]-'a'+10;
		}
		else
			return 0;
		su += (temp<<(cnt*4));
		cnt++;
	}
	return su;
}

void CreateMagicSptList(char *name)
{
	FILE *fp;

	if(( fp =fopen(name,"rt"))==NULL) 
		Error(name,"<-�� �����̾����ϴ�.");

	stMaxMagicSptNum=2048;
	stMagicSptList=(_MAGIC_SPT_LIST * )Dmalloc( sizeof(_MAGIC_SPT_LIST)*stMaxMagicSptNum );
	memset(stMagicSptList,0,sizeof(_MAGIC_SPT_LIST)*stMaxMagicSptNum );

	char hole[256];

    while(1)	//�պκ� ��ŵ.
    {
		if( fscanf(fp,"%s",hole)==EOF)	
			return;
        if (!strcmp(hole,"[MagcicSptList]"))
		{
			break;
		}
	}  

	stMagicSptNum=0;
    while(1)
    {
		if( fscanf(fp,"%s",hole)==EOF)	
			break;
		stMagicSptList[stMagicSptNum].id=atoi(hole);
		if( stMagicSptList[stMagicSptNum].id == 0)
		{
			stMagicSptList[stMagicSptNum].id=ConvertHexa(hole);
			if( stMagicSptList[stMagicSptNum].id == 0)	//������.
				continue;
		}
		fscanf(fp,"%s",hole);
		if( strlen(hole) > 89  )
		{
			Warning("hole","<-������ũ��Ʈ �н��� �ʹ���ϴ�.89���̳��� ���ּ���.");
			continue;
		}
		strncpy(stMagicSptList[stMagicSptNum].name,hole,89);
		stMagicSptNum++;
		if( stMagicSptNum >= stMaxMagicSptNum)	//�����.
		{
			stMagicSptList = (_MAGIC_SPT_LIST *)ReAlloc(stMagicSptList
				,sizeof(_MAGIC_SPT_LIST)*(stMaxMagicSptNum)
				,sizeof(_MAGIC_SPT_LIST)*(stMaxMagicSptNum+128));
			memset(stMagicSptList+stMaxMagicSptNum,0,sizeof(_MAGIC_SPT_LIST)*128 );
			stMaxMagicSptNum+=128;
		}
	}  
	fclose(fp);
}

void DestroyMagicSptList()
{
	if( stMagicSptList )
		Dfree(stMagicSptList);
	stMagicSptList=NULL;
	stMagicSptNum=0;
}
