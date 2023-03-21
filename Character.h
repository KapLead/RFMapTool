#ifndef __CHARACTER__
#define __CHARACTER__

#include <D3DX8.h>
#include "render.h"
#include "texture.h"
#include "Animation.h"

class CHARACTEROBJECT;
class	ChInterface : public CRender 
{
	
public:
//////////////////////////////////////////////////////////////////////////////////////	
	void				LoadAnimationData(CHARACTEROBJECT *co,char *fname);
	void				InitCharacter(LPDIRECT3DDEVICE8 pDevice);
//////////////////////////////////////////////////////////////////////////////////////	
	//CHARACTEROBJECT* 	LoadMeshData(TextureManager *tm,char *fname,bool twoblend,CHARACTEROBJECT *bone);
	CHARACTEROBJECT* 	LoadMeshData(char *fname,bool twoblend,CHARACTEROBJECT *bone = NULL);
	CHARACTEROBJECT*	LoadMeshData(CObjectManager *om,char *fname,bool twoblend,CHARACTEROBJECT *bone= NULL);
	CHARACTEROBJECT*	ChInterface::LoadMeshData(TextureManager *tm,CObjectManager *om,char *fname,bool twoblend,CHARACTEROBJECT *bone=NULL);
	//void				LoadMeshData(TextureManager *tm,char *fname,bool twoblend,CHARACTEROBJECT *bone);
	//CHARACTEROBJECT*	LoadMeshData(CObjectManager *om,TextureManager *tm,char *fname,bool twoblend,CHARACTEROBJECT *bone);
	
	//CHARACTEROBJECT*	LoadBoneData(TextureManager *tm,char *fname,bool twoblend);
	CHARACTEROBJECT*	ChInterface::LoadBoneData(char *fname,bool twoblend);
	CHARACTEROBJECT*	GetMeshData(CObjectManager *om,char *name);
	CHARACTEROBJECT*	GetMeshData(CObjectManager *om,DWORD id);
	DWORD				GetID(CObjectManager *om,char *name);
	CHARACTEROBJECT*	GetMeshData(char *name);
	CHARACTEROBJECT*	GetBoneData(char *name);
	void				DeleteMeshData(char *fname);
	void				DeleteMeshData(DWORD id);
	void				DeleteBoneData(char *name);
	void				DeleteBoneData(DWORD id);
//////////////////////////////////////////////////////////////////////////////////////		
	//void				LoadTextureData(TextureManager	*tm,char *fname);
	void				LoadTextureData(TextureManager	*tm,char *path,char *fname);
	void				MatchTextureToMesh(TextureManager	*tm,CHARACTEROBJECT *co);
	void				DeleteTextueData(char *fname);
//////////////////////////////////////////////////////////////////////////////////////		
	ChAnimation*		AddAnimationData(AnimationManager *am,char *anifilename);
	void				AnimationReset(CHARACTEROBJECT *co);
	ChAnimation*		GetAnimationData(AnimationManager *am,char *aniname);
	void				MatchAnimationToMesh(ChAnimation  *ani,CHARACTEROBJECT *co);
	void				DeleteAnimationData(AnimationManager *am,char *anifilename);
	void				DeleteAnimationData(AnimationManager *am,DWORD aniid);
//////////////////////////////////////////////////////////////////////////////////////	
	void				AddPartToMM(char *name);
	CObjectManager*		GetPart(char *name);
	//
	void				SetTexPath(char *path){ m_TM.SetTexPath(path);}
//////////////////////////////////////////////////////////////////////////////////////	
	void				LoadScale(char *sclfname,char * bname){ ::ScaleLoad(sclfname,GetBoneData(bname));} 
	void				LoadScale(char *sclfname,CHARACTEROBJECT *bn){ ::ScaleLoad(sclfname,bn);} 
//////////////////////////////////////////////////////////////////////////////////////	
//	Effect
	void				SetAlpha(CHARACTEROBJECT *co,float den);
	void				SetMaterial(CHARACTEROBJECT *co,D3DMATERIAL8 &m);
//////////////////////////////////////////////////////////////////////////////////////
	void 				LoadVertexShader(char *name);
	void				SetVertexShader(D3DXMATRIX worldViewProjMat,CHARACTEROBJECT *bone);
	BOOL				ImportBoundBox(char *fname,float min[3],float max[3]);

	void				SetNewParent(CHARACTEROBJECT	*p,char *name,D3DXMATRIX &m,CHARACTEROBJECT	*o);
	void				RestoreParent(CHARACTEROBJECT	*o);
	void				SetTransition(CHARACTEROBJECT	*co,DWORD max) {
							co->m_Transition = true;
							co->m_TransFrame = 0;
							co->m_TransitionStart= true;
							co->m_TransTime = max;
						}
	void				RematchParent(CHARACTEROBJECT *bn,CHARACTEROBJECT *obj);
//////////////////////////////////////////////////////////////////////////////////////
	void				SetEyeVec(char * name,CHARACTEROBJECT *bn,D3DXMATRIX eye) ;
	
};


#endif