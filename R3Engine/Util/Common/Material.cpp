#include <sys/stat.h>
#include <stdio.h>
#include "R3BspUtil.h"
#include "Loadase.h"

//#pragma comment(linker, "/DEBUG")


BOOL MaterialExport(_MATERIAL *mat,char *mat_path)
{
	int i;
	FILE *fp,*fp1;
	char m_name[256],path[256],tname[256],su[256];
	char name[256]=".\\Map\\";
	float r,g,b,a;

	_INIFILE *Ini=GetIniFile();

	if( mat == NULL )
		return FALSE;
	strcpy(m_name,Ini->In_file);
	StripPath(m_name);
	StripEXT(m_name);
	if( mat_path != NULL )
	{
		strcpy(name,mat_path);
//		MakeDir(name);
		strcpy(path,name);
		strcat(name,"MainMaterial.mst");
	}
	else
	{
		strcat(name,m_name);
		strcat(name,"\\Tex\\");
		MakeDir(name);
		strcpy(path,name);
		strcat(name,"MainMaterial.mst");
	}
	if( (fp = fopen(name,"wt"))==NULL )
		return FALSE;

	fprintf(fp,"*MATERIAL_NUM %d\n",mat->all_num);
	fprintf(fp,"{\n");

	for(i=0; i<mat->all_num; i++)
	{
		sprintf(su,"%s_%d",mat[i].name,i);	//������͸��� �̸��� ���� ��츦 ����...
		fprintf(fp,"%16s\t\t%d\n",su,i);
		strcpy(name,path);
		strcat(name,su);
		strcat(name,".mst");
		if( (fp1 = fopen(name,"wt"))==NULL )
			return FALSE;
		fprintf(fp1,";��� ���̾� ����� R3���۷��� ����.\n");
//		if( mat[i].oppa )	//������ �ʿ����.
//			mat[i].lgt_id=-1;
//		else
//		mat[i].lgt_id=0;

		if( mat[i].lgt_id < 0 )
			fprintf(fp1,"light_map\tFALSE\n");
		else
			fprintf(fp1,"light_map\tTRUE\n");
		fprintf(fp1,"layer_num\t1\n");


		fprintf(fp1,"layer 0\n");
		fprintf(fp1,"{\n");

		if(mat[i].tex->mapname == NULL )
		{
			strcpy(tname,"NULL");
		}
		else
		{
			strcpy(tname,mat[i].tex->mapname);
			StripPath(tname);
			StripEXT(tname);
			RemoveSpace(tname);
			strcat(tname,".dds");

		}
		int type=0;
		if( mat[i].oppa )
			type=1;
		else
		if(mat[i].transparency != 1.0f )
		{
			type=2;
		}
		if(mat[i].attr & _MAT_REFLECT )
			type=256;	//�ſ��̴�.
		else
		if(mat[i].attr & _MAT_VOLUME_FOG )
			type=255;	//���� ���״�.

		fprintf(fp1,"\ttype %d\n",type);//\t\t; 0 ���� �Ⱦ�,1 8888����,2������,3�����,  5��������, 6�������, 255���� ����, 256�ſ�(���÷���)\n",type);
//		fprintf(fp1,"\talpha_sort FALSE		;���� �����̹ݵ�� �ʿ��Ѱ�츸 TRUE\n");
		fprintf(fp1,"\tmap_name %s\t",tname);

		if(mat[i].tex->mapname != NULL )
		{
			struct _stat status;
			char *full_path=GetFullTexPath(mat[i].tex->mapname);
			if( _stat( full_path, &status )==0 )   // static function
			{
				LONG mtime=status.st_mtime;
				fprintf(fp1,"org_file_day %d",mtime);
			}
		}
		fprintf(fp1,"\n");


		r=1.0f;//_MAX_INTENSITY;	//_MAX_INTENSITY ���� ��������.
		g=1.0f;//_MAX_INTENSITY;
		b=1.0f;///_MAX_INTENSITY;
		if(mat[i].attr & _MAT_REFLECT )
			a= 1-mat[i].reflect;	//�ݻ緮�� �����
		else
			a= mat[i].transparency;
		fprintf(fp1,"\talpha\t%3d\n",(DWORD)(a*255));
	    fprintf(fp1,"\tcolor\t%3d %3d %3d\n",(DWORD)(r*255),(DWORD)(g*255),(DWORD)(b*255));

		fprintf(fp1,"}\n");
		fclose(fp1);		
	}
	fprintf(fp,"}\n");
	fclose(fp);
	return TRUE;

}

