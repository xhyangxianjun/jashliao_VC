// Get_CRLFDlg.h : header file
//

#if !defined(AFX_GET_CRLFDLG_H__5E843B34_A3BB_4040_9E8E_D5D28138A6D8__INCLUDED_)
#define AFX_GET_CRLFDLG_H__5E843B34_A3BB_4040_9E8E_D5D28138A6D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGet_CRLFDlg dialog

class CGet_CRLFDlg : public CDialog
{
// Construction
public:
	CGet_CRLFDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGet_CRLFDlg)
	enum { IDD = IDD_GET_CRLF_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGet_CRLFDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGet_CRLFDlg)
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

#endif // !defined(AFX_GET_CRLFDLG_H__5E843B34_A3BB_4040_9E8E_D5D28138A6D8__INCLUDED_)
