#ifndef __R3VERTEXBUFFER_H__
#define __R3VERTEXBUFFER_H__

#include "R3d3d8.h"


#define _USE_DYNAMIC_VP		0x00000001
#define _USE_STATIC_VP		0x00000002
#define _USE_MEMORY_VP		0x00000003		//�׳� �޸𸮿� ��´�.

//---------------------���ؽ� ���� ���� Ŭ����...
class CVertexBuffer{
private:
	DWORD m_Flag;
	DWORD m_Size;	//���ؽ� ���� �ѻ�����
public:
	union{
	LPDIRECT3DVERTEXBUFFER8 m_lpVertexBuffer;
	_D3DR3VERTEX_TEX1 *m_VertexBufferTex1;	//�޸𸮻��� ���ؽ� ����.
	_D3DR3VERTEX_TEX2 *m_VertexBufferTex2;	//�޸𸮻��� ���ؽ� ����.
	};
	CVertexBuffer();
	~CVertexBuffer();
//	void *GetVertexBuffer();
	void InitVertexBuffer(int size,int flag,DWORD fvf=0);
	void ReleaseVertexBuffer();
	BYTE* VPLock(int start=0,int size=0,DWORD option=0);
	void VPUnLock();
	DWORD GetVertexBufferSize()		{	return m_Size;	};
};

class CIndexBuffer{
private:
	DWORD m_Flag;
	DWORD m_Size;	//�ε��� ���� �ѻ�����
public:
	union{
	LPDIRECT3DINDEXBUFFER8 m_lpIndexBuffer;
	WORD *m_IndexBuffer;	//�޸𸮻��� ���ؽ� ����.
	};
	CIndexBuffer();
	~CIndexBuffer();
	void InitIndexBuffer(int size,int flag=_USE_STATIC_VP);
	void ReleaseIndexBuffer();
	BYTE* VPLock(int start=0,int size=0,DWORD option=0);
	void VPUnLock();
	DWORD GetVertexBufferSize()		{	return m_Size;	};
};

//���ؽ� ���ۺ��ʿ��� ������ �������� ĳ��.
void ResetVertexBufferCache();	//������ �������� �����Ҷ� �ݵ�� ȣ������.. 
void SetVPIPTex1(void *vb,void *ib);
void SetVPIPTex2(void *vb,void *ib);
void SetVPIPTex1IndexPrimitiveTL(void *vb,void *ib);
void SetVPIPTex1IndexPrimitive(void *vb,void *ib);


//���ؽ� ���� ����.
void ResetTotalVertexBufferInfo();
DWORD GetTotalVertexBufferSize();	//�ε������۵� ����.
DWORD GetTotalVertexBufferCnt();		//�ε��� ��������.


#endif