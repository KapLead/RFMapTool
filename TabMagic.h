#if !defined(AFX_TABMAGIC_H__10BA3D5A_52BF_4D57_8EC3_D121901A0DD3__INCLUDED_)
#define AFX_TABMAGIC_H__10BA3D5A_52BF_4D57_8EC3_D121901A0DD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabMagic.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CMagicFile dialog

class CMagicFile : public CDialog
{
	DECLARE_DYNCREATE(CMagicFile);
// Construction
public:
	CMagicFile(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMagicFile)
	enum { IDD = IDD_MAGIC_FILE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMagicFile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMagicFile)
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CMagicEdit dialog

class CMagicEdit : public CDialog
{
	DECLARE_DYNCREATE(CMagicEdit);
// Construction
public:
	CMagicEdit(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMagicEdit)
	enum { IDD = IDD_MAGIC_EDIT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMagicEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMagicEdit)
	afx_msg void OnButton1();
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	afx_msg void OnCheck3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


void ProgressMagicEdit(void *v_level);
void DrawMagicPosBox(void *v_level);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABMAGIC_H__10BA3D5A_52BF_4D57_8EC3_D121901A0DD3__INCLUDED_)

