#ifndef __R3BSP_H__
#define __R3BSP_H__


#pragma	pack(push, 1)		//����Ʈ ����.


/*
�浹�� bsp�� �ִ� 32767�� ���������� ��尹���� ���� �� ����.
���ؽ� ������ ������ ������ 65536���� ���� ���� ����.
*/

#define _READ_SPARE_CNT	26		//����Ȯ���� ���� ����ī��Ʈ.
#define _FREE_SPARE_CNT	32


#define _R3BSP_VERSION	39	// 000���� ���
//------------------- ���� export�Ǵ� BSP 
typedef struct{
	DWORD offset;		//�ɼ�.
	DWORD size;			//��Ʈ�� ������.
}_ENTRY;

typedef struct{
	DWORD version;		//_R3BSP_VERSION

//------------------- �������� �����͸� ������ �޸𸮿� �ٷ� �ű� �͵�...
	_ENTRY Cplanes;		// �� ���	d���� ����.
	_ENTRY Cface_id;		// �� �ε�������Ʈ	�ӽ÷� ���.... ���߿� ����ȭ
	_ENTRY node;		// ��� 
	_ENTRY leaf;		// ����

	_ENTRY MatListInLeaf;	//	������ �ִ� ���͸���׷츮��Ʈ.

	//---	�ִϸ��̼� ����.
	_ENTRY Object;	//����� ���̳��� ������Ʈ�� ����. 
	_ENTRY Track;	//�ִϸ��̼� Ʈ��

	//----  �浹���ΰ��� -- ���������� �������� �߰��Ҽ� �ִٴ� ���̴�.
	_ENTRY CFVertex;	//	�浹 ���̽� ��.
	_ENTRY CFLine;		//	�浹 ���̽� ����
	_ENTRY CFLineId;	//	�浹 ���̽� ���̵� ����Ʈ.
	_ENTRY CFLeaf;		//	�浹 ���̽� ����.

	_ENTRY EntityList;		//	��ƼƼ ���� ����Ʈ
	_ENTRY EntityID;		//	 map entities list ���̵� �¾�.
	_ENTRY LeafEntityList;		//	���� ��ƼƼ ����Ʈ. ������ ���� ���̵�� ������ ������ ����.

	_ENTRY SoundEntityID;		//	���� ���� ��ƼƼ ����Ʈ ���̵�
	_ENTRY LeafSoundEntityList;		//	���� ���� ��ƼƼ ����Ʈ. 	������ ���� ���̵�� ������ ������ ����.
	
	_ENTRY EventObjectID;		//	�̺�Ʈ ������Ʈ ���̵�.	 ���̳��� ������Ʈ�� �̺�Ʈ ������Ʈ �ִϸ��̼� ���̵�.

	_ENTRY ReadSpare[_READ_SPARE_CNT];

//------------------- �������� �����͸� �а��� Free�� �� ��.
	//-------- ȭ�� ���÷��̿�...�а��� free�ع����°�.
	_ENTRY BVertex;		//	byte ��
	_ENTRY WVertex;		//	word ��
	_ENTRY FVertex;		//	float��
	_ENTRY VertexColor;	// ���ؽ� �÷� 
	_ENTRY UV;			// uv��ǥ
	_ENTRY LgtUV;		// ����Ʈ�� uv��ǥ	//WORD���̴�.
	_ENTRY Face;		//	��
	_ENTRY FaceId;		// �� �ε�������Ʈ
	_ENTRY VertexId;	// �� �ε�������Ʈ
	_ENTRY MatGroup;	//	���͸��� �׷�..

	_ENTRY FreeSpare[_FREE_SPARE_CNT];
}_BSP_FILE_HEADER;

typedef struct{
	DWORD f_normal_id;		//	���̽� ��� ���̵�
	float d;				//	ax+by+cz = d �� d
	WORD vertex_num;			//	vertex����
	DWORD vertex_start_id;	//	���� vertex id = uv�� ���� id = light��uv ���� id(���� ����.)
}_BSP_C_FACE;

typedef struct{
	DWORD f_normal_id;		//	��带 ������ ��� ������
	float d;				//	ax+by+cz = d �� d
	SHORT front;
	SHORT back;
	Vector3s bb_min,bb_max;	//����� �ٿ�� �ڽ� �뷫 5%���� ��ũ��...
}_BSP_NODE;


typedef struct{
	WORD v_num;		//�ش� �� ����
	DWORD v_start_id;	//����...
//	DWORD color;			//	ARGB ���� �������� �÷���.
}_BSP_FACE;

#define _VERTEX_ARC_TYPE_CHAR		0x8000
#define _VERTEX_ARC_TYPE_SHORT		0x4000
#define _VERTEX_ARC_TYPE_FLOAT		0x2000
#define _Y_BILLBOARD		0x0800

typedef struct {
	WORD attr;		//���� ����Ʈ�� ����Ÿ�� 0 ����������,1 ����Ŀ��?
	WORD face_num;	//�ش� �� ����
	DWORD face_start_id;
	SHORT mtl_id;	//���͸��� ���̵�
	SHORT lgt_id;	//����Ʈ �� ���̵�
	Vector3s bb_min,bb_max;		//���͸��� �ٿ�� �ڽ�
	Vector3f pos;				//��ġ
	float scale;				//�����ϰ�...
	WORD object_id;		//������Ʈ id= ���� id+1 ����� ���̳��� ������Ʈ�� ���� ���.
}_BSP_M_GROUP;

typedef struct {
	BYTE type;					//����� Ư��. water�����...
	WORD face_num;				//bsp ���̽� ����
	DWORD face_start_id;		//����bsp face id��ȣ

	WORD m_group_num;			//���͸��� �׷�  ����
	DWORD m_group_start_id;		//���� ���͸��� �׷� id��ȣ.

	Vector3s bb_min,bb_max;		//����� �ٿ�� �ڽ�
}_BSP_LEAF;




#pragma	pack(pop)

#endif
