// glDoc.h : interface of the CGlDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLDOC_H__20493539_47E2_4B48_9AD2_F661C3B1EC5D__INCLUDED_)
#define AFX_GLDOC_H__20493539_47E2_4B48_9AD2_F661C3B1EC5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGlDoc : public CDocument
{
protected: // create from serialization only
	CGlDoc();
	DECLARE_DYNCREATE(CGlDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGlDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGlDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGlDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLDOC_H__20493539_47E2_4B48_9AD2_F661C3B1EC5D__INCLUDED_)
