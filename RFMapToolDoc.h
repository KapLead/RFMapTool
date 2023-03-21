// RFMapToolDoc.h : interface of the CRFMapToolDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFMAPTOOLDOC_H__8F47B194_2FF0_4A6C_95BA_D92D19D6AFE6__INCLUDED_)
#define AFX_RFMAPTOOLDOC_H__8F47B194_2FF0_4A6C_95BA_D92D19D6AFE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CMyD3dApp.h"
#include "Crender.h"
#include "RightBack.h"
//#include "Collision.h"
#include "TabParticle.h"
#include "RollupCtrl.h"

class CRFMapToolDoc : public CDocument
{
protected: // create from serialization only
	CRFMapToolDoc();
	DECLARE_DYNCREATE(CRFMapToolDoc)

// Attributes
public:

	CRender m_cRender;
	CR3D3DApp m_cD3dApp;


	// �� �Ұ͵�...
	CRollupCtrl m_wndCollRollupCtrl;	//�̰� �浹�κ�...
	CRollupCtrl m_wndEntityRollupCtrl;	//�̰� ��ƼƼ�κ�...
	CRollupCtrl m_wndParticleRollupCtrl;	//�̰� ��ƼŬ�κ�...
	CRollupCtrl m_wndMagicRollupCtrl;		//�̰� �����κ�...
	CRollupCtrl m_wndSoundRollupCtrl;		//�̰� ����κ�...
	CRollupCtrl m_wndDummyRollupCtrl;		//�̰� �������� �� �� ���� �κ�...


	CRightBack m_RightBack;	//������ ������׻� �ߴ°�..

	CParticleEdit *ParticleEdit;	//��ƼŬ ���� ������..

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRFMapToolDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRFMapToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRFMapToolDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CRFMapToolDoc *g_pDoc;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RFMAPTOOLDOC_H__8F47B194_2FF0_4A6C_95BA_D92D19D6AFE6__INCLUDED_)
