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

#define _PICKING_STYLE_NEAR		0	//���� ����� ��.
#define _PICKING_STYLE_FAR		1	//���� �հ�.
#define _PICKING_STYLE_BEFORE_POINT		2	//ù��° �� ���̸� ���󰣴�.


#define _SOUND_EDIT_MODE		0	// ���� ����Ʈ ���..
#define _SOUND_SELECT_MODE		1	// ���� ���ø��.....


#define _DUMMY_EDIT_MODE		0	// ���� ����Ʈ ���..
#define _DUMMY_SELECT_MODE		1	// ���� ���ø��.....

//---------spin �޴� ������
#define _SPIN_P_PNUM			1
#define _SPIN_P_LIVE_TIME		2
#define _SPIN_P_TIME_SPEED		3

// ��ü ����.
void SetTabMode(int TabMode);
int GetTabMode();
void ChangeAllTabSpinControl(); //��� ������Ʈ�ѿ����ѰŴ� ���⿡�� ó���Ѵ�.


//---Top�� �ִ°�.
void SetSelectMode(int mode);
int GetSelectMode();

void SetPickingStyle(int mode);
int GetPickingStyle();

void SetFlySpeed(float speed);
float GetFlySpeed();

int GetNowTabIndex();	//���� ���� ��� ����Ű�� �ִ°� 

// �κ� ����..
int WhatCollisionMode();
void SetCollisionMode(int mode);

int WhatEntityMode();
void SetEntityMode(int mode);

int WhatSoundMode();
void SetSoundMode(int mode);

int WhatDummyMode();
void SetDummyMode(int mode);

//���� �޴�.
void SetWhatSpin(int mode);
int GetWhatSpin();


#endif