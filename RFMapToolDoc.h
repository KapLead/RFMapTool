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


	// 롤 할것들...
	CRollupCtrl m_wndCollRollupCtrl;	//이건 충돌부분...
	CRollupCtrl m_wndEntityRollupCtrl;	//이건 엔티티부분...
	CRollupCtrl m_wndParticleRollupCtrl;	//이건 파티클부분...
	CRollupCtrl m_wndMagicRollupCtrl;		//이건 매직부분...
	CRollupCtrl m_wndSoundRollupCtrl;		//이건 사운드부분...
	CRollupCtrl m_wndDummyRollupCtrl;		//이건 서버에서 쓸 맵 더미 부분...


	CRightBack m_RightBack;	//오른쪽 상단의항상 뜨는거..

	CParticleEdit *ParticleEdit;	//파티클 에딧 포인터..

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
