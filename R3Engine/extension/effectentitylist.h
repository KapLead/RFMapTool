#ifndef __EFFECT_ENTITY_LIST_H__
#define __EFFECT_ENTITY_LIST_H__

#include <windows.h>
#include <stdio.h>
#include <jerror.h>
#include <jmalloc.h>
#include "R3Particle.h"
#include "Entity.h"

void LoadEffectEntityList(char *name);	//��ũ��Ʈ���� ��ƼƼ����Ʈ�� �д´�.
void ReleaseEffectEntityList();

DWORD GetRealID(DWORD id);	//�����Ⱦ��̵��� ���� �ε����� ����.
CEntity *GetEntityFromEffectEntityList(DWORD real_id);
CParticle *GetParticleFromEffectEntityList(DWORD real_id,float mat[4][4]);
BOOL IsParticle(DWORD real_id);

void *GetFirstTexureFromEffectEntityList(DWORD entity_id);//��ƼƼ ���̵� ���ڷ� ������..ù��° �ؽ��ĸ� ����.



DWORD GetEffectEntityNum();				//����Ʈ ��ƼƼ ��.
DWORD GetAllEffectEntityTextureSize();	//���� ����Ʈ ��ƼƼ�� ���Ǵ� ��� �ؽ��ļ�.
DWORD GetAllMemorySizeEffectEntity();	//���� �뷫 ���Ǵ� effectentitylist �޸𸮷�	�ؽ��� ����
DWORD GetLoadedEffectEntityNum();		//�ε��� ����Ʈ ��ƼƼ ��.

#endif