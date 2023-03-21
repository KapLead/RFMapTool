#if !defined(AFX_TABCOLLISION_H__3B15E173_8360_4EED_9669_CC29D6681FFE__INCLUDED_)
#define AFX_TABCOLLISION_H__3B15E173_8360_4EED_9669_CC29D6681FFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabCollision.h : header file
//

#include "level.h"


void ProgressCollisionEdit(void *level);
void ProgressSelectEdit(void *level);
void DrawCollisionPoly();
void DrawCollisionPoint();


int GetColLineNum();
void LoadColFaceFromBSP(CBsp *bsp);
CToolCollisionFace *GetColface();
void CheckContinueLine(BOOL state);
BOOL GetCheckContinueLine(BOOL state);
int GetEditHeight();
void SetEditHeight(int su);

int GetEditCameraDist();
void SetEditCameraDist(int su);
void LoadCFFile();
void SaveCFFile();


/////////////////////////////////////////////////////////////////////////////
// CCollFile dialog

class CCollFile : public CDialog
{
	DECLARE_DYNCREATE(CCollFile)
// Construction
public:
	CCollFile(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCollFile)
	enum { IDD = IDD_COLL_FILE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCollFile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCollFile)
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CCollEdit dialog

class CCollEdit : public CDialog
{
	DECLARE_DYNCREATE(CCollEdit)
// Construction
public:
	CCollEdit(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCollEdit)
	enum { IDD = IDD_COLL_EDIT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CEdit	m_Height;
	CButton	m_ContinueLine;
	CButton	m_Delete;
	CComboBox	m_Select;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCollEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void OnOK();	//esc와 enter키를 흡수한다.
	void OnCancel();

	// Generated message map functions
	//{{AFX_MSG(CCollEdit)
	afx_msg void OnCheck1();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnSelchangeList1();
	afx_msg void OnSetfocusCombo1();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnCheck2();
	afx_msg void OnChangeEdit1();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton1();
	afx_msg void OnCheck26();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABCOLLISION_H__3B15E173_8360_4EED_9669_CC29D6681FFE__INCLUDED_)
