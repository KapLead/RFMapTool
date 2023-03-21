// MySpin.cpp : implementation file
//

#include "stdafx.h"
#include "rfmaptool.h"
#include "MySpin.h"
#include "ToolManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySpin

CMySpin::CMySpin()
{
	mIsClick=0;
}

CMySpin::~CMySpin()
{
}


BEGIN_MESSAGE_MAP(CMySpin, CSpinButtonCtrl)
	//{{AFX_MSG_MAP(CMySpin)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySpin message handlers

void CMySpin::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	mIsClick=TRUE;
	mPickPointX=point.x;
	mPickPointY=point.y;
	SetCapture();
	CSpinButtonCtrl::OnLButtonDown(nFlags, point);
}

void CMySpin::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	mIsClick=FALSE;
	mYDiff=(mPickPointY-point.y);
	mYDiff=0;
	ReleaseCapture();
//	ChangeAllTabSpinControl();
	CSpinButtonCtrl::OnLButtonUp(nFlags, point);
}
int CMySpin::GetmYdiff()
{
	return mYDiff;
}


void CMySpin::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if( mIsClick )
	{
		mYDiff=(mPickPointY-point.y);
		ChangeAllTabSpinControl();
	}
	CSpinButtonCtrl::OnMouseMove(nFlags, point);
}
