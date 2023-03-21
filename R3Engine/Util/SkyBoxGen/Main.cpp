#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include "C_vector.h"
#include "World.h"
#include "Loadase.h"
#include "Jmalloc.h"
//#include "Rad.h"
#include "MeshUtil.h"
#include "Material.h"
#include "jerror.h"
#include "patch.h"
#include "rad.h"
#include "R3bspUtil.h"
#include "DDSExport.h"
#include "entity.h"
#include "r3blockmat.h"
#include "loadprescript.h"



void StripSky(char *name)
{
	int s=strlen(name);
	if( name[s-3]=='s' && name[s-2]=='k' && name[s-1]=='y' )
	{
		name[s-3]=NULL;
	}
	else
	{
		Error(GetIniFile()->In_file," <- �̸����� sky�� �ٿ����մϴ�.");
	}
}


//--------------------------------------------------------------------------------------
R3BlockBsp *TempCBlockBsp;

void main(int argc, char **argv)
{
//-------------------- Map�� ����â���� �ҷ��´�.
	static OPENFILENAME ofn; // zero the ofn members out
    char ase_name[256]="1.map";

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = ase_name;
	ofn.nMaxFile = sizeof(ase_name);
	ofn.lpstrFilter = "R3MAP MAP (*.MAP)\0*.MAP\0All (*.*)\0*.*\0";
	ofn.lpstrTitle = "Select MAP file";
	ofn.Flags = OFN_NOCHANGEDIR | OFN_HIDEREADONLY |
               OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


	if (!GetOpenFileName(&ofn))
		return;
	strlwr(ase_name);

	strcpy(GetIniFile()->In_file,ase_name);

	_INIFILE *Ini=GetIniFile();
	SetR3BspInfoFileName(Ini->In_file);	//�α����� �ʱ�ȭ
	Ini->CPUNum=1;
	PutR3BspInfoText("Start Skybox Generator!\n");

	//-------------------------------------------------
	_MESH *load_mesh;	//÷ �ε��ߴ� �Ž�
	_MESH *w_mesh;		//����� ��ģ�޽�
	_MESH *p_mesh;
	_MATERIAL *org_mat,*mat;

    _SCENE *scene=LoadSceneASE(ase_name);
    load_mesh=LoadMeshASE(ase_name);
    if(load_mesh==NULL)
    {
        puts("object not found!");
        exit(0);
    }
    GetInvScaleMatrix(load_mesh);		//�ִϸ��̼������� inv�����ϸ�Ʈ������ ���Ѵ�.

	int i;
	for(i=0; i<load_mesh->objectnum; i++)
	{
		if( load_mesh->object[i].Pos_cnt || load_mesh->object[i].Rot_cnt )
		{
			load_mesh->object[i].start_frame = 0;
			load_mesh->object[i].end_frame = scene->EndFrame;
			load_mesh->object[i].type|=_DYNAMIC_OBJECT;	//�ִϸ��̼� ������Ʈ
			load_mesh->object[i].type|=_NO_MERGE_OBJECT;	//�ִϸ��̼� ������Ʈ�� �����ϸ� �ȵȴ�.
			FixTrack(&load_mesh->object[i]);
		}
	}
	
	
	org_mat = LoadMaterialASE(ase_name);

	SetWorldMesh(load_mesh,org_mat);  // wx,wy,wz�� ���ϱ�����
	w_mesh = load_mesh;

//	one_mesh=ConvOneMesh(w_mesh,org_mat);  // �����ΰ��� �ϳ��� �޽���
//	SetMeshFaceNormal(one_mesh,load_mesh);			// �ﰢ�޽��� normal�� ������. 
//	p_mesh=ConvMeshTriToPoly(one_mesh);  // �ϳ��� �ﰢ�޽��� ������޽���

	SetMeshFaceNormal(w_mesh,load_mesh);			// �ﰢ�޽��� normal�� ������. 
	p_mesh=ConvMeshTriToPoly(w_mesh);  // �ϳ��� �ﰢ�޽��� ������޽���

	mat = ConvOneMaterial(org_mat);	//�ݵ�� ConvOneMesh ������ ȣ���Ұ�...�������� �޽��� �ϳ���...
	AdjustMeshMaterial(p_mesh,mat);	//���͸��� ����.....

	//-----��� ������ �غ��Ѵ�.
	DWORD p_face_num = GetFaceNumFromMesh(p_mesh);

	R3BlockBsp CBlockBsp(p_face_num);						//��ϼ��� �غ�..
	CBlockBsp.PrepareBlockBsp(p_mesh,mat);			//��ϵ��� ���ĳ��´�.
	TempCBlockBsp = &CBlockBsp;

	//------------��ġ����
	MakeVertexPatchAndWinding(p_mesh);	// ���ڸ޽��� ���ڷ�....

	//------------����Ʈ���
    _LIGHTSOURCE *lgt_source=LoadLightASE(ase_name);	//����Ʈ�ε�
	if( lgt_source )
	{
		for( i=0; i<lgt_source->light_num; i++)
		{
			lgt_source[i].is_shadow = FALSE;		//��ī�̹ڽ��� ���ؽ� �������̹Ƿ� �׸��ڴ� ����.
		}
	}

	GetLightRadiosity(p_mesh,mat,lgt_source);		// ����Ʈ�� ���� �������Ƽ�� ���Ѵ�.


	char t_path[256];
	char skyname[256];

	strcpy(skyname,ase_name);
	StripEXT(skyname);
	StripPath(skyname);
	char filename[256]=".\\Map\\";
	strcat(filename,skyname);
	StripSky(filename);
	strcat(filename,"\\");
	strcat(filename,"Sky\\");
	strcpy(t_path,filename);
	strcat(filename,skyname);
	strcat(filename,".R3E");

	MakeDir(t_path);
	if( !MaterialExport(mat,t_path) )		//���͸��� ��ũ���ͷ� ����Ѵ�.
	{
		StripSky(skyname);
		Error(skyname,"<-�̸��̸��� ������ �����ϴ�. ��ī�̸��� ��������� �ʺ��� ������ּ���");
	}
	//-------------------- �ؽ��ĸ� DDS�� ����Ʈ 
	PutR3BspInfoText("DDS Texture Export!\n");
	DDSTextureExport(p_mesh,mat,t_path);		//dds �ؽ��ķ� ����Ʈ�Ѵ�.

	EntityExport(filename,p_mesh,mat,&CBlockBsp);

	PutR3BspInfoText("Complete!\n");
	PutR3BspInfoText("Press any key to continue\n");
	getch();
}
