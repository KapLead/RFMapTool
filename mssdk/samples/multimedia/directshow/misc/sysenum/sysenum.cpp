//------------------------------------------------------------------------------
// File: SysEnum.cpp
//
// Desc: DirectShow sample code - an MFC based application for device 
//       enumeration.
//
// Copyright (c) 2000, Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


#include "stdafx.h"
#include "SysEnum.h"
#include "SysEnumDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysEnumApp

BEGIN_MESSAGE_MAP(CSysEnumApp, CWinApp)
	//{{AFX_MSG_MAP(CSysEnumApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysEnumApp construction

CSysEnumApp::CSysEnumApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSysEnumApp object

CSysEnumApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSysEnumApp initialization

BOOL CSysEnumApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CSysEnumDlg dlg;
	m_pMainWnd = &dlg;

	int nResponse = dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
