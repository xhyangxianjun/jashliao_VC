// menuDoc.cpp : implementation of the CMenuDoc class
//

#include "stdafx.h"
#include "menu.h"

#include "menuDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuDoc

IMPLEMENT_DYNCREATE(CMenuDoc, CDocument)

BEGIN_MESSAGE_MAP(CMenuDoc, CDocument)
	//{{AFX_MSG_MAP(CMenuDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuDoc construction/destruction

CMenuDoc::CMenuDoc()
{
	// TODO: add one-time construction code here

}

CMenuDoc::~CMenuDoc()
{
}

BOOL CMenuDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMenuDoc serialization

void CMenuDoc::Serialize(CArchive& ar)
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
// CMenuDoc diagnostics

#ifdef _DEBUG
void CMenuDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMenuDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMenuDoc commands
