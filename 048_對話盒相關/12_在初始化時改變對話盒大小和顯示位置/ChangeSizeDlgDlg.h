// ChangeSizeDlgDlg.h : header file
//

#if !defined(AFX_CHANGESIZEDLGDLG_H__703A0D33_0902_4A40_939B_35F90A616DAD__INCLUDED_)
#define AFX_CHANGESIZEDLGDLG_H__703A0D33_0902_4A40_939B_35F90A616DAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CChangeSizeDlgDlg dialog

class CChangeSizeDlgDlg : public CDialog
{
// Construction
public:
	CChangeSizeDlgDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CChangeSizeDlgDlg)
	enum { IDD = IDD_CHANGESIZEDLG_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChangeSizeDlgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CChangeSizeDlgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANGESIZEDLGDLG_H__703A0D33_0902_4A40_939B_35F90A616DAD__INCLUDED_)
