#ifndef __RAD_H__
#define __RAD_H__

#include "world.h"

#define _MAX_INTENSITY	2.0f	//�ٽ� 2.0���� ����..

void GetLightRadiosity(_MESH *mesh,_MATERIAL *mat, _LIGHTSOURCE *Lgt_mesh );	//����Ʈ��길
BOOL CalculateFaceRadiosity(_MESH *mesh);	//face Radiosity
float GetEnergyLimit( _LIGHTSOURCE *lgt_mesh );	//������ ����Ʈ�� ���Ѵ�.

#endif