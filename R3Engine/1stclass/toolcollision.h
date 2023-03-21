#ifndef __TOOLCOLLISION_H__
#define __TOOLCOLLISION_H__

#include "windows.h"
#include "c_vector.h"

//-----�浹 ����(������) �Ӽ� ���� ���߿� �� �Ӽ����� ������ �浹���� ýũ ���θ� �����Ѵ�.
#define _LINE_ATTR_SELECT	0x80000000
#define _LINE_ATTR_FREEZE	0x40000000

#pragma	pack(push, 1)		//����Ʈ ����.
typedef struct{
	float v[3];
}_TOOL_COL_POINT;

typedef struct{
	DWORD attr;		//���μӼ�		//���� ����Ʈ�� �̺�Ʈ ���̵�� �� �����̴�. (���߿� ������ �⵿ýũ�϶�..����ض�..)
	WORD start_v,end_v;	//�� �ε���
	float height;
	WORD front,back;	//���� �ε���
}_TOOL_COL_LINE;		//�������� �浹 ������� ����.

typedef struct{
	DWORD start_id;
	WORD line_num;
}_TOOL_COL_LEAF;
#pragma	pack(pop)

class  CBsp;

class CToolCollisionFace{
private:
public:
	_TOOL_COL_POINT *mColPoint;
	_TOOL_COL_LINE *mColLine;
	Vector4f *mNormal;
	Vector3f *mVNormal;
//	_TOOL_COL_LEAF *mColLeaf;
//	WORD *mColLineId;
	int mCOLPOINTMAXCNT;
	int mCOLLINEMAXCNT;
	int mCOLVNORMALMAXCNT;
	int mColPointCnt;
	int mColLineCnt;
	int mVNormalCnt;
	int mSelectVertex;		//���õ� ��...
	BOOL mOnePointCreate;
	WORD GetSearchLineIndex(WORD v_id);
	BOOL mbContinueLine;
public:
	WORD mBeforeVId;	//���õ� ���ε����� ����Ʈ���� 

	CToolCollisionFace();
	~CToolCollisionFace();
// ����ºκ�
	BOOL SaveCF(char *name);
	BOOL LoadCF(char *name);


	BOOL LoadColFaceFromBSP(CBsp *bsp);

// �浹 ���̽����� �浹����  ����.
	BOOL GetCrossPoint(Vector3f from,Vector3f at,Vector3f *get);

	int GetCrossFace(Vector3f from,Vector3f at);	//�浹 ������ �ε����� �˾Ƴ���.
	int GetCrossFace(int x,int y);					//�����ǵ� ���콺 ��ǥ�� �ѱ�� ����.
// ����Ʈ Tool�� ���Ѱ�.	
	BOOL GetContinueLine();
	void SetContinueLine(BOOL state);
	void SelectAll();	//���δ� �����Ѵ�.
	void UnSelectAll();	//���δ� Un�����Ѵ�.
	void SelectFace(int id);	//�� ���̽��� �����Ѵ�.
	BOOL SelectFaceGroup(int id);	//���õ� ���̽� �׷��� �����Ѵ�.

	//---������,�������� ���� 
	void FreezeFromSelect();	//���õ� ���̽��� ������ ��Ű�� ������ Ǭ��.
	void UnFreezeAll();	//��� �������� �Ѵ�.
	void UnFreezeFace(int id);	//�� ���̽��� �������� �Ѵ�.
	BOOL UnFreezeFaceGroup(int id);	//���õ� ���̽� �׷��� �����Ѵ�.

	
	WORD CreateVertex(Vector3f v);	//���ϰ��� ������ id���̴�.
	BOOL DeleteVertex(WORD id);		//����� ������ true
	void DeleteSelectFace();		//���õ� ����� �����Ѵ�.
	BOOL GetVertex(Vector3f *get,WORD id);
	int GetColLineCnt();
	int GetColPointCnt();
	int GetBeforeId();
	void SetBeforeId(WORD v_id);	//���� ���� ������ ����Ѵ�.
	float GetFaceHeght(WORD f_id);	//���̸� ���س���.

	void AdjustVertex();		//��� ���ؽ��� ���ش�.
	void AdjustFace();			//��ũ�ȵ� ���� ��ģ��.
	void AdjustSelectFaceHeight();	//���� �� ��������.
	void AdjustFixFaceLink();		//��ũ �߸��� ����� ��ģ��.

	WORD CreateLine(WORD v_id,BOOL b_ontinue_line);	//���ϰ��� ������ id���̴�.
	BOOL DeleteLine(WORD id);		//����� ������ true
	void DeleteCurLine();	
	void DrawCollisionPoly();	//�浹 ���� �Ѹ���.
	void DrawCollisionPoint();	//�浹 ���� �Ѹ���.
	WORD IsExistPoint(Vector3f pos);	//���ϰ��� 0�̸� ����

	void GetLineNormal(Vector4f *get,_TOOL_COL_LINE *col_line);

	void GetPathFinder(Vector3f from,Vector3f at,Vector3f get[16],DWORD *cnt);
	void GetPath(Vector3f from,Vector3f at);
	int GetPathCrossPoint(Vector3f from,Vector3f at,Vector3f *get,int now_f1,int now_f2);
	BOOL EdgeTest(Vector3f cross,int id);

	void GetFaceFrontPoint(Vector3f *get_temp,int v_id);
	BOOL IsExistSelfPoint(int id,int face);
	void FixLinkFace();	//���̽� ��ũ ���� ýũ�ؼ� ��ģ��.
	void MoveSelectFace(BOOL yaxis,BOOL xzaxis,LONG xdiff, LONG ydiff);
	void MoveSelectVertex(BOOL yaxis,BOOL xzaxis,LONG xdiff, LONG ydiff);
	void SetSelectVertex(WORD id);
	WORD GetSelectVertex();
	void UnSetSelectVertex();

	int GetFinalPath(void *node,Vector3f from,Vector3f get[16]);//���� ����ȭ �н��� �����Ѵ�.

	void MakeVnormal();	//���� ���õ��ִ� ���̽��鿡�� ���ؽ������ ���Ѵ�.
	void MakeFnormal();	//���� ���õ��ִ� ���̽������ ���Ѵ�.

	//bsp ���̺긦 ���� ������������.
	void LoadCollisonInfoInBSP(CBsp *bsp,void *v_add_bsp);
	void ReleaseCollisonInfoInBSP(void *v_add_bsp);	//����� �Ұ�.
};

void SetHeight(int su);
int GetHeight();


#endif