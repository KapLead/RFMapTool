#include "R3VertexBuffer.h"
#include "jerror.h"
#include "jmalloc.h"


//���ؽ� ���� ���� ĳ���κ�...
static BYTE *stOldVB=NULL;
static BYTE *stOldIB=NULL;

static DWORD stTotalVertexSize,stTotalVertexCnt;	//���ؽ� ���� �ѿ뷮 �˾Ƴ���.

void ResetVertexBufferCache()	//������ �������� ���� �Ҷ� �ݵ�� ȣ������.. 
{
	stOldVB=NULL;
	stOldIB=NULL;
}

void SetVPIPTex1(void *vb,void *ib)
{
	LPDIRECT3DDEVICE8 pd3dDevice = GetD3dDevice();

	if( vb != stOldVB )
	{
	    pd3dDevice->SetStreamSource( 0, ((LPDIRECT3DVERTEXBUFFER8)vb), sizeof(_D3DR3VERTEX_TEX1) );
		stOldVB = (BYTE*)vb;
	}
	if( ib != stOldIB )
	{
	    pd3dDevice->SetIndices( ((LPDIRECT3DINDEXBUFFER8)ib), 0 );
		stOldIB = (BYTE*)ib;
	}
	pd3dDevice->SetVertexShader( D3DFVF_R3VERTEX_TEX1 );
}

void SetVPIPTex2(void *vb,void *ib)
{
	LPDIRECT3DDEVICE8 pd3dDevice = GetD3dDevice();

	if( vb != stOldVB )
	{
		pd3dDevice->SetStreamSource( 0, ((LPDIRECT3DVERTEXBUFFER8)vb), sizeof(_D3DR3VERTEX_TEX2) );
		stOldVB = (BYTE*)vb;
	}
	if( ib != stOldIB )
	{
	    pd3dDevice->SetIndices( ((LPDIRECT3DINDEXBUFFER8)ib), 0 );
		stOldIB = (BYTE*)ib;
	}
	pd3dDevice->SetVertexShader( D3DFVF_R3VERTEX_TEX2 );
}
void SetVPIPTex1IndexPrimitiveTL(void *vb,void *ib)
{
	LPDIRECT3DDEVICE8 pd3dDevice = GetD3dDevice();

	stOldVB = NULL;
	stOldIB = NULL;
	pd3dDevice->SetVertexShader( D3DFVF_R3TLVERTEX_TEX1 );
}
void SetVPIPTex1IndexPrimitive(void *vb,void *ib)
{
	LPDIRECT3DDEVICE8 pd3dDevice = GetD3dDevice();

	stOldVB = NULL;
	stOldIB = NULL;
	pd3dDevice->SetVertexShader( D3DFVF_R3VERTEX_TEX1 );
}


//--------------------------���ؽ� ���� ���� Ŭ����...
CVertexBuffer::CVertexBuffer()
{
	memset(this,0,sizeof(CVertexBuffer));
}
CVertexBuffer::~CVertexBuffer()
{
}

void CVertexBuffer::InitVertexBuffer(int size,int flag,DWORD fvf)
{
	HRESULT result;
	m_Size= size;
	stTotalVertexSize+=m_Size;
	if( size== 0 )
	{
		m_lpVertexBuffer=NULL;
		return;
	}
	stTotalVertexCnt++;

    LPDIRECT3DVERTEXBUFFER8 pMeshSourceVB;

	m_Flag=flag;
	switch(flag)
	{
	case _USE_DYNAMIC_VP:
		result = GetD3dDevice()->CreateVertexBuffer( size,D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC
//			, fvf, D3DPOOL_DEFAULT,&pMeshSourceVB );//���̳���
			, fvf, D3DPOOL_MANAGED,&pMeshSourceVB );//2003  4/18�� ��ħ 
		break;
	case _USE_STATIC_VP:
		result = GetD3dDevice()->CreateVertexBuffer( size,D3DUSAGE_WRITEONLY
			, fvf, D3DPOOL_MANAGED,&pMeshSourceVB );//����ƽ
		break;
	case _USE_MEMORY_VP:	//������ ����ü
		pMeshSourceVB=(LPDIRECT3DVERTEXBUFFER8)Dmalloc(size);
		result = D3D_OK;
		break;	
	}
	if( pMeshSourceVB == NULL )
		Error("���ؽ� ���۸� �����Ҽ������ϴ�.","");
	m_lpVertexBuffer = pMeshSourceVB;
}
void CVertexBuffer::ReleaseVertexBuffer()
{
	if( m_lpVertexBuffer == NULL )
		return;
	if( m_Flag == _USE_MEMORY_VP )	//������ ����ü
		Dfree(m_lpVertexBuffer);
	else
		m_lpVertexBuffer->Release();
	stTotalVertexSize-=m_Size;
	m_lpVertexBuffer=NULL;
	m_Flag=0;
	m_Size=0;
}


BYTE* CVertexBuffer::VPLock(int start,int size,DWORD option)
{
	BYTE* bp=NULL;

	if(m_lpVertexBuffer == NULL)
		return NULL;
	switch(m_Flag)
	{
	case _USE_DYNAMIC_VP:
		m_lpVertexBuffer->Lock( 
			start,size, (BYTE**)&bp
//			, D3DLOCK_NOOVERWRITE );
//			, D3DLOCK_DISCARD  );
			, option );
//		,0);//start ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
		break;
	case _USE_STATIC_VP:
		m_lpVertexBuffer->Lock( 
			start,size, (BYTE**)&bp, 0 );
		break;
	case _USE_MEMORY_VP:
		bp=(BYTE *)m_lpVertexBuffer;
		break;	
	}
	if( bp== NULL )
		Error("���ؽ����۸� lock �Ҽ��� �����ϴ�.","");
	return bp;
}

void CVertexBuffer::VPUnLock()
{
	if(m_lpVertexBuffer == NULL)
		return;
	if( m_Flag == _USE_MEMORY_VP )
		return;

	m_lpVertexBuffer->Unlock();
}

CIndexBuffer::CIndexBuffer()
{
	memset(this,0,sizeof(CIndexBuffer));
}

CIndexBuffer::~CIndexBuffer()
{
	if( m_lpIndexBuffer )
		ReleaseIndexBuffer();
}

void CIndexBuffer::InitIndexBuffer(int size,int flag)
{
	m_Size= size;
	stTotalVertexSize+=m_Size;
	if( size== 0 )
	{
		m_lpIndexBuffer=NULL;
		return;
	}
	stTotalVertexCnt++;
	LPDIRECT3DINDEXBUFFER8  pMeshSourceIB;
	m_Flag =flag;
	if( m_Flag == _USE_MEMORY_VP )
		pMeshSourceIB=(LPDIRECT3DINDEXBUFFER8)Dmalloc(size);
	else
		GetD3dDevice()->CreateIndexBuffer( size,
                                      D3DUSAGE_WRITEONLY,
                                      D3DFMT_INDEX16, D3DPOOL_MANAGED,
                                      &pMeshSourceIB );
	if( pMeshSourceIB == NULL )
		Error("�ε��� ���۸� �����Ҽ������ϴ�.","");
	m_lpIndexBuffer=pMeshSourceIB;
}
void CIndexBuffer::ReleaseIndexBuffer()
{
	if( m_lpIndexBuffer == NULL )
		return;
	if( m_Flag == _USE_MEMORY_VP )
		Dfree(m_lpIndexBuffer);
	else
		m_lpIndexBuffer->Release();
	stTotalVertexSize-=m_Size;
	m_lpIndexBuffer=NULL;
	m_Flag=0;
	m_Size= 0;
}
BYTE* CIndexBuffer::VPLock(int start,int size,DWORD option)
{
	BYTE* bp=NULL;

	if(m_lpIndexBuffer == NULL)
		return NULL;
	switch(m_Flag)
	{
	case _USE_DYNAMIC_VP:
		m_lpIndexBuffer->Lock( 
			start,size, (BYTE**)&bp
//			, D3DLOCK_NOOVERWRITE );
//			, D3DLOCK_DISCARD  );
			, option );
//		,0);//start ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
		break;
	case _USE_STATIC_VP:
		m_lpIndexBuffer->Lock( 
			start,size, (BYTE**)&bp, 0 );
		break;
	case _USE_MEMORY_VP:
		bp=(BYTE *)m_lpIndexBuffer;
		break;	
	}
	if( bp== NULL )
		Error("���ؽ����۸� lock �Ҽ��� �����ϴ�.","");
	return bp;
}

void CIndexBuffer::VPUnLock()
{
	if(m_lpIndexBuffer == NULL)
		return;
	if( m_Flag == _USE_MEMORY_VP )
		return;

	m_lpIndexBuffer->Unlock();
}
 
void ResetTotalVertexBufferInfo()
{
	stTotalVertexSize=0;
	stTotalVertexCnt=0;
}

DWORD GetTotalVertexBufferSize()	//�ε������۵� ����.
{
	return stTotalVertexSize;
}

DWORD GetTotalVertexBufferCnt()		//�ε��� ��������.
{
	return stTotalVertexCnt;
}