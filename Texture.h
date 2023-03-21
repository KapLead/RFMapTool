#ifndef __CHARACTER_TEXTURE__
#define __CHARACTER_TEXTURE__

#include <Windows.h>
#include <D3DX8.h>
//#include <List>
#include <vector>

using namespace std;

#define MAX_TEXTURE_NAME 256
class ChTexture
{
public:
	char 				m_Name[MAX_TEXTURE_NAME];
	DWORD				m_Ident;
	LPDIRECT3DTEXTURE8	m_pTexture;
	DWORD				m_Size;

	ChTexture();
	~ChTexture();
};


typedef vector<ChTexture*>		TextureList;

class TextureManager
{
public:
	static TextureManager*		m_TManager;
	LPDIRECT3DDEVICE8			m_Device;
	TextureList					m_TxtList;
	DWORD						m_TotalSize;
	DWORD						m_Count;//Unique Identifier
	char						m_TexPath[128];
public:
	Init();
	TextureManager(){m_TManager = this;Init();}
	~TextureManager();

	void SetTextureDevice(LPDIRECT3DDEVICE8 device) { m_Device = device;}

	DWORD	TextureManager::GetTextureID(char *name);

	LPDIRECT3DTEXTURE8 TextureManager::GetTexture(char *name);
	LPDIRECT3DTEXTURE8 TextureManager::GetTexture2(DWORD ident);

	//void TextureManager::AddTexture(char *name);
	bool  AddTexture(char *path,char *name);
	void SetTexPath(char *path){ strcpy(m_TexPath,path);}
	void DeleteTexture(char *name);
	void DeleteTexture2(DWORD ident);
	
};


#endif 