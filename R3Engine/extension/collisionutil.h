#ifndef _COLLISIONUTIL_H_
#define _COLLISIONUTIL_H_

#include "C_Vector.h"
#include <windows.h>

class CCollision{
private:
	Vector4f *mFNormalList;//���̽� ���.
	Vector4f *mVList;//���ؽ� ����Ʈ
	int mFaceNum;

public:
	CCollision();
	~CCollision();

	void InitColision(Vector4f *vlist,int face_num);
	void ReleaseCollision();
	BOOL CheckCollision(Vector3f char_pos,Vector3f old_pos,Vector3f new_pos);
};

#endif