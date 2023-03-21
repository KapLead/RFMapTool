#ifndef __ANIMATION_H__
#define __ANIMATION_H__

//#include "R3math.h"

#pragma	pack(push, 1)		//����Ʈ ����.

typedef struct{
	float frame;		//������ ��ȣ ( ��:12.1 ������)
	Vector3f pos;
}_POS_TRACK;			//��ġƮ��

typedef struct{
	float frame;		//������ ��ȣ ( ��:12.1 ������)
	Vector4f quat;		//���ʹϿ�
}_ROT_TRACK;			//ȸ�� Ʈ��

typedef struct{
	float frame;		//������ ��ȣ ( ��:12.1 ������)
	Vector3f scale;
	Vector4f scale_axis;	//�������� ���ʹϿ�.
}_SCALE_TRACK;			//������Ʈ��

//-------������Ʈ Ÿ�� �Ӽ�.	word������ �س�����.
//		0x000f�� �̺�Ʈ �ִϿ��� ������Ʈ ������ ����.bspŬ������ �ִ�.
#define _DYNAMIC_OBJECT		0x8000
#define _NO_MERGE_OBJECT	0x4000
#define _EVENT_ANI			0x2000		//�̺�Ʈ �ִϸ��̼��ϰ��.
#define _DEFAULT_ANI		0x1000		//����� �ִϸ��̼��� ���.

typedef struct {
	WORD	flag;			// ������Ʈ �÷���?
	WORD	parent;			// 0�� �θ� ����. parent-1 = index
	int frames;				//�ش� ������Ʈ�� �����Ӽ�
	int Pos_cnt;			// ��ġ Ʈ�� ���� 
	int Rot_cnt;			// ������Ʈ Ʈ�� ����
	int Scale_cnt;			// ������ Ʈ�� ���� 

	Vector3f scale;		//������ �ִϰ� ���� ��� �̵��� 
	Vector4f scale_quat;		//������ �� ���ʹϿ�.
	Vector3f pos;		//�̵��ִϰ� ���� ��� �̵��� 
	Vector4f quat;		//ȸ���ִϰ� ���� ��� ���ʹϿ� �� 
	DWORD pos_offset;
	DWORD rot_offset;
	DWORD scale_offset;
}_READ_ANI_OBJECT;

typedef struct{
	char ObjectName[64];	//������Ʈ �̸�
	char ParentName[64];	//�θ� �̸� 
	WORD flag;				// �����Ǵ°�?
	WORD parent;			// 0�� �θ� ����. parent-1 = index
	int frames;				//�ش� ������Ʈ�� �����Ӽ�
	int Pos_cnt;			// ��ġ Ʈ�� ���� 
	int Rot_cnt;			// ������Ʈ Ʈ�� ����
	int Scale_cnt;			// ������ Ʈ�� ���� 

	Vector3f scale;			//������ �ִϰ� ���� ��� �̵��� 
	Vector4f scale_quat;	//������ �� ���ʹϿ�.
	Vector3f pos;			//�̵��ִϰ� ���� ��� �̵��� 
	Vector4f quat;			//ȸ���ִϰ� ���� ��� ���ʹϿ� �� 
	_SCALE_TRACK *Scale;	// ��ġ Ʈ��		x,y,z					��������[4]
	_POS_TRACK *Pos;		// ��ġ Ʈ��		x,y,z					��������[4]
	_ROT_TRACK *Rot;		// ������Ʈ Ʈ��	x,y,z,w �� ���ʹϿ�		��������[5]

	//�̰͵��� ���� �ȿ��� �������� ���־���Ұ͵��̴�.
	BYTE AniFrameCache;		//�ִ� ������ ������ �ѹ���....
	float s_matrix[4][4];   //scale local matrix
	float now_frame;		//���� ������
    float f_matrix[4][4];   //���� ��Ʈ����
}_ANI_OBJECT;

#pragma	pack(pop)


void GetObjectMatrix(float w_matrix[4][4],WORD id,_ANI_OBJECT *ani_object,float time);
void GetAniMatrix(float ani_mat[4][4],_ANI_OBJECT *obj,float now_frame);
void ConvAniObject(int num,BYTE *track,_READ_ANI_OBJECT *r_object,_ANI_OBJECT *object);		//Ʈ���ε����� �����ͷ� �ٲ�����.

#endif
