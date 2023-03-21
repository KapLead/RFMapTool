#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <gl\glut.h>
#include <math.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include "C_vector.h"
#include "World.h"
#include "Loadase.h"
#include "Jmalloc.h"
#include "R3BspUtil.h"
#include "R3Bsp.h"
#include "Rad.h"
#include "R3BspTool.h"
#include "MeshUtil.h"
#include "Material.h"
#include "MoveCamera.h"
#include "jerror.h"
#include "Patch.h"
#include "r3blockMat.h"
#include "Lightmap.h"
#include "LoadPreScript.h"
#include "DDSExport.h"

#define _READ_Y	1	//������ �ٲٷ���...
#define _READ_Z	2

Vector3f camera_pos, camera_lookat, camera_up;
char Wintitle[]="R3Bsp ver 0.81";

double near_plane=1, far_plane=6000, camera_fov;
enum {STOP,FILTER,MOVE_CAMERA,CAMERA}; // menu ids
enum {LIT_MODEL=1, INDEX_MODEL}; // display lists

GLfloat viewanglex,viewangley, viewanglez, orgviewanglex,orgviewangley, orgviewanglez;

_CAMERA *Camera;
_MOVE_CAMERA MoveCamera;
static _MESH *RenderMesh;

//---------------------------------- open-gl������ϴ°Ϳ� ���� �� 
void InitRender3DSMAXModel(_MESH *mesh)	//���� ������ ������ �Ѹ���. 
{
	Vector3f c0;
	int i,j;


	glNewList(LIT_MODEL, GL_COMPILE);
        glShadeModel(GL_SMOOTH);
		glBegin(GL_TRIANGLES);

		if(mesh->poly_type == TRUE)
		{
			for(i=0; i<mesh->objectnum ; i++)
			{
				for(j=0; j<mesh->object[i].facenum; j++)
				{
					c0[0]=(rand()%128)/256.0+.5;	c0[1]=(rand()%128)/256.0+.5;	c0[2]=(rand()%128)/256.0+.5;
					int v0=mesh->object[i].p_face[j].vertex_id[0];
					int v1=mesh->object[i].p_face[j].vertex_id[1];
					int v2=mesh->object[i].p_face[j].vertex_id[2];

					glColor3f(c0[0], c0[1], c0[2]);
					glVertex3f(mesh->object[i].vertex[v0].local[0], mesh->object[i].vertex[v0].local[1],mesh->object[i].vertex[v0].local[2]);
					glVertex3f(mesh->object[i].vertex[v1].local[0], mesh->object[i].vertex[v1].local[1],mesh->object[i].vertex[v1].local[2]);
					glVertex3f(mesh->object[i].vertex[v2].local[0], mesh->object[i].vertex[v2].local[1],mesh->object[i].vertex[v2].local[2]);
				}
			}
		}
		else
		{
			for(i=0; i<mesh->objectnum ; i++)
			{
				for(j=0; j<mesh->object[i].facenum; j++)
				{
					c0[0]=(rand()%128)/256.0+.5;	c0[1]=(rand()%128)/256.0+.5;	c0[2]=(rand()%128)/256.0+.5;
					int v0=mesh->object[i].face[j].v[0];
					int v1=mesh->object[i].face[j].v[1];
					int v2=mesh->object[i].face[j].v[2];

					glColor3f(c0[0], c0[1], c0[2]);
					glVertex3f(mesh->object[i].vertex[v0].local[0], mesh->object[i].vertex[v0].local[1],mesh->object[i].vertex[v0].local[2]);
					glVertex3f(mesh->object[i].vertex[v1].local[0], mesh->object[i].vertex[v1].local[1],mesh->object[i].vertex[v1].local[2]);
					glVertex3f(mesh->object[i].vertex[v2].local[0], mesh->object[i].vertex[v2].local[1],mesh->object[i].vertex[v2].local[2]);
				}
			}
		}
		glEnd();
	glEndList();
}
void DeleteLightTexture(void)
{
	CGenLightMap *gen_map = GetLightmap();
	if( gen_map==NULL )
		return;//		Error("���� ����Ʈ���� ������.","");
	int lgt_cnt=GetLightmapCnt();
	for(GLuint i=0; i<(GLuint)lgt_cnt; i++)
	{
		_LIGHT_MAP *lgt_map = &gen_map->mLgtMap;
		glBindTexture(GL_TEXTURE_2D, i+1);
		glDeleteTextures (lgt_map->xl*lgt_map->yl*3,&i);
	}
}

void MakeLightTextureFromLightmap(void)
{

    glDisable(GL_LIGHTING);
    glDisable(GL_DITHER);
    glShadeModel(GL_FLAT);
	glEnable(GL_TEXTURE_2D);	//1����..

//	glBindTexture(GL_TEXTURE_2D, i+1);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_DEPTH_TEST);

	CGenLightMap *gen_map = GetLightmap();
	if( gen_map==NULL )
	{
		Warning("���� ����Ʈ���� ������.(�Ǵ� ��ü�� ���ؽ�����Ʈ�ϼ�����)","");
		return;
	}

	int lgt_cnt=GetLightmapCnt();
	for(GLuint i=0; i<(GLuint)lgt_cnt; i++)
	{
		_LIGHT_MAP *lgt_map = &gen_map->mLgtMap;
		glPixelStorei(GL_UNPACK_ALIGNMENT,1);
		glBindTexture(GL_TEXTURE_2D, i+1);
//WINGDIAPI GLboolean APIENTRY glAreTexturesResident (GLsizei n, const GLuint *textures, GLboolean *residences);
//WINGDIAPI void APIENTRY glDeleteTextures (GLsizei n, const GLuint *textures);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	//���͸�no
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	//���͸�no
		GLubyte *g_buf = (GLubyte *)Dmalloc(lgt_map->xl*lgt_map->yl*3);

		for(int j=0; j<lgt_map->xl*lgt_map->yl; j++)
		{
			g_buf[3*j+0]=lgt_map->bitmap[j].R;
			g_buf[3*j+1]=lgt_map->bitmap[j].G;
			g_buf[3*j+2]=lgt_map->bitmap[j].B;
		}
		glTexImage2D(GL_TEXTURE_2D,0,3,lgt_map->xl,lgt_map->yl,0,
			GL_RGB,GL_UNSIGNED_BYTE,g_buf);
		Dfree(g_buf);
	}
//	glBindTexture(GL_TEXTURE_2D, 4);
}
void InitRenderLightmapFrom3DSMAXModel(_MESH *mesh)
{
	int i,j,k,cnt=0;

//	int test[10];
//	test[1]=0;
//	test[2]=0;
//	test[3]=0;

	glNewList(LIT_MODEL, GL_COMPILE);
//        glShadeModel(GL_SMOOTH);
	for(k=0; k<mesh->objectnum; k++)
	{
		for(j=0; j<mesh->object[k].facenum; j++)
		{
			glBegin(GL_TRIANGLES);
			for(i=0; i<mesh->object[k].p_face[j].vertex_num-2; i++)
			{
				int v0=mesh->object[k].p_face[j].vertex_id[0];
				int v1=mesh->object[k].p_face[j].vertex_id[i+1];
				int v2=mesh->object[k].p_face[j].vertex_id[i+2];

				glNormal3f(0.0, 1.0, 0.0);
//				glBindTexture(GL_TEXTURE_2D, mesh->object[k].p_face[j].mtlid+1);
				glBindTexture(GL_TEXTURE_2D, mesh->object[k].p_face[j].l_mtlid+1);
//				test[mesh->object[k].p_face[j].l_mtlid+1]++;
				glTexCoord2f(mesh->object[k].p_face[j].lu[0],mesh->object[k].p_face[j].lv[0]);
				glVertex3f(mesh->object[k].vertex[v0].local[0], mesh->object[k].vertex[v0].local[_READ_Y],mesh->object[k].vertex[v0].local[_READ_Z]);
				glTexCoord2f(mesh->object[k].p_face[j].lu[i+1],mesh->object[k].p_face[j].lv[i+1]);
				glVertex3f(mesh->object[k].vertex[v1].local[0], mesh->object[k].vertex[v1].local[_READ_Y],mesh->object[k].vertex[v1].local[_READ_Z]);
				glTexCoord2f(mesh->object[k].p_face[j].lu[i+2],mesh->object[k].p_face[j].lv[i+2]);
				glVertex3f(mesh->object[k].vertex[v2].local[0], mesh->object[k].vertex[v2].local[_READ_Y],mesh->object[k].vertex[v2].local[_READ_Z]);
				cnt++;
			}
			glEnd();
		}
	}
	glEndList();
}

//--------------------------------------------------------------------------------------

static int Rad_cnt=0;

void Step() // the main radiosity step
{
/*
	int gop=(GetTickCount()-Start_time)/(1000.0);
	PutR3BspInfoText("all process finished!\n");
	PutR3BspInfoText("RenderTime %2d:%2d:%2d seconds!\n",(gop/3600),(gop%3600)/60,gop%60);
*/
}

void SetGlCamera(_CAMERA *cam)
{
	memset(&MoveCamera,0,sizeof(_MOVE_CAMERA));
	camera_fov=(30*cam->fov)/0.5236;
	viewanglex=0;
	viewanglez=0;
	if( cam->type == 1 ) //free
	{
		MoveCamera.is_free = TRUE;
		memcpy(MoveCamera.tr_matrix,cam->m_matrix,sizeof(float)*16);
		camera_pos[0]=cam->m_matrix[3][0];
		camera_pos[1]=cam->m_matrix[3][1];
		camera_pos[2]=cam->m_matrix[3][2];
		camera_up[0]=0.0;
		camera_up[1]=1.0;
		camera_up[2]=0.0;
	}
	else
	{
		camera_pos[0]=cam->pos[0];
		camera_pos[1]=cam->pos[1];
		camera_pos[2]=cam->pos[2];

		camera_lookat[0]=cam->target[0];
		camera_lookat[1]=cam->target[1];
		camera_lookat[2]=cam->target[2];

		viewanglex=0.0;
		viewanglez=0.0;
		camera_up[0]=0.0;
		camera_up[1]=1.0;
		camera_up[2]=0.0;

		MoveCamera.rho=1;
		MoveCamera.point[0] = camera_lookat[0];
		MoveCamera.point[1] = camera_lookat[1];
		MoveCamera.point[2] = camera_lookat[2];
	}
}

void DrawDebugPatch(int ob_num,int num)	//�ش� ��ġ�� ���� ����ش�.
{
	_LFACE **lf = GetLFace();
	if( lf==NULL )
		return;

	glPointSize(5);
  	glBegin(GL_POINTS);

	glColor3f(0,0,0);
	for(int i=0; i< lf[ob_num][num].patch->yl; i++)
		for(int j=0; j< lf[ob_num][num].patch->xl; j++)
		{
//			if( !lf[ob_num][num].patch->winding[lf[ob_num][num].patch->xl*i+j].is_out_face )
				glVertex3fv(lf[ob_num][num].patch->winding[lf[ob_num][num].patch->xl*i+j].pos);
		}

	glEnd();
	glPointSize(1);

	Vector3f temp;
	glBegin(GL_LINES);
	glColor3f(1,0,0);
	for(i=0; i< lf[ob_num][num].patch->yl; i++)
		for(int j=0; j< lf[ob_num][num].patch->xl; j++)
		{
//			if( !lf[ob_num][num].patch->winding[lf[ob_num][num].patch->xl*i+j].is_out_face )
			{
				glVertex3fv(lf[ob_num][num].patch->winding[lf[ob_num][num].patch->xl*i+j].pos);

				Vector3fScale(lf[ob_num][num].patch->winding[lf[ob_num][num].patch->xl*i+j].normal,4,temp);
				Vector3fAdd(lf[ob_num][num].patch->winding[lf[ob_num][num].patch->xl*i+j].pos,temp,temp);
				glVertex3fv(temp);
			}
		}
	glEnd();

	glBegin(GL_LINES);
	Vector3f pos;
	for(i=0; i< RenderMesh->object[ob_num].p_face[num].vertex_num; i++)
	{
		glColor4f(.9f,.9f,.9f,1);
		Vector3fCopy(RenderMesh->object[ob_num].vertex[RenderMesh->object[ob_num].p_face[num].vertex_id[i]].local,pos);
		glVertex3fv(pos);
		Vector3fScale(RenderMesh->object[ob_num].p_face[num].v_normal[i],6,temp);
		Vector3fAdd(pos,temp,temp);
		glVertex3fv(temp);
	}
	glEnd();
	float aa[4];
	glGetFloatv(GL_CURRENT_COLOR,aa);
}


// GLUT callbacks /////////////////////////////////////////////////////////////////////////
void redraw(void)
{
	glClearColor(248/255.0f,193/255.0f,7/255.0f,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION); // put camera in the correct position
	glLoadIdentity();
	gluPerspective(camera_fov, 1, near_plane, far_plane);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_CULL_FACE);
	if( MoveCamera.move_camera )
	{
		gluLookAt(MoveCamera.pos[0], MoveCamera.pos[1]
			, MoveCamera.pos[2], MoveCamera.point[0]
			, MoveCamera.point[1], MoveCamera.point[2]
			, camera_up[0], camera_up[1], camera_up[2]); 
	}
	else
	{
		if( MoveCamera.is_free )
		{
			glDisable(GL_CULL_FACE);
			D3DMATRIX d3dmatrix;
			//float matrix[16];
			glRotatef(viewanglex, 1.f, 0.f, 0.f);
			glRotatef(viewanglez, 0.f, 0.f, 1.f);
			//glGetFloatv(GL_MODELVIEW_MATRIX,matrix);
			//glTranslatef(-MoveCamera.tr_matrix[3][0],-MoveCamera.tr_matrix[3][1],-MoveCamera.tr_matrix[3][2]);
			
			D3DMath_MatrixInvert(d3dmatrix,*(D3DMATRIX *)MoveCamera.tr_matrix);
			glMultMatrixf((float*)&d3dmatrix);
  			//glLoadMatrixf((float*)MoveCamera.tr_matrix);
		}
		else
		{
			glRotatef(viewanglex, 1.f, 0.f, 0.f);
			glRotatef(viewanglez, 0.f, 1.f, 0.f);
	//		glRotatef(viewanglez, 0.f, 0.f, 1.f);
			gluLookAt(camera_pos[0], camera_pos[1], camera_pos[2], camera_lookat[0], camera_lookat[1], camera_lookat[2], camera_up[0], camera_up[1], camera_up[2]); 
			float matrix[16];
			glGetFloatv(GL_MODELVIEW_MATRIX,matrix);
		}
	}

	glCallList(LIT_MODEL);	//�����ϵ� ���� �Ѹ���.

//	DrawDebugPatch(0,0);
//	DrawDebugPatch(0,1);
//	DrawDebugPatch(0,2);
//	DrawDebugPatch(1,0);
//	DrawDebugPatch(2,0);
//	DrawDebugPatch(0,2);
//	DrawDebugPatch(0,3);
//	DrawDebugPatch(0,4);
//	DrawDebugPatch(0,5);
//	DrawDebugPatch(0,6);

	//	DrawBspSplitPolygon();	//�߷��� �������� �Ѹ���.
	DrawBspLeafBBox();		//�ٿ�� �ڽ��� �Ѹ���.

	glutSwapBuffers();
}

void special(int key, int x, int y)
{
	switch (key) 
	{
	case GLUT_KEY_LEFT:
		viewanglez += 10.f; glutPostRedisplay(); break;
	case GLUT_KEY_RIGHT:
		viewanglez -= 10.f; glutPostRedisplay(); break;
	case GLUT_KEY_UP:
		viewanglex -= 10.f; glutPostRedisplay(); break;
	case GLUT_KEY_DOWN:
		viewanglex += 10.f; glutPostRedisplay(); break;
	break;
	}
}

void key(unsigned char key, int x, int y)
{
	switch (key) 
	{
	case ' ':
		break;
	case '\033':
		exit(0);
		break;

	case 'w':
		MoveCamera.speed=1;
		ReMoveCamera(&MoveCamera);
	break;
	case 's':
		MoveCamera.speed=-1;
		ReMoveCamera(&MoveCamera);
	break;
	case 'd':
		MoveCamera.step=-1;
		ReMoveCamera(&MoveCamera);
	break;
	case 'a':
		MoveCamera.step=1;
		ReMoveCamera(&MoveCamera);
	break;

	}
	glutPostRedisplay();
}

static int startx, starty; // used for dragging
void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_UP) 
	{
		viewanglex=orgviewanglex+(starty-y); viewanglez=orgviewanglez+(startx-x);
	} 
	else // GLUT_DOWN 
	{              
		startx = x; starty = y;
		orgviewanglex=viewanglex; orgviewanglez=viewanglez;
	}
	if(viewanglex<-179)
		viewanglex=-179;
	if(viewanglex>179)
		viewanglex=179;
	MoveCamera.ph = (viewanglez*_PI)/360.0f;
	MoveCamera.th = -(viewanglex*_PI)/360.0f;
	CalculateMoveCamera(&MoveCamera);
	glutPostRedisplay();
}

void motion(int x, int y)
{
	viewanglex=orgviewanglex+(starty-y);
	viewanglez=orgviewanglez+(startx-x);
	if(viewanglex<-179)
		viewanglex=-179;
	if(viewanglex>179)
		viewanglex=179;
		
	MoveCamera.ph = (viewanglez*_PI)/360.0f;
	MoveCamera.th = -(viewanglex*_PI)/360.0f;
	CalculateMoveCamera(&MoveCamera);
	glutPostRedisplay();
}

static int Spotnum[256];
static int Spotcnt=0;

void menu(int sel)
{
	if(sel==MOVE_CAMERA)
	{
		MoveCamera.move_camera=1;
		glutPostRedisplay();
	}
	if(sel==STOP)
	{
	}
	if(sel==FILTER)
	{
		static int filter_on=0;
		filter_on=~filter_on;
	}
    if(sel>=CAMERA && sel < CAMERA+Camera->num)
	{
		SetGlCamera(&Camera[sel-CAMERA]);
    }
	glutPostRedisplay();
}

void reshape(int w, int h)
{
// dummy to prevent the viewport from resizing
}

void ReadObject(char* asefile)
{
	_MESH *load_mesh;	//÷ �ε��ߴ� �Ž�
	_MESH *w_mesh;		//����� ��ģ�޽�
	_MESH *p_mesh;		//������ �޽�
	_MESH *col_one_mesh;	//�浹 ������ �޽�...
	_MESH *col_p_mesh;	//�浹 ������ �޽�...
	_MATERIAL *org_mat,*one_mat,*composite_mat;

	_INIFILE *Ini=GetIniFile();
	//---------- ����� BSP�����̸��� ����.
	GetR3BspOutFileName(Ini->In_file,Ini->Out_file);	//����� BSP�����̸��� ����.

	PutR3BspInfoText("Loading!...\n");
#ifdef 	_DEBUG
//	No_warning=1;	//���� ����...-.-
#endif
    _SCENE *scene=LoadSceneASE(asefile);
    load_mesh = LoadMeshASE(asefile);
    GetInvScaleMatrix(load_mesh);		//�ִϸ��̼������� inv�����ϸ�Ʈ������ ���Ѵ�.
	
	GetPreScriptAtt(asefile,load_mesh);	//���� ��ũ��Ʈ�� ������ ���´�.

	if(load_mesh==NULL)
    {
        puts("object not found!");
        exit(0);
    }
	org_mat = LoadMaterialASE(asefile);
	one_mat = ConvOneMaterial(org_mat);	//�ݵ�� ConvOneMesh ������ ȣ���Ұ�...�������� �޽��� �ϳ���...

	GetCompositeMat(one_mat,&composite_mat);	// ���͸��󿡼� composite���͸��� �����س���...
    Camera=LoadCameraASE(asefile);		// ī�޶� �ε�

	/*	�浹 �������� ���� �ȵα�� ����.
	col_mesh = load_mesh;
    col_mesh = LoadCollisionMeshASE(asefile);		//�浹 ������޽�...
	if( col_mesh->objectnum == 0 )
	{
		//Warning("�浹�������� ��� �׳���������� �浹 ���������� ����մϴ�.");
		col_mesh = load_mesh;
	}
	else
		SetWorldMesh(load_mesh,org_mat);  // wx,wy,wz�� ���ϱ�����. �浹�Ž��� ����Ž��� ������ �ѹ��� ȣ������...
	*/
	SetWorldMesh(load_mesh,org_mat);  // wx,wy,wz�� ���ϱ�����. �浹�Ž��� ����Ž��� ������ �ѹ��� ȣ������...

	w_mesh = load_mesh;
	GetIniFile()->w_mesh = w_mesh;

	//���� ���׿����� ��������� �����Ѵ�. ���������� ���������ϳ��� ���͸����� �ϳ��� �þ��.
//	one_mat = MakeVolumeFogObject(w_mesh,one_mat);

	SetMeshFaceNormal(w_mesh,w_mesh);			// �ﰢ�޽��� normal�� ������. 
	p_mesh=ConvMeshTriToPoly(w_mesh);  // �ﰢ�޽��� ������޽���

	CalculateMeshSmooth(p_mesh);	// �������׷� normal�� ������. 
	SetMeshCompositeMat(p_mesh,org_mat);	//Composite������Ʈ�� ���. �� ������ ������Ʈ�� �Ӽ��� �־��ش�.

	GetIniFile()->p_mesh = p_mesh;

	AdjustMeshMaterial(p_mesh,one_mat);	//���͸��� ����.....

	if( Camera != NULL )
		SetGlCamera(&Camera[0]);
	else
	{
		camera_pos[0]=0;
		camera_pos[1]=0;
		camera_pos[2]=50;

		camera_lookat[0]=0;
		camera_lookat[1]=0;
		camera_lookat[2]=0;

		camera_fov=(30*0.78)/0.5236;
		viewanglex=0.0;
		viewanglez=0.0;
		camera_up[0]=0.0;
		camera_up[1]=0.0;
		camera_up[2]=1.0;
	}
	
	//------------��ġ����
	PutR3BspInfoText("Make Patch!\n");
	MakePatchAndWinding(p_mesh);	// ��Ƽ������Ʈ �޽��� ���ڷ�....
//	MakePatchAndWinding(p_mesh,1);	// ��Ƽ������Ʈ�� ���ڷ�....

	//------------����Ʈ���� ����Ʈ�� ����
    _LIGHTSOURCE *lgt_source=LoadLightASE(asefile);	//����Ʈ�ε�

	char buf[512];
	sprintf(buf,"Dmalloc size = %4.2fMByte\n",GetDmallocSize()/1000000.0f);
	PutR3BspInfoText(buf);

	GetLightRadiosity(p_mesh,one_mat,lgt_source);		// ����Ʈ�� ���� �������Ƽ�� ���Ѵ�.

	PutR3BspInfoText("Make Material Block...\n");
	//-----��� ������ �غ��Ѵ�.
	DWORD p_face_num = GetFaceNumFromMesh(p_mesh);

	R3BlockBsp CBlockBsp(p_face_num);						//��ϼ��� �غ�..
	CBlockBsp.PrepareBlockBsp(p_mesh,one_mat);				//��ϵ��� ���ĳ��´�.
	
	//-----������ ������Ʈ�� bsp�� �����.
	col_one_mesh=ConvOneMesh(w_mesh,org_mat);  // �����ΰ��� �ϳ��� �޽���
//	SetMeshFaceNormal(col_one_mesh,w_mesh);			// �ﰢ�޽��� normal�� ������. 
	_MESH *temp_p_mesh=ConvMeshTriToPoly(col_one_mesh);  // �ϳ��� �ﰢ�޽��� ������޽���

	col_p_mesh = CBlockBsp.MakePMeshBlockBsp(temp_p_mesh,p_mesh);

//	if( col_p_mesh->object->facenum

	PutR3BspInfoText("Now Bsp Compile...\n");
	MakeBSPTree(col_p_mesh);	//bsp-tree�� ����.. �浹�޽��� �༭...

	PutR3BspInfoText("Ok end!\n");
	BspPrint();	//bsp������

	sprintf(buf,"Dmalloc size = %4.2fMByte\n",GetDmallocSize()/1000000.0f);

	MakeLightmapsFromPatchs(p_mesh,one_mat,&CBlockBsp,composite_mat);				// ��ġ�κ��� ����Ʈ���� �����Ѵ�.
	SaveLightMap(p_mesh,composite_mat);


	RenderMesh = p_mesh;

	PutR3BspInfoText("Bsp Export!\n");
	BspExport(col_p_mesh,p_mesh,Ini->Out_file,&CBlockBsp);	//bsp ���� ����.
	//-------------------- �ؽ��ĸ� DDS�� ����Ʈ 
	PutR3BspInfoText("DDS Texture Export!\n");

	//--���͸��� ���� �н� ����.
	char mat_path[256]=".\\Map\\";
	char m_name[256];
	strcpy(m_name,Ini->In_file);
	StripPath(m_name);
	StripEXT(m_name);
	if( mat_path != NULL )
	{
		strcat(mat_path,m_name);
		strcat(mat_path,"\\tex");
		MakeDir(mat_path);
	}
//	printf("%s",mat_path);
	DDSTextureExport(p_mesh,one_mat);		//dds �ؽ��ķ� ����Ʈ�Ѵ�.

	//dds������ �����ؾ� ������ export�Ǿ��� ������ �������� �ʴ´�.
	MaterialExport(one_mat);		//���͸��� ��ũ���ͷ� ����Ѵ�.

//	if( org_mat )
//		MakeLightTextureFromLightmap();					//����Ʈ���� open-gl�ؽ��Ŀ� �ø���.

//	InitRenderLightmapFrom3DSMAXModel(p_mesh);		//open-gl�� �������� �غ� �Ѵ�.

	EndTimer();
	int gop=GetTimer()/1000;
	PutR3BspInfoText("****************************************\n");
	sprintf(buf,"Render Time %4d:%2d:%2d Seconds!\n",(gop/3600),(gop%3600)/60,gop%60);
	PutR3BspInfoText(buf);
	PutR3BspInfoText("****************************************\n");
	PutR3BspInfoText("�������� �������ϴ�. �ƹ�Ű�� �����ÿ�\n");
	getch();
	exit(0);
}
void ReadR3EngineIniFile(const char* inifile)
{
	char entry[256];

	_INIFILE *Ini=GetIniFile();

    GetPrivateProfileString("Path", "MapPath", "0", entry, 256, inifile);
	strcpy(Ini->MapPath,entry);
}
void ReadR3BspIniFile(const char* inifile)
{
	char entry[256];

	_INIFILE *Ini=GetIniFile();
	SetR3BspInfoFileName(Ini->In_file);	//�α����� �ʱ�ȭ

    GetPrivateProfileString("R3BSP", "R3BSPOptimize", "0", entry, 256, inifile);
    GetPrivateProfileString("ExportLightmapBMP", "FALSE", "0", entry, 256, inifile);
	strlwr(entry);
	if( !strcmp(entry,"true"))
		Ini->IsExportLightmapBMP = TRUE;
	else
		Ini->IsExportLightmapBMP = FALSE;

	if( !strcmp(entry,"NONE") )
		Ini->R3BspOptimize=_R3BSP_OPTIMIZE_NONE;
	else
		Ini->R3BspOptimize=_R3BSP_OPTIMIZE_HIGH;


	GetPrivateProfileString("RenderState","LightMapBits","16",entry, 256, inifile);
	Ini->LightMapBits=atoi(entry);	//����Ʈ��	��Ʈ��..

	GetPrivateProfileString("RenderState","RenderSpeed","FASTEST",entry, 256, inifile);
	if(!strcmp("RADIOSITY",entry))
		Ini->RenderState = _R_OPTIMIZE_RADIOSITY;
	else
	if(!strcmp("FAST",entry))
		Ini->RenderState = _R_OPTIMIZE_FAST;
	else
		Ini->RenderState = _R_OPTIMIZE_FASTEST;

	GetPrivateProfileString("RenderState","CPUNum","1",entry, 256, inifile);
	Ini->CPUNum=atoi(entry);	//���������� �����忡 ������.

	GetPrivateProfileString("RenderState","PatchSize","4.0",entry, 256, inifile);
	Ini->PatchSize=atof(entry);

//------------------------
	Ini->Ambient[0]=GetPrivateProfileInt("RenderEnv","AmbientRed", 0, inifile)/255.0f;
	Ini->Ambient[1]=GetPrivateProfileInt("RenderEnv","AmbientGreen", 0, inifile)/255.0f;
	Ini->Ambient[2]=GetPrivateProfileInt("RenderEnv","AmbientBlue", 0, inifile)/255.0f;
/*
	Ini->Reflection[0]=GetPrivateProfileInt("RenderEnv","ReflectionRed", 0, inifile)/255.0f;
	Ini->Reflection[1]=GetPrivateProfileInt("RenderEnv","ReflectionGreen", 0, inifile)/255.0f;
	Ini->Reflection[2]=GetPrivateProfileInt("RenderEnv","ReflectionBlue", 0, inifile)/255.0f;
	GetPrivateProfileString("RenderEnv","EnergyLimit","0.01",entry, 256, inifile);
	Ini->EnergyLimit=atof(entry);
//	Ini.res=GetPrivateProfileInt("Settings", "resolution", 256, inifile);
*/
	FILE *fp;
	fp = fopen(inifile,"rt");
	if( fp == NULL)
	{		
		Error((char*)inifile,"������ ����.");
	}
	char hole[256];

	while(1)
	{
		if( fscanf(fp,"%s",&hole[0])==EOF)
			break;
		if( !strcmp(hole,"[TexturePath]"))
		{
			while(1)
			{
				if( fscanf(fp,"%s",&hole[0])==EOF)
					break;
				AddTexturePath(hole);
			}
			break;
		}
	}	
	fclose(fp);
}

void main(int argc, char **argv)
{
	/*
	HDC dc=GetDC(0); // desktop dc
	int bpp=GetDeviceCaps(dc, BITSPIXEL);
	ReleaseDC(0, dc);
	if(bpp<24)
	{
		MessageBox(0, "Please change to 24 or 32 bit colour depth", "Warning", MB_TASKMODAL);
		return;
	}*/

//	TestDebugOutPut(2);
//-------------------- Ase�� ����â���� �ҷ��´�.
	static OPENFILENAME ofn; // zero the ofn members out
    char ase_name[256]="1.map";

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = ase_name;
	ofn.nMaxFile = sizeof(ase_name);
	ofn.lpstrFilter = "R3MAP Map (*.MAP)\0*.MAP\0All (*.*)\0*.*\0";
	ofn.lpstrTitle = "Select MAP file";
	ofn.Flags = OFN_NOCHANGEDIR | OFN_HIDEREADONLY |
               OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (!GetOpenFileName(&ofn))
		return;

	strcpy(GetIniFile()->In_file,ase_name);
//-------------------------------------------------
	StartTimer();
	glutInit(&argc, argv);

    ReadR3BspIniFile(".\\r3bsp.ini");	//������ ���� ������ �д´�.
    ReadR3EngineIniFile(".\\r3engine.ini");	//���� ���� ������ �д´�.

    glutInitWindowSize(512,512);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow(Wintitle);
	glutDisplayFunc(redraw);
	glutKeyboardFunc(key);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutIdleFunc(Step);
	glutReshapeFunc(reshape);

    ReadObject(GetIniFile()->In_file);

	glutCreateMenu(menu);
	glutAddMenuEntry("Stop", STOP);
	glutAddMenuEntry("Move Camera", MOVE_CAMERA);
	glutAddMenuEntry("Filtering", FILTER);

	if( Camera )
	{
		for(int i=0;i<Camera->num;i++)		//ī�޶� 
		{
			char name[128];
			sprintf(name,"camera%3d",i+1);
			glutAddMenuEntry(name,CAMERA+i);
		}
	}
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	viewanglex=viewanglez=0;

	glutMainLoop(); // the action starts with the Step function above
}
