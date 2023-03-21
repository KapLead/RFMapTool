//#include "..\\gaudio.h"
#include "jerror.h"
#include "CJCache.h"
#include "r3sound.h"
#include "core.h"
#include "Commonutil.h"


//#define NO_WAVE_PLAY

#ifdef	_SERVER_DEBUG_NO_SOUND	//���� ������� ��� ���Լ����� ȣ���Ѵ�.

BOOL InitR3SoundSystem(char *wav_spt_name){ return 1; }		//���̺꽺ũ��Ʈ �̸��� �Է��Ѵ�.
void ReleaseR3SoundSystem(){}
void LoadWaveList(char *name){}	//ȿ���� ���ø���Ʈ �⺻������ �����ʱ�ȭ�� ����� ���ش�....
void ReleaseWaveList(){}		//

BOOL SetGlobalWavVolume(float vol){ return 1; }	//0-1����..	���̺� ��ü����.
float GetGlobalWavVolume(){ return 1.0f; }	//0-1����..	���̺� ��ü����.
BOOL SetGlobalMusicVolume(float vol){ return 1; }	//0-1����..	���� ��ü����.
float GetGlobalMusicVolume(){	return 1;	}	//0-1����..	���� ��ü����.

//MP3 �÷��̿� ���Ѱ�.
BOOL LoadR3MP3(char *name){ return 1; }
void ReleaesR3MP3(){}
void PlayR3MP3(){}
void StopR3MP3(){}
BOOL SetVolumeMP3(float vol){ return 1; }	//0-1����..
void SetPlayWaveState( BOOL state ){}
void SetPlayMusicState( BOOL state ){}
void PlayWave(DWORD id,DWORD flag,float vol,float pan){}
void StopWave(DWORD id){}		//�����Ǿ� �÷������� ���̺긦 ������Ų��.
void SetWaveVolume(DWORD id,float vol,float pan){}	// vol 0 ~ 1���� pan -1 ~ 1����  -1�� ���ʿ�ġ��ħ.1�� �����ʿ� ġ��ħ.
void SetWaveVolAndPan(DWORD id,float vol,float pan){}
BOOL IsExistFromSoundSpt(DWORD id){return 1;}		//�����Ⱦ��̵� ��ũ��Ʈ�� �ִ°�?


BOOL LoadStreamR3MP3(char *name,DWORD loop_cnt){ return TRUE; }
void ReleaesStreamR3MP3(){}

void PlayStreamR3MP3(){}
void StopStreamR3MP3(){}
BOOL SetVolumeStreamMP3(float vol){ return TRUE; }

//-------------- ���̺� �÷��̰��Ѱ�. ���������� ����ϹǷ� �ε��� ������ ������Ѵ�.-------------//
DWORD IM_LoadWave(char *name,DWORD flag){ return 1; }	//�ε����� ���ϵȴ�.�ϳ��� �ε������� �ȿ��� ���̺� �����ڰ� ����ԵǹǷ� .. �������� IM_ReleaseAllWaves()ȣ�� �Ұ�.
void IM_ReleaseWave(DWORD id){}				// �����ϰ� ���̺� 
void IM_ReleaseAllWaves(){}					//���� ���� �ε��Ұ�쿡 ���Լ��� ����ϸ� �����ϰ� ������ȴ�.
void IM_PlayWave(DWORD id,float vol,float pan){}
void IM_StopWave(DWORD id){}		//�����Ǿ� �÷������� ���̺긦 ������Ų��.
void IM_SetWaveVolumeAndPan(DWORD id,float vol,float pan){}	//���� 0 ~ 1���� ���� -1�� ���ʿ�ġ��ħ.1�� �����ʿ� ġ��ħ.
void UpdateStreamMP3(){}  // mp3�� ������ �������� ���Լ��� 1�ʿ� 10���� ȣ���Ϸ���...�Ѥ�
void IM_SetLoopCntWave(DWORD id,DWORD loop_cnt){}

#else


#include "..\\MSS.H"


static CJcache stWaveCache;

void LoadWaveList(char *name);
void ReleaseWaveList();

HDIGDRIVER  stDigital = NULL;           // Digital playback device

#define DIGITALRATE		44100
#define DIGITALBITS		16		//
#define DIGITALCHANNELS	2	//���׷�������.

#define _SOUND_CACHE_NUM	128
#define _MAX_PALY_CHANNELS 10

static BOOL stIsInit=FALSE;
//static BOOL GetR3State()->mSoundOn=TRUE;
//static BOOL GetR3State()->mMusicOn=TRUE;



static float stGlobalWavVolume=1.0f;	//���̺� ��ü ������ ����Ѵ�.
static float stGlobalMusicVolume=1.0f;	//���̺� ��ü ������ ����Ѵ�.

BOOL SetGlobalWavVolume(float vol)	//0-1����..	���̺� ��ü����.
{
	stGlobalWavVolume=vol;
	return 1; 
}
float GetGlobalWavVolume()	//0-1����..	���̺� ��ü����.
{
	return stGlobalWavVolume;
}
BOOL SetGlobalMusicVolume(float vol)	//0-1����..	���� ��ü����.
{
	stGlobalMusicVolume=vol;
	return 1; 
}
float GetGlobalMusicVolume()	//0-1����..	���� ��ü����.
{
	return stGlobalMusicVolume;
}


void SetPlayMusicState( BOOL state )
{
//	=state;
}

void SetPlayWaveState( BOOL state )
{
//	GetR3State()->mSoundOn=state;
}

//���� ���̺� ������...
typedef struct{
	U32 *ptr;	//�޸� ������.
	HSAMPLE Handle;
	DWORD id;
}_WAVE_PTR_LIST;

BOOL InitR3SoundSystem(char *wav_spt_name)
{
	AIL_startup();

	AIL_set_preference(DIG_MIXER_CHANNELS,_SOUND_CACHE_NUM+16);

#ifndef	NO_WAVE_PLAY
	LoadWaveList(wav_spt_name);
#endif

	stDigital = AIL_open_digital_driver( DIGITALRATE, DIGITALBITS, DIGITALCHANNELS, 0 );
	if( !stDigital )
		return FALSE;

	stIsInit=TRUE;

	return TRUE;
}

//HSAMPLE stMp3Handle=0;
CPlayMP3::CPlayMP3()
{
	m_Mp3Handle=0;
}
CPlayMP3::~CPlayMP3()
{
	if( m_Mp3Handle )
		ReleaesR3MP3();
}
BOOL CPlayMP3::LoadR3MP3(char *name)
{
	if( stIsInit ==0 )	//���� �ʱ�ȭ�� ���ߵ�.
		return TRUE;	
	if( !GetR3State()->mMusicOn )
		return TRUE;

	U32        *ptr;
    ptr = (U32*)AIL_file_read( name, FILE_READ_WITH_SIZE );
	m_ptr = ptr;

	if (ptr == NULL)
	{
		Warning( name, "<- File not found\n" );
		m_Mp3Handle = 0;
		return FALSE;
	}

	// allocate an HSAMPLE to use
	m_Mp3Handle = AIL_allocate_sample_handle( stDigital );
	if (m_Mp3Handle == NULL)
	{
		AIL_mem_free_lock(ptr);
		m_ptr=NULL;
		Warning( AIL_last_error(),"" );
		return FALSE;
	}

	// initialize the sample
	AIL_init_sample( (HSAMPLE)m_Mp3Handle );

	// point the sample at the loaded file image (the first dword is the size)
	if (AIL_set_named_sample_file( (HSAMPLE)m_Mp3Handle, name, ptr+1, ptr[0], -1 ) == 0)
	{
		AIL_mem_free_lock(ptr);
		m_ptr=NULL;
		Warning( AIL_last_error(),"" );
		return FALSE;
	}

	AIL_set_sample_loop_count((HSAMPLE)m_Mp3Handle,0);

	return TRUE;
}

BOOL CPlayMP3::SetVolumeMP3(float vol)	//0-1����..
{
	if( stIsInit ==0 )	//���� �ʱ�ȭ�� ���ߵ�.
		return TRUE;	
	if( !GetR3State()->mMusicOn )
		return TRUE;
	if( !m_Mp3Handle )
		return FALSE;

	AIL_set_sample_volume_pan( (HSAMPLE)m_Mp3Handle,vol*stGlobalMusicVolume,0.5f );
	return TRUE;
}

void CPlayMP3::ReleaesR3MP3()
{
	if( stIsInit ==0 )	//���� �ʱ�ȭ�� ���ߵ�.
		return;	
	if( !GetR3State()->mMusicOn )
		return;
	if( m_Mp3Handle )
	{
		AIL_release_sample_handle( (HSAMPLE)m_Mp3Handle );
		m_Mp3Handle=0;
	}
	if( m_ptr )
	{
		AIL_mem_free_lock(m_ptr);
		m_ptr=NULL;
	}
}
void CPlayMP3::PlayR3MP3()
{
	if( stIsInit ==0 )	//���� �ʱ�ȭ�� ���ߵ�.
		return;	
	if( !GetR3State()->mMusicOn )
		return;
	if( !m_Mp3Handle )
		return;
    AIL_start_sample( (HSAMPLE)m_Mp3Handle );
	SetVolumeMP3(1.0f);	//0-1����..
}
void CPlayMP3::StopR3MP3()
{
	if( stIsInit ==0 )	//���� �ʱ�ȭ�� ���ߵ�.
		return;	
	if( !GetR3State()->mMusicOn )
		return;
	if( !m_Mp3Handle )
		return;

    AIL_stop_sample( (HSAMPLE)m_Mp3Handle );
}

void ReleaseR3SoundSystem()
{
	if( stIsInit ==0 )	//���� �ʱ�ȭ�� ���ߵ�.
		return;	
#ifndef	NO_WAVE_PLAY
	ReleaseWaveList();
#endif

	if (stDigital)
	{
		AIL_close_digital_driver( stDigital );
	}
	AIL_shutdown();
	stIsInit=FALSE;
}



HSTREAM  stStream=NULL;
S32 stStreamSize;

BOOL LoadStreamR3MP3(char *name,DWORD loop_cnt)
{
	if( stIsInit ==0 )	//���� �ʱ�ȭ�� ���ߵ�.
		return TRUE;	
	if( !GetR3State()->mMusicOn )
		return TRUE;
	if( stStream )
		ReleaesStreamR3MP3();

	stStream = AIL_open_stream(stDigital, name, 0  );

	// loop the stream forever
	AIL_set_stream_loop_count( stStream, loop_cnt );

	// get the info about the file
	//

	if (stStream == 0)
	{
		Warning(name," Could not open file \n");
		return FALSE;
	}
	S32 mem,rate;

	AIL_stream_info( stStream, &rate, 0, &stStreamSize, &mem );
	return TRUE;
}


void ReleaesStreamR3MP3()
{
	if( stIsInit ==0 )	//���� �ʱ�ȭ�� ���ߵ�.
		return;	
	AIL_close_stream( stStream );
	stStream=NULL;
}

void PlayStreamR3MP3()
{
	if( stIsInit ==0 )	//���� �ʱ�ȭ�� ���ߵ�.
		return;	
	AIL_start_stream( stStream );
	SetVolumeStreamMP3(1.0f);	//0-1����..
}

void StopStreamR3MP3()
{
	if( stIsInit ==0 )	//���� �ʱ�ȭ�� ���ߵ�.
		return;	
	AIL_pause_stream(stStream,1);
}

BOOL SetVolumeStreamMP3(float vol)	//0-1����..
{
	if( stIsInit ==0 )	//���� �ʱ�ȭ�� ���ߵ�.
		return TRUE;	
	AIL_set_stream_volume_pan( stStream,vol*stGlobalMusicVolume,0.5f );
	return TRUE;
}


void UpdateStreamMP3()  // mp3�� ������ �������� ���Լ��� 1�ʿ� 10���� ȣ���Ϸ���...�Ѥ�
{
	if( stIsInit ==0 )	//���� �ʱ�ȭ�� ���ߵ�.
		return;	
	if(!stStream)
		return;

    AIL_service_stream( stStream, 0 );
}

//���� %��ġ ����..
BOOL IsEndStreamMP3()
{
	if( stIsInit ==0 )	//���� �ʱ�ȭ�� ���ߵ�.
		return FALSE;	
	if(!stStream)
		return FALSE;

	if( AIL_stream_status(stStream) == SMP_DONE )
		return TRUE;
	return FALSE;
/*  
	S32 per = AIL_stream_position( stStream );
	per = (per*100)/stStreamSize ;
	if(per >= 100)
		return TRUE;
	return FALSE;*/
}

//-------------------------------------ȿ���� ����...-------------------------------------------------
typedef struct{
	char name[59];
	BYTE flag;
	DWORD id;
//	WAVstream	*WAV;
}_WAVE_LIST;


static _WAVE_LIST *stWaveList=NULL;
static int stMaxWaveNum;	//�ִ� ��ƼƼ ����.. ���� �ε��� ���� 
static int stWaveNum;	//��ƼƼ ���� 

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


void LoadWaveList(char *name)
{
	if( name==NULL )
		return;

//	if( stIsInit ==0 )	//���� �ʱ�ȭ�� ���ߵ�.
//		return;	
	FILE *fp;
	if(( fp =fopen(name,"rt"))==NULL) 
		Error(name,"<-�� �����̾����ϴ�.");

	stMaxWaveNum=256;
	stWaveList=(_WAVE_LIST * )Dmalloc( sizeof(_WAVE_LIST)*stMaxWaveNum );
	memset(stWaveList,0,sizeof(_WAVE_LIST)*stMaxWaveNum );
	stWaveCache.CreateJcache(_SOUND_CACHE_NUM,sizeof(_WAVE_PTR_LIST));

	char hole[256];

    while(1)	//�պκ� ��ŵ.
    {
		if( fscanf(fp,"%s",hole)==EOF)	
		{
			fclose(fp);
			return;
		}
        if (!strcmp(hole,"[WaveList]"))
		{
			break;
		}
	}

	stWaveNum=0;
    while(1)
    {
		if( fscanf(fp,"%s",hole)==EOF)	
			break;
CONTINUE_PROSS:
		int id = atoi(hole);
		if( id == 0)
		{
			id = ConvertHexa(hole);
			if( id==0 )
				continue;
		}
		stWaveList[stWaveNum].id=id;
		fscanf(fp,"%s",hole);
		strlwr(hole);
		if( strlen(hole) > 58  )
		{
			Warning(hole,"<-���� �н��� �ʹ���ϴ�.58���̳��� ���ּ���.");
			continue;
		}
		strcpy(stWaveList[stWaveNum].name,hole);
		stWaveNum++;
		if( stWaveNum >= stMaxWaveNum)	//�����.
		{
			stWaveList = (_WAVE_LIST *)ReAlloc(stWaveList
				,sizeof(_WAVE_LIST)*(stMaxWaveNum)
				,sizeof(_WAVE_LIST)*(stMaxWaveNum+64));
			memset(stWaveList+stMaxWaveNum,0,sizeof(_WAVE_LIST)*64 );
			stMaxWaveNum+=64;
		}

		if( fscanf(fp,"%s",hole)==EOF)	
			break;
		strlwr(hole);
        if (!strcmp(hole,"loop"))
			stWaveList[stWaveNum-1].flag = _PLAY_WAVE_LOOP;
		else
			goto CONTINUE_PROSS;
	}
	fclose(fp);
}

void ReleaseWaveList()
{
	DWORD i;

	if( stIsInit ==0 )	//���� �ʱ�ȭ�� ���ߵ�.
	{
		if( stWaveList )
			Dfree(stWaveList);
		stWaveList=NULL;
		stWaveNum=0;
		return;	
	}
	for(i=0; i<stWaveNum; i++)	//ĳ���� ȿ�������� �����.
	{
		_WAVE_PTR_LIST *get_wav;
		if( stWaveCache.GetCache(stWaveList[i].id,(void**)&get_wav) )	//ĳ���� ���۸� ��´�. ������ FALSE�� ����
		{
			if( get_wav->Handle )
			{
				AIL_release_sample_handle( get_wav->Handle );
				get_wav->Handle=NULL;
			}
			if( get_wav->ptr )
			{
				AIL_mem_free_lock((void*)get_wav->ptr);
				get_wav->ptr=NULL;
			}
		}
	}
	stWaveCache.DestroyJcache();

	if( stWaveList )
		Dfree(stWaveList);
	stWaveList=NULL;
	stWaveNum=0;
}

static DWORD GetRealID(int id,BOOL no_warning=TRUE)
{
	DWORD i;
	for(i=0; i<stWaveNum; i++)
	{
		if(stWaveList[i].id == id )
			return i;
	}
	if( no_warning )
	{
		char buf[256];
		if( id & 0xf0000000 )
			sprintf(buf,"0x%x",id);
		else
			sprintf(buf,"%d",id);
		Warning(buf,"<-ȿ���� ��ũ��Ʈ���� �����ϰ����ϴ� �� ���̵� ���׿�.");
	}
	return -1;
}

static BOOL LoadWave(char *name,DWORD flag, HSAMPLE *Handle,U32 **mem_ptr )
{
	U32        *ptr;

	if( stIsInit ==0 )	//���� �ʱ�ȭ�� ���ߵ�.
		return FALSE;	
    ptr = (U32*)AIL_file_read( name, FILE_READ_WITH_SIZE );
	*mem_ptr = ptr;

	if (ptr == NULL)
	{
		Warning( name, "<- File not found\n" );
		*Handle = 0;
		return FALSE;
	}

	//
	// allocate an HSAMPLE to use
	//

	*Handle = AIL_allocate_sample_handle( stDigital );

	if (*Handle == NULL)
	{
		if( ptr )
		{
			AIL_mem_free_lock(ptr);
			*mem_ptr=NULL;
		}
		Warning( name,AIL_last_error() );
		return FALSE;
	}

	//
	// initialize the sample
	//

	AIL_init_sample( *Handle );

	//
	// point the sample at the loaded file image (the first dword is the size)
	//

	if (AIL_set_named_sample_file( *Handle, name, ptr+1, ptr[0], -1 ) == 0)
	{
		if( ptr )
		{
			AIL_mem_free_lock(ptr);
			*mem_ptr=NULL;
		}
		Warning( name,AIL_last_error() );
		return FALSE;
	}
	if( flag == _PLAY_WAVE_LOOP )
		AIL_set_sample_loop_count(*Handle,0);

	return TRUE;
}

BOOL IsExistFromSoundSpt(DWORD id)		//�����Ⱦ��̵� ��ũ��Ʈ�� �ִ°�?
{
	DWORD real_id = GetRealID(id,FALSE);
	if( real_id == -1 )
		return FALSE;
	return TRUE;
}

void PlayWave(DWORD id,DWORD flag,float vol,float pan)
{
	if( !GetR3State()->mSoundOn )
		return;
	if( stIsInit ==NULL )	//���� �ʱ�ȭ�� ���ߵ�.
		return;	

#ifdef	NO_WAVE_PLAY
	return;
#endif
	if( AIL_active_sample_count(stDigital) >= _MAX_PALY_CHANNELS )
		return;


	_WAVE_PTR_LIST *get_wav;
	DWORD real_id = GetRealID(id);
	if( real_id == -1 )
		return;
	char *name;

	//ĳ������ �ܾ� �´�.������ ���θ��� ����...
	LONG result = stWaveCache.HitCache(id,(void**)&get_wav);
	if( result == _HITCACHE_RET_NEW )
	{
		name = stWaveList[real_id].name;
		if( !IsExistFile(name))
		{
			Warning(name,"�� ������ ����. �ٿ��� �Ƚ�Ű����..");
			return;
		}
		stWaveList[real_id].flag =flag;
		LoadWave(name,stWaveList[real_id].flag,&get_wav->Handle,&get_wav->ptr);
	}
	else
	if( result == _HITCACHE_RET_DEL_AND_NEW )	//������ �ٽþ���.
	{
		name = stWaveList[real_id].name;
		if( !IsExistFile(name))
		{
			Warning(name,"�� ȿ������ ����. �ٿ��� �Ƚ�Ű����..");
			return;
		}
		AIL_release_sample_handle( get_wav->Handle );
		stWaveList[real_id].flag =flag;
		LoadWave(name,stWaveList[real_id].flag,&get_wav->Handle,&get_wav->ptr);
	}
	else// _HITCACHE_RET_OLD ������ �����Ƿ� �ٷ� ����.
	{
	}
	if( get_wav->Handle )
	{
		get_wav->id = id;
		AIL_set_sample_volume_pan(get_wav->Handle,vol*stGlobalWavVolume,(pan+1)*0.5f);
	    AIL_start_sample( get_wav->Handle );
	}
}

void SetWaveVolAndPan(DWORD id,float vol,float pan)
{
	if( stIsInit ==NULL )	//���� �ʱ�ȭ�� ���ߵ�.
		return;	
#ifdef	NO_WAVE_PLAY
	return;
#endif
	_WAVE_PTR_LIST *get_wav;

	if( stWaveCache.GetCache(id,(void**)&get_wav) )
		AIL_set_sample_volume_pan(get_wav->Handle,vol*stGlobalWavVolume,(pan+1)*0.5f);
}

void StopWave(DWORD id)		//�����Ǿ� �÷������� ���̺긦 ������Ų��.
{
	if( stIsInit ==NULL )	//���� �ʱ�ȭ�� ���ߵ�.
		return;	
#ifdef	NO_WAVE_PLAY
	return;
#endif
	_WAVE_PTR_LIST *get_wav;

	if( stWaveCache.GetCache(id,(void**)&get_wav) )
	{
		if( get_wav->Handle )
			AIL_stop_sample( get_wav->Handle );
	}
}

//-----------------------------IM��� API ---------------------------------------------//



typedef struct{
	BYTE flag;
	HSAMPLE Handle;
	U32 *ptr;
	DWORD mLoopCnt;
}_IM_WAVE_LIST;


static _IM_WAVE_LIST *stIM_WaveList=NULL;
static int stIM_MaxWaveNum=0;	//�ִ� ��ƼƼ ����.. ���� �ε��� ���� 
static int stIM_WaveNum=0;	//��ƼƼ ���� 

DWORD IM_LoadWave(char *name,DWORD flag)	//�ε����� ���ϵȴ�.�ϳ��� �ε������� �ȿ��� ���̺� �����ڰ� ����ԵǹǷ� .. �������� IM_ReleaseAllWaves()ȣ�� �Ұ�.
{
	int i;
	DWORD slut_id=0;

	if( stIsInit ==NULL )	//���� �ʱ�ȭ�� ���ߵ�.
		return 0;	
	if( stIM_MaxWaveNum == 0 )	//ó�� �ϰ��..
	{
		stIM_MaxWaveNum=128;
		stIM_WaveList=(_IM_WAVE_LIST * )Dmalloc( sizeof(_IM_WAVE_LIST)*stIM_MaxWaveNum );
		memset(stIM_WaveList,0,sizeof(_IM_WAVE_LIST)*stIM_MaxWaveNum );
	}

	if( !IsExistFile(name))
		return 0;

	for( i=0; i<stIM_WaveNum; i++)
	{
		if( stIM_WaveList[i].Handle == NULL )
			break;
	}
	if( stIM_WaveNum == i )
		slut_id = stIM_WaveNum;
	else
		slut_id = i;

	if( flag == _PLAY_WAVE_LOOP )
		stIM_WaveList[slut_id].mLoopCnt = 0;
	else
		stIM_WaveList[slut_id].mLoopCnt = 1;
	LoadWave(name,flag, &stIM_WaveList[slut_id].Handle, &stIM_WaveList[slut_id].ptr);

	if( stIM_WaveNum == i )
	{
		stIM_WaveNum++;
		if( stIM_WaveNum >= stIM_MaxWaveNum)	//�����.
		{
			stIM_WaveList = (_IM_WAVE_LIST *)ReAlloc(stIM_WaveList
				,sizeof(_IM_WAVE_LIST)*(stIM_MaxWaveNum)
				,sizeof(_IM_WAVE_LIST)*(stIM_MaxWaveNum+32));
			memset(stIM_WaveList+stIM_MaxWaveNum,0,sizeof(_IM_WAVE_LIST)*32 );
			stIM_MaxWaveNum+=32;
		}
	}
	return slut_id+1;
}

void IM_ReleaseWave(DWORD id)				// �����ϰ� ���̺� 
{
	if( stIsInit ==NULL )	//���� �ʱ�ȭ�� ���ߵ�.
		return;	
	if( !id )
		return;
	if( id > stIM_WaveNum )
		return;
	if(!stIM_WaveList)
		return;

	if( stIM_WaveList[id-1].Handle)
	{
		AIL_release_sample_handle( stIM_WaveList[id-1].Handle );
		stIM_WaveList[id-1].Handle=NULL;
	}
	if( stIM_WaveList[id-1].ptr)
	{
		AIL_mem_free_lock( stIM_WaveList[id-1].ptr );
		stIM_WaveList[id-1].ptr=NULL;
	}
	/*
	if( stIM_WaveList )	//�ƿ�.
	{
		Dfree(stIM_WaveList);
		stIM_WaveList=0;
		stIM_MaxWaveNum=0;
	}*/
}


void IM_ReleaseAllWaves()					//���� ���� �ε��Ұ�쿡 ���Լ��� ����ϸ� �����ϰ� ������ȴ�.
{
	if( stIsInit ==NULL )	//���� �ʱ�ȭ�� ���ߵ�.
		return;	
	for(int i=0; i<stIM_WaveNum; i++)
	{
		if( stIM_WaveList[i].Handle )
		{
			AIL_release_sample_handle( stIM_WaveList[i].Handle );
			stIM_WaveList[i].Handle=NULL;
		}
		if( stIM_WaveList[i].ptr)
		{
			AIL_mem_free_lock( stIM_WaveList[i].ptr );
			stIM_WaveList[i].ptr=NULL;
		}
	}
	stIM_WaveNum=0;
	if( stIM_WaveList )	//�ƿ�.
	{
		Dfree(stIM_WaveList);
		stIM_WaveList=0;
		stIM_MaxWaveNum=0;
	}
}


void IM_PlayWave(DWORD id,float vol,float pan)
{
	if( stIsInit ==0 )	//���� �ʱ�ȭ�� ���ߵ�.
		return;	
	if( !GetR3State()->mSoundOn ) //�÷��� ��������...
		return;
	if( stIM_WaveList == NULL )
		return;
	if( !id )	//0�����̵�� �ȵȴ�.
		return;
	if( stIM_WaveList[id-1].Handle==NULL )
		return;
	IM_SetWaveVolumeAndPan(id,vol*stGlobalWavVolume,pan);
    AIL_start_sample( stIM_WaveList[id-1].Handle );
}

void IM_SetLoopCntWave(DWORD id,DWORD loop_cnt)
{
	if( stIsInit ==0 )	//���� �ʱ�ȭ�� ���ߵ�.
		return;	
	if( !GetR3State()->mSoundOn ) //�÷��� ��������...
		return;
	if( stIM_WaveList == NULL )
		return;
	if( !id )	//0�����̵�� �ȵȴ�.
		return;
	if( stIM_WaveList[id-1].Handle==NULL )
		return;
	if( loop_cnt !=  stIM_WaveList[id-1].mLoopCnt )
	{
		stIM_WaveList[id-1].mLoopCnt=loop_cnt;
		AIL_set_sample_loop_count(stIM_WaveList[id-1].Handle,loop_cnt);
	}
}

void IM_StopWave(DWORD id)		//�����Ǿ� �÷������� ���̺긦 ������Ų��.
{
	if( stIsInit ==0 )	//���� �ʱ�ȭ�� ���ߵ�.
		return;	
	if( stIM_WaveList == NULL )
		return;
	if( !id )	//0�����̵�� �ȵȴ�.
		return;
	if( stIM_WaveList[id-1].Handle==NULL )
		return;
    AIL_stop_sample( stIM_WaveList[id-1].Handle );
}


void IM_SetWaveVolumeAndPan(DWORD id,float vol,float pan)	//���� 0 ~ 1���� ���� -1�� ���ʿ�ġ��ħ.1�� �����ʿ� ġ��ħ.
{
	if( stIsInit ==0 )	//���� �ʱ�ȭ�� ���ߵ�.
		return;	
	if( stIM_WaveList == NULL )
		return;
	if( !id )	//0�����̵�� �ȵȴ�.
		return;
	if( stIM_WaveList[id-1].Handle==NULL )
		return;
	AIL_set_sample_volume_pan(stIM_WaveList[id-1].Handle,vol*stGlobalWavVolume,(pan+1)*0.5f);
}
#endif
