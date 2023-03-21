#ifndef _R3BSPUTIL_H_
#define _R3BSPUTIL_H_

#include "world.h"

#define _R3BSP_OPTIMIZE_NONE	0
#define _R3BSP_OPTIMIZE_HIGH	1

#define _R_OPTIMIZE_FASTEST		0
#define _R_OPTIMIZE_FAST		1
#define _R_OPTIMIZE_RADIOSITY	2

typedef struct{
	char In_file[256];
	char Out_file[256];
	int R3BspOptimize;		//bsp����ȭ �ɼ�.
	float EnergyLimit;
	Vector3f Ambient;	
	Vector3f Reflection;
	float PatchSize;
	int RenderState;
	int CPUNum;		//�����忡 ���ɿ���...
	int LightMapBits;		//����Ʈ�� ��Ʈ��.
	BOOL IsExportLightmapBMP;	//����Ʈ���� bmp�� export�Ұų�...

	_MESH *load_mesh;		//�޽�
	_MESH *w_mesh;			//�޽�
//	_MESH *o_mesh;			//���ڸ޽�..
	_MESH *p_mesh;			//�޽��� �ٰ����޽�..

	int finish;

	char MapPath[256];	//�� �н�
}_INIFILE;

void R3BspInfoTextState(int option);	// 0=ȭ��,����Ʈ �Ѵ� ,1=����Ʈ�� 
void PutR3BspInfoText(const char *fmt, ... );
void SetR3BspInfoFileName(char *name);
void GetR3BspOutFileName(char *name,char *t_name);
_INIFILE *GetIniFile();

void StripEXT(char *name);
void StripPath(char *name);
void StripName(char *name);
void RemoveSpace(char *name);	//�̸��� �����̽��� ���ش�.
void MakeDir(char *name);	//���丮�� ������ش�.


void AddTexturePath(char *name);
char *GetFullTexPath(char *name);
BOOL IsExistFile(char *name);
void CheckBmpExp(_TEXTURE *tex);

//int FixFloatToInt(float su);	//float�� int�� �ø��ؼ� �����Ѵ�.
int MaxFixFloatToInt(float su);
int MinFixFloatToInt(float su);

void StartTimer(void);
void EndTimer(void);
DWORD GetTimer(void);

void TestDebugOutPut(int num);
void TestDebugOutPutString(const char *fmt, ... );

#endif