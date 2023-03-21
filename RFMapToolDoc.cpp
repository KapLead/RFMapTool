// RFMapToolDoc.cpp : implementation of the CRFMapToolDoc class
//

#include "stdafx.h"
#include "RFMapTool.h"

#include "RFMapToolDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRFMapToolDoc

IMPLEMENT_DYNCREATE(CRFMapToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CRFMapToolDoc, CDocument)
	//{{AFX_MSG_MAP(CRFMapToolDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRFMapToolDoc construction/destruction
CRFMapToolDoc *g_pDoc;

CRFMapToolDoc::CRFMapToolDoc()
{
	// TODO: add one-time construction code here

//	m_pcTabSheet = new CTabSheet("dd");

	g_pDoc = this;
	g_pDoc->ParticleEdit=NULL;

}

CRFMapToolDoc::~CRFMapToolDoc()
{
//	delete m_pcTabSheet;

}

BOOL CRFMapToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CRFMapToolDoc serialization

void CRFMapToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRFMapToolDoc diagnostics

#ifdef _DEBUG
void CRFMapToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRFMapToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRFMapToolDoc commands
