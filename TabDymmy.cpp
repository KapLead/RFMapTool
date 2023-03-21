// TabDymmy.cpp : implementation file
//

#include "stdafx.h"
#include "rfmaptool.h"
#include "TabDymmy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabDymmy dialog


CTabDymmy::CTabDymmy(CWnd* pParent /*=NULL*/)
	: CDialog(CTabDymmy::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabDymmy)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTabDymmy::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabDymmy)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabDymmy, CDialog)
	//{{AFX_MSG_MAP(CTabDymmy)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabDymmy message handlers
