// MFC_testDlg.h : header file
//

#if !defined(AFX_MFC_TESTDLG_H__BDB652D3_C255_4672_8A3F_9418498693A5__INCLUDED_)
#define AFX_MFC_TESTDLG_H__BDB652D3_C255_4672_8A3F_9418498693A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMFC_testDlg dialog

class CMFC_testDlg : public CDialog
{
// Construction
public:
	CMFC_testDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMFC_testDlg)
	enum { IDD = IDD_MFC_TEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMFC_testDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMFC_testDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFC_TESTDLG_H__BDB652D3_C255_4672_8A3F_9418498693A5__INCLUDED_)
