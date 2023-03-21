#if !defined(AFX_SOUND_H__CC724BB0_80C1_448A_9073_F64D57E67EA4__INCLUDED_)
#define AFX_SOUND_H__CC724BB0_80C1_448A_9073_F64D57E67EA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Sound.h : header file
//

#include "r3bsp.h"

#define _SOUND_EDIT_POS_MODE		0
#define _SOUND_EDIT_SCALE_MODE		1
#define _SOUND_EDIT_ATTN_MODE		2		//감쇠모드...

#define _SOUND_EDIT_BOX_SCALE		3		//전체 들리는 거리모드...
#define _SOUND_EDIT_BOX_ATTN_MODE	4		//박스 감쇠모드...
#define _SOUND_EDIT_BOX_YROT_MODE	5		//박스 감쇠모드...
#define _SOUND_EDIT_BOX_XROT_MODE	6		//박스 감쇠모드...

/////////////////////////////////////////////////////////////////////////////
// CSoundFile dialog

class CSoundFile : public CDialog
{
	DECLARE_DYNCREATE(CSoundFile);
// Construction
public:
	CSoundFile(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSoundFile)
	enum { IDD = IDD_SOUND_FILE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSoundFile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSoundFile)
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CSoundEdit dialog

class CSoundEdit : public CDialog
{
	DECLARE_DYNCREATE(CSoundEdit);
// Construction
public:
	CSoundEdit(CWnd* pParent = NULL);   // standard constructor
	~CSoundEdit();   // standard distructor

// Dialog Data
	//{{AFX_DATA(CSoundEdit)
	enum { IDD = IDD_SOUND_EDIT };
	CEdit	m_BoxRotX;
	CEdit	m_BoxRotY;
	CEdit	m_EventTime;
	CEdit	m_BoxAttn;
	CEdit	m_BoxScaleZ;
	CEdit	m_BoxScaleY;
	CEdit	m_BoxScaleX;
	CEdit	m_Attn;
	CEdit	m_Scale;
	CComboBox	m_LoadedSoundList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSoundEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
//protected:

public:
	// Generated message map functions
	//{{AFX_MSG(CSoundEdit)
	afx_msg void OnButton3();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	afx_msg void OnCheck10();
	afx_msg void OnCheck7();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEdit2();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnCheck18();
	afx_msg void OnCheck27();
	afx_msg void OnCheck15();
	afx_msg void OnChangeEdit15();
	afx_msg void OnDestroy();
	afx_msg void OnCheck5();
	afx_msg void OnCheck24();
	afx_msg void OnChangeEdit25();
	afx_msg void OnCheck16();
	afx_msg void OnCheck11();
	afx_msg void OnCheck29();
	afx_msg void OnChangeEdit22();
	afx_msg void OnChangeEdit26();
	afx_msg void OnChangeEdit27();
	afx_msg void OnCheck28();
	afx_msg void OnChangeEdit3();
	afx_msg void OnCheck12();
	afx_msg void OnChangeEdit5();
	afx_msg void OnCheck21();
	afx_msg void OnChangeEdit6();
	afx_msg void OnCheck22();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


void ProgressSoundEdit(void *v_level,Vector3f cam_pos);
void DrawSoundEntities();	// 사운드 원. 박스를 그리자.
void HearSoundFromTool();	//툴상에서 듣는거....

void GetSoundEntityInfoInBSP(CBsp *bsp,_ADD_BSP_SAVE *add_bsp);	//메모리 잡고.
void ReleaseSoundEntityInfoInBSP(_ADD_BSP_SAVE *add_bsp); //메모리 프리...
void LoadSoundEntityFromBSP(CBsp *bsp);

void LoadSound();
void SaveSound();


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_SOUND_H__CC724BB0_80C1_448A_9073_F64D57E67EA4__INCLUDED_)
