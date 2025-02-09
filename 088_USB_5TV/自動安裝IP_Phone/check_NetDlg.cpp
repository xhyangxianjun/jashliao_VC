// check_NetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "check_Net.h"
#include "check_NetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
TCHAR strCurDrt[500];
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheck_NetDlg dialog

CCheck_NetDlg::CCheck_NetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCheck_NetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheck_NetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCheck_NetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheck_NetDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCheck_NetDlg, CDialog)
	//{{AFX_MSG_MAP(CCheck_NetDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheck_NetDlg message handlers

BOOL CCheck_NetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	//////////////////////////////////////////
	////Step_02
	CFileFind   finder;
	CString m_S1;
	int nLen = ::GetCurrentDirectory(500,strCurDrt);//取得目前程式路徑
	if( strCurDrt[nLen]!='\\' )
	{
		strCurDrt[nLen++] = '\\';
		strCurDrt[nLen] = '\0';
	}
	m_S1= strCurDrt;
	//AfxMessageBox(m_S1);  
	//static   const   TCHAR   szFileToFind[]   =   _T("C:\\GROWSYS");   
	static   const   TCHAR   szFileToFind[]   =   _T("C:\\Program Files\\miTalk");
	if   (finder.FindFile(szFileToFind))   
	{   
		finder.FindNextFile();   
		//AfxMessageBox(finder.GetFileName());   
		if   (!finder.IsDirectory())   
		{   
			//AfxMessageBox("為文件");   
		}   
		else   
		{   
			//AfxMessageBox("為資料夾");
			m_S1+="WowTV.exe";	
			ShellExecute(NULL,"open",m_S1,NULL,NULL,SW_SHOWNORMAL);
			this->OnOK();
		}   
	}   
	else   
	{   
		MessageBox("準備開始安裝 CHUT Softphone","CHUT Softphone");
		m_S1+="software\\chut_wow.exe";
		ShellExecute(NULL,"open",m_S1,NULL,NULL,SW_SHOWNORMAL);
		this->ShowWindow(SW_HIDE); 
		SetTimer(1,1000,NULL);
	}   
	//////////////////////////////////////////
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCheck_NetDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCheck_NetDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCheck_NetDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCheck_NetDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd* hWnd1 = FindWindow(NULL, "CHUT Softphone 安裝程式");
	CString m_S1;
	m_S1= strCurDrt;
	this->ShowWindow(SW_HIDE); 
	if(hWnd1==NULL)
	{
		KillTimer(1);
		this->ShowWindow(SW_SHOW);
		this->ShowWindow(SW_SHOWNORMAL);
		MessageBox("CHUT Softphone 安裝完成","CHUT Softphone");
		m_S1+="WowTV.exe";	
		ShellExecute(NULL,"open",m_S1,NULL,NULL,SW_SHOWNORMAL);
		this->OnOK();
	}
	CDialog::OnTimer(nIDEvent);
}
