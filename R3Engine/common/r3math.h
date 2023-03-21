#ifndef __R3Math_H__
#define __R3Math_H__

#include "c_vector.h"
#include <windows.h>



float DotProduct(Vector3f a,Vector3f b);
void MatrixFromQuaternion( float mat[4][4], float x, float y, float z,float w );
void QuaternionFromRotation( Vector4f quat,Vector3f v, float fTheta );
void GetQuaternionFromVector(Vector4f quat,Vector3f vec);	//���غ��ʹ� {0,1,0}
void GetQuaternionFromVector(Vector4f quat,Vector3f vec,Vector3f axis);	//���غ��� ����...

void QuaternionSlerp( float& Qx, float& Qy, float& Qz, float& Qw,
                              float Ax, float Ay, float Az, float Aw,
                              float Bx, float By, float Bz, float Bw,
                              float fAlpha );
void GetMatrixFrom3DSMAXMatrix( float mat[4][4] );
BOOL MatrixInvert( float get[4][4], float a[4][4] );
void MatrixMultiply( float get[4][4],float a[4][4], float b[4][4] );
void MatrixIdentity( float get[4][4] );
void MatrixCopy( float get[4][4], float source[4][4]);
void MatrixScale( float get_mat[4][4],float xscale,float yscale,float zscale );
void MatrixRotate(float get_mat[4][4],float x_rot,float y_rot,float z_rot); //������ 360��
void MatrixRotateX(float get_mat[4][4],float x_rot);
void MatrixRotateY(float get_mat[4][4],float y_rot);
void MatrixRotateZ(float get_mat[4][4],float z_rot);


void GetNormal(Vector4f get,Vector3f vv0,Vector3f vv1,Vector3f vv2);
void CrossVector( Vector3f a, Vector3f b, Vector3f r );


//Ʈ����������.
void SetTransformClipInfo(float clip_width,float clip_height, float x_center, float y_center );//�Ʒ��Լ��� ȣ�� ������ �Ÿ� ����������..
int TransformVertex( float xyzw[4], float f_matrix[4][4],float pos[3]);
int TransformVertex( float xyzw[4], float f_matrix[4][4],float pos[3],float add[3]);//�̰Ŵ� SetClipInfo�� ���� �ؾߵȴ�.

void Vector3fTransform( float get[3],float pos[3],float f_matrix[4][4]);


//Ŀ�� �Լ��� ������Ŀ��� ��Ʈ�� ����Ʈ�� ���İ����ʴ´�.
//ť��Ŀ��� ��Ʈ�� ����Ʈ�� ���İ��� �Ŵ�.
//������ Ŀ�� get_pos ���ϴ� ��ġ, pos ��Ʈ������Ʈ���� ,��Ʈ�� ����Ʈ ��, 0 ~ 1 �ش�Ǵ� ��
void GetBezierPoint(Vector3f *get_pos,Vector3f *pos,DWORD ctrl_cnt,float alpha);// 0-1����.
void CalcCubicCurve(Vector4f *get_abcd,int n, float *vptr);		//ť��Ŀ��
float CalcEvalCubicCurve(Vector4f v,float u);

#endif
