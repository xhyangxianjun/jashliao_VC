// matlab_testDlg.h : header file
//

#if !defined(AFX_MATLAB_TESTDLG_H__ED21EA77_3695_4472_9C73_8C42EC1F949F__INCLUDED_)
#define AFX_MATLAB_TESTDLG_H__ED21EA77_3695_4472_9C73_8C42EC1F949F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMatlab_testDlg dialog

class CMatlab_testDlg : public CDialog
{
// Construction
public:
	CMatlab_testDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMatlab_testDlg)
	enum { IDD = IDD_MATLAB_TEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMatlab_testDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMatlab_testDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MATLAB_TESTDLG_H__ED21EA77_3695_4472_9C73_8C42EC1F949F__INCLUDED_)
