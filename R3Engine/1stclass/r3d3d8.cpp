#include <assert.h>
#include "d3d8.h"
#include "d3dx8.h"
#include <stdio.h>

#include "R3Input.h"
#include "CommonUtil.h"
#include "Jmalloc.h"
#include "JError.h"
#include "Core.h"
#include "R3VertexBuffer.h"
#include "R3d3dtex.h"
#include "R3math.h"
#include "d3dutil.h"
#include "r3VertexShader.h"
#include "2dSprite.h"
#include "R3Util.h"


//--------------------------------------------------------------------------------------


void SetDefaultFlags()
{
	LPDIRECT3DDEVICE8 pd3dDevice = GetD3dDevice();

	pd3dDevice->SetRenderState( D3DRS_CULLMODE,   D3DCULL_CCW );
    pd3dDevice->SetRenderState( D3DRS_LIGHTING,     FALSE );
	pd3dDevice->LightEnable( 0,FALSE);
	pd3dDevice->LightEnable( 1,FALSE);
	pd3dDevice->LightEnable( 2,FALSE);
	pd3dDevice->LightEnable( 3,FALSE);
	pd3dDevice->LightEnable( 4,FALSE);
	pd3dDevice->LightEnable( 5,FALSE);
	pd3dDevice->LightEnable( 6,FALSE);
	pd3dDevice->LightEnable( 7,FALSE);
	
	pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE ); 
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2 );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
//	pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_POINT );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR );
//D3DTEXF_NONE            = 0,
  //  D3DTEXF_POINT           = 1,
    //D3DTEXF_LINEAR          

	pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 1 );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE2X );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
    pd3dDevice->SetTextureStageState( 1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );

	pd3dDevice->SetTextureStageState( 2, D3DTSS_TEXCOORDINDEX, 2 );
	pd3dDevice->SetTextureStageState( 2, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 2, D3DTSS_COLORARG2, D3DTA_CURRENT );
//	pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState( 2, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
    pd3dDevice->SetTextureStageState( 2, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetTextureStageState( 2, D3DTSS_MIPFILTER, D3DTEXF_LINEAR );
}



static LPDIRECT3DDEVICE8 stPd3dDevice;
//static DWORD stdwVShader[256];

#define SHADERTYPE_VERTEX		1
#define SHADERTYPE_PIXEL		2
//-------------------
// Loads a .vso file and creates a vertex shader for it
HRESULT LoadAndCreateShader(char *strFilePath, const DWORD* pDeclaration, DWORD Usage, int ShaderType, DWORD* pHandle)
{
	assert(stPd3dDevice);
	try
	{
		HANDLE hFile;
		HRESULT hr;

		hFile = CreateFile(strFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			Warning("Could not find file ",strFilePath);
			return E_FAIL;
		}

		DWORD dwFileSize = GetFileSize(hFile, NULL);

		const DWORD* pShader = (DWORD*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwFileSize);
		if (!pShader)
		{
			Warning("Failed to allocate memory to load shader ", strFilePath);
			return E_FAIL;
		}

		ReadFile(hFile, (void*)pShader, dwFileSize, &dwFileSize, NULL);

		CloseHandle(hFile);

		if (ShaderType == SHADERTYPE_VERTEX)
		{
			hr = stPd3dDevice->CreateVertexShader(pDeclaration, pShader, pHandle, Usage);
		}
		else if (ShaderType == SHADERTYPE_PIXEL)
		{
			hr = stPd3dDevice->CreatePixelShader(pShader, pHandle);
		}

		HeapFree(GetProcessHeap(), 0, (void*)pShader);

		if (FAILED(hr))
		{
			Warning("Failed to create shader ", strFilePath);; 
			return E_FAIL;
		}
	}
	catch(...)
	{
		Warning("Error opening file ", strFilePath);
		return E_FAIL;
	}

	return S_OK;
}



//---R3 ����̽� ����.
static void ReleaseVShader(DWORD shader)
{
    stPd3dDevice->DeleteVertexShader( shader );
}


static DWORD CreateVShader(char *filename,DWORD *dwDecl)
{
    HRESULT hr;
	DWORD shader;

//	stPd3dDevice->SetRenderState(D3DRS_SOFTWAREVERTEXPROCESSING, TRUE);
	hr = LoadAndCreateShader(filename, dwDecl, 0, SHADERTYPE_VERTEX, &shader);
	if (FAILED(hr))
	{
		Warning("","<- �� ���̴��� �ʱ�ȭ �ȵ�.");
		return 0;
	}


/*
    LPD3DXBUFFER pCode;
    // Assemble the vertex shader from the file
    if( FAILED( hr = D3DXAssembleShaderFromFile( filename, 
                                                 0, NULL, &pCode, NULL ) ) )
	{
  		Error(filename,"<-���̴� �ε��Ұ�");
	}

    // Create the vertex shader
    hr = stPd3dDevice->CreateVertexShader( dwDecl, (DWORD*)pCode->GetBufferPointer(),
                                           &shader, 0 );
    pCode->Release();
    if( FAILED(hr) )
	{
		Error("���̴� �����Ұ�","");
	}
*/
	return shader;
}

//	D3DFMT_R8G8B8
//	D3DFMT_X8R8G8B8
//	D3DFMT_A8R8G8B8
//	D3DFMT_R5G6B5
//	D3DFMT_X1R5G5B5
//	D3DFMT_A1R5G5B5
BOOL IsTextureFormatOk( D3DFORMAT TextureFormat, D3DFORMAT AdapterFormat ) 
{
	LPDIRECT3D8 temp;
	stPd3dDevice->GetDirect3D(&temp);
    HRESULT hr = temp->CheckDeviceFormat( D3DADAPTER_DEFAULT,
                                          D3DDEVTYPE_HAL,
                                          AdapterFormat,
                                          0,
                                          D3DRTYPE_TEXTURE,
                                          TextureFormat);
	temp->Release();
    return SUCCEEDED( hr );
}

LPDIRECT3DSURFACE8 stOldRenderTarget;
LPDIRECT3DSURFACE8 stOldStencilZ;

HRESULT D3D_R3InitDevice( LPDIRECT3DDEVICE8 pd3dDevice )
{
	stPd3dDevice=pd3dDevice;
	_R3ENGINE_STATE *state = GetR3State();

//#include "renderflag.h"	//ó�� �÷��׵��� �����ϱ����� 
//	SaveRenderState();
//	SaveTextureStage();

	//��밡���� �ؽ��ĸ޸�..
	state->mAvailableTextureMem = pd3dDevice->GetAvailableTextureMem();
	if( state->mAvailableTextureMem < 80000000 )		//80�ް� 
		state->mThisVidMemLevel = 2;
	else
	if( state->mAvailableTextureMem < 128000000 )		//128�ް� 
		state->mThisVidMemLevel = 1;
	else
		state->mThisVidMemLevel = 0;

	//�ؽ��� ���� 
	D3DDISPLAYMODE display;
	pd3dDevice->GetDisplayMode(&display);
	display.Format;

	BOOL test = IsTextureFormatOk( D3DFMT_DXT1, display.Format );
	
	if( test )	//DDS�����Ѵ�...�ѽ�.
	{
		state->mBestTextureFormat=D3DFMT_DXT1;		//���� ������ �⺻ �ؽ�������
		state->mBestSpriteTextureFormat=D3DFMT_DXT1;	//��������Ʈ�� �� ���� ������ �ؽ�������
		state->mBestAlphaTextureFormat=D3DFMT_DXT5;	//���İ� ���� ���� ������ �ؽ�������
//		test = IsTextureFormatOk( D3DFMT_R5G6B5, display.Format );
//		if( !test )
//			Warning("�� ī�� �ؽ��������� �� �����ϴ�.","");
//		state->mBest2ndTextureFormat=D3DFMT_R5G6B5;		//���� ������ 2��° �⺻ �ؽ�������
	}
	else//��dds �������ϴ±�...
	{
		test = IsTextureFormatOk( D3DFMT_R5G6B5, display.Format );
		if( !test )
		{
			Warning("�� ī�� �ؽ��������� �� �����ϴ�.","");
		}
		state->mBestTextureFormat=D3DFMT_R5G6B5;		//���� ������ �⺻ �ؽ�������
//		state->mBest2ndTextureFormat=D3DFMT_R5G6B5;		//���� ������ 2��° �⺻ �ؽ�������
		//������ ��������Ʈ ����
		test = IsTextureFormatOk( D3DFMT_A1R5G5B5, display.Format );
		if( !test )
		{
			test = IsTextureFormatOk( D3DFMT_A4R4G4B4, display.Format );
			if( !test )
			{
				test = IsTextureFormatOk( D3DFMT_A8R8G8B8, display.Format );
				if( !test )
				{
					Warning("�����ؽ��� ������ �� 3Dī�忡���� �����ϴ�.","");
					state->mBestSpriteTextureFormat=D3DFMT_A1R5G5B5;
				}
				else
				{
					state->mBestSpriteTextureFormat=D3DFMT_A8R8G8B8;
				}
			}
			else
			{
				state->mBestSpriteTextureFormat=D3DFMT_A4R4G4B4;
			}
		}
		else
		{
			state->mBestSpriteTextureFormat=D3DFMT_A1R5G5B5;
		}
		//������ ���� ����
		test = IsTextureFormatOk( D3DFMT_A4R4G4B4, display.Format );
		if( !test )
		{
			test = IsTextureFormatOk( D3DFMT_A8R8G8B8, display.Format );
			if( !test )
			{
				Warning("�����ؽ��� ������ �� 3Dī�忡���� �����ϴ�.","");
				state->mBestAlphaTextureFormat=D3DFMT_A4R4G4B4;
			}
			else
			{
				state->mBestAlphaTextureFormat=D3DFMT_A8R8G8B8;
			}
		}
		else
		{
			state->mBestAlphaTextureFormat=D3DFMT_A4R4G4B4;
		}
	}

	D3DCAPS8 caps;
	pd3dDevice->GetDeviceCaps(&caps);
	if( caps.MaxSimultaneousTextures < 2 )
		Error("������ 3Dī�尡 ���� APP�� ������ ��� ����� �����ϴ�.","");

	state->mMaxTexSize= caps.MaxTextureWidth;	//��ī���� �ִ� �ؽ��� ������


	if( state->mIsUsedVertexShader )
	{
		if(	LoadVertexShaderList() == FALSE)	//���̴��� �дµ� �����ߴ�.
			state->mIsUsedVertexShader = FALSE;
	}

    return S_OK;
}

//-------- �����ӹ��� ����.
static BOOL stRenderTargetFrameBuffer=TRUE;
void SetTextureRenderTargetFrameBuffer(BOOL state)	
{
	stRenderTargetFrameBuffer = state;
}

BOOL IsRenderTargetFrameBuffer()	//���� �����ӹ��۰� ���� Ÿ���ΰ�?
{
	return stRenderTargetFrameBuffer;
}

HRESULT D3D_R3DeleteDevice()
{
	ReleaseVertexShaderList();
	stPd3dDevice=NULL;
	return S_OK;
}


void DrawLogo()	//�ΰ� �Ѹ���.
{
	_R3ENGINE_STATE *state=GetR3State();
	if( !state->mIsDrawLogo )	//�ΰ� �Ѹ��� �����Ÿ�.
		return;
	float y1 = (640*state->mScreenYsize)/768.0f;
	float y2 = (748*state->mScreenYsize)/768.0f;
	//����Ʈ Ǯ�� �ٲٰ�...
	SetViewPortArea(0,0,(LONG)state->mScreenXsize,(LONG)state->mScreenYsize);
	Draw2DTexture(0,(LONG)y1,(LONG)state->mScreenXsize,(LONG)y2,0xfeffffff,state->mLogoTexture);
}


//---------- beginscene �� ��� ��ο� ��Ʈ

CTextureRender stShadow;	//�����쾵��.
CTextureRender stRToTMain;	//���λ��� ���η������Ұ��� �̰����� �����Ѵ�.
CTextureRender stRToTBlur;	//���� ��Ǻ�ο��� ����... 
static DWORD stTexSize;
static DWORD stFullScreenEffectState=FALSE;		//�̰� �÷��� 
static DWORD stRealFullScreenEffectState=FALSE;	//�̰� ��¥��ξ� ����..
static DWORD stOldRealFullScreenEffectState=FALSE;

void R3ClearFrameBuffer()
{
	LPDIRECT3DDEVICE8 pDevice = GetD3dDevice();
	if( pDevice== NULL )
		return;

	_R3ENGINE_STATE *state = GetR3State();
/*
	if( !IsRenderTargetFrameBuffer() )	//���� �ؽ��ķ��������� ����Ÿ���� �ı��Ȱ����..
	{
		SetTextureRenderTargetFrameBuffer(TRUE);	//Ʈ��� �ٲ��ְ�.
		pDevice->SetRenderTarget(stOldRenderTarget, stOldStencilZ);
		PopViewPortArea();
		SetViewPortArea(0,0,(LONG)state->mScreenXsize,(LONG)state->mScreenYsize);
	}
*/
	if( state->mWideOn )
	{
		SetViewPortArea(0,0,(LONG)state->mScreenXsize,(LONG)state->mScreenYsize );
		pDevice->Clear( 0, NULL, D3DCLEAR_TARGET
			,D3DXCOLOR(0,0,0,0), 1.0f, 0L );
		LONG wide_yl = (LONG)(state->mScreenXsize*(352.0f/640.0f));
		SetViewPortArea(0,(LONG)(state->mScreenYsize-wide_yl)/2,(LONG)state->mScreenXsize,wide_yl);
		SetFrustumNormalPlane();
	}
	if( !stRealFullScreenEffectState )
	{
		pDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER
	//    pDevice->Clear( 0, NULL,D3DCLEAR_ZBUFFER
	//		,0xffff00ff
			,GetR3State()->mFogColor
			, 1.0f, 0L );
	}
	else
		stRToTMain.ClearFrameBuffer();	//�̳༮�� �⺻���� ������ �־�ߵȴ�.

}

BOOL R3BeginScene()
{
	LPDIRECT3DDEVICE8 pDevice = GetD3dDevice();

	if( pDevice== NULL )
		return FALSE;

	if( !IsRenderTargetFrameBuffer() )	//���� �ؽ��ķ��������� ����Ÿ���� �ı��Ȱ����..
	{
		_R3ENGINE_STATE *state = GetR3State();
		ResetVertexBufferCache();
		SetTextureRenderTargetFrameBuffer(TRUE);	//Ʈ��� �ٲ��ְ�.
		pDevice->SetRenderTarget(stOldRenderTarget, stOldStencilZ);
		SetViewPortArea(0,0,(LONG)state->mScreenXsize,(LONG)state->mScreenYsize);
	}
	
	if( stFullScreenEffectState )
		stRealFullScreenEffectState = stFullScreenEffectState;
	else
		stRealFullScreenEffectState = FALSE;

	if( !stRealFullScreenEffectState )
	{/*
		_R3ENGINE_STATE *state = GetR3State();
		state->mScreenXsize=(float)state->mViewXL;
		state->mScreenYsize=(float)state->mViewYL;
		SetViewPortArea((LONG)state->mViewSx,(LONG)state->mViewSy
			,(LONG)state->mViewXL,(LONG)state->mViewYL);
*/
		if( !SUCCEEDED( pDevice->BeginScene()))
			return FALSE;
	}
	else
	{
		stRToTMain.BeginScene();
		/*
		_R3ENGINE_STATE *state = GetR3State();
		state->mScreenXsize=state->mViewXL;
		state->mScreenYsize=state->mViewYL;
		SetViewPortArea((LONG)state->mViewSx,(LONG)state->mViewSy
			,(LONG)state->mViewXL,(LONG)state->mViewYL);
			*/
	}

	return TRUE;
}

void R3EndScene()
{
	LPDIRECT3DDEVICE8 pDevice = GetD3dDevice();
	if( pDevice== NULL )
		return;
	if( !stRealFullScreenEffectState )
		pDevice->EndScene();// End the scene.
	else
		stRToTMain.EndScene();
}

void ShadowBeginScene()
{
	stShadow.BeginScene();
	stShadow.ClearFrameBuffer(0xff000000);
}

void ShadowEndScene()
{
	stShadow.EndScene();
}
LPDIRECT3DTEXTURE8 GetShadowTexture()
{
	return stShadow.GetD3DTexture();
}

void InitFullScreenEffect()		//���� ����̽� ������..
{
	_R3ENGINE_STATE *state = GetR3State();

	if( state->mThisVidMemLevel == 2 )		//������� ���� �޸� ��Ȳ���� �Ѵ�.
		stTexSize = 256;
	else
		stTexSize = 512;

	stRToTMain.CreateTexture(GetD3dDevice() ,stTexSize ,stTexSize ,TRUE);	//������ ��� zbuffer�� �վ�ߵǱ�.
	stRToTBlur.CreateTexture(GetD3dDevice() ,stTexSize ,stTexSize, FALSE);	//��������� zbuffer�� �ʿ����.
	stShadow.CreateTexture(GetD3dDevice() ,256,256,FALSE);	//���������.
}

void ReleaseFullScreenEffect()
{
	stRToTMain.ReleaseTexture();
	stRToTBlur.ReleaseTexture();
	stShadow.ReleaseTexture();
}


void FullScreenEffect()
{
	if( !stRealFullScreenEffectState )	//�۵� ���ϸ� �����Ͼ���.
		return;

	stRToTMain.EndScene();
	
	stRToTBlur.BeginScene();
	float u[2]={0,1};
	float v[2]={0,1};

	if( stFullScreenEffectState & _FSE_IN_WATER )
	{
		stRToTBlur.ClearFrameBuffer(0xff000000);
		//Draw2DSprite(0,0,stTexSize,stTexSize,u,v,stRToTMain.GetD3DTexture(),0x00000001);
		DrawFullScreenLava(stTexSize,stTexSize,stRToTMain.GetD3DTexture(),0xffffffff);
		Draw2DSprite(0,0,(float)stTexSize,(float)stTexSize,NULL,0x3f0000ff);
//		extern DWORD dwLightColor;
//		Draw2DSprite(0,0,(float)stTexSize,(float)stTexSize,NULL,(dwLightColor&0x00ffffff)|0x5f000000);
	}
	else
	if( stFullScreenEffectState & _FSE_MOTION_BLUR )
	{
		if( stRealFullScreenEffectState != stOldRealFullScreenEffectState )	//���ʽ� Ŭ���� ��Ų��.
		{
			stOldRealFullScreenEffectState = stRealFullScreenEffectState;
			Draw2DSprite(0,0,(float)stTexSize,(float)stTexSize,u,v,stRToTMain.GetD3DTexture(),0xffffffff);
			//stRToTBlur.ClearFrameBuffer(0xff000000);
		}

		Draw2DSprite(0,0,(float)stTexSize,(float)stTexSize,u,v,stRToTMain.GetD3DTexture(),0x3fffffff);
//		stRToTBlur.ClearFrameBuffer(0xff000000);
//		Draw2DSprite(0,0,(float)stTexSize,(float)stTexSize,u,v,stRToTMain.GetD3DTexture(),0xffffffff);
	}
	stRToTBlur.EndScene();

	BOOL temp_bool= stFullScreenEffectState;	//����� r3���ſ��� �򰥷� ���� �ʴ´�.
	stFullScreenEffectState = FALSE;	//����� r3���ſ��� �򰥷� ���� �ʴ´�.

	R3BeginScene();
	
	_R3ENGINE_STATE *state = GetR3State();
	ResetVertexBufferCache();
	SetTextureRenderTargetFrameBuffer(TRUE);	//Ʈ��� �ٲ��ְ�.
	GetD3dDevice()->SetRenderTarget(stOldRenderTarget, stOldStencilZ);
	SetViewPortArea(0,0,(LONG)state->mScreenXsize,(LONG)state->mScreenYsize);

	stFullScreenEffectState=temp_bool;

	GetD3dDevice()->SetRenderState( D3DRS_ZENABLE, FALSE );
	Draw2DSprite(0,0,GetR3State()->mScreenXsize,GetR3State()->mScreenYsize,u,v,stRToTBlur.GetD3DTexture(),0xffffffff);
//	���͸� �Լ�.
//	Filter(7);		//1,3,5,7,9 
	GetD3dDevice()->SetRenderState( D3DRS_ZENABLE, TRUE );
}

void SetStateFullScreenEffect(DWORD run_id)	//0�� �������� �ƴϴ�. 1�� ��Ǻ�ξ�
{
	stFullScreenEffectState = run_id;
	if( !run_id )
		stOldRealFullScreenEffectState = 0;
}

DWORD GetStateFullScreenEffect()
{
	return stFullScreenEffectState;
}




HRESULT D3D_R3ConfirmDevice( D3DCAPS8* pCaps, DWORD dwBehavior,
                                          D3DFORMAT Format )
{
	_R3ENGINE_STATE *state = GetR3State();
	
	state->mTextureUnitNum = pCaps->MaxSimultaneousTextures;	//���û�� �ؽ��� ����
	if( ((pCaps->VertexShaderVersion&0x0000ff00)>>8) < 1 )	//���ؽ� ���̴� ��밡������..üũ.
		state->mIsUsedVertexShader = FALSE;
	else
		state->mIsUsedVertexShader = TRUE;
/*
	char buf[256];
	sprintf(buf,"ver %d.%d",(pCaps->VertexShaderVersion&0x0000ff00)>>8,(pCaps->VertexShaderVersion&0x000000ff));
	Warning(buf,"");
*/
	return S_OK;
}

void SetFov(float pi)	//fov���� ���� ����..
{
	_R3ENGINE_STATE *state = GetR3State();

//    D3DXMatrixPerspectiveFovRH( &state->mMatProj, state->mFov
//		, state->mAsfectRatio, state->mNear, state->mFar );
	state->mFov =  (pi);
	D3DXMatrixPerspectiveFovLH( &state->mMatProj, state->mFov
		, state->mAsfectRatio, state->mNear, state->mFar );
    stPd3dDevice->SetTransform( D3DTS_PROJECTION, &state->mMatProj );
}

//#define _DEFAULT_FOV (D3DX_PI/(2.8f))		//48.29���̴�.
void SetDefaultFov()	//fov���� ���� ����..
{
//	SetFov(_DEFAULT_FOV);	//fov���� ���� ����..
	SetFov(GetR3State()->mFov);	//fov���� ���� ����..
}


HRESULT D3D_R3RestoreDevice()
{
//	if( !IsInitCore() )
//		Error("�ʱ�ȭ�� �ؾ���..","");
	_R3ENGINE_STATE *state = GetR3State();

//    m_pFont->RestoreDeviceObjects();
	
    //---------�̸������� �ؽ��ĵ��� �ؽ��ĸ޸𸮿� �ø���.������ �ؽ��ĸ� ���⼭ �����ʴ´�.
//    R3RestoreAllTextures();	//

	InitFullScreenEffect();	//����̽�..Ǯ��ũ�� ����Ʈ �ʱ�ȭ.
//	CreateBlurVBuffer(256,256);

	//------------------- ��Ʈ������ ��������...
    D3DXMatrixIdentity( &state->mMatWorld );

	SetViewPortArea(state->mViewSx,state->mViewSy,state->mViewXL,state->mViewYL);
    D3DXMatrixPerspectiveFovLH( &state->mMatProj, state->mFov
		, state->mAsfectRatio, state->mNear, state->mFar );
	stPd3dDevice->SetRenderState( D3DRS_CULLMODE,   D3DCULL_CCW );

    stPd3dDevice->SetTransform( D3DTS_WORLD, &state->mMatWorld );
    stPd3dDevice->SetTransform( D3DTS_VIEW, &state->mMatView );
    stPd3dDevice->SetTransform( D3DTS_PROJECTION, &state->mMatProj );


	//z-buffer
    stPd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
    // Set any appropiate state
    stPd3dDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
    stPd3dDevice->SetRenderState( D3DRS_LIGHTING,     FALSE );
//    stPd3dDevice->SetRenderState( D3DRS_ZBIAS,   16 );

//	stPd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
	stPd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
    stPd3dDevice->SetRenderState( D3DRS_ALPHAREF,        0x8 );
    stPd3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );

//    stPd3dDevice->SetTextureStageState(0, D3DTSS_MIPMAPLODBIAS,*((LPDWORD) (&state->mMipMapBias)));
	SetDefaultFlags();

	HRESULT hr;
    if( FAILED( hr = stPd3dDevice->GetRenderTarget( &stOldRenderTarget ) ) )
        return hr;

    if( FAILED( hr = stPd3dDevice->GetDepthStencilSurface( &stOldStencilZ ) ) )
        return hr;

	return S_OK;
}


HRESULT D3D_R3InvalidateDevice()
{
	//�ӽ��ڵ�
//    m_pFont->InvalidateDeviceObjects();
	//---------�ؽ��� �޸𸮸� Ŭ�����Ѵ�.
//    R3ReleaseAllTextures();		//���� �ؽ��ĸ� ���⿡�� �����ʴ´�..
	ReleaseBlurVBuffer();
	ReleaseFullScreenEffect();
	if( stOldRenderTarget )
	{
		stOldRenderTarget->Release();
		stOldRenderTarget=0;
	}
	if( stOldStencilZ )
	{
		stOldStencilZ->Release();
		stOldStencilZ=0;
	}
	return S_OK;
}


void GetYBillboardMatrix(D3DXMATRIX *get,Vector3f src,Vector3f tar)//	������
{
    // Set up a rotation matrix to orient the billboard towards the camera.
	Vector3f dir;
	Vector3fSub(tar,src,dir);
	if( dir[0] == 0)
	{
		D3DXMatrixIdentity(get);
		return;
	}
    if( dir[0] > 0.0f )
        D3DXMatrixRotationY( get, -atanf(dir[2]/dir[0])+D3DX_PI/2 );
    else
        D3DXMatrixRotationY( get, -atanf(dir[2]/dir[0])-D3DX_PI/2 );
}

void GetYBillboardMatrix(D3DXMATRIX *get,D3DXVECTOR3 src,D3DXVECTOR3 tar)//	������
{
    // Set up a rotation matrix to orient the billboard towards the camera.
	D3DXVECTOR3 dir;
	dir = tar-src;
	if( dir.x == 0)
	{
		D3DXMatrixIdentity(get);
		return;
	}
    if( dir.x > 0.0f )
        D3DXMatrixRotationY( get, -atanf(dir.z/dir.x)+D3DX_PI/2 );
    else
        D3DXMatrixRotationY( get, -atanf(dir.z/dir.x)-D3DX_PI/2 );
}

void GetZBillboardMatrix(D3DXMATRIX *get,Vector3f src,Vector3f tar)//	������
{
    // Set up a rotation matrix to orient the billboard towards the camera.
	Vector3f dir;
	Vector3fSub(tar,src,dir);
	if( dir[0] == 0)
	{
		D3DXMatrixIdentity(get);
		return;
	}
    if( dir[0] < 0.0f )
        D3DXMatrixRotationZ( get, atanf(dir[1]/dir[0])-D3DX_PI/2 );
    else
        D3DXMatrixRotationZ( get, atanf(dir[1]/dir[0])+D3DX_PI/2 );
}


void GetBillboardMatrix(D3DXMATRIX *get,Vector3f src,Vector3f tar)//	������
{
    // Set up a rotation matrix to orient the billboard towards the camera.
	_R3ENGINE_STATE *state = GetR3State();
	*get=state->mInvMatView;
	get->m[3][0]=0;	//ȸ���� �־����...
	get->m[3][1]=0;
	get->m[3][2]=0;
}

#define _MAX_VIEWPORT_STACK		10

LONG stViewSx[_MAX_VIEWPORT_STACK],stViewSy[_MAX_VIEWPORT_STACK],stViewXL[_MAX_VIEWPORT_STACK],stViewYL[_MAX_VIEWPORT_STACK];
DWORD stViewPortStackCnt=0;
void CleanViewPortStack()
{
	stViewPortStackCnt=0;
}

void PushViewPortArea()
{
	if( stViewPortStackCnt >= _MAX_VIEWPORT_STACK )	//�ȳ�ġ�� �����Ұ�.
		stViewPortStackCnt=_MAX_VIEWPORT_STACK-1;

	_R3ENGINE_STATE *state = GetR3State();
	stViewSx[stViewPortStackCnt] = state->mViewSx;
	stViewSy[stViewPortStackCnt] = state->mViewSy;
	stViewXL[stViewPortStackCnt] = state->mViewXL;
	stViewYL[stViewPortStackCnt] = state->mViewYL;
	stViewPortStackCnt++;
}
void PopViewPortArea()
{

	_R3ENGINE_STATE *state = GetR3State();
	stViewPortStackCnt--;
	if( stViewPortStackCnt < 0 )	//�ȳ�ġ�� �����Ұ�.
		stViewPortStackCnt= 0;
	state->mViewSx = stViewSx[stViewPortStackCnt];
	state->mViewSy = stViewSy[stViewPortStackCnt];
	state->mViewXL = stViewXL[stViewPortStackCnt];
	state->mViewYL = stViewYL[stViewPortStackCnt];
	state->mScreenXsize=(float)state->mViewXL;
	state->mScreenYsize=(float)state->mViewYL;
//	SetViewPortArea(stViewSx,stViewSy,stViewXL,stViewYL);
}

void SetViewPortArea(LONG sx,LONG sy,LONG xl,LONG yl)
{
	_R3ENGINE_STATE *state = GetR3State();

//	if( state->mViewSx == sx &&	state->mViewSy == sy 
//	&& state->mViewXL == xl &&	state->mViewYL == yl )
//		return;

	state->mViewSx = sx;
	state->mViewSy = sy;
	state->mViewXL = xl;
	state->mViewYL = yl;


//	state->mAsfectRatio = (float)xl/(float)yl;

	if( stPd3dDevice )
	{
		D3DVIEWPORT8 view_port;
		view_port.X = sx;
		view_port.Y = sy;
		view_port.Width = xl;
		view_port.Height = yl;
		view_port.MaxZ=1.0f;
		view_port.MinZ=0.0f;
		D3DXMatrixPerspectiveFovLH( &state->mMatProj, state->mFov
			, state->mAsfectRatio, state->mNear, state->mFar );
		stPd3dDevice->SetTransform( D3DTS_PROJECTION, &state->mMatProj );
		stPd3dDevice->SetViewport(&view_port);
	}
	float x_center = (float)(state->mViewXL/2+state->mViewSx);
	float y_center = (float)(state->mViewYL/2+state->mViewSy);
	SetTransformClipInfo((float)state->mViewXL,(float)state->mViewYL,x_center ,y_center );
//	SetFrustumNormalPlane();
}


LPDIRECT3DDEVICE8 GetD3dDevice()
{
	return( stPd3dDevice );
}

#define _DBCLICK_TIME	0.3 //���� Ŭ���ð�.

static float st_bLDBClick;
static float st_bRDBClick;

int
ScanCodeToVKCode( int nScanCode )
{
	switch( nScanCode )
	{
		case 0x10 : return VK_Q;
		case 0x11 : return VK_W;
		case 0x12 : return VK_E;
		case 0x13 : return VK_R;
		case 0x14 : return VK_T;
		case 0x15 : return VK_Y;
		case 0x16 : return VK_U;
		case 0x17 : return VK_I;
		case 0x18 : return VK_O;
		case 0x19 : return VK_P;
		case 0x1E : return VK_A;
		case 0x1F : return VK_S;
		case 0x20 : return VK_D;
		case 0x21 : return VK_F;
		case 0x22 : return VK_G;
		case 0x23 : return VK_H;
		case 0x24 : return VK_J;
		case 0x25 : return VK_K;
		case 0x26 : return VK_L;
		case 0x2C : return VK_Z;
		case 0x2D : return VK_X;
		case 0x2E : return VK_C;
		case 0x2F : return VK_V;
		case 0x30 : return VK_B;
		case 0x31 : return VK_N;
		case 0x32 : return VK_M;
	}

	return -1;
}

void D3D_R3MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam,LPARAM lParam )
{
//------------------------������ϳ��� �߰��߽���.
#define WM_MOUSEWHEEL                   0x020A
    switch( uMsg )
    {
		case WM_KEYDOWN:
			/*
			// by J.S.H
			if ( gScan[ ( int )wParam] == 0 )
			{
				gScan[ ( int )wParam ] = 1;
				gAsci[ ( int )wParam ] = 1;
				gGetKey[ ( int )wParam ] = 1;
			}
			break;*/
			// by J.S.H
			if(wParam < 255 )
			{
				if( gScan[ ( int )wParam] == 0 )
				{
					if( wParam == 0x000000E5 )
					{
						int VKCode = ScanCodeToVKCode( lParam >> 16 );
						if( VKCode != -1 )
						{
							gScan[ VKCode ] = 1;
							gScanExt[ VKCode ] = 1;
							gAsci[ VKCode ] = 1;
							gGetKey[ VKCode ] = 1;
						}
					}
					else
					{
						gScan[ ( int )wParam] = 1;
						gScanExt[ ( int )wParam] = 1;
						gAsci[ ( int )wParam] = 1;
						gGetKey[ ( int )wParam ] = 1;
					}
				}
			}
			break;
		case WM_KEYUP:
			if(wParam < 255 )
			{
				gScan[ ( int )wParam ] |= _R3_KEY_UP;
				gScanExt[ ( int )wParam ] |= _R3_KEY_UP;
			}
			break;
		case WM_LBUTTONDOWN:
			if( gMouse.LDBClick == FALSE )
			{
				if( R3GetTime()-st_bLDBClick < _DBCLICK_TIME )
				{
					gMouse.LDBClick = TRUE;
				}
				st_bLDBClick=R3GetTime();
			}
			gMouse.button |= _LEFT_BUTTON_DOWN;
//			gMouse.button &= ( ~_LEFT_BUTTON_UP );
			break;
		case WM_LBUTTONUP:
			//gMouse.button &= ( ~_LEFT_BUTTON_DOWN );
			gMouse.button |= _LEFT_BUTTON_UP;
			break;
		case WM_MBUTTONDOWN:
			::SetCapture(hWnd);
			gMouse.button |= _MIDDLE_BUTTON_DOWN;
//			gMouse.button &= ( ~_MIDDLE_BUTTON_UP );
			break;
		case WM_MBUTTONUP:
			::ReleaseCapture();
			gMouse.button |= _MIDDLE_BUTTON_UP;
			gMouse.extX = LOWORD( lParam );
			gMouse.extY = HIWORD( lParam );
			break;
		case WM_RBUTTONDOWN:
			if( gMouse.LDBClick == FALSE )
			{
				if( R3GetTime()-st_bRDBClick < _DBCLICK_TIME )
				{
					gMouse.RDBClick = TRUE;
				}
				st_bRDBClick=R3GetTime();
			}
			::SetCapture(hWnd);
			gMouse.button |= _RIGHT_BUTTON_DOWN;
//			gMouse.button &= ( ~_RIGHT_BUTTON_UP );
			break;
		case WM_RBUTTONUP:
			::ReleaseCapture();
//			gMouse.button &= ( ~_RIGHT_BUTTON_DOWN );
			gMouse.button |= _RIGHT_BUTTON_UP;
			break;
		case WM_MOUSEWHEEL:
			gMouse.z = (int)(wParam)>>16; // wheel rotation 
			break;
		case WM_MOUSEMOVE:
//			gMouse.oldx = gMouse.x;
//			gMouse.oldy = gMouse.y;

			gMouse.x = LOWORD( lParam );
			gMouse.y = HIWORD( lParam );

			if ( gMouse.x < 0 )
				gMouse.x = 0;

			if( gMouse.y < 0 )
				gMouse.y = 0;
			break;
/*
		case WM_TIMER:
			memset
			DWORD GetTickCount();
			break;
*/
	}
	return ;
}

