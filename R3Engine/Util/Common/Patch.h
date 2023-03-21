#ifndef __PATCH_H__
#define __PATCH_H__

#include"world.h"

#define _NO_SPLIT	0
#define _SPLIT		1

#pragma	pack( push, 1 )	//����� �����ϱ�...
typedef struct Winding{	//
	BYTE is_out_face;	//����� ���ε��ΰ�.
	Vector3f normal;	//���
	Vector3f pos;		//��ġ
	Vector3uc energy;		//unshot�� ����(unshot�� �ѹ� ����ϰ� ���� Ŭ�����Ѵ�.),���� ����Ʈ��
	Vector3uc temp_energy;	//���̳��� ����Ʈ�׽������ ��θ� ����...
//	float ff;			//�� ���� (���� ���� ����:intensity)
//	Vector3f unshot;		//���� ����ϰ��ִ�(����Ʈ,��ġ)�� * ������ * reflectance.r ,g, b
}_WINDING;
#pragma pack(pop)

typedef struct{	//��ġ�� 3�� �Ǵ� 4���̴�. �ٰ����� �ȴ�.
	int xl,yl;				//winding�� x,y ����
	int mtlid;				//���͸��� id
	Vector4f fnormal;	// face normal
	_WINDING *winding;

//--radiosity����Ұ�� 
//	float sqare;			//����
//	Vector3f reflectance;		//��ü(�Ǵ� ��ġ)�� �ݻ��ϴ� r,g,b ��
//	Vector3f emission;		//���ɷ�(����Ʈ�� ����) ����Ʈ R.G.B
}_PATCH;

typedef struct{
//	int friend_num;	//�̿��������� 1����, �簢���� ����
//	int option;
	_PATCH *patch;
}_LFACE;


void MakePatchAndWinding(_MESH *mesh,int option=_NO_SPLIT);
_LFACE **GetLFace(void);	//���� ����Ʈ�� ���̽� ����ü�����͸� ����

void VectorScale(Vector3f s,Vector3f e,float t,Vector3f get);
void VectorSubtract(Vector3f sou1,Vector3f sou2,Vector3f tar);
float VectorLength(Vector3f sou1);
void VectorPlus(Vector3f sou1,Vector3f sou2,Vector3f tar);
void MakeVertexPatchAndWinding(_MESH *mesh);	// ���ڸ޽��� ���ڷ�.... ��ī�� �ڽ��� ���Ѱ�...

//int CheckEdge(_VERTEX *a,_VERTEX *b,_VECTOR *v,_FACE_NORMAL *fv);


#endif
