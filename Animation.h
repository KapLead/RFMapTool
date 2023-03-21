#ifndef __CHARACTER_ANIMATION__
#define __CHARACTER_ANIMATION__

#include <Windows.h>
#include <D3DX8.h>
//#include <List>
#include <vector>

using namespace std;

class AnimationData;
#define MAX_ANIMATION_NAME 256
class ChAnimation
{
public:
	char 				m_Name[MAX_ANIMATION_NAME];
	DWORD				m_Ident;
	BYTE*				m_pAnimation;
	//
	AnimationData		*m_pACData;
	//MaxFrame
	DWORD				m_MaxFrame;
	//

	void				*m_pExtra;			// Added By J.S.H

	ChAnimation();
	~ChAnimation();
};


typedef vector<ChAnimation*>		AnimationList;

class AnimationManager
{
public:
	static AnimationManager*		m_AManager;
	AnimationList					m_AniList;
	DWORD							m_TotalSize;
	DWORD							m_Count;//Unique Identifier
public:
	Init();
	AnimationManager(){m_AManager = this;Init();}
	~AnimationManager();

	
	DWORD	AnimationManager::GetAnimationID(char *name);
	
	bool	AddAnimation(char *name);
	ChAnimation	*GetAnimationData(char *name);
	ChAnimation	*GetAnimationDataByNoPath(char *name);
	ChAnimation	*GetAnimationData(DWORD id);
	void	DeleteAnimation(char *name);
	void	DeleteAnimation2(DWORD ident);
	
};


#endif 