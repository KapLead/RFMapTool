#include "C_vector.h"
#include "R3D3Dtex.h"
#include "R3Render.h"
#include "R3Text.h"
#include "R3Font.h"
#include "Jerror.h"
#include "core.h"

#define NTYPES1 8  // �ʼ��� ����
#define NTYPES2 4  // �߼��� ����
#define NTYPES3 4  // ������ ����


static char type1tbl_no[] = { // �߼��� ���� �ʼ� ����/��ħ ����
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3,
	3, 3, 1, 2, 4, 4, 4, 2, 1, 3, 0
};

static char type1tbl_yes[] = { // �߼��� ���� �ʼ� ����/��ħ ����
	5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 7,
	7, 7, 6, 6, 7, 7, 7, 6, 6, 7, 5
};

static char type3tbl[] = { // �߼��� ���� ���� ����
	0, 0, 2, 0, 2, 1, 2, 1, 2, 3, 0,
	2, 1, 3, 3, 1, 2, 1, 3, 3, 1, 1
};

#define U_LENG	(8/256.f)
#define V_LENG	(16/512.f)

#define HAN_U_LENG	(16/256.f)
#define HAN_V_LENG	(16/512.f)

static WORD JohapTable[18432];			//�������� ������ �ִ� ���̺�

static Vector2f stHanFont1UV[NTYPES1][20];	//�ʼ��߼������� uv��ǥ...
static Vector2f stHanFont2UV[NTYPES2][22];
static Vector2f stHanFont3UV[NTYPES3][28];
static Vector2f AsciiFontUV[256];
static int stTextID=-1;


static unsigned short GetHanWord(BYTE first, BYTE second)
{
	unsigned short ret;
 
	if (0x81 <= first && first <= 0xc8)
		ret = ((((unsigned short)first) << 8) | second) - 0x8100;
	else
		ret = 0;
 
	return ret;
}

static char Johap[256];
static char *ConvertJohapCode(char *wan)
{
	int i,leng = strlen(wan);
	WORD code;

	for(i=0;i<leng;i++)
	{
		code = GetHanWord(wan[i],wan[i+1]);
		if(  code == 0 )
		{
			Johap[i]=wan[i];
		}
		else
		{
			Johap[i]=(char)((JohapTable[code]>>8)&0xff);
			Johap[i+1]=(char)(JohapTable[code]&0xff);
			i++;
		}
	}
	Johap[i]=NULL;
	return Johap;
}
static BYTE *FillVbufferFromHan(BYTE *vbuf,float x,float y,unsigned short code,DWORD color)
{
	float *v = (float *)vbuf;
	DWORD *dv = (DWORD *)v;

	unsigned short h1, h2, h3, type[3];

	h1 = (code>>10) & 0x1f; // �ʼ� �ڵ�
	h2 = (code>>5)	& 0x1f; // �߼� �ڵ�
	h3 = code		& 0x1f; // ���� �ڵ�

	type[0] = h3 ? type1tbl_yes[h2] : type1tbl_no[h2];
	type[1] = ((h1 == 0 || h1 == 1 || h1 == 16) ? 0 : 1) + (h3 ? 2: 0);
	type[2] = type3tbl[h2];
	
	float *uv;
	for(int i=0; i<3; i++)
	{
		switch(i)
		{
			case 0:
				uv = stHanFont1UV[type[0]][h1];
				break;
			case 1:
				uv = stHanFont2UV[type[1]][h2];
				break;
			case 2:
				uv = stHanFont3UV[type[2]][h3];
				break;
		}
		v[0]=x;
		v[1]=y;
		v[2]=0.01f;
		v[3]=1;
		dv[4]=color;
		v[5]=uv[0];	//u
		v[6]=uv[1];	//v

		v+=sizeof(_D3DR3TLVERTEX_TEX1)/4;	//float ���̶� 
		dv+=sizeof(_D3DR3TLVERTEX_TEX1)/4;	//dword ���̶� 

		v[0]=x+16;
		v[1]=y;
		v[2]=0.01f;
		v[3]=1;
		dv[4]=color;
		v[5]=uv[0]+HAN_U_LENG;	//u
		v[6]=uv[1];	//v

		v+=sizeof(_D3DR3TLVERTEX_TEX1)/4;	//float ���̶� 
		dv+=sizeof(_D3DR3TLVERTEX_TEX1)/4;	//dword ���̶� 

		v[0]=x;
		v[1]=y+16;
		v[2]=0.01f;
		v[3]=1;
		dv[4]=color;
		v[5]=uv[0];	//u
		v[6]=uv[1]+HAN_V_LENG;	//v

		v+=sizeof(_D3DR3TLVERTEX_TEX1)/4;	//float ���̶� 
		dv+=sizeof(_D3DR3TLVERTEX_TEX1)/4;	//dword ���̶� 

		v[0]=x;
		v[1]=y+16;
		v[2]=0.01f;
		v[3]=1;
		dv[4]=color;
		v[5]=uv[0];	//u
		v[6]=uv[1]+HAN_V_LENG;	//v

		v+=sizeof(_D3DR3TLVERTEX_TEX1)/4;	//float ���̶� 
		dv+=sizeof(_D3DR3TLVERTEX_TEX1)/4;	//dword ���̶� 

		v[0]=x+16;
		v[1]=y;
		v[2]=0.01f;
		v[3]=1;
		dv[4]=color;
		v[5]=uv[0]+HAN_U_LENG;	//u
		v[6]=uv[1];	//v

		v+=sizeof(_D3DR3TLVERTEX_TEX1)/4;	//float ���̶� 
		dv+=sizeof(_D3DR3TLVERTEX_TEX1)/4;	//dword ���̶� 

		v[0]=x+16;
		v[1]=y+16;
		v[2]=0.01f;
		v[3]=1;
		dv[4]=color;
		v[5]=uv[0]+HAN_U_LENG;	//u
		v[6]=uv[1]+HAN_V_LENG;	//v

		v+=sizeof(_D3DR3TLVERTEX_TEX1)/4;	//float ���̶� 
		dv+=sizeof(_D3DR3TLVERTEX_TEX1)/4;	//dword ���̶� 
	}

	return (BYTE*)v;
}

static BYTE *FillVbufferFromAscii(BYTE *vbuf,float x,float y,char ascii,DWORD color)
{
	float *v = (float *)vbuf;
	DWORD *dv = (DWORD *)v;

	v[0]=x;
	v[1]=y;
	v[2]=0.01f;
	v[3]=1;
	dv[4]=color;
	v[5]=AsciiFontUV[ascii][0];	//u
	v[6]=AsciiFontUV[ascii][1];	//v

	v+=sizeof(_D3DR3TLVERTEX_TEX1)/4;	//float ���̶� 
	dv+=sizeof(_D3DR3TLVERTEX_TEX1)/4;	//dword ���̶� 

	v[0]=x+8;
	v[1]=y;
	v[2]=0.01f;
	v[3]=1;
	dv[4]=color;
	v[5]=AsciiFontUV[ascii][0]+U_LENG;	//u
	v[6]=AsciiFontUV[ascii][1];	//v

	v+=sizeof(_D3DR3TLVERTEX_TEX1)/4;	//float ���̶� 
	dv+=sizeof(_D3DR3TLVERTEX_TEX1)/4;	//dword ���̶� 

	v[0]=x;
	v[1]=y+16;
	v[2]=0.01f;
	v[3]=1;
	dv[4]=color;
	v[5]=AsciiFontUV[ascii][0];	//u
	v[6]=AsciiFontUV[ascii][1]+V_LENG;	//v

	v+=sizeof(_D3DR3TLVERTEX_TEX1)/4;	//float ���̶� 
	dv+=sizeof(_D3DR3TLVERTEX_TEX1)/4;	//dword ���̶� 

	v[0]=x;
	v[1]=y+16;
	v[2]=0.01f;
	v[3]=1;
	dv[4]=color;
	v[5]=AsciiFontUV[ascii][0];	//u
	v[6]=AsciiFontUV[ascii][1]+V_LENG;	//v

	v+=sizeof(_D3DR3TLVERTEX_TEX1)/4;	//float ���̶� 
	dv+=sizeof(_D3DR3TLVERTEX_TEX1)/4;	//dword ���̶� 

	v[0]=x+8;
	v[1]=y;
	v[2]=0.01f;
	v[3]=1;
	dv[4]=color;
	v[5]=AsciiFontUV[ascii][0]+U_LENG;	//u
	v[6]=AsciiFontUV[ascii][1];	//v

	v+=sizeof(_D3DR3TLVERTEX_TEX1)/4;	//float ���̶� 
	dv+=sizeof(_D3DR3TLVERTEX_TEX1)/4;	//dword ���̶� 

	v[0]=x+8;
	v[1]=y+16;
	v[2]=0.01f;
	v[3]=1;
	dv[4]=color;
	v[5]=AsciiFontUV[ascii][0]+U_LENG;	//u
	v[6]=AsciiFontUV[ascii][1]+V_LENG;	//v

	v+=sizeof(_D3DR3TLVERTEX_TEX1)/4;	//float ���̶� 
	dv+=sizeof(_D3DR3TLVERTEX_TEX1)/4;	//dword ���̶� 

	return (BYTE*)v;
}
/*
void R3LoadTextTexture()
{
	//-��Ʈ �ؽ��ĸ� �ε�.
	stTextID =R3GetPreTextureId(".\\System\\han.tga");//ó���� �ε��� �ؽ��� ���̵� ����.������ �ؽ��� �޸𸮿� ������ �ʴ´�.
	R3LoadTextureMem(stTextID);	//Ư�� �ؽ��ĸ� �ؽ��ĸ޸𸮿� ��´�.
}
void R3ReleaseTextTexture()
{
	R3ReleaseTextureMem(stTextID);	//Ư�� �ؽ��ĸ� �ؽ��ĸ޸𸮿� ������.
	stTextID=-1;
}
void InitR3Text()
{
	int i,j;


	float xpos=0;
	float ypos=384;
	for(i=0 ;i<256; i++)	//uv��ǥ�� ����...
	{
		AsciiFontUV[i][0] = xpos/256.f;
		AsciiFontUV[i][1] = ypos/512.f;
		xpos+=8;
		if( xpos >=256 )
		{
			xpos=0;
			ypos+=16;
		}
	}

	xpos=0;
	ypos=0;
	for(i=0;i<NTYPES1;i++)	//�ʼ� uv��ǥ�� ����...
	{
		for(j=0;j<20;j++)
		{
			stHanFont1UV[i][j][0] = xpos/256.f;
			stHanFont1UV[i][j][1] = ypos/512.f;
			xpos+=16;
			if( xpos >=256 )
			{
				xpos=0;
				ypos+=16;
			}
		}
	}
	xpos=0;
	ypos=160;
	for(i=0;i<NTYPES2;i++)	//�߼� uv��ǥ�� ����...
	{
		for(j=0;j<22;j++)
		{
			stHanFont2UV[i][j][0] = xpos/256.f;
			stHanFont2UV[i][j][1] = ypos/512.f;
			xpos+=16;
			if( xpos >=256 )
			{
				xpos=0;
				ypos+=16;
			}
		}
	}
	xpos=0;
	ypos=256;
	for(i=0;i<NTYPES3;i++)	//���� uv��ǥ�� ����...
	{
		for(j=0;j<28;j++)
		{
			stHanFont3UV[i][j][0] = xpos/256.f;
			stHanFont3UV[i][j][1] = ypos/512.f;
			xpos+=16;
			if( xpos >=256 )
			{
				xpos=0;
				ypos+=16;
			}
		}
	}

	FILE *fp;
    if ((fp = fopen (".\\System\\hangul.tbl", "rb")) == NULL)
        Error(".\\System\\hangul.tbl","�� ������ �Ⱥ��δ�.");
	fread (JohapTable,18432*2, 1, fp);

	fclose (fp);

}
static void DrawR3Hangul(LONG x,LONG y,char *buf,DWORD color,BOOL out_line
		,LONG shadow_add_x,LONG shadow_add_y,DWORD shadow_color)
{
	int buf_leng = strlen(buf);
	float xpos,xx,yy;
	BYTE vbuf[24*5440];	//�ִ� 80��(�ѱ�)����...

	if( stTextID == -1 )
		Error(" ��Ʈ �ؽ��ĸ޸𸮿� �b������..","");
	if( buf_leng == 0 )
		return;
	if( buf_leng >= 160 )
	{
		buf[158]=NULL;	//�ϴ� ©��...
		buf[159]=NULL;
	}


	buf = ConvertJohapCode(buf);	//�������ڵ�� �ٲ۴�...

	xx=(float)x-.5f;
	yy=(float)y-.5f;
	xpos = xx;
//	float *v = (float *)GetDynamicVertexBuffer();
	float *v = (float *)vbuf;
	DWORD *dv = (DWORD *)v;

	int polys=0;
	char *data=buf;
	BYTE *temp;
	while (*data)
	{
		if(*data & 0x80)                             // �ѱ� �ڵ���
		{
			WORD hancode = ((*((short *)data)&0x00ff)<<8) | ((*((short *)data)&0xff00)>>8);
			if( out_line )
			{
				temp=FillVbufferFromHan((BYTE *)v,xpos+shadow_add_x,yy+shadow_add_y,hancode,shadow_color);
				polys+=((temp-(BYTE *)v)/(24*6))*2;	//���ؽ����� �����Ѹ�ŭ�����ﰹ���� �ø���.
				v=(float*)temp;
			}
			temp=FillVbufferFromHan((BYTE *)v,xpos,yy,hancode,color);
			polys+=((temp-(BYTE *)v)/(24*6))*2;	//���ؽ����� �����Ѹ�ŭ�����ﰹ���� �ø���.
			v=(float*)temp;
			data += 2;
			xpos+=16;
		}
		else // �ƽ�Ű �ڵ���
		{
			if( out_line )
			{
				temp=FillVbufferFromAscii((BYTE *)v,xpos+shadow_add_x,yy+shadow_add_y,*data,shadow_color);
				v=(float*)temp;
				polys+=2;
			}
			v=(float*)FillVbufferFromAscii((BYTE *)v,xpos,yy,*data,color);
			xpos+=8;
			data++;
			polys+=2;
		}
	}

	BOOL alpha_on;
//	if( (color&0xff000000) != 0xff000000)
		alpha_on=TRUE;
//	else
//		alpha_on=FALSE;

	DrawTLVertex(stTextID,polys,_BLEND_OPPA,(_LPD3DR3VERTEX *)vbuf);	//�̸� ���ؽ����ۿ� �����ؾߵȴ�.
}

void DrawR3ShadowedHangul(LONG x,LONG y,char *buf,DWORD color
		,LONG shadow_add_x,LONG shadow_add_y,DWORD shadow_color)
{
	DrawR3Hangul(x,y,buf,color,TRUE,shadow_add_x,shadow_add_y,shadow_color);
}

void DrawR3Hangul(LONG x,LONG y,char *buf,DWORD color)
{
	DrawR3Hangul(x,y,buf,color,FALSE,0,0,0);
}
void ReleaseR3Text()
{
	stTextID = -1;
}
*/
//-----------���� ��� Ŀ��.

static CR3Font stCR3Font;

void InitR3Text()
{
}
void ReleaseR3Text()
{
}

BOOL st_bIsTexTexture=FALSE;

void R3LoadTextTexture()	//�ؽ�Ʈ�� �ؽ��ĸ޸𸮸� ����. �ø�������ߵȴ�.
{
	if( st_bIsTexTexture )
		return;
	st_bIsTexTexture=TRUE;
	LPDIRECT3DDEVICE8 pd3dDevice = GetD3dDevice();
	if( pd3dDevice==NULL )
		Error("R3LoadTextTexture()�� d3d�� �ʱ�ȭ�ϰ� ������.","");

	DWORD vid_mem = GetR3State()->mAvailableTextureMem;	//�����޸𸮺��� �����Ҵ�.
	if( vid_mem > 160000000 )
		stCR3Font.InitDeviceObjects( pd3dDevice ,1024,1024);
	else
	if( vid_mem > 100000000 )
		stCR3Font.InitDeviceObjects( pd3dDevice ,1024,512);
	else
		stCR3Font.InitDeviceObjects( pd3dDevice ,512,512);
	stCR3Font.RestoreDeviceObjects(  );

}
void R3ReleaseTextTexture()
{
	if( !st_bIsTexTexture )
		return;
	st_bIsTexTexture=FALSE;
	stCR3Font.InvalidateDeviceObjects( );
	stCR3Font.DeleteDeviceObjects(  );
}
void DrawR3Hangul(LONG x,LONG y,char *buf,DWORD color,DWORD flag)
{
	GetD3dDevice()->SetRenderState( D3DRS_ZENABLE, FALSE );
	stCR3Font.DrawText( (float)x, (float)y, color,buf,flag);
	GetD3dDevice()->SetRenderState( D3DRS_ZENABLE, TRUE );
}
void DrawR3Hangul3D(float xyzw[4],char *buf,DWORD color,DWORD flag)
{
	GetD3dDevice()->SetRenderState( D3DRS_ZENABLE, TRUE );
	stCR3Font.DrawText( xyzw,color,buf,flag);
}

void DrawR3Hangul(LONG x,LONG y,char *buf,DWORD color, float clipping[2][2], DWORD flag)
{
	GetD3dDevice()->SetRenderState( D3DRS_ZENABLE, FALSE );
	stCR3Font.DrawText( (float)x, (float)y, color, buf, clipping, flag );
	GetD3dDevice()->SetRenderState( D3DRS_ZENABLE, TRUE );
}


DWORD GetOutLineColor()
{
	return stCR3Font.GetOutLineColor();
}

void SetOutLineColor(DWORD color)
{
	stCR3Font.SetOutLineColor(color);
}

