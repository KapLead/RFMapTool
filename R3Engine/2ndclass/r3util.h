#ifndef _R3UTIL_H_
#define _R3UTIL_H_

#include "C_vector.h"
#include "d3dx8math.h"
#include "d3d8.h"
#include "r3move.h"
#include "r3render.h"

#define _R3PI 3.1415926535

#define PiToAngle(a)  (((double)(a)*360.0)/(2.0*_R3PI))
#define AngleToPi(a)  ((2.0*_R3PI*(double)(a))/360.0)

float GetDist(Vector3f Pos,Vector3f Tar);	//�� ���Ͱ��� �Ÿ��� ���Ѵ�.
float GetYAngle(Vector3f pos,Vector3f tar);	// ������ġ(pos)�� ������ġ(tar) ����(360)�� �����Ѵ�.
BOOL GetYAngle(float *get_yangle, Vector3f Pos,Vector3f Tar);	// ����(360)�� �����Ѵ�. ����ġ�� �ʹ� ������ FALSE�� 
float GetXAngle(Vector3f Pos,Vector3f Tar);	// ����(360)�� �����Ѵ�.

// A �� B�� �մ� ������ A�κ��� dist��ŭ ������ ��ǥ�� ��� �Լ���.
void GetPosByDistFromATOB(Vector3f *get,Vector3f src,Vector3f tar,float dist);


BOOL GetTransformVertex(Vector4f *xyzw,Vector3f pos);	//pos���� ��ũ����ǥ�� �����´�.
//�� Ʈ������ trans_pos[0]�� ��ũ��x, trans[1]�� ��ũ��y, trans[2] ��� z��ǥ
void GetInverseTransformVertex( float get[3],float trans_pos[3]);

void GetViewMatrix(D3DXMATRIX *get_matrix);
void GetProjectMatrix(D3DXMATRIX *get_matrix);
//���� ���̷��� ����Ʈ ���͸� �ٲ��ش�.
void GetMainDirLight(Vector3f *get_pos);
void SetMainDirLight(Vector3f pos);
void GetMatLightFromColor( D3DLIGHT8 *light,D3DMATERIAL8 *mtrl,DWORD color,float density=1.0f );

void GetBBoxRotate(Vector3f *get_bb_min,Vector3f *get_bb_max,Vector3f bb_min,Vector3f bb_max,float y_angle);
void GetHeadMatrix(D3DXMATRIX *get_mat,R3Camera *camera,float yangle,LONG mouse_x,LONG mouse_y);

//���͸� ��Ʈ������ ����ִ� ��ƿ.
//Ȱ��� ���غ��ʹ� {0,0,-1}�ƽ����� ����Ʈ�ǰ��.
//�Ƕ���� �׷��Ŵ� {0,1,0}�ƽ����� top�ǰ�� 
void GetMatrixFromVector(float get_mat[4][4],Vector3f vec,Vector3f axis);
//void GetMatrixFromAtoB(Vector3f a,Vector3f b);	//a�� b�� ���� ������ ��Ʈ������ ���Ѵ�.
void GetMatrixFromAtoB(float get_mat[4][4],Vector3f a,Vector3f b);	//a�� b�� ���� ������ ��Ʈ������ ���Ѵ�.


//z���� ������ Ŭ����.
void Clean2DRectangleZbuffer(LONG x,LONG y,LONG ex,LONG ey);
// ��ũ�� ��ǥ �������ٿ�� �ڽ��� �̵����� ������ �Լ�. 2d������ 3D������Ʈ�� ������ ��Ȯ�� �°ԻѸ��� ����.
void Get2DTo3DTranslation(Vector3f *get,Vector3f bb_min,Vector3f bb_max
					  ,float sx,float sy,float ex,float ey,float scale=1.0f);

//------------------------ Ư�� ó�� �Լ���...
//���� ȿ��
void R3GetQuakeMatrix(float mat[4][4]);	//Quake ��Ʈ������ ���������.
void R3GetQuakeVector(Vector3f pos);	//Quake ���͸� ���������.
void R3EnvironmentQuake(float time,float density);


//���̳��Ͷ���Ʈ...�ŷ������� �־��־�� �Ѵ�...
void ClearDynamicLight();
BOOL InsertDynamicLight(Vector3f pos,float diameter,DWORD color,void *texture=0,BOOL magic_light=FALSE,DWORD blend_type=_BLEND_BRIGHT);
DWORD GetDynamicLightNum();
void GetDynamicLight(DWORD id,Vector3f pos,float *diameter,DWORD *color,void **texture,DWORD *blend_type);
BOOL IsMagicLight(DWORD id);	//��������Ʈ�� �������̴�.
void GetDynamicLightBBox(DWORD id,Vector3f *bbmin,Vector3f *bbmax);


//�����츦 ���� ����Ʈ ��ġ ����.
void SetLightForShadow(Vector3f pos);//�����츦 ���� ����Ʈ ��ġ...
void GetLightForShadow(Vector3f pos);//�����츦 ���� ����Ʈ ��ġ...
void PushLightForShadow();//�����츦 ���� ����Ʈ ��ġ ����
void PopLightForShadow();//�����츦 ���� ����Ʈ ��ġ ����

//���� �̸����� ��ƼŬ�ΰ� Ȯ���ϱ�..
BOOL IsParticle(char *name);


// ī�޶� ��ǥ�� ����ִ� ��ƿ��Ƽ...
void GetCameraPos(Vector3f *get_pos);	//ī�޶� ������� ��ġ����.
float GetDensityFromPos(Vector3f pos);	//������ ��ġ�� ī�޶���� �Ÿ��� ������ �������ش�. ȿ�����̳� ����ũ���� ����.

//���̵��� ���̵� �ƿ�
BOOL DrawFadeScreen();	//���ϰ��� ���̵��� �� �����϶� TRUE,�������̳� ���̵� �ƿ��Ȼ��´� FALSE
BOOL IsFadeOut();		//���̵�ƿ� �� �����ΰ�?
BOOL IsFadeIn();		//���̵��ε� �����ΰ�?
BOOL IsFading();		//���̵尡 �������ΰ�?
void SetFadeOut(DWORD color,float time,float density=1.0f);		//�ش�Ǵ� �÷��� ���̵�Ǵ� �ð�.	density�� ������ �ش���� �����̴�.
void SetFadeIn(DWORD color,float time,float density=1.0f);			//�ش�Ǵ� �÷��� ���̵�Ǵ� �ð�.
void SetFadeFactor(DWORD color,float alpha,void *tex=NULL);	//���̵� �ξƿ� ���� ����.���İ� 1�� �������� �ش� ���̵� ���� ���ϴ�.

// �ϴ� ���̵��� �ƿ�.
void SetFadeSky(DWORD color,float fade_time);		//�ش�Ǵ� �÷��� ���̵�Ǵ� �ð�.
BOOL DrawFadeSKy();

//�ؽ��� ���̵��� �ƿ�.
void SetFadeTex(DWORD color,float fade_time,void *tex);		//�ش�Ǵ� �÷��� ���̵�Ǵ� �ð�.
BOOL DrawFadeTex(void);

//��ũ����
void FramebufferToBMP(char *name);
void FramebufferToJPG(char *name);
DWORD PixelFromFramebuffer(short cl_x,short cl_y);

//�簢���ڽ��� �׸���.
void DrawTestBox(float hor,float ver,float height,DWORD color);
void CleanZbuffer(float sx,float sy,float w,float h);	//�簢������ zbuffer�� Ŭ�����Ѵ�.


//��θ��� ���� ���ؽ� ���۵�.
void CreateBlurVBuffer(DWORD width,DWORD height);
void DrawBlurVBuffer(DWORD level);
void ReleaseBlurVBuffer();


#endif


