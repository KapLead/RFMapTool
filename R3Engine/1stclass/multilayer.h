#ifndef __MULTILAYER_H__
#define __MULTILAYER_H__

#include "windows.h"
#include "R3Material.h"
#include "R3Render.h"

#define _NONE_MATRIX_FLAG (_UV_ENV|_ANI_TEXTURE)	//��Ʈ������ �����ϴ� �͵�
#define _FLICKER_FLAG (_ANI_ALPHA_FLICKER)
#define _MATRIX_FLAG (_UV_METAL|_UV_SCROLL_U|_UV_SCROLL_V|_UV_ROTATE|_UV_SCALE)	//��Ʈ������ ����
//#define _NONE_TEXTURE_MATRIX_FLAG (_MATRIX_FLAG | _ANI_TEXTURE|_ANI_ALPHA_FLICKER|_UV_GRADIENT_ALPHA_UV|_MAT_VOLUME_FOG)
#define _NEED_LOCK_FLAG (_UV_LAVA)	//���ؽ� ���� ���� �ʿ��� ���.

void SetMultiLayerTime(float ftime);
void SetMultiLayerCamera(Vector3f camera);
float GetMultiLayerTime();		//��Ƽ���̾ ���̴� Ÿ��.

int GetTileAniTextureAddId(_R3MATERIAL *mat,int l_id,float start_time=0);	//Ÿ�Ͼִ��ؽ����߰� ���̵� ���´�.

//-�ʿ�
void MakeUV(_D3DR3VERTEX_TEX2 *tex2,int v_num,void *source_uv,void *source_st,_R3MATERIAL *mat,int l_id,float start_time=0);
void MakeUVExt(_D3DR3VERTEX_TEX2 *tex2,int v_num,void *source_uv,void *source_st,_R3MATERIAL *mat,int l_id,float start_time=0);
//-��ī�̹ڽ���
void MakeUV(_D3DR3VERTEX_TEX1 *tex1,int v_num,void *source_uv,_R3MATERIAL *mat,int l_id);
BOOL GetTextureMatrix(_R3MATERIAL *mat,int l_id,D3DXMATRIX *uv_matrix,float start_time=0);


#endif