#ifndef __RENDERPARTICLE_H__
#define __RENDERPARTICLE_H__

#include "R3Particle.h"
#include "entity.h"

void DrawCParticle(CParticle *particle,CEntity *entity,float world_mat[4][4],DWORD argb=0xffffffff);	//�̸� ���ؽ����ۿ� �����ؾߵȴ�.
//void DrawCParticle(CParticle *particle,CEntity *entity,Vector3f pos,DWORD argb=0xffffffff);	//�̸� ���ؽ����ۿ� �����ؾߵȴ�.

#endif