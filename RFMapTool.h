// RFMapTool.h : main header file for the RFMAPTOOL application
//

#if !defined(AFX_RFMAPTOOL_H__2725D8A5_2743_4187_AFFD_51913C80E402__INCLUDED_)
#define AFX_RFMAPTOOL_H__2725D8A5_2743_4187_AFFD_51913C80E402__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "RFMapToolDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CRFMapToolApp:
// See RFMapTool.cpp for the implementation of this class
//

class CRFMapToolApp : public CWinApp
{
public:
	CRFMapToolApp();
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRFMapToolApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CRFMapToolApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileDummyOpen();
	afx_msg void OnFileDummySave();
	afx_msg void OnFileCollisionOpen();
	afx_msg void OnFileCollisionSave();
	afx_msg void OnFileMapEntitySave();
	afx_msg void OnFileMapEntityOpen();
	afx_msg void OnFileMapEntityAddOpen();
	afx_msg void OnFileSoundEntitySave();
	afx_msg void OnFileSoundEntityOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RFMAPTOOL_H__2725D8A5_2743_4187_AFFD_51913C80E402__INCLUDED_)
