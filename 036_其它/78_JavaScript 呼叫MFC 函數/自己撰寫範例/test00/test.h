// test.h : main header file for the TEST application
//

#if !defined(AFX_TEST_H__D21BCAAE_BCD4_4EE2_8D1B_D14B14E7FCE5__INCLUDED_)
#define AFX_TEST_H__D21BCAAE_BCD4_4EE2_8D1B_D14B14E7FCE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestApp:
// See test.cpp for the implementation of this class
//
class CImpIDispatch;//modify by jash

class CTestApp : public CWinApp
{
public:
	CTestApp();
public:
	CImpIDispatch*		m_pCustDisp;//modify by jash
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();//modify by jash
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CTestApp theApp;//modify by jash

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST_H__D21BCAAE_BCD4_4EE2_8D1B_D14B14E7FCE5__INCLUDED_)
