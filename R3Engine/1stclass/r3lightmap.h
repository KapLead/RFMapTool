#ifndef __R3LIGHTMAP_H__
#define __R3LIGHTMAP_H__

#include "c_vector.h"

typedef struct{
	WORD xl;
	WORD yl;
	WORD *buf;	//565����
}_LIGHTMAP;

void LoadLightMap(char *name);
void ReleaseLightMap();
void *GetLightMapSurface(int i);

DWORD GetLightMapTexSize();	//����Ʈ�� �ؽ��� ����� �˾Ƴ���.

DWORD GetLightMapColor(Vector2f lgt_uv, int id);	//�ش����Ʈ�ʾ��̵�� ����Ʈ�� uv�� �ֳ��ָ� �÷����� ���´�.



#endif