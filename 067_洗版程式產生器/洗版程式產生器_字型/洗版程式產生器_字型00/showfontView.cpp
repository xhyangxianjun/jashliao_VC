// showfontView.cpp : implementation of the CShowfontView class
//

#include "stdafx.h"
#include "showfont.h"

#include "showfontDoc.h"
#include "showfontView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShowfontView

IMPLEMENT_DYNCREATE(CShowfontView, CView)

BEGIN_MESSAGE_MAP(CShowfontView, CView)
	//{{AFX_MSG_MAP(CShowfontView)
	ON_COMMAND(ID_BUT1, OnBut1)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowfontView construction/destruction

CShowfontView::CShowfontView()
{
	// TODO: add construction code here

}

CShowfontView::~CShowfontView()
{
}

BOOL CShowfontView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CShowfontView drawing

void CShowfontView::OnDraw(CDC* pDC)
{
	CShowfontDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CShowfontView printing

BOOL CShowfontView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CShowfontView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CShowfontView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CShowfontView diagnostics

#ifdef _DEBUG
void CShowfontView::AssertValid() const
{
	CView::AssertValid();
}

void CShowfontView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CShowfontDoc* CShowfontView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CShowfontDoc)));
	return (CShowfontDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CShowfontView message handlers

void CShowfontView::OnBut1() 
{
	CFontDialog dlg;
	FILE *pf;
	pf=fopen("test.txt","w");
	int i,j;
	int R,G,B;
	COLORREF RGB1,RGB2;
	if (dlg.DoModal() == IDOK)
	{
	   // Create the font using the selected font from CFontDialog.
	   LOGFONT lf;
	   memcpy(&lf, dlg.m_cf.lpLogFont, sizeof(LOGFONT));

	   CFont font;
	   VERIFY(font.CreateFontIndirect(&lf));

	   // Do something with the font just created...
	   CClientDC dc(this);
	   CFont* def_font = dc.SelectObject(&font);
	   dc.TextOut(5, 5, "End",3);
	   dc.SelectObject(def_font);
		for(j=0;j<150;j++)
		{
			for(i=0;i<600;i++)
			{
				RGB1=dc.GetPixel(i,j);
				R=GetRValue(RGB1);
				G=GetGValue(RGB1);
				B=GetBValue(RGB1);
				if((B==255)&&(G==255) &&(R==255) )
				{
					RGB2=RGB(0,0,0);
					fprintf(pf,"1 ");
				}
				else
				{
					RGB2=RGB(255,255,255);
					fprintf(pf,"  ");
				}
				dc.SetPixel(i,j,RGB2); 
			}
			fprintf(pf,"\n");
		}
	   // Done with the font. Delete the font object.
	   font.DeleteObject();
	}
fclose(pf);
}
