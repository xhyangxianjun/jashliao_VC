// testHuaweiDlg.h : header file
//

#if !defined(AFX_TESTHUAWEIDLG_H__9B4D0739_BFE8_494B_BBDE_8AFC98A78ED6__INCLUDED_)
#define AFX_TESTHUAWEIDLG_H__9B4D0739_BFE8_494B_BBDE_8AFC98A78ED6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTestHuaweiDlg dialog

class CTestHuaweiDlg : public CDialog
{
// Construction
public:
	CTestHuaweiDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestHuaweiDlg)
	enum { IDD = IDD_TESTHUAWEI_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestHuaweiDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestHuaweiDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTHUAWEIDLG_H__9B4D0739_BFE8_494B_BBDE_8AFC98A78ED6__INCLUDED_)
