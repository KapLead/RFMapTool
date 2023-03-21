#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "C_Vector.h"
#include "windows.h"

float GetYposInLeaf(Vector3f a,Vector3f b,float able_height,float now_height,int leaf_id);
// source�� target�� ��Ȯ�� ����ߵȴ�.(BBOX�� ���������� ýũ���Ѵ�.)
BOOL IsRayAABB(Vector3f BBmin,Vector3f BBmax,Vector3f source, Vector3f target, Vector3f *get);
BOOL IsRayAABB(Vector3s BBmin,Vector3s BBmax,Vector3f source, Vector3f target, Vector3f *get);

//�������� üũ�Ҽ��ִ°��
BOOL IsBothRayAABB(Vector3f BBmin,Vector3f BBmax,Vector3f source, Vector3f target);
BOOL IsBothRayAABB(Vector3s BBmin,Vector3s BBmax,Vector3f source, Vector3f target);

//��� �ٿ�� �ڽ����� �浹 ����..TURE�� �浹 ...
BOOL IsCollisionBBoxToFace(Vector4f p_normal[6],Vector3f a,Vector3f b,Vector3f c);
//�ٿ�� �ڽ� ������ �浹 ����...
BOOL IsCollisionBBox(Vector3s a_min,Vector3s a_max,Vector3f b_min,Vector3f b_max);
BOOL IsCollisionBBox(Vector3f a_min,Vector3f a_max,Vector3f b_min,Vector3f b_max);
BOOL IsCollisionBBox(Vector3s a_min,Vector3s a_max,Vector3s b_min,Vector3s b_max);

//�ٿ�� �ڽ��ȿ� ���� �ִ°�?
BOOL IsCollisionBBoxPoint(Vector3f a_min,Vector3f a_max,Vector3f pos);

//---low level �Լ�
DWORD GetPlaneCrossPoint(Vector3f a,Vector3f b,Vector3f cross,Vector3f f_normal,float d);	//-1�̸� �������浹 1�̸� �������浹.
BOOL GetPlaneCrossPoint(Vector3f a,Vector3f b,Vector3f cross,Vector3f f_normal,float d,float option);
inline int CheckEdge(Vector3f a,Vector3f b,Vector3f v,Vector3f f_normal,float d);
int CheckEdgeEpsilon(Vector3f a,Vector3f b,Vector3f v,Vector3f f_normal,float d);

#endif
