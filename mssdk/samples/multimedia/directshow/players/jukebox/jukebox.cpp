//------------------------------------------------------------------------------
// File: Jukebox.cpp
//
// Desc: DirectShow sample code - an MFC based C++ jukebox application.
//
// Copyright (c) 1998 - 2000, Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


#include "stdafx.h"
#include "Jukebox.h"
#include "JukeboxDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJukeboxApp

BEGIN_MESSAGE_MAP(CJukeboxApp, CWinApp)
	//{{AFX_MSG_MAP(CJukeboxApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJukeboxApp construction

CJukeboxApp::CJukeboxApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CJukeboxApp object

CJukeboxApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CJukeboxApp initialization

BOOL CJukeboxApp::InitInstance()
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

	CJukeboxDlg dlg;
	m_pMainWnd = &dlg;

	int nResponse = dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
