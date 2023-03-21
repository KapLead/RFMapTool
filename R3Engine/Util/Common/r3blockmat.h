#ifndef __R3BLOCKBSP_H__
#define __R3BLOCKBSP_H__

typedef struct OptObject{
	Vector3f bb_min,bb_max;	//�ٿ�� �ڽ�
	SHORT mtlid;
	SHORT lgtid;		//	//������ ����Ʈ�� ���̵� �ȴ�.
	DWORD start_id;	//�� ��ŸƮ�ε���
	DWORD f_cnt;	//�鰹�� 
	OptObject *next;
	WORD type;
//	bool no_merge;
//	bool loop_ani;
	WORD object_id;		//������Ʈ id= ���� id+1 ����� ���̳��� ������Ʈ�� ���� ���.
	int i;	//index��ȣ ����׿�...
}_OptObject;

class R3BlockBsp{
private:
	void MergeOptObject();

public:
	_MESH *MakePMeshBlockBsp(_MESH *p_mesh,_MESH *r_p_mesh);

	_POLYFACE **mPolyFace;	// ����� ������..
	DWORD *mPolyWhatObj;	// ����� � ������Ʈ�� ������ �ֳ�...
	int mPolys;				// �� ���..
	_OptObject *mOptObject;
	int mOptObjNum;		//�� ��Ƽ������ ������Ʈ��..
	R3BlockBsp(int face_num);
	~R3BlockBsp();
	void PrepareBlockBsp(_MESH *mesh,_MATERIAL *material);
	void TestFree();
};

WORD GetDynamicObjectId(_MESH *mesh,int n_id);
WORD GetObjectId(_MESH *mesh,int d_id);		//���̳��� ������Ʈ �ε����� �־ ��� ������Ʈ�ΰ��� �˾Ƴ���.


#endif