// check_NetDlg.h : header file
//

#if !defined(AFX_CHECK_NETDLG_H__FFAC3505_D8FD_4AAB_8D05_A779D32591CB__INCLUDED_)
#define AFX_CHECK_NETDLG_H__FFAC3505_D8FD_4AAB_8D05_A779D32591CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCheck_NetDlg dialog

class CCheck_NetDlg : public CDialog
{
// Construction
public:
	CCheck_NetDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCheck_NetDlg)
	enum { IDD = IDD_CHECK_NET_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheck_NetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCheck_NetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECK_NETDLG_H__FFAC3505_D8FD_4AAB_8D05_A779D32591CB__INCLUDED_)
