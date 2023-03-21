#include "stdafx.h"
#include "RFMapTool.h"

#include "ToolManager.h"
#include "RFMapToolDoc.h"

int stCollisionMode=0;
int stEntityMode=0;
int stSelectMode=0,stVertexid=-1;
float stVertex[3];
int stPickingStyle=0;

int WhatCollisionMode()
{
	return stCollisionMode;
}

void SetCollisionMode(int mode)
{
	stCollisionMode = mode;
}


int WhatEntityMode()
{
	return stEntityMode;
}

void SetEntityMode(int mode)
{
	stEntityMode = mode;
}


int stSoundMode=0;
int WhatSoundMode()
{
	return stSoundMode;
}
void SetSoundMode(int mode)
{
	stSoundMode=mode;
}

int stDummyMode=0;
int WhatDummyMode()
{
	return stDummyMode;
}
void SetDummyMode(int mode)
{
	stDummyMode=mode;
}



void SetSelectMode(int mode)
{
	stSelectMode = mode;
}

int GetSelectMode()
{
	return stSelectMode;
}

void SetPickingStyle(int mode)
{
	stPickingStyle = mode;
}
int GetPickingStyle()
{
	return stPickingStyle;
}
/*
int GetNowTabIndex()
{
	return g_pDoc->m_cToolBase.m_cRightTab.GetNowTab();
}
*/
static int stTabMode = _COLLISION_MODE;
void SetTabMode(int TabMode)
{
	stTabMode = TabMode;
	if( TabMode == 0 )
	{
		g_pDoc->m_wndCollRollupCtrl.ShowWindow(SW_SHOWNORMAL);
		g_pDoc->m_wndEntityRollupCtrl.ShowWindow(SW_HIDE);
		g_pDoc->m_wndParticleRollupCtrl.ShowWindow(SW_HIDE);//	SW_SHOWNA //SW_SHOWNORMAL 
		g_pDoc->m_wndMagicRollupCtrl.ShowWindow(SW_HIDE);//	SW_SHOWNA //SW_SHOWNORMAL 
		g_pDoc->m_wndSoundRollupCtrl.ShowWindow(SW_HIDE);//	SW_SHOWNA //SW_SHOWNORMAL 
		g_pDoc->m_wndDummyRollupCtrl.ShowWindow(SW_HIDE);//	SW_SHOWNA //SW_SHOWNORMAL 
	}
	else
	if( TabMode == 1 )
	{
		g_pDoc->m_wndCollRollupCtrl.ShowWindow(SW_HIDE);
		g_pDoc->m_wndEntityRollupCtrl.ShowWindow(SW_SHOWNORMAL);
		g_pDoc->m_wndParticleRollupCtrl.ShowWindow(SW_HIDE);//	SW_SHOWNA //SW_SHOWNORMAL 
		g_pDoc->m_wndMagicRollupCtrl.ShowWindow(SW_HIDE);//	SW_SHOWNA //SW_SHOWNORMAL 
		g_pDoc->m_wndSoundRollupCtrl.ShowWindow(SW_HIDE);//	SW_SHOWNA //SW_SHOWNORMAL 
		g_pDoc->m_wndDummyRollupCtrl.ShowWindow(SW_HIDE);//	SW_SHOWNA //SW_SHOWNORMAL 
	}
	else
	if( TabMode == 2 )
	{
		g_pDoc->m_wndCollRollupCtrl.ShowWindow(SW_HIDE);
		g_pDoc->m_wndEntityRollupCtrl.ShowWindow(SW_HIDE);
		g_pDoc->m_wndParticleRollupCtrl.ShowWindow(SW_SHOWNORMAL);//	SW_SHOWNA //SW_SHOWNORMAL 
		g_pDoc->m_wndMagicRollupCtrl.ShowWindow(SW_HIDE);//	SW_SHOWNA //SW_SHOWNORMAL 
		g_pDoc->m_wndSoundRollupCtrl.ShowWindow(SW_HIDE);//	SW_SHOWNA //SW_SHOWNORMAL 
		g_pDoc->m_wndDummyRollupCtrl.ShowWindow(SW_HIDE);//	SW_SHOWNA //SW_SHOWNORMAL 
	}
	else
	if( TabMode == 3 )
	{
		g_pDoc->m_wndCollRollupCtrl.ShowWindow(SW_HIDE);
		g_pDoc->m_wndEntityRollupCtrl.ShowWindow(SW_HIDE);
		g_pDoc->m_wndParticleRollupCtrl.ShowWindow(SW_HIDE);//	SW_SHOWNA //SW_SHOWNORMAL 
		g_pDoc->m_wndMagicRollupCtrl.ShowWindow(SW_SHOWNORMAL);//	SW_SHOWNA //SW_SHOWNORMAL 
		g_pDoc->m_wndSoundRollupCtrl.ShowWindow(SW_HIDE);//	SW_SHOWNA //SW_SHOWNORMAL 
		g_pDoc->m_wndDummyRollupCtrl.ShowWindow(SW_HIDE);//	SW_SHOWNA //SW_SHOWNORMAL 
	}
	else
	if( TabMode == 4 )
	{
		g_pDoc->m_wndCollRollupCtrl.ShowWindow(SW_HIDE);
		g_pDoc->m_wndEntityRollupCtrl.ShowWindow(SW_HIDE);
		g_pDoc->m_wndParticleRollupCtrl.ShowWindow(SW_HIDE);//	SW_SHOWNA //SW_SHOWNORMAL 
		g_pDoc->m_wndMagicRollupCtrl.ShowWindow(SW_HIDE);//	SW_SHOWNA //SW_SHOWNORMAL 
		g_pDoc->m_wndSoundRollupCtrl.ShowWindow(SW_SHOWNORMAL);//	SW_SHOWNA //SW_SHOWNORMAL 
		g_pDoc->m_wndDummyRollupCtrl.ShowWindow(SW_HIDE);//	SW_SHOWNA //SW_SHOWNORMAL 
	}
	else
	if( TabMode == 5 )
	{
		g_pDoc->m_wndCollRollupCtrl.ShowWindow(SW_HIDE);
		g_pDoc->m_wndEntityRollupCtrl.ShowWindow(SW_HIDE);
		g_pDoc->m_wndParticleRollupCtrl.ShowWindow(SW_HIDE);//	SW_SHOWNA //SW_SHOWNORMAL 
		g_pDoc->m_wndMagicRollupCtrl.ShowWindow(SW_HIDE);//	SW_SHOWNA //SW_SHOWNORMAL 
		g_pDoc->m_wndSoundRollupCtrl.ShowWindow(SW_HIDE);//	SW_SHOWNA //SW_SHOWNORMAL 
		g_pDoc->m_wndDummyRollupCtrl.ShowWindow(SW_SHOWNORMAL);//	SW_SHOWNA //SW_SHOWNORMAL 
	}
}
int GetTabMode()
{
	return stTabMode;
}
//스핀 작업 내용..
static int stWhatSpin;

void SetWhatSpin(int mode)	
{
	stWhatSpin=mode;
}
int GetWhatSpin()
{
	return stWhatSpin;
}

void ChangeAllTabSpinControl()
{
	if( stTabMode ==_COLLISION_MODE )
	{
	}
	else
	if( stTabMode ==_ENTITY_MODE )
	{
	}
	else
	if( stTabMode ==_PARTICLE_MODE )
	{
//		if( g_pDoc->ParticleEdit )
//			g_pDoc->ParticleEdit->DoDataChangeSpin();
	}
}

float stFlySpeed;
void SetFlySpeed(float speed)
{
	stFlySpeed = speed;
}

float GetFlySpeed()
{
	return stFlySpeed;
}

