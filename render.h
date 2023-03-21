#ifndef __MY_RENDER__
#define __MY_RENDER__

#include <tchar.h>
#include <vector>
#include <List>
//#include "D3DUtil.h"
#include "Import.h"
#include "Texture.h"


using namespace std;
////////////////////////////////////////////////////////////////////////////
#define CV_WORLDVIEWPROJ_0	0
#define CV_WORLDVIEWPROJ_1	1
#define CV_WORLDVIEWPROJ_2	2
#define CV_WORLDVIEWPROJ_3	3
#define CV_LIGHT_DIRECTION	4
#define CV_CONSTANTS		5
#define CV_LIGHT_DENS		6
#define CV_BONESTART		7

VOID InitMaterial( D3DMATERIAL8& mtrl, FLOAT r, FLOAT g, FLOAT b,
                           FLOAT a = 1.0);


class ObjectData;
class AnimationData;
//
struct OpaAni;
struct TexEffect;
/*
struct CHARACTERVERTEX
{
    FLOAT		x, y, z;		// The transformed position for the vertex
    FLOAT       blend;			// Referenced as v1.x in the vertex shader
    DWORD		index1;
	D3DXVECTOR3 n;				// Referenced as v3 in the vertex shader
    FLOAT       u, v;			// Referenced as v7 in the vertex shader
};
//*/
class CHARACTERVERTEX
{
public:
	D3DXVECTOR3 Position;
	float blend;
	//float index1;  //two indices, one for each bone per vertex
	D3DXVECTOR3 n;
	D3DXVECTOR2 Texture;
};



struct SHADERVERTEX
{
	D3DXVECTOR3 Position;
	float blend, blend2;
	float index1, index2;  //two indices, one for each bone per vertex
	D3DXVECTOR3 n;
	D3DXVECTOR2 Texture;
};


#define D3DFVF_CHARACTERVERTEX (D3DFVF_XYZB1|D3DFVF_NORMAL|D3DFVF_TEX1)
//#define D3DFVF_CHARACTERVERTEX (D3DFVF_XYZB2|D3DFVF_LASTBETA_UBYTE4|D3DFVF_NORMAL|D3DFVF_TEX1)
/*
struct CHARACTERVERTEX
{
	FLOAT		x, y, z;		// The transformed position for the vertex
    FLOAT       blend[2];		// Referenced as v1.x in the vertex shader
	BYTE		bid[4];

	D3DXVECTOR3 n;
    FLOAT       u, v;			// Referenced as v7 in the vertex shader
	
	
};
#define D3DFVF_CHARACTERVERTEX (D3DFVF_XYZB3|D3DFVF_LASTBETA_UBYTE4|D3DFVF_NORMAL|D3DFVF_TEX1)
*/
//#define D3DFVF_CHARACTERVERTEX (D3DFVF_XYZB3|D3DFVF_LASTBETA_UBYTE4|D3DFVF_DIFFUSE|D3DFVF_TEX1)
//

struct EDGE {
	D3DXVECTOR3			m_Vector[2];
	D3DXVECTOR3			m_Fnormal[2];//인접한 두 FaceNormal
	DWORD				m_MaxFace;//1~2
	float				m_Blend[2];
	EDGE(){m_MaxFace = 0;}
};


//
#define	MAXPARENT	4

struct ObjectMesh
{
	char				m_Name[256];
	char				m_ParentName[256];
	bool				m_HaveParent;
	//
	bool				m_NewParent;
	float				m_BackupTransMax;
	//
	ObjectMesh			*m_pParent;
	
	DWORD				m_BlendNum;
	ObjectMesh			*m_pBlendParent[MAXPARENT];
	D3DXMATRIX			*m_pMatBlend;
	
	LPDIRECT3DVERTEXBUFFER8 m_pVB;					//Vertex Buffer
	LPDIRECT3DINDEXBUFFER8  m_pIB;					//Index Buffer
	
	CHARACTERVERTEX		*m_pVertexList;
	
	D3DXMATRIX			m_TransformOrg;
	D3DXMATRIX			m_Transform;
	D3DXMATRIX			m_TransformInv;
	D3DXMATRIX			m_LocalTransform;
	D3DXMATRIX			m_LocalScale;
	D3DXMATRIX			m_MatResult;
	D3DXMATRIX			m_BefMatResult;
	D3DXMATRIX			m_BefMatLocal;
	D3DXMATRIX			m_TMRotate;

	DWORD				m_MaxVertex;			
	DWORD				m_MaxIndex;
	///////////////////////////////			Animation			////////////////////////////////
	bool				m_HaveAnimation;
	AnimationData		*m_pAnimation;
	DWORD				m_MaxFrame;
	/////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////			Texture				////////////////////////////////
	LPDIRECT3DTEXTURE8	m_Texture;
	DWORD				m_TextureID;
	/////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////			Texture	2			////////////////////////////////
	LPDIRECT3DTEXTURE8	m_Texture2;
	DWORD				m_TextureID2;
	DWORD				m_BlendFactor;
	/////////////////////////////////////////////////////////////////////////////////////////////
	TexEffect*			m_TexEffect;
	DWORD				m_TexEffectFrame;
	DWORD				m_TexEffectTime;
	/////////////////////////////////////////////////////////////////////////////////////////////
	BYTE				m_bAlphaBlend;
	BYTE				m_AlpahSrc;
	BYTE				m_AlpahDes;
	/////////////////////////////////////////////////////////////////////////////////////////////
	
	float				m_AlphaBlend;
	/////////////////////////////////////////////////////////////////////////////////////////////
	BYTE				m_bTwoSide;
	/////////////////////////////////////////////////////////////////////////////////////////////
	BYTE				m_bLighting;
	/////////////////////////////////////////////////////////////////////////////////////////////
	float				m_MaxPoint[3];
	float				m_MinPoint[3];
	/////////////////////////////////////////////////////////////////////////////////////////////
	float				m_Scale[3];	
	/////////////////////////////////////////////////////////////////////////////////////////////
	BYTE				m_PIndex[256];				//Palette Index
	/////////////////////////////////////////////////////////////////////////////////////////////
	DWORD				m_MaxEdge;
	DWORD				m_MaxSilEdge;
	EDGE				*m_pEdge;
	D3DXVECTOR4			*m_pCurEdge;
	D3DXVECTOR4			*m_pSilEdge;
	D3DXVECTOR4			*m_pCurNormal;
	/////////////////////////////////////////////////////////////////////////////////////////////
	BOOL				m_bBoneParent;

	/////////////////////////////////////////////////////////////////////////////////////////////
	BOOL				m_bEyePos;
	D3DXMATRIX			m_MatEyeVec;
	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	BOOL				m_bHide;
	//
	ObjectMesh();
	~ObjectMesh();
	/*
	{
		if (m_pVB)m_pVB->Release();
		if (m_pIB)m_pIB->Release();
		SAFE_DELETE(m_pAnimation);
		SAFE_DELETE_ARRAY(m_pMatBlend);
		SAFE_DELETE(m_TexEffect);
	}
	*/
};


class CHARACTEROBJECT
{
public:
	DWORD				m_ID;
	char				m_Name[256];
	DWORD				m_MaxObj;		
	ObjectMesh			*m_pMesh;	
	//View
	D3DXMATRIX			m_Rotate;
	D3DXMATRIX			m_Eye;

	//FrameStep 
	DWORD				m_BeforeTime;
	DWORD				m_Frame;
	DWORD				m_OMaxFrame;

	BOOL				m_Transition;
	BOOL				m_TransitionStart;
	DWORD				m_TransFrame;
	DWORD				m_TransTime;
	//Material
	D3DMATERIAL8		m_Mtrl;
	//Collision Detection
	D3DXVECTOR4			*m_pCurVxList;
	CHARACTEROBJECT()
	{
		m_ID			= 0;
		m_MaxObj		= 0;
		
		m_Frame			= 0;
		m_BeforeTime	= 0;
		m_OMaxFrame		= 0;
		
		D3DXMatrixIdentity(&m_Eye);
		D3DXMatrixIdentity(&m_Rotate);
		//
		InitMaterial( m_Mtrl, 1.0f, 0.0f, 0.0f );
		m_Mtrl.Diffuse.r = m_Mtrl.Diffuse.g = m_Mtrl.Diffuse.b = 0.6f;
		m_Mtrl.Ambient.r = m_Mtrl.Ambient.g = m_Mtrl.Ambient.b = 0.3f;
		//
		m_pCurVxList	= NULL;
		
		m_TransitionStart= false;
		m_Transition	= false;
		m_TransFrame	= 0;
		m_TransTime		= 3000;
	}
	~CHARACTEROBJECT()
	{
		SAFE_DELETE_ARRAY( m_pMesh);
		SAFE_DELETE_ARRAY( m_pCurVxList);
	}
};


typedef vector<CHARACTEROBJECT*>	CHLIST;

#define COBJNAMELEN		128
class CObjectManager
{
	
	CHLIST	m_List;
public:
	DWORD	m_IDCounter;
	char	m_Name[COBJNAMELEN];
	CObjectManager(){m_IDCounter = 0;}
	~CObjectManager();
public:
	CHLIST* GetList()	{	return &m_List;	}
	void AddCharacter(CHARACTEROBJECT *co);
	CHARACTEROBJECT* GetCharacter(char  *name);
	CHARACTEROBJECT* GetCharacter(DWORD name);
	DWORD			 GetID(char *name);

	void DeleteCharacter(char *name);
	void DeleteCharacter(DWORD id);
};

typedef	std::vector<CObjectManager*>	MESHVECTOR;

class CMeshManager
{
	MESHVECTOR				m_vMesh;				//MeshManager
public:
	~CMeshManager();
	MESHVECTOR*		GetPartVector()	{ return &m_vMesh;}
	void			AddObjectManager(CObjectManager *om);

	CObjectManager* GetObjectManager(char *name);
	void			DeleteObjectManager(char*name);
};




#define VIEWER_FAR_PLANE	(1500.0f)
#define VIEWER_NEAR_PLANE	(1.0f)
#define MAX_VB				100	

class CRender
{
	
	static CRender*			m_Render;
	LPDIRECT3DDEVICE8		m_pd3dDevice;
	DWORD					m_dwFogColor;			//BackgroundColor
	D3DLIGHT8				m_Light;
public:
	
	CObjectManager			m_OM;					//MeshManager
	CObjectManager			m_BM;					//BoneManager
	//
	//Texture//

	TextureManager			m_TM;
	//
	CMeshManager			m_MM;
	DWORD m_dwCurrentShader;
	//
	bool					m_bClip;
	float					m_FrustumPlane[6][4];
	//
	bool					m_bShader;
public:
	CRender(){m_Render = this;Init();}
	~CRender() ;
	static CRender&  GetRender() { return (*m_Render);}
	
public:
	
	void Init();
	
	//void InitMeshManager(int n);
	//void DeleteMeshManager();

	void MakeVB(LPDIRECT3DVERTEXBUFFER8 *vb,CHARACTERVERTEX *vx,DWORD n);
	void MakeIB(LPDIRECT3DINDEXBUFFER8 *ib,WORD *ix,DWORD n);

	void InitEviroment();
	void SetState();
	
	void UnSetState();
	void SetDevice(LPDIRECT3DDEVICE8 device){m_pd3dDevice = device;m_TM.SetTextureDevice(device);}
	LPDIRECT3DDEVICE8 GetDevice(){return m_pd3dDevice;}
	
	void FrameMove(CHARACTEROBJECT *co);
	void FrameStep(CHARACTEROBJECT *co);
	void CRender::DrawCharacter(CHARACTEROBJECT *co,float pos[3],float roty,float sc=1.0f);
	void CRender::DrawCharacter1(CHARACTEROBJECT *co,float pos[3],float roty,int m,D3DXMATRIX &eye);
	void SetClip(float	FrustumPlane[6][4]){
		memcpy(m_FrustumPlane,FrustumPlane,sizeof(m_FrustumPlane));
		m_bClip = true;
	}
	void CRender::SetLight(D3DLIGHT8 light);
	
	D3DXVECTOR4* GetCurrentfacelist(CHARACTEROBJECT *co,int *max,float roty);
	void  MakeEdgelist(CHARACTEROBJECT *co);
	void CRender::EdgeFrameMove(CHARACTEROBJECT *co,float roty);
	void DrawEdge(CHARACTEROBJECT *co);
	void CRender::GetSilhouette(CHARACTEROBJECT *co,D3DXVECTOR3 &eye);
	void CRender::DrawSilhouette(CHARACTEROBJECT *co);
};

#endif

