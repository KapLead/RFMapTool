#if !defined(AFX_MYSPIN_H__623D2FB4_B9D9_4199_8108_BCE7DD5259E7__INCLUDED_)
#define AFX_MYSPIN_H__623D2FB4_B9D9_4199_8108_BCE7DD5259E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySpin.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMySpin window

class CMySpin : public CSpinButtonCtrl
{
// Construction
public:
	CMySpin();

// Attributes
public:

// Operations
public:
	BOOL mIsClick;
	int mPickPointX;
	int mPickPointY;
	int mYDiff;
	int GetmYdiff();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySpin)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMySpin();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMySpin)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSPIN_H__623D2FB4_B9D9_4199_8108_BCE7DD5259E7__INCLUDED_)
