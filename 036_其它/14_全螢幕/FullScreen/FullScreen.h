// FullScreen.h : main header file for the FULLSCREEN application
//

#if !defined(AFX_FULLSCREEN_H__F5D40DA7_673D_11D7_8890_00C04F4AD348__INCLUDED_)
#define AFX_FULLSCREEN_H__F5D40DA7_673D_11D7_8890_00C04F4AD348__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CFullScreenApp:
// See FullScreen.cpp for the implementation of this class
//

class CFullScreenApp : public CWinApp
{
public:
	CFullScreenApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFullScreenApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CFullScreenApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FULLSCREEN_H__F5D40DA7_673D_11D7_8890_00C04F4AD348__INCLUDED_)
