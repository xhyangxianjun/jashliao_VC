// ThreadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Thread.h"
#include "ThreadDlg.h"
//////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include<afxmt.h>
#include <afxtempl.h>//CArray Step_01 
//////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////
CEvent g_start(FALSE, TRUE);//如此一來就可以一個事件驅動多個Thead
//CEvent g_start;//如此一來就可以g_start.SetEvent()只能驅動一個
////////////////////////////
CArray <CString,CString&> m_ArrayAll;//Step 2:CArray//存放來源檔案資料
int gintcount;
CWinThread* m_pThreadLog;
CWinThread* m_pThread[3];
UINT ThreadFun(LPVOID a);
UINT Threadlog(LPVOID a);
UINT Threadlog(LPVOID a)
{
	FILE *pf;
	CString StrBuf;
	while(true)
	{
		if(m_ArrayAll.GetSize()>0)
		{
			pf=fopen("All.txt","a");
			StrBuf=m_ArrayAll.GetAt(0);
			fprintf(pf,"%s\n",StrBuf.GetBuffer(0));
			m_ArrayAll.RemoveAt(0);
			fclose(pf);
			pf=NULL;
			if(gintcount==0 && m_ArrayAll.GetSize()<=0)
			{
				break;
			}
		}
		Sleep(100);
	}
	AfxMessageBox("log ko");
	return 0;
}
UINT ThreadFun(LPVOID a)
{
	
	int b;
	FILE *pf;
	CString StrBuf;
	char head[2],*head1;
	CString str;
	b=(int)a;
	str=".txt";
	_itoa(b,head,10);
	str=head+str;
	head1=str.GetBuffer(0); 
	pf=fopen(head1,"w");
	g_start.Lock(INFINITE); 
	for(int i=0;i<b;i++)
	{
		fprintf(pf,"%d\n",i);
		StrBuf.Format("%d->%d",b,i);
		m_ArrayAll.Add(StrBuf);
		//Sleep(b*10);
		Sleep(b);
	}
	fclose(pf);
	gintcount--;
	AfxMessageBox("ko");
	return 0;
}
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
// CThreadDlg dialog

CThreadDlg::CThreadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThreadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CThreadDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThreadDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CThreadDlg, CDialog)
	//{{AFX_MSG_MAP(CThreadDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThreadDlg message handlers

BOOL CThreadDlg::OnInitDialog()
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

void CThreadDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CThreadDlg::OnPaint() 
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
HCURSOR CThreadDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CThreadDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	int b=10;
	m_pThread[0]=AfxBeginThread(ThreadFun,(LPVOID)b,0,0,0,NULL);
	b=20;
	m_pThread[1]=AfxBeginThread(ThreadFun,(LPVOID)b,0,0,0,NULL);
	b=30;
	m_pThread[2]=AfxBeginThread(ThreadFun,(LPVOID)b,0,0,0,NULL);
	gintcount=3;
}

void CThreadDlg::OnButton2() 
{
	int b=10;
	g_start.SetEvent();
	m_pThreadLog=AfxBeginThread(Threadlog,(LPVOID)b,0,0,0,NULL);
	//g_start.SetEvent();
}
