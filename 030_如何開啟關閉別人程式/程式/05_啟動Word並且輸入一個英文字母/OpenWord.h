// OpenWord.h : main header file for the OPENWORD application
//

#if !defined(AFX_OPENWORD_H__70C3DA04_25DB_11DA_B776_0040F43A9B72__INCLUDED_)
#define AFX_OPENWORD_H__70C3DA04_25DB_11DA_B776_0040F43A9B72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// COpenWordApp:
// See OpenWord.cpp for the implementation of this class
//

class COpenWordApp : public CWinApp
{
public:
	COpenWordApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenWordApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(COpenWordApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENWORD_H__70C3DA04_25DB_11DA_B776_0040F43A9B72__INCLUDED_)
