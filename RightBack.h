#if !defined(AFX_RIGHTBACK_H__26FCA252_1BF2_4869_A106_3CC676448F60__INCLUDED_)
#define AFX_RIGHTBACK_H__26FCA252_1BF2_4869_A106_3CC676448F60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RightBack.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRightBack dialog

class CRightBack : public CDialog
{
// Construction
public:
	CRightBack(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRightBack)
	enum { IDD = IDD_ALWAYS_BACK };
	CEdit	m_CameraDist;
	CEdit	m_FlySpeed;
	CComboBox	m_PikedStyle;
	//}}AFX_DATA

	//탭이미지 버튼...
	CBitmapButton m_TabCollButton;
	CBitmapButton m_TabEntity;
	CBitmapButton m_TabParticle;
	CBitmapButton m_TabMagic;
	CBitmapButton m_TabSound;
	CBitmapButton m_TabDummy;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRightBack)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	virtual void OnOK();
	virtual void OnCancel();
	// Generated message map functions
	//{{AFX_MSG(CRightBack)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCheck4();
	afx_msg void OnCheck5();
	afx_msg void OnCheck3();
	afx_msg void OnChangeEdit3();
	afx_msg void OnChangeEdit4();
	afx_msg void OnCheck6();
	afx_msg void OnCheck7();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnCheck8();
	afx_msg void OnCheck9();
	afx_msg void OnCheck1();
	afx_msg void OnTabCollButton();
	afx_msg void OnTabEntityButton();
	afx_msg void OnTabParticleButton();
	afx_msg void OnTabMagicButton();
	afx_msg void OnTabSoundButton();
	afx_msg void OnTabDummyButton();
	afx_msg void OnCheck10();
	afx_msg void OnCheck27();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int GetNowTab(); //현재탭이 어디에 있는지 알아낸다. 
};

BOOL GetYButton();	//어느 축 버튼이 켜졌는가..
BOOL GetXZButton();
void SetYXZAxis(BOOL y_axis,BOOL xz_axis);	//축 버튼 셋팅..

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RIGHTBACK_H__26FCA252_1BF2_4869_A106_3CC676448F60__INCLUDED_)
