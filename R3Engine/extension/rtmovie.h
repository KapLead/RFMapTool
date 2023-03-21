#ifndef __RTMOVIE_H_
#define __RTMOVIE_H_

#include "core.h"
#include "AniCamera.h"
#include "jerror.h"
#include "R3util.h"
#include "Level.h"

#define	_RTM_CHAR_FLAG_COLOR		0x00000001
#define	_RTM_CHAR_FLAG_MAGIC		0x00000002		//������ ��� �ϳ��� ĳ���͸� �����Ѵ�.
#define	_RTM_CHAR_FLAG_COLLISION	0x00000004		//ĳ���Ͱ� �ٴ��� Ÿ�� �ٴϴ� �ɼ��̴�.
#define	_RTM_CHAR_FLAG_SHADOW		0x00000008		//ĳ���� �����찡 �����Ѵ�.

//�ش緹���� �̸��� ���� rvpȮ���ڰ� ������ �ڵ����� �ε��ȴ�.
BOOL IsLoadedRTMovie();		//rt������ �ε��Ǿ��°�?
BOOL IsLoadedRTCamera();	//rtī�޶� �ε��Ǿ��°�? 
void RTMovieCreate(char *name,CLevel *level);	//����Ÿ�� �������� ������ �ε��Ѵ�.
void RTMovieRelease();
void RTMovieFrameMove();
void RTMovieRender();



#define RTM_STATE_NO_WIDE			0x1
#define RTM_STATE_NO_LOGO			0x2
#define RTM_STATE_NO_CAMERAMOVE		0x4

void RTMovieSetState(DWORD state);
DWORD RTMovieGetState();
void RTMovieStartPlay(BOOL is_beginning);	// TRUE�� ó������ FALSE�� ���ᶧ����..
void RTMoviePause(BOOL pause);	//������� TRUE �ٽ� �۵���ų���� FALSE
void RTMoiveGetCameraPos(Vector3f get_pos);
void RTMoiveGetCameraMatrix(float mat[4][4]);
BOOL IsRTMoviePlaying();	//���� �÷��̵����ΰ�?


#endif
 

