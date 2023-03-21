// R3MRobot.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
//#include <sys/stat.h>
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

//static 
static int st_ext_roop_num;
static char stTargetRoot[256];

char *CopyEXT[]={
	"*.r3e"
	,"*.r3m"
	,"*.r3t"
	,"*.r3x"
	,"*.bsp"
	,"*.spt"
};
void GetFinalPath(char *get,char *path)
{
	int i,j;

	for( i=strlen(path)-2; i>=0; i--)
	{
		if(path[i]=='\\')
		{
			break;
		}
	}
	if( i==0 )
	{
		strcpy(get,".\\");
		return;
	}
	for( j=i+1; j<(int)strlen(path); j++)
	{
		get[j-(i+1)] = path[j];
	}
	get[j-(i+1)] =NULL;
}

void SearchDir(char *dir)	//Ȯ���� �ִ� 
{
	char make_dir[1256];
	char path[1256];
	char temp[1256];
	int i;

	//----------------------------������..

	sprintf(temp,"ConvMaterial %s\\",dir);
	system(temp);

	//-----------------------------------------------------------------------

	strcpy(path,dir);
	strcat(path,"\\*.*");

	strcpy(make_dir,&dir[2]);
	if( dir[3] != NULL )
	{
		GetFinalPath(temp,dir);
		strlwr(temp);
		if( strcmp(temp,"tex") )//tex������ �ȸ����.
		{
			sprintf(temp,"md %s%s",stTargetRoot,make_dir);
			system(temp);
		}
	}

	for(i=0; i<st_ext_roop_num; i++)
	{
		//ī���Ұ͵� ���� ī��...
//		sprintf(temp,"copy %s%s%s %s\\%s",stTargetRoot,make_dir,CopyEXT[i],dir,CopyEXT[i]);
		sprintf(temp,"copy %s\\%s %s%s\\%s",dir,CopyEXT[i],stTargetRoot,make_dir,CopyEXT[i]);
		system(temp);
	}

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile(path , &FindFileData);
	int bWorking = true;
	if (hFind==INVALID_HANDLE_VALUE)
		return;
	while (bWorking)
	{
		bWorking = FindNextFile(hFind,&FindFileData);
		if (!bWorking) 
			break;

		if (strcmp(".",FindFileData.cFileName)==0||strcmp("..",FindFileData.cFileName)==0)
		{
			continue;
		}
		if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			sprintf(temp,"%s\\%s",dir,FindFileData.cFileName);
			SearchDir(temp);
			strlwr(FindFileData.cFileName);
			if(!strcmp(FindFileData.cFileName,"tex") )	//�ؽ������� ������ �� ��Ʈ��.. 
			{
				sprintf(temp,"copy %s\\*.dds %s%s\\*.dds",dir,stTargetRoot,make_dir);
				system(temp);
			}
		}
		else
		{
		}
	}
	FindClose(hFind);
	return;
}

int main(void)
{
	st_ext_roop_num=sizeof(CopyEXT)/4;

	GetPrivateProfileString("Path","target","����",stTargetRoot, 256,".\\R3MRobot.ini");
	if( !strcmp(stTargetRoot,"����"))
	{
		printf("R3MRobot.ini ������ �����ϴ�..\n");
		getch();
		return 0;
	}

	if(stTargetRoot[strlen(stTargetRoot)-1] != '\\' )
	{
		int i = strlen(stTargetRoot);
		stTargetRoot[i]='\\';
		stTargetRoot[i+1]=NULL;
	}
	SearchDir(".");
	printf("��� �۾� �Ϸ�!\n");
	getch();
	return 1;
}



