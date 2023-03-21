

#ifndef __CVIEWER_IMPORT__
#define __CVIEWER_IMPORT__

#include <tchar.h>
//#include "AseLoader.h"
#include <D3DX8.h>
//#include "DXUtil.h"
#include "Texture.h"


#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

typedef enum tagEBSHADERTYPE
{
	SHADERTYPE_VERTEX = 0,
	SHADERTYPE_PIXEL = 1,
	SHADERTYPE_FORCEDWORD = 0xFFFFFFFF
} EBSHADERTYPE;

////////////////////////////////////////////////////////////////////////////////
#define MAXBPARENT	4
#define	MAX_STR_NAME	100
////////////////////////////////////////////////////////////////////////////////
class			ObjectData;
class			CHARACTEROBJECT;
class			ChAnimation;
struct			ObjectMesh;
////////////////////////////////////////////////////////////////////////////////
ObjectData* ImportMeshData(char *fname);
BYTE * ReadAnimationData(char *fname);
void	FindMaxAni(ChAnimation *ani,BYTE *buffer);
void	ImportAnimationData(CHARACTEROBJECT *co,BYTE *buffer);
//void	ImportTextureData(TextureManager *tm,char *fname);
void	ImportTextureData(TextureManager *tm,char *path,char *fname);
void	InitVertex(ObjectData* o,CHARACTEROBJECT *bone,CHARACTEROBJECT *co,TextureManager *tm,bool twoblend);
void	ScaleLoad(char *str,CHARACTEROBJECT *co);
HRESULT LoadAndCreateShader(LPDIRECT3DDEVICE8 device,const std::string& strFilePath, const DWORD* pDeclaration, DWORD Usage, DWORD* pHandle);
BOOL ImportBoundBox(char *str,char name[256],float min[3],float max[3]);
int RematchParent(ObjectMesh *bone,int bmax,ObjectMesh *omesh,int max);
////////////////////////////////////////////////////////////////////////////////
struct POINT3D {
	float x, y, z;
};
struct MOTION {
	float	x, y, z;
	int		frame;
};
struct ALPHA
{
	float	a;
	DWORD	frame;
};
struct QUATERNION
{
	D3DXQUATERNION q;
	int	frame;
};

struct PHYSIQUE
{
	int		VertexNum;
	int		NodeNum;
	char	ParentsName[MAXBPARENT][100];
	float	Weight[MAXBPARENT];
};	


struct PHYSIQUE2
{
	int		VertexNum;
	int		NodeNum;
	int		ParentNum[MAXBPARENT];
	float	Weight[MAXBPARENT];
};	

struct EXPTVERTEX {
	float x, y, z, w;
	float nx, ny, nz;
};


struct EXPTPOINT3D_1 {
	BYTE x, y, z;
};

typedef struct tagTEXCOORDFLOAT {
	union {
		float u;
		float uz; 
	};
	union {
		float v;
		float vz;
	};
	float oz;
} TEXCOORDFLOAT;

/*
struct EXPTPOINT3D_2 {
	WORD x, y, z;
};
*/
struct EXPTFACE {
	int						VertexIndex[3];
	POINT3D					VertexNormal[3];
	TEXCOORDFLOAT			VertexTexture[3];
	LONG					MaterialID;
};
///////////////////////////////////////////////////
class	AnimationData {
public:
		//Rotate Animation
		WORD			m_MaxRotate;
		QUATERNION		*m_pQuater;//회전

		//Translate Animation
		WORD			m_MaxTranslate;
		MOTION			*m_pMotion;//이동 
		
		//Scale Animation
		WORD			m_MaxScale;
		MOTION			*m_pScale;

		//Alpha Animtion
		WORD			m_MaxAlpha;
		ALPHA			*m_pAlpha;

		AnimationData()
		{
			m_pQuater = NULL;
			m_pMotion = NULL;
		}
		~AnimationData()
		{
			//SAFE_DELETE_ARRAY(m_pQuater);
			//SAFE_DELETE_ARRAY(m_pMotion);
		}
};

#pragma pack( push, 1)
class	AnimationHeader{
public:
		char			m_Name[MAX_STR_NAME];
		WORD			m_MaxAnimationFrame;
		WORD			m_MaxAnimation;
		//
		char			Reserved[36];
};
#pragma pack( pop)

class	Animation {
public:
		AnimationHeader m_Header;
		AnimationData	*m_pAnimation;
};
///////////////////////////////////////////////////

class	TextureData {
public:
		char		m_TexName[256];	
		int			m_TwoSide;
};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

class	BoneMeshData {
public:
		//Matrix
		char			m_Name[32];
		char			m_NameParent[32];

		D3DXMATRIX			m_Transform;
		D3DXMATRIX			m_LocalTransform;
		D3DXMATRIX			m_LocalScale;
};
class	BoneData
{
public:
		//BoneType
		WORD			m_BoneType;
		//BoneMeshData
		WORD			m_MaxBoneMesh;
		BoneMeshData	*m_pBoneMesh;
};


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


#pragma pack( push, 1)

class	MeshPrototype{
public:
		char			m_Name[MAX_STR_NAME];
		char			m_ParentName[MAX_STR_NAME];
		//Matrix
		D3DXMATRIX			m_Transform;
		D3DXMATRIX			m_LocalTransform;
		D3DXMATRIX			m_LocalScale;
		
		//Vertex
		WORD			m_MaxVertex;
		//Face
		WORD			m_MaxFace;
		//Physique
		WORD			m_MaxPhysique;
		//Texture
		char			m_TextureName[MAX_STR_NAME];
		char			m_TextureName2[MAX_STR_NAME];
		DWORD			m_BlendFactor;
		//
		float			m_MaxPoint[3];
		float			m_MinPoint[3];
		//
		//Effect//////////////////////////////////////////////////////////
		//Texture Effect//5BYTE
		BYTE			m_TEWidth;			
		BYTE			m_TEHeight;
		BYTE			m_TEMaxFrame;
		WORD			m_TESpeed;
		//
		BYTE			m_bAlphaBlend;
		BYTE			m_SrcBlend;
		BYTE			m_DesBlend;
		
		//Lighting//1BYTE
		BYTE			m_bLighting;
		//BlendAni//4BYTE
		WORD			m_BHideTime;
		WORD			m_BShowTime;
		BYTE			m_TwoSide;
		//Effect//////////////////////////////////////////////////////////
		WORD			m_BHideTime2;
		DWORD			m_Version;
		BYTE			m_Reserved[47];
		//
};
#pragma pack( pop)

class	MeshData {
public:
		MeshPrototype	m_PType;
		EXPTVERTEX		*m_pVertex;
		EXPTFACE		*m_pFace;
		PHYSIQUE		*m_pPhysique;
		//////////////////////////////////////////////////////////////////
		DWORD			m_ParentNum;
		char			*m_pParentName;			
		PHYSIQUE2		*m_pPhysique2;
		
		MeshData()
		{
			m_pVertex = NULL;
			m_pFace   = NULL;
			m_pPhysique= NULL;
			m_pPhysique2 = NULL;
			m_pParentName = NULL;
		}
		~MeshData()
		{
			SAFE_DELETE_ARRAY (m_pVertex);
			SAFE_DELETE_ARRAY (m_pFace);
			SAFE_DELETE_ARRAY (m_pPhysique);
			SAFE_DELETE_ARRAY (m_pPhysique2);
			SAFE_DELETE_ARRAY (m_pParentName);
		}
};

class	ObjectData {
public:
		//BoneType 
		WORD			m_BoneType;
		//Mesh
		WORD			m_MaxMesh;
		MeshData		*m_pMeshData;

		//Animation
		WORD			m_MaxAnimation;
		Animation		*m_pAnimation;

		//Texture
		WORD			m_MaxTexture;
		TextureData		*m_pTexture;
		
		DWORD			m_MeshVersion;
		ObjectData()
		{
			m_pAnimation	=	NULL;
			m_pMeshData		=	NULL;
			m_pTexture		=	NULL;
		}
		~ObjectData()
		{
			SAFE_DELETE_ARRAY(m_pMeshData);
			SAFE_DELETE_ARRAY(m_pAnimation);
			SAFE_DELETE_ARRAY(m_pTexture);
		}
};


#endif