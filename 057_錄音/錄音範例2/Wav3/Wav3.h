// Wav3.h : main header file for the WAV3 application
//

#if !defined(AFX_WAV3_H__D86A7352_4DDA_43AB_B038_05493C19683C__INCLUDED_)
#define AFX_WAV3_H__D86A7352_4DDA_43AB_B038_05493C19683C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWav3App:
// See Wav3.cpp for the implementation of this class
//

class CWav3App : public CWinApp
{
public:
	CWav3App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWav3App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWav3App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAV3_H__D86A7352_4DDA_43AB_B038_05493C19683C__INCLUDED_)
