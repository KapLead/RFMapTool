#ifndef __2DSPRITE_H__
#define __2DSPRITE_H__

#include "windows.h"
#include "R3D3D8.h"
#include "C_vector.h"

#define _MAX_ACTIONS 16
#define _SELF_SIZE	65535
#define _NOW_STATE	65535

typedef struct{
	DWORD Format;
	DWORD Size;
	WORD OrgXL,OrgYL;		//��������Ʈ�� ���������� 
	WORD XL, YL;			//��������Ʈ �ؽ��� ������
	void *Tex;	//�ؽ��� ������
}_FRAME_INFO;

class CSprite{
private:
	BOOL mIsLoadedTexMem;
	DWORD mActions;
	BOOL mPageSPrite;	//������ ��������Ʈ�ΰ�?
	DWORD mPage;		//�ϳ��� �ؽ��ķ� �������� ��������Ʈ�� �����Ұ��.
	WORD mPageElementXL;	//������ ���� ���׸��� �ؽ����� ���α���..
	WORD mPageElementYL;	//������ ���� ���׸��� �ؽ����� ����..
	WORD mFrames[_MAX_ACTIONS];
	DWORD mNowAction;
	DWORD mNowFrame;
	float mAniFrame;	//������ 1/1000�ʴ�
	float mFPS;	//�ʴ� �����ӷ�
	_FRAME_INFO *mFrameInfo[_MAX_ACTIONS];
	char mFileName[128];
public:
	CSprite();
	~CSprite();
	void LoadSprite(char *name);	
	void ReleaseSprite();

	//�ؽ��� ū���� �������� ��������Ʈ�� ����ϴ°��. �߰� API
	void InitPageSprite(WORD xl,WORD yl);	//�ϳ��� �ؽ��ķ� �������� ��������Ʈ�� �����Ұ��.��ҿ�� ����� �����Ѵ�.
	void SetPage(DWORD page);				//�ش� �ؽ����� �������� �����Ѵ�.
	DWORD GetPageNum(){		return mFrames[0];	}	//������ ��������Ʈ ����������
	DWORD GetNowPage(){		return mPage;	}		//���� ������ ������ �ε���������...
	BOOL IsPageSprite(){	return mPageSPrite;	}	//������ ��������Ʈ�ΰ�?
	WORD GetPageElementXL() { return mPageElementXL; }
	WORD GetPageElementYL() { return mPageElementYL; }
	
	char *GetFileName();
	void RestoreTexMemSprite();	//��������Ʈ�� �ؽ��� �޸𸮿� �ø���.
	void ReleaseTexMemSprite();	//��������Ʈ�� �ؽ��� �޸𸮿� ������.

	void SetAction(DWORD action);		//�׼��� �ٲ۴�.
	DWORD GetNowActions();				//���� �������ִ� �׼��� �˾Ƴ���.
	DWORD GetMaxActions();				//�� ��������Ʈ�� �� �׼��� �˾Ƴ���.

	void SetFrame(DWORD frame);			//���� �������� �������ش�.
	DWORD GetNowFrames(DWORD action);	// ���� ���õ��ִ� �������� �˾Ƴ���.
	DWORD GetMaxFrames(DWORD action);	//���� �׼��� �� �����Ӽ��� �˾Ƴ���.

	void SetAniAnimationSpeed(DWORD speed);		//�ʴ� �����Ӽ��� �־��ش�. 1�� 30frame �϶� 30
	void RunAnimation();						//�ִϸ��̼��� �� ���.

	WORD GetSpriteOrgXL(DWORD action=_NOW_STATE,DWORD frame=_NOW_STATE); //���ڰ� ������ ���� �׼ǰ� ���� �������� ����� ���ϵȴ�.
	WORD GetSpriteOrgYL(DWORD action=_NOW_STATE,DWORD frame=_NOW_STATE); //���ڰ� ������ ���� �׼ǰ� ���� �������� ����� ���ϵȴ�.

	WORD GetSpriteXL(DWORD action=_NOW_STATE,DWORD frame=_NOW_STATE);
	WORD GetSpriteYL(DWORD action=_NOW_STATE,DWORD frame=_NOW_STATE);

	// 1:1�ȼ��� �Ѹ����� ���� x,y�� �־��ش�.DrawSprite(20,40);
	void DrawSprite(LONG x,LONG y,LONG ex=_SELF_SIZE,LONG ey=_SELF_SIZE,DWORD argb=0xffffffff);//�ݵ�� beginscene(),endscene()���̿� �־�ߵ�.
	//uv�� �����ϴ� ��������Ʈ API 
	void DrawSpriteUV(LONG x,LONG y,float uv[2][2],LONG ex=_SELF_SIZE,LONG ey=_SELF_SIZE,DWORD argb=0xffffffff);//�ݵ�� beginscene(),endscene()���̿� �־�ߵ�.
	//ȸ�� ��������ƮAPI angle���� ���� 0-360��
	void DrawSpriteRot(LONG x,LONG y,float angle,LONG ex=_SELF_SIZE,LONG ey=_SELF_SIZE,DWORD argb=0xffffffff);//�ݵ�� beginscene(),endscene()���̿� �־�ߵ�.
	//uv�� �����ϴ� ȸ�� ��������Ʈ ( by Nadia )
	void DrawSpriteRotUV( LONG x, LONG y, float angle, float uv[2][2], LONG ex=_SELF_SIZE, LONG ey=_SELF_SIZE, DWORD argb=0xffffffff );

	// vertex ����
	void DrawSpriteVertex( _D3DR3TLVERTEX_TEX1 * pVertex, int nPrimitiveNum );

	void *GetD3D8TexPtr(DWORD action=_NOW_STATE,DWORD frame=_NOW_STATE);//���ڰ� ������ ���� �׼ǰ� ���� �������� ����� ���ϵȴ�.
};

//�ؽ��� ���� �簢���� �Ѹ����.. �������� �ְ������ ARGB color�� ���İ��� ��������.
void Draw2DRectangle(LONG x,LONG y,LONG ex,LONG ey,DWORD color);//�Ѹ���.
void Draw2DTexture(LONG x,LONG y,LONG ex,LONG ey,DWORD color,void *tex);//�Ѹ���.

void Draw2DLineList(Vector2f *line,DWORD line_num,DWORD color);//���ε� �Ѳ����� �Ѹ���.
void Draw2DLine(Vector2f start,Vector2f end,DWORD color);//���� �ϳ� �Ѹ���.


#endif