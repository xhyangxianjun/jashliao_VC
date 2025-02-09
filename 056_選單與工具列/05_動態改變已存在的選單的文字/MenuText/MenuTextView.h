// MenuTextView.h : interface of the CMenuTextView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENUTEXTVIEW_H__3381FBA5_881A_4A7F_A4BE_5905F6A0391C__INCLUDED_)
#define AFX_MENUTEXTVIEW_H__3381FBA5_881A_4A7F_A4BE_5905F6A0391C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMenuTextView : public CView
{
protected: // create from serialization only
	CMenuTextView();
	DECLARE_DYNCREATE(CMenuTextView)

// Attributes
public:
	CMenuTextDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuTextView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMenuTextView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMenuTextView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MenuTextView.cpp
inline CMenuTextDoc* CMenuTextView::GetDocument()
   { return (CMenuTextDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENUTEXTVIEW_H__3381FBA5_881A_4A7F_A4BE_5905F6A0391C__INCLUDED_)
