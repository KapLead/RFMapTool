#ifndef __R3BSPTOOL_H__
#define __R3BSPTOOL_H__

#include "world.h"
#include "r3blockMat.h"

#pragma	pack(push, 1)		//����Ʈ ����.


#define	_MAX_ONE_FACE_VERTEX	16
typedef struct bspface{
	LONG v_num;			//���ؽ� ��
	LONG v_list[_MAX_ONE_FACE_VERTEX];
	LONG org_facenum;	//���� �޽��� ���̽���ȣ
	bspface *before;
	bspface *next;
}_TOOL_BSP_FACE;			//�ڸ� ���� ����ü...

typedef struct {
	Vector4f plane;		//��带 ������ ��� ������
	DWORD f_normal_id;		//	��带 ������ ��� �������� id
	LONG front;
	LONG back;
	Vector3f bb_min,bb_max;	//����� �ٿ�� �ڽ� ���������� 10%���� ��ũ��...
	LONG face_num;			//����� �������
	_TOOL_BSP_FACE *bsp_face;	//��忡�� ���۵Ǵ� face ; �������������� �ٸ���.
}_TOOL_NODE;

typedef struct {
	BYTE type;	//����� Ư��. water�����...
	WORD face_num;			//bsp ���̽� ����
	_TOOL_BSP_FACE *face;			//����bsp face ��ȣ
	Vector3f bb_min,bb_max;	//����� �ٿ�� �ڽ�
}_TOOL_LEAF;
#pragma	pack(pop)


void MakeBSPTree(_MESH *mesh);	//���� ��¥�ٰ��� �޽���.
void BspPrint(void);	//bsp������
void DrawBspLeafBBox(void);		//bsp leaf �ٿ�� �ڽ� ǥ�� 
void DrawBspSplitPolygon(void);		//bsp leaf ������ ǥ�� 
void BspExport(_MESH *c_mesh,_MESH *m_mesh,char *name,R3BlockBsp *BspExport);

#endif
