#include"world.h"
#include"jerror.h"
#include "R3BspUtil.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

static _INIFILE Ini;
static int S_state;
static char Radpath[]=".\\���߰���\\R3BspInfo.text\\";
static char RadName[256];
/*
static char RadName[256]=".\\RadInfo.text\\";

void Normalize(_DVECTOR *vec)
{
	double a= sqrt(vec->x*vec->x+vec->y*vec->y+vec->z*vec->z);
	if(a==0)
	{
		vec->x=0;
		vec->y=0;
		vec->z=1;
		return;
	}
	vec->x/=a;
	vec->y/=a;
	vec->z/=a;
}
*/


void R3BspInfoTextState(int option)	// 0=ȭ��,����Ʈ �Ѵ� ,1=����Ʈ�� 
{
	S_state = option;
}
void StripEXT(char *name)
{
	for(int i=strlen(name)-1; i>=0; i--)
	{
		if(name[i]=='.')
		{
			name[i]=NULL;
			return;
		}
	}
}
void StripPath(char *name)
{
	char r_name[256];

	for(int i=strlen(name)-1; i>=0; i--)
	{
		if(name[i]=='\\')
		{
			break;
		}
	}
	int st=i+1;
	for(i=st; i<(int)strlen(name); i++)
	{
		r_name[i-st]=name[i];
	}
	r_name[i-st]=NULL;
	strcpy(name,r_name);
}
void StripName(char *name)
{
	for(int i=strlen(name)-1; i>=0; i--)
	{
		if(name[i]=='\\')
		{
			break;
		}
	}
	if( i==0 )
		return;
	name[i+1]=NULL;
}

void RemoveSpace(char *name)
{
	for(int i=strlen(name)-1; i>=0; i--)
	{
		if(name[i]==' ')
		{
			name[i]='%';
		}
	}
}
void TestDebugOutPut(int num)
{
	FILE *fp;
	fp=fopen("debug.txt","wt");
	fprintf(fp,"%d\n",num);
	fclose(fp);
}

void TestDebugOutPutString(const char *fmt, ... )
{
	FILE *fp;
	va_list arg_ptr;
	char Str[256];

	va_start(arg_ptr, fmt);
	vsprintf(Str, fmt, arg_ptr);
	fp=fopen("debug.txt","rt+");
	if(fp==NULL)
		return;
	fseek(fp,0,SEEK_END);
	fprintf(fp,Str);
	fclose(fp);
	va_end(arg_ptr);
}

void SetR3BspInfoFileName(char *name)
{
	char r_name[256];

	strcpy(r_name,name);
	StripEXT(r_name);
	StripPath(r_name);
	strcpy(RadName,Radpath);
	strcat(RadName,r_name);
	strcat(RadName,".txt");

	FILE *fp;
	fp=fopen(RadName,"wt");
	if( fp == NULL )
		Error("������������ �����ϼ���..","");
	fclose(fp);
}

void GetR3BspOutFileName(char *name,char *t_name)
{
	char r_name[256],buf[256];

	strcpy(r_name,name);
	StripEXT(r_name);
	StripPath(r_name);
	strcpy(t_name,".\\Map\\");
	strcat(t_name,r_name);
	strcat(t_name,"\\");
	strcat(t_name,r_name);
	strcat(t_name,".BSP");

	//������ ������.
	sprintf(buf,"md .\\Map\\%s",r_name);
	system(buf);
}

void PutR3BspInfoText(const char *fmt, ... )
{
	FILE *fp;
	va_list arg_ptr;
	char Str[256];

	va_start(arg_ptr, fmt);
	vsprintf(Str, fmt, arg_ptr);
	fp=fopen(RadName,"rt+");
	if(fp==NULL)
		return;
	fseek(fp,0,SEEK_END);
	fprintf(fp,Str);
	fclose(fp);

	va_end(arg_ptr);
	if( S_state != 1 )
		printf(Str);
}

_INIFILE *GetIniFile()
{
	return &Ini;
}

void MakeDir(char *name)	//���丮�� ������ش�.
{
	char buf[256];

	sprintf(buf,"cd %s",name);
	if( system(buf) )
	{
		system("cd..");
		sprintf(buf,"md %s",name);
		system(buf);
	}
	else
		system("cd..");
}


static St_time,End_time;
void StartTimer(void)
{
	St_time = timeGetTime();
}
void EndTimer(void)
{
	End_time = timeGetTime();
}
DWORD GetTimer(void)
{
	return End_time - St_time;
}

int MaxFixFloatToInt(float su)
{
	su+=0.999f;
	return (int )su;
}

int MinFixFloatToInt(float su)
{
	su-=0.999f;
	return (int )su;
}
/*
int FixFloatToInt(float su)
{
	if( su>0)
		su+=0.999f;
	if( su < 0)
		su-=0.999f;

	return (int )su;
}*/

//------------------------ �ؽ��� �н����� �߰�....
char stTexPath[64][128];
int stTexPathCnt=0;
char stRetTexPath[128];

static void AdjustPath(char *name)
{
	int i=strlen(name)-1;
	if( name[i]=='\\' )
		name[i]=NULL;
}
void AddTexturePath(char *name)
{
	AdjustPath(name);
	strcpy(stTexPath[stTexPathCnt],name);
	stTexPathCnt++;
	if( stTexPathCnt >= 64 )
		Error("Ini���Ͽ��� �ؽ����н��� �ʹ� ����. �Ⱦ��°��� �����..","");
}/*
static void StripPath(char *name)
{
	char r_name[256];

	for(int i=strlen(name)-1; i>=0; i--)
	{
		if(name[i]=='\\')
		{
			break;
		}
	}
	int st=i+1;
	for(i=st; i<(int)strlen(name); i++)
	{
		r_name[i-st]=name[i];
	}
	r_name[i-st]=NULL;
	strcpy(name,r_name);
}*/
char *GetFullTexPath(char *name)
{
	char path[256];
	int i;

	FILE *fp;
	fp = fopen(name,"rb");
	if( fp == NULL )
	{
		strcpy(path,name);
		StripPath(&path[1]);
		path[0]='\\';
		for(i=0; i<stTexPathCnt; i++)
		{
			strcpy(stRetTexPath,stTexPath[i]);
			strcat(stRetTexPath,path);
			if( fp = fopen(stRetTexPath,"rb") )
			{
				fclose(fp);
				return stRetTexPath;
			}
		}
	}
	else
	{
		fclose(fp);
		return name;
	}
	Warning(path,"<-������ ��𿡵� ����.");
	return name;
}

BOOL IsExistFile(char *name)
{
	BYTE Bmpbyte[54];

	FILE *fp=fopen(name,"rb");
	if(fp==NULL)
	{
		Warning(name," <- �������� ����.");
		return FALSE;
	}
    fread(Bmpbyte,54,1,fp);
	fclose(fp);

	int lwTempW = (Bmpbyte[0x13]<<8)+Bmpbyte[0x12];
	int lwTempH = (Bmpbyte[0x17]<<8)+Bmpbyte[0x16];
	while ((lwTempW & 1) == 0)
		lwTempW = lwTempW >> 1;
	while ((lwTempH & 1) == 0)
		lwTempH = lwTempH >> 1;
	if (lwTempW != 1 || lwTempH != 1)
	{
		Warning(name," <- �������� ���ζǴ� ���α��̰� 2�� �������� �ƴ�.");
		return FALSE;
	}
	return TRUE;
}
void CheckBmpExp(_TEXTURE *tex)
{
	BOOL x_right=FALSE;
	BOOL y_right=FALSE;
	int temp=2;
	
	while(1)
	{
		if( tex->mapxl == temp )
			x_right=TRUE;
		if( tex->mapyl == temp )
			y_right=TRUE;
		if( temp ==2048 )
			break;
		temp*=2;
	}
	if( x_right && y_right )
		return;
	Error(tex->mapname,"<-���μҽ��� 2�ǹ���� �ƴմϴ�. -.-");
}