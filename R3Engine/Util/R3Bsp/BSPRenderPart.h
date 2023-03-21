#ifndef _BSPRENDERPART_H__
#define _BSPRENDERPART_H__

#include "world.h"
#include "R3bsp.h"
#include "R3BlockMat.h"

typedef struct {
	int MatObjNum;
	int BMaxVertexNum;
	int WMaxVertexNum;
	int FMaxVertexNum;
//---------------- 
	int BVertexNum;
	int WVertexNum;
	int FVertexNum;
	int FaceNum;
//	int FaceIdNum;	//���̽� ������ �����Ƿ�...
	int VertexIdNum;
	Vector3c *BVertex;			//	byte ��
	Vector3s *WVertex;			//	word ��
	Vector3f *FVertex;		//	float��
	Vector2f *UV;			//	float uv �ؽ���
	Vector2s *LGTUV;		//	short ����Ʈ�� uv
	_BSP_FACE *Face;		//	��
	DWORD *FaceId;		// �� �ε�������Ʈ	�ӽ÷� ���.... ���߿� ����ȭ
	DWORD *VertexId;		// �� �ε�������Ʈ	�ӽ÷� ���.... ���߿� ����ȭ
	DWORD *VertexColor;	//���ؽ� �÷�.
}BSPRenderPart;


BSPRenderPart *GetBSPRenderPart(void);
void CreateBSPRenderPart(_BSP_M_GROUP *bsp_m_group,_MESH *m_mesh,R3BlockBsp *block_bsp);
void ReleaseBSPRenderPart(void);


#endif