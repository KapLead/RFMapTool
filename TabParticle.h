#if !defined(AFX_TABPARTICLE_H__2318E82A_78C1_482C_8F9C_4F4487743AA2__INCLUDED_)
#define AFX_TABPARTICLE_H__2318E82A_78C1_482C_8F9C_4F4487743AA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabParticle.h : header file
//
#include "myspin.h"
#include "mycolorButton.h"
/////////////////////////////////////////////////////////////////////////////
// CTabParticle dialog


void ProgressParticleEdit(void *v_level);
void DrawCreatedParticle();	//파티클메뉴에서 제작되는 파티클 루프...


/////////////////////////////////////////////////////////////////////////////
// CParticleProperty dialog

class CParticleProperty : public CDialog
{
// Construction
public:
	CParticleProperty(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CParticleProperty)
	enum { IDD = IDD_PARTICLE_PROPERTY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParticleProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParticleProperty)
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	afx_msg void OnCheck3();
	afx_msg void OnCheck6();
	afx_msg void OnCheck5();
	afx_msg void OnCheck8();
	afx_msg void OnCheck4();
	afx_msg void OnCheck10();
	afx_msg void OnCheck14();
	afx_msg void OnCheck15();
	afx_msg void OnCheck16();
	afx_msg void OnCheck17();
	afx_msg void OnCheck18();
	afx_msg void OnCheck9();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnCheck25();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CParticleFile dialog

class CParticleFile : public CDialog
{
// Construction
	DECLARE_DYNCREATE(CParticleFile);
public:
	CParticleFile(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CParticleFile)
	enum { IDD = IDD_PARTICLE_FILE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParticleFile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParticleFile)
	afx_msg void OnButton2();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CParticleEdit dialog

class CParticleEdit : public CDialog
{
	DECLARE_DYNCREATE(CParticleEdit);
// Construction
public:
	CParticleEdit(CWnd* pParent = NULL);   // standard constructor
	void ChangeTrackValue(int id);
//	void FixIntersect(int id);	//새로 트랙을 갱신해서 사이값을 구한다.
	void AttachEntity(char *name);	//엔티티 Attach
	void OnOK();	//esc와 enter키를 흡수한다.
	void OnCancel() ;
	DWORD mTrackFlag;
/*
	CMyColorButton	m_Track0;
	CMyColorButton	m_Track1;
	CMyColorButton	m_Track2;
	CMyColorButton	m_Track3;
	CMyColorButton	m_Track4;
	CMyColorButton	m_Track5;
*/
// Dialog Data
	//{{AFX_DATA(CParticleEdit)
	enum { IDD = IDD_PARTICLE_EDIT };
//	CSliderCtrl	m_AlphaGradient;
	CComboBox	m_ParticleList;

	//}}AFX_DATA

	CParticleProperty ParticleProperty;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParticleEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParticleEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	afx_msg void OnCheck3();
	afx_msg void OnButton3();
	afx_msg void OnCheck13();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnButtonEntityLoad();
	afx_msg void OnClose();
	afx_msg void OnKillfocusEdit6();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.



#endif // !defined(AFX_TABPARTICLE_H__2318E82A_78C1_482C_8F9C_4F4487743AA2__INCLUDED_)
