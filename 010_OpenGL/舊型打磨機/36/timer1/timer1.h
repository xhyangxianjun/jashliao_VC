// timer1.h : main header file for the TIMER1 application
//

#if !defined(AFX_TIMER1_H__6D2F5A72_7290_4CF3_93A1_162BD6C6956B__INCLUDED_)
#define AFX_TIMER1_H__6D2F5A72_7290_4CF3_93A1_162BD6C6956B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTimer1App:
// See timer1.cpp for the implementation of this class
//

class CTimer1App : public CWinApp
{
public:
	CTimer1App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimer1App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTimer1App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMER1_H__6D2F5A72_7290_4CF3_93A1_162BD6C6956B__INCLUDED_)
