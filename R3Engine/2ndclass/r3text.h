#ifndef __R3TEXT_H__
#define __R3TEXT_H__

void InitR3Text();
void ReleaseR3Text();
void R3LoadTextTexture();	//�ؽ�Ʈ�� �ؽ��ĸ޸𸮸� ����. �ø�������ߵȴ�.
void R3ReleaseTextTexture();

//--������ �ѱ� dwFlag�Դϴ�.
#define R3_HAN_BOLD			0x00000001
#define R3_HAN_ITALIC		0x00000002
#define R3_HAN_ZENABLE		0x00000004
#define R3_HAN_UNDERLINE	0x00000008
#define R3_HAN_SHADOW		0x00000010
#define R3_HAN_OUTLINE		0x00000020
#define R3_HAN_BRIGHT_BLEND	0x00000040	//���� �κ��� ���� ����ϴ� ȿ��...

void DrawR3Hangul(LONG x,LONG y,char *buf,DWORD color,DWORD flag=0);
void DrawR3Hangul3D(float xyzw[4],char *buf,DWORD color,DWORD flag=0);

void DrawR3Hangul(LONG x,LONG y,char *buf,DWORD color, float clipping[2][2], DWORD flag=0);

//�ƿ����ι� ������ �÷� ����..
DWORD GetOutLineColor();
void SetOutLineColor(DWORD color);

/*
void DrawR3ShadowedHangul(LONG x,LONG y,char *buf,DWORD color
		,LONG shadow_add_x,LONG shadow_add_y,DWORD shadow_color);
*/

#endif