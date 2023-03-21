// RFMapToolView.h : interface of the CRFMapToolView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFMAPTOOLVIEW_H__F7841DE4_5110_4E40_9DE0_E3FE5109D4FC__INCLUDED_)
#define AFX_RFMAPTOOLVIEW_H__F7841DE4_5110_4E40_9DE0_E3FE5109D4FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CRFMapToolView : public CFormView
{
protected: // create from serialization only
	CRFMapToolView();
	DECLARE_DYNCREATE(CRFMapToolView)

public:
	//{{AFX_DATA(CRFMapToolView)
	enum{ IDD = IDD_RFMAPTOOL_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CRFMapToolDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRFMapToolView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRFMapToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRFMapToolView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenEbp();
	afx_msg void OnFileSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RFMapToolView.cpp
inline CRFMapToolDoc* CRFMapToolView::GetDocument()
   { return (CRFMapToolDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RFMAPTOOLVIEW_H__F7841DE4_5110_4E40_9DE0_E3FE5109D4FC__INCLUDED_)
