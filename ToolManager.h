#ifndef __TOOL_MANAGER_H__
#define __TOOL_MANAGER_H__

#include "stdafx.h"

#define _COLLISION_MODE		0
#define _ENTITY_MODE		1
#define _PARTICLE_MODE		2
#define _MAGIC_MODE			3
#define _SOUND_MODE			4
#define _DUMMY_MODE			5

#define _SELECT_MODE_VERTEX	0
#define _SELECT_MODE_FACE	1
#define _SELECT_MODE_GROUP	2

#define _COLLISON_TEST_MODE		0
#define _COLLISON_EDIT_MODE		1
#define _COLLISON_SELECT_MODE	2

#define _ENTITY_EDIT_MODE		0
#define _ENTITY_SELECT_MODE		1

#define _PICKING_STYLE_NEAR		0	//가장 가까운 곳.
#define _PICKING_STYLE_FAR		1	//가장 먼곳.
#define _PICKING_STYLE_BEFORE_POINT		2	//첫번째 점 높이를 따라간다.


#define _SOUND_EDIT_MODE		0	// 사운드 에디트 모드..
#define _SOUND_SELECT_MODE		1	// 사운드 선택모드.....


#define _DUMMY_EDIT_MODE		0	// 더미 에디트 모드..
#define _DUMMY_SELECT_MODE		1	// 더미 선택모드.....

//---------spin 메뉴 디파인
#define _SPIN_P_PNUM			1
#define _SPIN_P_LIVE_TIME		2
#define _SPIN_P_TIME_SPEED		3

// 전체 관련.
void SetTabMode(int TabMode);
int GetTabMode();
void ChangeAllTabSpinControl(); //모든 스핀콘트롤에대한거는 여기에서 처리한다.


//---Top에 있는곳.
void SetSelectMode(int mode);
int GetSelectMode();

void SetPickingStyle(int mode);
int GetPickingStyle();

void SetFlySpeed(float speed);
float GetFlySpeed();

int GetNowTabIndex();	//현재 탭이 어디를 가르키고 있는가 

// 부분 관련..
int WhatCollisionMode();
void SetCollisionMode(int mode);

int WhatEntityMode();
void SetEntityMode(int mode);

int WhatSoundMode();
void SetSoundMode(int mode);

int WhatDummyMode();
void SetDummyMode(int mode);

//스핀 메뉴.
void SetWhatSpin(int mode);
int GetWhatSpin();


#endif