// GPSGetDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GPSGetData.h"
#include "GPSGetDataDlg.h"
#include "GPSString.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CGPSGetDataDlg dialog

CGPSGetDataDlg::CGPSGetDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGPSGetDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGPSGetDataDlg)
	m_Input = _T("");
	m_head = _T("");
	m_time = _T("");
	m_strLatitude = _T("");
	m_strLongitude = _T("");
	m_direction = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGPSGetDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGPSGetDataDlg)
	DDX_Text(pDX, IDC_EDIT1, m_Input);
	DDX_Text(pDX, IDC_EDIT2, m_head);
	DDX_Text(pDX, IDC_EDIT3, m_time);
	DDX_Text(pDX, IDC_EDIT4, m_strLatitude);
	DDX_Text(pDX, IDC_EDIT5, m_strLongitude);
	DDX_Text(pDX, IDC_EDIT6, m_direction);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGPSGetDataDlg, CDialog)
	//{{AFX_MSG_MAP(CGPSGetDataDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGPSGetDataDlg message handlers

BOOL CGPSGetDataDlg::OnInitDialog()
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
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGPSGetDataDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGPSGetDataDlg::OnPaint() 
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
HCURSOR CGPSGetDataDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CGPSGetDataDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	CGPSString GPS;
	UpdateData(true);
		GPS.SetData(m_Input);
		GPS.Operation(); 
		m_head=GPS.GetHead(); 
		m_time=GPS.GetTime();
		m_strLatitude=GPS.GetLatitude();
		m_strLongitude=GPS.GetLongitude();
		m_direction=GPS.GetDirection();
	UpdateData(false);		
}

void CGPSGetDataDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
		m_Input=""; 
		m_head=""; 
		m_time="";
		m_strLatitude="";
		m_strLongitude="";
		m_direction="";
	UpdateData(false);		
}
