#ifndef _COMMONUTIL_H_
#define _COMMONUTIL_H_

#include "windows.h"

void StripEXT(char *name);			//Ȯ���ڸ� ���ش�.
void StripPath(char *name);			//�н��� ���ش�.
void StripName(char *name);			//�̸��� ���ش�.
BOOL IsExistFile(char *name);		//���� ������ �����ϴ°�?
BOOL GetNowPath(char *get_path);	//���� �����ϰ� �ִ� �н��� ���´�.
void GetFinalPath(char *get,char *path);	//�н��߿� ������ �о��� ���´�.
void StripFinalPath(char *name);			// ������ �н��� ���ش�.
void GetEXT(char *get,char *name);		//Ȯ���ڸ� ����.

int GetFileSize(char *name);		//���� ����� �˾Ƴ���.

int MaxFixFloatToInt(float su);
int MinFixFloatToInt(float su);

//--- Ÿ�̸� ����	������ 1.0�� 1�� 
void R3CalculateTime();			//�������� ����� �ð��� ���� �ȴ�.
float R3GetLoopTime();			//���������� �ѷ����� ����  �ð��� ������.
void R3SetMinFPS(float min_fps);	//�ּ� ������ ����Ʈ�� �����Ѵ�.
void R3SetTime(float time);			//�ð��� �� ����...
float R3GetTime();					//���� �ð��� �˾Ƴ���.
void R3SetLoopTime(float time);	//������ ���� ����. �׷����� ������ ������Ѵ�.

LONG R3GetToggle30fps();		//30fps�� ��� ���غ�����.
LONG R3GetToggle15fps();		//30fps�� ��� ���غ�����.
LONG R3GetToggle7fps();			//7fps�� ����� ���ش�.


float GetFPS();		//�����ӷ���Ʈ�� �����Ѵ�.
void SaveBMP(char *name,int xl,int yl,int pitch,BYTE *argb);
void SaveJPG(char *name,int xl,int yl,int pitch,BYTE *argb);


#endif