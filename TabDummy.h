#if !defined(AFX_TABDUMMY_H__2EEC17BE_AB01_4ED3_8B2C_FC0EFBAE83F7__INCLUDED_)
#define AFX_TABDUMMY_H__2EEC17BE_AB01_4ED3_8B2C_FC0EFBAE83F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabDummy.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDummyEdit dialog
/////////////////////////////////////////////////////////////////////////////
// CDummyFile dialog

class CDummyFile : public CDialog
{
	DECLARE_DYNCREATE(CDummyFile);
// Construction
public:
	CDummyFile(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDummyFile)
	enum { IDD = IDD_DUMMY_FILE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDummyFile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDummyFile)
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CDummyNameDialog dialog

class CDummyNameDialog : public CDialog
{
// Construction
public:
	CDummyNameDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDummyNameDialog)
	enum { IDD = IDD_DUMMY_DIALOG };
	CEdit	m_DummyName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDummyNameDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDummyNameDialog)
	virtual void OnOK();
	afx_msg void OnChangeEdit2();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CDummySearchDialog dialog

class CDummySearchDialog : public CDialog
{
// Construction
public:
	CDummySearchDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDummySearchDialog)
	enum { IDD = IDD_DUMMY_SEARCH_DIALOG };
	CEdit	m_DummyName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDummySearchDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDummySearchDialog)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnChangeEdit2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CDummySplitDialog dialog

class CDummySplitDialog : public CDialog
{
// Construction
public:
	CDummySplitDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDummySplitDialog)
	enum { IDD = IDD_DUMMY_SPLIT_DIALOG };
	CEdit	m_Height;
	CEdit	m_Width;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDummySplitDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDummySplitDialog)
	afx_msg void OnChangeEdit15();
	afx_msg void OnChangeEdit24();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CDummyErrorDialog dialog

class CDummyErrorDialog : public CDialog
{
// Construction
public:
	CDummyErrorDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDummyErrorDialog)
	enum { IDD = IDD_DUMMY_ERROR_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDummyErrorDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDummyErrorDialog)
	virtual void OnOK();
	afx_msg void OnOk2();
	virtual void OnCancel();
	afx_msg void OnOk3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

class CDummyEdit : public CDialog
{
	DECLARE_DYNCREATE(CDummyEdit);
// Construction
public:
	CDummyEdit(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CDummyEdit)
	enum { IDD = IDD_DUMMY_EDIT };
	CEdit	m_Rot;
	CEdit	m_DummyName;
	CEdit	m_ScaleZ;
	CEdit	m_ScaleY;
	CEdit	m_ScaleX;
	CComboBox	m_SelectScroll;
	CComboBox	m_EditScroll;
	//}}AFX_DATA

	CDummyNameDialog DummyNameDialog;
	CDummySearchDialog DummySearchDialog;
	CDummySplitDialog DummySplitDialog;
	CDummyErrorDialog DummyErrorDialog;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDummyEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDummyEdit)
	afx_msg void OnCheck7();
	afx_msg void OnCheck10();
	afx_msg void OnRadio2();
	afx_msg void OnRadio1();
	afx_msg void OnButton3();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEdit15();
	afx_msg void OnChangeEdit16();
	afx_msg void OnChangeEdit17();
	afx_msg void OnChangeEdit2();
	afx_msg void OnButton5();
	afx_msg void OnButton1();
	afx_msg void OnCheck18();
	afx_msg void OnButton2();
	afx_msg void OnButton10();
	afx_msg void OnCheck15();
	afx_msg void OnChangeEdit23();
	afx_msg void OnDestroy();
	afx_msg void OnButton11();
	afx_msg void OnButton12();
	//}}AFX_MSG
public:
	afx_msg void OnSelchangeCombo3();
	afx_msg void OnSelchangeCombo1();

	DECLARE_MESSAGE_MAP()
};


void ProgressDummyEdit(void *v_level,Vector3f cam_pos);
void DrawDummyEntities();
void LoadDummy();
void SaveDummy();


#endif // !defined(AFX_TABDUMMY_H__2EEC17BE_AB01_4ED3_8B2C_FC0EFBAE83F7__INCLUDED_)
