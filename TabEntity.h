#if !defined(AFX_TABENTITY_H__45635D44_C3AC_4400_9CDE_0559E68E3633__INCLUDED_)
#define AFX_TABENTITY_H__45635D44_C3AC_4400_9CDE_0559E68E3633__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabEntity.h : header file
//

#include "level.h"

void LoadMapEntityFile();
void AddMapEntityFile();
void SaveMapEntityFile();

void TabEntityHotKey();	//엔티티쪽 핫키..


void ProgressEntityEdit(void *v_level,Vector3f cam_pos);
void DrawCursorEntity();
void DrawEntities(Vector3f camera_pos);
void DrawEntitiesBBox();	//DrawEntities() 호출후 앤티티 바운딩 박스를 그리자.

void LoadEntityFromBSP(CBsp *bsp);

void CleanMapEntityAndEntities();	//그동안 로딩된 엔티티를 날린다.
//---bsp에 추가해야될부분에 
void GetEntityInfoInBSP(CBsp *bsp,_ADD_BSP_SAVE *add_bsp);//bsp에 포함될 엔티티정보를 넣는다.
//	,_READ_ENTITY_LIST **EntityListPtr,_LEAF_ENTITIES_LIST_INFO **LeafEntityListPtr
//	,_READ_MAP_ENTITIES_LIST **MapEntitiesListPtr,WORD **EntityIDPtr); 
void ReleaseEntityInfoInBSP(_ADD_BSP_SAVE *add_bsp); //bsp에 포함될 엔티티정보를 넣는다.

/////////////////////////////////////////////////////////////////////////////
// CEntityFile dialog

class CEntityFile : public CDialog
{
	DECLARE_DYNCREATE(CEntityFile)
// Construction
public:
	CEntityFile(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEntityFile)
	enum { IDD = IDD_ENTITY_FILE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEntityFile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEntityFile)
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CEntityEdit dialog

class CEntityEdit : public CDialog
{
	DECLARE_DYNCREATE(CEntityEdit)
// Construction
public:
	CEntityEdit(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEntityEdit();

// Dialog Data
	//{{AFX_DATA(CEntityEdit)
	enum { IDD = IDD_ENTITY_EDIT };
	CEdit	m_PosZ;
	CEdit	m_PosY;
	CEdit	m_PosX;
	CEdit	m_Factor2;
	CEdit	m_Factor1;
	CEdit	m_EntityDisEnd;
	CEdit	m_EntityDisStart;
	CEdit	m_RotX;
	CEdit	m_RotY;
	CEdit	m_Scale;
	CComboBox	m_LoadedEntityList;
	CComboBox	m_LoadedShaderList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEntityEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void OnOK();
	virtual void OnCancel();

public:
	// Generated message map functions
	//{{AFX_MSG(CEntityEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton7();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnSelchangeCombo2();
	afx_msg void OnChangeEdit2();
	afx_msg void OnChangeEdit3();
	afx_msg void OnChangeEdit4();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton5();
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	afx_msg void OnButton4();
	afx_msg void OnChangeEdit5();
	afx_msg void OnChangeEdit6();
	afx_msg void OnCheck7();
	afx_msg void OnCheck10();
	afx_msg void OnCheck11();
	afx_msg void OnCheck12();
	afx_msg void OnCheck18();
	afx_msg void OnChangeEdit18();
	afx_msg void OnChangeEdit19();
	afx_msg void OnChangeEdit20();
	afx_msg void OnChangeEdit21();
	afx_msg void OnChangeEdit22();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABENTITY_H__45635D44_C3AC_4400_9CDE_0559E68E3633__INCLUDED_)
