#if !defined(AFX_MYCOLORBUTTON_H__F211EFAD_2C01_4CC3_A4A0_3A954B7DECCF__INCLUDED_)
#define AFX_MYCOLORBUTTON_H__F211EFAD_2C01_4CC3_A4A0_3A954B7DECCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyColorButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyColorButton window

class CMyColorButton : public CButton
{
// Construction
public:
	CMyColorButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyColorButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyColorButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyColorButton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

DECLARE_DYNAMIC(CMyColorButton)
private:
	COLORREF m_fg, m_bg, m_disabled;
	UINT m_bevel;
public:

	BOOL Attach(const UINT nID, CWnd* pParent, 
		const COLORREF BGColor = RGB(192, 192, 192),		// gray button
		const COLORREF FGColor = RGB(1, 1, 1),				// black text 
		const COLORREF DisabledColor = RGB(128, 128, 128),	// dark gray disabled text
		const UINT nBevel = 2
	);
	void SetBgColor(COLORREF color){ m_bg= color;};

	
protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	void DrawFrame(CDC *DC, CRect R, int Inset);
	void DrawFilledRect(CDC *DC, CRect R, COLORREF color);
	void DrawLine(CDC *DC, CRect EndPoints, COLORREF color);
	void DrawLine(CDC *DC, long left, long top, long right, long bottom, COLORREF color);
	void DrawButtonText(CDC *DC, CRect R, const char *Buf, COLORREF TextColor);

	COLORREF GetFGColor() { return m_fg; }	
	COLORREF GetBGColor() { return m_bg; }
    COLORREF GetDisabledColor() { return m_disabled; }
	UINT GetBevel() { return m_bevel; }

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCOLORBUTTON_H__F211EFAD_2C01_4CC3_A4A0_3A954B7DECCF__INCLUDED_)
