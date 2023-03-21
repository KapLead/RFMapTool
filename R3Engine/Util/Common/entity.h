#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "world.h"
#include "r3blockmat.h"

#pragma	pack(push, 1)		//����Ʈ ����.

#define _ENTITY_VERSION 113	//110�� 111�� ���̴� ����Ʈ 2�� ���.

typedef struct{
	DWORD offset;		//�ɼ�.
	DWORD size;			//��Ʈ�� ������.
}_ENTITY_ENTRY;

//���� �պκ� 8byte�� �̰��� �ִ�.
//	DWORD version;		//_R3BSP_VERSION
//	DWORD identity;		//��ī�̹ڽ��� ��ƼƼ���� �׻� 0�̴�.

typedef struct{
	_ENTITY_ENTRY CompHeader;	//	�������� ���.
	_ENTITY_ENTRY Vertex;		//	float��
	_ENTITY_ENTRY VColor;		// vertex �÷�
	_ENTITY_ENTRY UV;			// uv��ǥ
	_ENTITY_ENTRY Face;		//	��
	_ENTITY_ENTRY FaceId;		// �� �ε�������Ʈ
	_ENTITY_ENTRY VertexId;	// �� �ε�������Ʈ
	_ENTITY_ENTRY MatGroup;	//	���͸��� �׷�..
	_ENTITY_ENTRY Object;	//	������Ʈ.
	_ENTITY_ENTRY Track;	//	Ʈ��.
}_ENTITY_FILE_HEADER;

typedef struct {
	WORD face_num;	//�ش� �� ����
	DWORD face_start_id;
	SHORT mtl_id;	//���͸��� ���̵�
	WORD object_id;	//������Ʈ ���̵�... ���̳��� ������Ʈ�� ����...
	Vector3s bb_min,bb_max;		//���͸��� �ٿ�� �ڽ�
}_ENTITY_M_GROUP;

typedef struct{
	WORD v_num;		//�ش� �� ����
	DWORD v_start_id;	//����...
}_ENTITY_FACE;

//��ƼƼ ���� ���
typedef struct {
	WORD type;		//���ؽ� ����Ÿ�� 
	Vector3s bb_min,bb_max;		//���͸��� �ٿ�� �ڽ�
	Vector3f pos;				//��ġ
	float scale;				//�����ϰ�...
	//������ǥ
	float uv_min,uv_max;
}_ENTITY_COMP_HEADER;

//------- ��ƼƼ�� ����Ʈ



typedef struct {
}_ENTITY_LIST;


#pragma	pack(pop)

void EntityExport(char *name,_MESH *mesh,_MATERIAL *mat,R3BlockBsp *block_mat);
_D_OBJECT *AllocDObject(_MESH *m_mesh);
void FreeDObject(_D_OBJECT *object);
BYTE *AllocTrack(_MESH *m_mesh);
void FreeTrack(BYTE *track);
void StoreDObjectAndTrack(_D_OBJECT *object,BYTE *ani_track,_MESH *m_mesh);
int	GetDObjectNum(_MESH *m_mesh);
int GetTrackSize(_MESH *m_mesh);


#endif