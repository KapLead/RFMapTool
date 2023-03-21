#ifndef __R3Sound_H__
#define __R3Sound_H__

//���� �ʱ�ȭ�׸�����
BOOL InitR3SoundSystem(char *wav_spt_name);		//���̺꽺ũ��Ʈ �̸��� �Է��Ѵ�.
void ReleaseR3SoundSystem();

void LoadWaveList(char *name);	//ȿ���� ���ø���Ʈ �⺻������ �����ʱ�ȭ�� ����� ���ش�....
void ReleaseWaveList();		//


//��ü ������ ���� API
BOOL SetGlobalWavVolume(float vol);	//0-1����..	���̺� ��ü����.
float GetGlobalWavVolume();	//0-1����..	���̺� ��ü����.
BOOL SetGlobalMusicVolume(float vol);	//0-1����..	���� ��ü����.
float GetGlobalMusicVolume();	//0-1����..	���� ��ü����.


class CPlayMP3{
private:
	void *m_Mp3Handle;
	DWORD *m_ptr;	//�޸� ��.
public:
	CPlayMP3::CPlayMP3();
	CPlayMP3::~CPlayMP3();

	//MP3 �÷��̿� ���Ѱ�.
	//---������ ������ �޸𸮿� �ѹ��� �ø��°��� 
	BOOL LoadR3MP3(char *name);
	void ReleaesR3MP3();
	void PlayR3MP3();
	void StopR3MP3();
	BOOL SetVolumeMP3(float vol);	//0-1����..
};

//��Ʈ���� ������ �߱ݾ߱� �о �÷����ϴ°���̴�. 
BOOL LoadStreamR3MP3(char *name,DWORD loop_cnt=0);
void ReleaesStreamR3MP3();
void PlayStreamR3MP3();
void StopStreamR3MP3();
BOOL SetVolumeStreamMP3(float vol);	//0-1����..

void UpdateStreamMP3();  // mp3�� ������ �������� ���Լ��� 1�ʿ� 10���� ȣ���Ϸ���...�Ѥ�
BOOL IsEndStreamMP3();		//��Ʈ�� mp3�� �����°�??

#define		SND_DEFAULT		-65536.0f


// ���̺� �÷��̴� ĳ���� ����ϴ� ����� ���������� ����ϴ� �ΰ��� ������ֵ�....
// ���� ���������� ���ȿ������ ��� ���������� ����ϰ�(IM��� ) ���ӻ󿡼��� ����ĳ���� ���� ����� ���Ѵ�.
//------------------- ���̺� �÷��̰��Ѱ�. ���� ĳ���� ����ϴ� ����� API -----------------------//

#define _PLAY_WAVE_LOOP		1


void SetPlayWaveState( BOOL state );
void SetPlayMusicState( BOOL state );

void PlayWave(DWORD id,DWORD flag=0,float vol=1.0f,float pan=0);
void StopWave(DWORD id);		//�����Ǿ� �÷������� ���̺긦 ������Ų��.
void SetWaveVolume(DWORD id,float vol,float pan);	// vol 0 ~ 1���� pan -1 ~ 1����  -1�� ���ʿ�ġ��ħ.1�� �����ʿ� ġ��ħ.

BOOL IsExistFromSoundSpt(DWORD id);		//�����Ⱦ��̵� ��ũ��Ʈ�� �ִ°�?


//-------------- ���̺� �÷��̰��Ѱ�. ���������� ����ϹǷ� �ε��� ������ ������Ѵ�.-------------//

DWORD IM_LoadWave(char *name,DWORD flag=0);	//�ε����� ���ϵȴ�.�ϳ��� �ε������� �ȿ��� ���̺� �����ڰ� ����ԵǹǷ� .. �������� IM_ReleaseAllWaves()ȣ�� �Ұ�.
void IM_ReleaseWave(DWORD id);				// �����ϰ� ���̺� 
void IM_ReleaseAllWaves();					//���� ���� �ε��Ұ�쿡 ���Լ��� ����ϸ� �����ϰ� ������ȴ�.

void IM_PlayWave(DWORD id,float vol=1.0f,float pan=0);
void IM_StopWave(DWORD id);		//�����Ǿ� �÷������� ���̺긦 ������Ų��.
//void IM_SetWaveVolume(DWORD id,float vol);	// 0 ~ 1���� 
//void IM_SetWavePan(DWORD id,float pan);	//-1 ~ 1����  -1�� ���ʿ�ġ��ħ.1�� �����ʿ� ġ��ħ.
void IM_SetWaveVolumeAndPan(DWORD id,float vol,float pan);	//���� 0 ~ 1���� ���� -1�� ���ʿ�ġ��ħ.1�� �����ʿ� ġ��ħ.
void IM_SetLoopCntWave(DWORD id,DWORD loop_cnt);



#endif