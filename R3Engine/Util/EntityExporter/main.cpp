#include <stdio.h>
#include <conio.h>
#include "loadase.h"
#include "r3bsputil.h"
#include "meshutil.h"
#include "patch.h"
#include "rad.h"
#include "jmalloc.h"
#include "ddsexport.h"
#include "material.h"
#include "entity.h"
#include "jerror.h"
#include "loadprescript.h"


void main(void)
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

	strcpy(GetIniFile()->In_file,ase_name);

	_INIFILE *Ini=GetIniFile();
	SetR3BspInfoFileName(Ini->In_file);	//�α����� �ʱ�ȭ
	Ini->CPUNum=1;
	PutR3BspInfoText("Start Entity Generator!\n");

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
		if( load_mesh->object[i].Pos_cnt || load_mesh->object[i].Rot_cnt || load_mesh->object[i].Scale_cnt)
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
	Ini->w_mesh = w_mesh;
	SetMeshFaceNormal(w_mesh,load_mesh);			// �ﰢ�޽��� normal�� ������. 
	p_mesh=ConvMeshTriToPoly(w_mesh);  // �ϳ��� �ﰢ�޽��� ������޽���

	mat = ConvOneMaterial(org_mat);	//�ݵ�� ConvOneMesh ������ ȣ���Ұ�...�������� �޽��� �ϳ���...
	AdjustMeshMaterial(p_mesh,mat);	//���͸��� ����.....

	DWORD p_num=0;
	for( i=0; i< p_mesh->objectnum; i++ )
		p_num += p_mesh->object[i].facenum;

	//-----��� ������ �غ��Ѵ�.
	R3BlockBsp CBlockBsp(p_num);	//��ϼ��� �غ�..
	CBlockBsp.PrepareBlockBsp(p_mesh,mat);			//��ϵ��� ���ĳ��´�.
	//------------��ġ����
	MakeVertexPatchAndWinding(p_mesh);	// ���ڸ޽��� ���ڷ�....

	//------------����Ʈ���

    No_warning=1;	//���� ����...-.-
	_LIGHTSOURCE *lgt_source=LoadLightASE(ase_name);	//����Ʈ�ε�
    No_warning=0;	//���� Ű��...-.-
	if( lgt_source )
	{
		for(int i=0; i<lgt_source->light_num; i++)
		{
			lgt_source[i].is_shadow =FALSE;		//��ī�̹ڽ��� ���ؽ� �������̹Ƿ� �׸��ڴ� ����.
		}
	}

	GetLightRadiosity(p_mesh,mat,lgt_source);		// ����Ʈ�� ���� �������Ƽ�� ���Ѵ�.


	int j,k;
	_LFACE **lf = GetLFace();
	if( lgt_source )
	{
		//---------��ƼƼ�� ���ؽ� �÷���...�̳��� multiply 2�� �����Ѵ�.
		for(k=0; k<p_mesh->objectnum; k++)
		{
			for(i=0; i<p_mesh->object[k].facenum; i++)
			{
				for(j=0; j<p_mesh->object[k].p_face[i].vertex_num; j++)
				{
					_WINDING *winding=&lf[k][i].patch->winding[j];
		//			winding->energy[0] = min(1.0f,winding->energy[0]);
		//			winding->energy[1] = min(1.0f,winding->energy[1]);
		//			winding->energy[2] = min(1.0f,winding->energy[2]);
					winding->energy[0] = min(255,winding->energy[0]);
					winding->energy[1] = min(255,winding->energy[1]);
					winding->energy[2] = min(255,winding->energy[2]);
				}
			}
		}
	}
	else
	{
		//---------����Ʈ�� �ϳ��� ������ ��ƼƼ�� ���ؽ� �÷���...
		for(k=0; k<p_mesh->objectnum; k++)
		{
			for(i=0; i<p_mesh->object[k].facenum; i++)
			{
				for(j=0; j<p_mesh->object[k].p_face[i].vertex_num; j++)
				{
					_WINDING *winding=&lf[k][i].patch->winding[j];
		//			winding->energy[0] = min(1.0f,winding->energy[0]);
		//			winding->energy[1] = min(1.0f,winding->energy[1]);
		//			winding->energy[2] = min(1.0f,winding->energy[2]);
					winding->energy[0]=127;
					winding->energy[1]=127;
					winding->energy[2]=127;
				}
			}
		}
	}

	char entityname[256],buf[256];
	char entitypath[256];

	strcpy(entitypath,ase_name);
	StripName(entitypath);

	strcpy(entityname,ase_name);
	StripEXT(entityname);
	StripPath(entityname);

	if( strlen(entityname) > 15)
		Error(entityname,"�����̸��� �ʹ� ��ϴ�. 15���̳��� �ٿ��ּ���.");

	strcat(entitypath,entityname);
	strcat(entitypath,"\\");

	//-------------------- �ؽ��ĸ� DDS�� ����Ʈ 
	PutR3BspInfoText("Material and DDS Texture Export!\n");
	for( i=0; i<mat->all_num; i++)
		mat[i].lgt_id= -1;
	char mdpath[256];
	sprintf(mdpath,"md %s",entitypath);
	system(mdpath);
	
	MaterialExport(mat,entitypath);		//���͸��� ��ũ���ͷ� ����Ѵ�.

	DDSTextureExport(p_mesh,mat,entitypath);		//dds �ؽ��ķ� ����Ʈ�Ѵ�.

	strcat(entitypath,entityname);
	strcat(entitypath,".R3E");
	EntityExport(entitypath,p_mesh,mat,&CBlockBsp);

	PutR3BspInfoText("Entity Export Complete!\n");
	PutR3BspInfoText("Press any key to continue\n");
	getch();
}

// ���� ��������.... 
// ����Ʈ�� ������ 0xffffffff  �۾���....
// �׸��� ���������� ������..... ���� ���������� ȣȯ�����ϰ�.....

