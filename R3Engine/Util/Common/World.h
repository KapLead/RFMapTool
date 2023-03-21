#ifndef __WORLD_H__
#define __WORLD_H__

#include "d3d.h"
#include "d3dmath.h"
//#include <d3dutil.h>
#include <windows.h>
#include "C_vector.h"

#define _PI 3.1415926535
#define _R3T_FILE_VERSION	1.1

#pragma	pack(push, 1)		//����Ʈ ����.

typedef struct{
	union{
	float local[1];
	float x;
	};
	float y,z;
}_VERTEX;

typedef struct{
	union{
	float local[1];
	float x;
	};
	float y,z;
	union{
	float lgt_uv[1];
	float lu;
	};
	float lv;
	float normal[3];	
}_VERTEX_UV;

typedef struct{
	Vector2f uv;
}_TVERTEX;

typedef struct{
    Vector3l tvnum;
}_TVFACE;

typedef struct{
    Vector3l v;		// �ﰢ��	�ε���
    Vector3l tv;	// �ؽ���	�ε��� 
    Vector3l tv2;	// �ؽ���	2��ä��.�ε��� 
    
    Vector3f fu;
    Vector3f fv;
    int mtlid;      //material id
	Vector4f f_normal;	//��� ������
	DWORD smooth_group;			//0�̸� ������ �׷��� ������� �ʴ´�. ��Ʈ�ʵ�
//	Vector3f v_normal[3];

	//------------ ���۾� ����Ʈ����ǥ�� ����...
    Vector3f lu;	//����Ʈ�� u,v��ǥ 
	Vector3f lv;

/*
// ����Ʈ���� ���� 
	int l_mtlid;	//����Ʈ�� ���͸��� ID
	_LIGHT_MAP *lgt_ptr;
    Vector3f lu;	//����Ʈ�� u,v��ǥ 
	Vector3f lv;
*/
}_FACE;		//�ﰢ����ü.

typedef struct{
	BYTE B,G,R;
}_LIGHTMAP_COLOR;

typedef struct{
	int xl,yl;
	_LIGHTMAP_COLOR *bitmap;	//ARGB 8888 ���� 
}_LIGHT_MAP;

//----------

typedef struct{
	LONG vertex_num;
	LONG *vertex_id;
	Vector3f *v_normal;
	float *fv,*fu;
	float *lv,*lu;
	DWORD smooth_group;			//0�̸� ������ �׷��� ������� �ʴ´�. ��Ʈ�ʵ�
	Vector4f f_normal;	//��� ������
	DWORD f_normal_id;

	union{
	LONG temp;		//BSP���� ����ϴ� �������� 
	float f_temp;
	};
	DWORD	argb;		//�÷��� ���İ�...
	SHORT mtlid;	//���͸��� ���̵�
	SHORT l_mtlid;	//����Ʈ�� ���͸��� ���̵�
	_LIGHT_MAP *lgt_ptr;	//����Ʈ�� ������
}_POLYFACE;		//�ٰ����� ���� ����ü...
//----------


typedef float _MATRIX[4][4];  

typedef struct{
	float frame;		//������ ��ȣ ( ��:12.1 ������)
	float x,y,z;
}_POS_TRACK;			//��ġƮ��
typedef struct{
	float frame;		//������ ��ȣ ( ��:12.1 ������)
	float x,y,z,w;		//���ʹϿ�
}_ROT_TRACK;			//ȸ�� Ʈ��
typedef struct{
	float frame;		//������ ��ȣ ( ��:12.1 ������)
	float x,y,z;		
	float qx,qy,qz,qw;	//������ ��.(���ʹϿ�.)
}_SCALE_TRACK;			//������ Ʈ��

//-------������Ʈ Ÿ�� �Ӽ�.	word������ �س�����.
// ���������� ���� 1byte�� ���ؽ� ���� Ÿ������ �ٲ��.
#define _DYNAMIC_OBJECT		0x8000
#define _NO_MERGE_OBJECT	0x4000
#define _EVENT_ANI			0x2000		//�̺�Ʈ �ִϸ��̼��ϰ��.
#define _DEFAULT_ANI		0x1000		//����� �ִϸ��̼��� ���.
#define _Y_BILLBOARD		0x0800		//����� �ִϸ��̼��� ���.
#define _CORONA				0x0400		//�ڷγ��� ���
#define _V_FOG				0x0200		//���� ������ ���
#define _NO_LIGHT_MAP		0x0100		//����Ʈ ���� �ʿ� ���� �ϴ� ���. = ���ؽ� ����Ʈ��.
#define _COMPOSITE_LIGHT_MAP	0x0080		//������ ����Ʈ ��

//������Ʈ Ÿ�� 
#define _SHADOW_CAST_OFF	0x08000000
#define _SHADOW_RECEIVE_OFF 0x04000000		// ���ú� �����Ǹ� �� ����Ʈ ���̵ȴ�.
#define _OBJECT_TYPE_COMPOSITE		0x02000000		// ������ Ÿ��.

typedef struct{
    char objectname[128];
    char parentname[128];	//�θ� �̸� ������ [0]�� NULL/
	DWORD type;				
	DWORD composite_id;			//������ ����Ʈ�� ���̵� 
//	int dynamic_object_num;		// ���̳��� object ��ȣ
//	int dynamic_object_index;		// ���̳��� object index
    int vertexnum;
    int tvertexnum;
    int tvertexnum_ch2;	//ä��2�� �ؽ��Ĺ��ؽ�
    int facenum;
    int tvfacenum;
    int material_ref;       // use material number
	union{
	    float ani_matrix[4][4];  // �ڽ��� ���� �ִ� ��Ʈ���� 
		D3DMATRIX d3d_ani_matrix;	
	};
    
	union{
		float revmatrix[4][4];  // rotate normal matrix
		D3DMATRIX d3d_rev_matrix;	
	};

	union{
		float trmatrix[4][4];
		D3DMATRIX d3d_trmatrix;
	};
//    float tmscale[3];       // x,y,z scale
    _VERTEX *vertex;        // start vertex pointer
	union{
    _FACE *face;            // start tri polygon pointer
    _POLYFACE *p_face;      // start polygon pointer
	};
    _TVERTEX *tvertex;      // texture vertex pointer
	_TVERTEX *tvertex_ch2;
	Vector3f *fnormal;		// face normal
    Vector3f *vnormal;       // vertex normal

	Vector3f bb_min,bb_max;		// ������Ʈ �ٿ�� �ڽ�
	DWORD *vcolor;			//���ؽ� �÷�
//-------------------------- new ani ����
	WORD start_frame,end_frame;		//�� ������Ʈ�� �ִϸ��̼ǽ��� �������Ӽ�.

	int Pos_cnt;			// ��ġ Ʈ�� ���� 
	_POS_TRACK *Pos;		// ��ġ Ʈ��		x,y,z					��������[4]
	int Rot_cnt;			// ������Ʈ Ʈ�� ����
	_ROT_TRACK *Rot;		// ������Ʈ Ʈ��	x,y,z,w �� ���ʹϿ�		��������[5]
	int Scale_cnt;			// ������ Ʈ�� ����
	_SCALE_TRACK *Scale;	// ��ĳ�� Ʈ��	x,y,z	��������[4]
	Vector4f scale_quat;	// ������ �� ���ʹϿ� �� 
	Vector3f scale;			// �����Ͼִϰ� ������� �����ϰ�.
	Vector4f quat;			//ȸ���ִϰ� ���� ��� ���ʹϿ� �� 
	Vector3f pos;			//�̵� �ִϰ� ���� ��� ����Ѵ�.
	union{
	    float s_matrix[4][4];     //scale local matrix
		D3DMATRIX d3d_s_matrix;
	};
	WORD event_id;	//�̺�Ʈ�ִ��� ���� ���̵�.
}_OBJECT;

//-----���̳��� ������Ʈ ����ü... ��ƼƼ�� bsp�ȿ��� ���� ����Ѵ�.
typedef struct {
	WORD	flag;			// ������Ʈ �÷���?
	WORD	parent;			// 0�� �θ� ����. parent-1 = index
	int frames;				//�ش� ������Ʈ�� �����Ӽ�
	int Pos_cnt;			// ��ġ Ʈ�� ���� 
	int Rot_cnt;			// ������Ʈ Ʈ�� ����
	int Scale_cnt;			// ������ Ʈ�� ���� 

	Vector3f scale;		//�̵��ִϰ� ���� ��� �̵��� 
	Vector4f scale_quat;		//�̵��ִϰ� ���� ��� �̵��� 
	Vector3f pos;		//�̵��ִϰ� ���� ��� �̵��� 
	Vector4f quat;		//ȸ���ִϰ� ���� ��� ���ʹϿ� �� 
	DWORD pos_offset;
	DWORD rot_offset;
	DWORD scale_offset;
/*
	union{
	    float s_matrix[4][4];     //scale local matrix
		D3DMATRIX d3d_s_matrix;
	};*/
//	_POS_TRACK *Pos;		// ��ġ Ʈ��		x,y,z					��������[4]
//	_ROT_TRACK *Rot;		// ������Ʈ Ʈ��	x,y,z,w �� ���ʹϿ�		��������[5]
}_D_OBJECT;


//���͸��� attr
#define _MAT_NORMAL_ALPHA		0x80000000	
#define _MAT_REFLECT			0x40000000	
#define _MAT_VOLUME_FOG			0x20000000	

typedef struct{
    char mapname[256];      // to texture buffer
    int mapxl;
    int mapyl;
    BYTE *buffer;
}_TEXTURE;

#define _MAT_TYPE_MUTISUBOBJECT		0
#define _MAT_TYPE_STANDARD			1
#define _MAT_TYPE_COMPOSITE			2
#define _MAT_TYPE_UNKNOWN			255		//�������ϴ°Ŵ�.


typedef struct _SUBMATERIAL{
//   Vector3f ambient;   // r,g,b
//    Vector3f diffuse;   // r,g,b
//    Vector3f specular;   // r,g,b
//    float shine;
	DWORD type;			//���͸��� Ÿ�� 
    float reflect;		//�ݻ簪
    float self_illum;	//�����ε带 �������� ��...
    float transparency;	//���͸����� ���� 0�� �������� 1�� ������
    int all_num;		//0������ �ִ�. ��ü ���͸��� ���� 
	int same_mat;		//���� �ؽ��ĸ� ���� ��� �� ���͸��� ��ȣ, ������ -1
    int sbnum;
	int new_matnum;		//���ο� ���͸��� ��ȣ 
    int attr;   // �� ���͸����� �Ӽ� ,�ݻ� ����,����,���� �ִϵ�
	int lgt_id;
	BOOL is_used;		//��������ʴ� ���͸����̶�� FALSE ��....
    char name[256];
	_TEXTURE *tex;		// �ؽ��ĸ�...
	_TEXTURE *oppa;		//���ĸ��� �ִ°��.
    struct _SUBMATERIAL *submaterial;
}_MATERIAL;


typedef struct{
	BOOL poly_type;			//�ﰢ������ FALSE �ٰ��� TRUE
	int objectnum;
	_OBJECT *object;
//    int material_num;
//    _MATERIAL *material;
//------------------------- new ani
}_MESH;	

typedef struct{
	int is_loop;			// �����Ǵ� �ִ��ϰ�� 1 �ƴҰ�� 0
	int StartFrame;		//���� �����Ӱ� �� ������.
	int EndFrame;
	int FrameSpeed;			// �ʴ� �� �������ΰ�...
	int TickPerFrame;		// �����Ӵ� ƽ 
}_SCENE;
//--------------------------����Ʈ�� ī�޶�.
#define _OMNI 1
#define _SPOT 2
#define _DIRECTIONAL 3

typedef struct{
	BOOL is_calculated;
    int light_num;   // only light[0].light_num uses;
    int type;   // omni=1 spot=2;
//------------ to spotlight
    float hotspot,falloff;
    int attn_nearon,attn_faron;

    float near_attnstart,near_attnend;	//attenuation���� 
    float attnstart,attnend;
    Vector3f tpos;
    float is_shadow;
    float shadow_color[3];
    float shadow_density;
    float shadow_size;
    float shadow_sample_range;
//-----------------------
    Vector3f rgb;
    Vector3f pos;
    float intens;		//0.0 - 2.0���� ����
}_LIGHTSOURCE;

typedef struct{
	int type;	//type 0 is target 1 is free
    int num;    // camera numbers   it's only 'Camera[0].num' uses
	union{
	    float m_matrix[4][4];     //main matrix
		D3DMATRIX d3d_trmatrix;
	};
	union{
	    float s_matrix[4][4];     //scale local matrix
		D3DMATRIX d3d_s_matrix;
	};
    float t_matrix[4][4];     //target matrix
    Vector3f pos,target;
    float fov;              // float
    float tdist;            // screen scale; if X 640

//---------�ִϸ��̼��� ���� ����ü...
    char objectname[128];
    char parentname[128];	//�θ� �̸� ������ [0]�� NULL/
	int Pos_cnt;			// ��ġ Ʈ�� ���� 
	_POS_TRACK *Pos;		// ��ġ Ʈ��		x,y,z					��������[4]
	int Rot_cnt;			// ������Ʈ Ʈ�� ����
	_ROT_TRACK *Rot;		// ������Ʈ Ʈ��	x,y,z,w �� ���ʹϿ�		��������[5]
//	int Scale_cnt;			// ������ Ʈ�� ���� 
//	_SCALE_TRACK *Scale;	// ������  Ʈ��		x,y,z				
	Vector4f quat;		//ȸ���ִϰ� ���� ��� ���ʹϿ� �� 
//	Vector3f pos;			//�̵� �ִϰ� ���� ��� ����Ѵ�.
}_CAMERA;

typedef struct{
	int num;		//0�� ����ü���� �ִ�.
	Vector3f bb_min;	//�ٿ�� �ڽ�
	Vector3f bb_max;	//�ٿ�� �ڽ�

    char objectname[256];
    char parentname[128];	//�θ� �̸� ������ [0]�� NULL/
	int Pos_cnt;			// ��ġ Ʈ�� ���� 
	_POS_TRACK *Pos;		// ��ġ Ʈ��		x,y,z					��������[4]
	int Rot_cnt;			// ������Ʈ Ʈ�� ����
	_ROT_TRACK *Rot;		// ������Ʈ Ʈ��	x,y,z,w �� ���ʹϿ�		��������[5]
//	int Scale_cnt;			// ������ Ʈ�� ���� 
//	_SCALE_TRACK *Scale;	// ������  Ʈ��		x,y,z				
	Vector4f quat;		//ȸ���ִϰ� ���� ��� ���ʹϿ� �� 
	Vector3f pos;			//�̵� �ִϰ� ���� ��� ����Ѵ�.
	union{
		float trmatrix[4][4];
		D3DMATRIX d3d_trmatrix;
	};
	union{
	    float s_matrix[4][4];     //scale local matrix
		D3DMATRIX d3d_s_matrix;
	};
}_HELPER;

#pragma	pack(pop)


#endif
