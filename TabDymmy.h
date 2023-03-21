#if !defined(AFX_TABDYMMY_H__445DA7D5_8479_4E3A_8DF3_FD0A9D4E273A__INCLUDED_)
#define AFX_TABDYMMY_H__445DA7D5_8479_4E3A_8DF3_FD0A9D4E273A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabDymmy.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabDymmy dialog

class CTabDymmy : public CDialog
{
// Construction
public:
	CTabDymmy(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabDymmy)
	enum { IDD = IDD_DUMMY_FILE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabDymmy)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabDymmy)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABDYMMY_H__445DA7D5_8479_4E3A_8DF3_FD0A9D4E273A__INCLUDED_)
