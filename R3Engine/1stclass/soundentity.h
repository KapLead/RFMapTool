#ifndef __SOUNDENTITY_H__
#define __SOUNDENTITY_H__

#include "c_vector.h"
#include <windows.h>

#pragma	pack(push, 1)		//����Ʈ ����.

typedef struct{
	WORD start_id;			//����� 60000�̻� ��������.
	WORD entities_num;
}_LEAF_SOUND_ENTITIES_LIST_INFO;

typedef struct{
	char name[64];
}_READ_SOUND_ENTITY_LIST;

typedef struct{
	WORD ID;			//�÷��̿� �� ���̵�...
	char Name[62];
}_SOUND_ENTITY_LIST;

typedef struct{
	WORD id;			// ���� ���̵�.
	WORD event_time;	// �̺�Ʈ Ÿ��.
	DWORD flag;			//���� �÷��� 
	float scale;		// �߽ɰŸ�...
	float attn;			// ����.
	Vector3f	pos;	//���� ��ġ��..
	Vector3f box_scale;
	float box_attn;
	float box_rot_x,box_rot_y;
	DWORD spare;		//������ ���� ����...
}_READ_SOUND_ENTITIES_LIST;	//���Ϸ� �д� ��ƾ


typedef struct{
	WORD ID;			// ���� ���̵�.
	WORD EventTime;		// ���� ����Ÿ��.
	DWORD Flag;			//���� �÷��� 
	float Scale;		// �߽ɰŸ�...
	float Attn;			// ����.
	Vector3f	Pos;	//���� ��ġ��..
	Vector3f BoxScale;
	float BoxAttn;
	float BoxRotX,BoxRotY;
	float NextPlayTime;
	Vector3f BBMin,BBMax;
	float mInvMat[4][4];
	float GetVolume(Vector3f pos);
	float GetPan(Vector3f pos);
	float GetBoxIntensity(Vector3f pos);		//�ڽ� ���������� ������ �˷��ش�. 0-1����
}_SOUND_ENTITIES_LIST;		// ���� �޸𸮿��� ������������.



#pragma	pack(pop)

#endif