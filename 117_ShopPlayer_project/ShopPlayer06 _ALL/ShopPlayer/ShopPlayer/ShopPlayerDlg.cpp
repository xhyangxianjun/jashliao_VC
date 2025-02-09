// ShopPlayerDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "ShopPlayer.h"
#include "ShopPlayerDlg.h"
#include <winbase.h>//MAP memory_step 01
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////
//MAP memory_step 02
#define	MAP_Data	"data info"
#define	MAP_LENGTH	1024
HANDLE hDataMap = NULL ;
static char szOldData[MAP_LENGTH] = "" ;
//////////////////////////////////////
// 對 App About 使用 CAboutDlg 對話方塊
int ginttimerrun;
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 對話方塊資料
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

// 程式碼實作
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CShopPlayerDlg 對話方塊




CShopPlayerDlg::CShopPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShopPlayerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShopPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VIDEO_WINDOW, m_videoWindow);
}

BEGIN_MESSAGE_MAP(CShopPlayerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_GRAPHNOTIFY, OnGraphNotify)//DirectShow_step 05
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CShopPlayerDlg 訊息處理常式

BOOL CShopPlayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 將 [關於...] 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
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

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO: 在此加入額外的初始設定
	InitialVariable();
	/////////////////////////////////////////////////////////////////////////////	
	ModifyStyle(0, WS_SYSMENU);
	SetWindowText(L"ShopPlayer");	
	this->MoveWindow(m_lngLeft,m_lngTop,m_lngWidth,m_lngHeight,TRUE);
	RECT rc;
	this->GetClientRect(&rc);
	m_videoWindow.MoveWindow( &rc, true );
	//////////////////////////////////////////////////////////////////////////////
	hDataMap = CreateFileMapping( (HANDLE)0xFFFFFFFF,NULL, PAGE_READWRITE, 0, MAP_LENGTH,	TEXT(MAP_Data) ) ;//MAP memory_step 03
	m_dblPosition=0.0;
	m_sourceFile=GetMapMemory(&m_dblPosition);//MAP memory_step 06
	m_pFilterGraph = NULL;//DirectShow_step 04
	MovieOpen();//DirectShow_step 14
	MoviePlay();//DirectShow_step 15
	SetTimer(1,100,NULL);
	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CShopPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CShopPlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CShopPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CShopPlayerDlg::InitialVariable(void)
{
	MSXML2::IXMLDOMDocumentPtr pDoc;
	int nLen=0;
	bool blnRead=false;
	_variant_t vtStr;
	CString StrValue;
	m_lngTop=0;m_lngLeft=0;m_lngWidth=0;m_lngHeight=0;
	HRESULT hr = pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument30));
	if(!SUCCEEDED(hr))
	{
	   return;
	}
	nLen= ::GetCurrentDirectory(500,m_chrCurDrt);//取得目前程式路徑
	m_StrSettingFileName+="Resources\\Config.xml";
	blnRead=pDoc->load((_bstr_t)m_StrSettingFileName);
	if(blnRead)
	{
		//////////////////////////////////////////////////
		MSXML2::IXMLDOMNodePtr Position_Size;
		Position_Size=pDoc->selectSingleNode("//Top");
		Position_Size->get_nodeTypedValue(&vtStr);
		StrValue=vtStr.bstrVal;
		m_lngTop=_ttoi(static_cast<LPCTSTR>(StrValue)); 
		//************************************************
		Position_Size=pDoc->selectSingleNode("//Left");
		Position_Size->get_nodeTypedValue(&vtStr);
		StrValue=vtStr.bstrVal;
		m_lngLeft=_ttoi(static_cast<LPCTSTR>(StrValue));  
		//************************************************
		Position_Size=pDoc->selectSingleNode("//Width");
		Position_Size->get_nodeTypedValue(&vtStr);
		StrValue=vtStr.bstrVal;
		m_lngWidth=_ttoi(static_cast<LPCTSTR>(StrValue));  
		//************************************************
		Position_Size=pDoc->selectSingleNode("//Height");
		Position_Size->get_nodeTypedValue(&vtStr);
		StrValue=vtStr.bstrVal;
		m_lngHeight=_ttoi(static_cast<LPCTSTR>(StrValue)); 
		//////////////////////////////////////////////////
	}
}
LRESULT CShopPlayerDlg::OnGraphNotify(WPARAM inWParam, LPARAM inLParam)//DirectShow_step 07
{
	IMediaEventEx *pEvent = NULL;
	if ((m_pFilterGraph!=NULL) && (pEvent = m_pFilterGraph->GetEventHandle()))
	{
		LONG eventCode = 0;
		LONG eventParam1  = 0;
		LONG eventParam2 = 0;
		
		while (SUCCEEDED(pEvent->GetEvent(&eventCode, &eventParam1, &eventParam2, 0)))
		{
			pEvent->FreeEventParams(eventCode, eventParam1, eventParam2);
			switch (eventCode)
			{
			case EC_COMPLETE:
				MovieStop();
				//MessageBox(L"finish ...");
				ginttimerrun=1;
				break;
			case EC_USERABORT:
			case EC_ERRORABORT:
				MovieStop();
				break;
			default:
				break;
			}
		}
	}
	return 0;
}

void CShopPlayerDlg::CreateGraph()//DirectShow_step 08
{
	DestroyGraph();                  //种障薦疏ん蟈桶芞
	m_pFilterGraph = new CDXGraph(); //斐膘CDXGraph勤砓
	if (m_pFilterGraph->Create())    //斐膘薦疏ん蟈桶奪燴ん
	{
		//if (!m_pFilterGraph->RenderFile(ch))//馺�壅諒槸躁�ㄛ凳膘薦疏ん蟈桶
		TCHAR *ch1 = m_sourceFile.GetBuffer(m_sourceFile.GetLength());
		
		if (!m_pFilterGraph->RenderFile(ch1))//馺�壅諒槸躁�ㄛ凳膘薦疏ん蟈桶
		{
			MessageBox(_T("拸楊馺�噹佸諒槸躁�ㄐ③�溜珅Й騋終區鉆婼熆踰鰷撘�ㄐ\n 麼氪森羸极恅璃眒囷輓ㄐ"),_T("炵苀枑尨"),MB_ICONWARNING); 
			return;
		}
		m_sourceFile.ReleaseBuffer();
         //扢离芞砉珆尨敦諳
		m_pFilterGraph->SetDisplayWindow(m_videoWindow.GetSafeHwnd());
         //扢离敦諳秏洘籵眭
		m_pFilterGraph->SetNotifyWindow(this->GetSafeHwnd());
		//珆尨菴珨痋芞砉
		m_pFilterGraph->Pause();

	}
}
void CShopPlayerDlg::DestroyGraph()//DirectShow_step 09
{
	if (m_pFilterGraph != NULL)
	{
		m_pFilterGraph->Stop();
		m_pFilterGraph->SetNotifyWindow(NULL);

		delete m_pFilterGraph;
		m_pFilterGraph = NULL;
	}
}
CString CShopPlayerDlg::GetFileTitleFromFileName(CString FileName, BOOL Ext)//DirectShow_step 10   
{   
    int Where;   
    Where = FileName.ReverseFind('\\');  
    if (Where == -1)  
        Where = FileName.ReverseFind('/');  
    CString FileTitle = FileName.Right(FileName.GetLength() - 1 - Where);  
    if (!Ext)  
    {  
        int Which = FileTitle.ReverseFind('.');   
        if (Which != -1)   
            FileTitle = FileTitle.Left(Which);   
    }   
    return FileTitle;   
}
void CShopPlayerDlg::MovieOpen()//DirectShow_step 11
{
	if(m_sourceFile=="No Data")
		m_sourceFile = _T("D:\\11.wmv");
	m_mediaFileName =GetFileTitleFromFileName(m_sourceFile,1);
	CreateGraph();
}
void CShopPlayerDlg::MoviePlay()//DirectShow_step 12
{
	if (m_pFilterGraph)
	{
		SetWindowText(_T("Media File Name: ") + m_mediaFileName);
		double duration =1.0;
		m_pFilterGraph->GetDuration(&duration);
		m_pFilterGraph->SetCurrentPosition(m_dblPosition);
		m_pFilterGraph->Run();

		//m_pFilterGraph->ChangeAudioVolume(m_volume);
	}
}
void CShopPlayerDlg::MovieStop()//DirectShow_step 13
{
	if (m_pFilterGraph != NULL)
	{
		m_pFilterGraph->Stop();
	}
}
CString CShopPlayerDlg::GetMapMemory(double *dblPosition)//MAP memory_step 05
{
	CString StrData;
	StrData="No Data";
	LPVOID mapView = MapViewOfFile( hDataMap,FILE_MAP_WRITE, 0, 0, 0 ) ;
	if( mapView != NULL ) 
	{
		StrData=(LPTSTR)mapView;
		if(StrData=="")
		{
			StrData="No Data";
			return StrData;
		}
	}

	//************************
	CString theString( "" ); 
	LPTSTR lpsz = new TCHAR[theString.GetLength()+1]; 
	_tcscpy(lpsz, theString);
	_tcscpy((LPTSTR)mapView,lpsz);
	UnmapViewOfFile( (LPVOID)mapView);
	delete []lpsz;

	//***********************
	//Unicode下CString轉換為char *
	//注意：以下n和len的值大小不同,n是按字元計算的，len是按位元組計算的
	int n = StrData.GetLength();
	//獲取寬位元組字元的大小，大小是按位元組計算的
	int len = WideCharToMultiByte(CP_ACP,0,StrData,StrData.GetLength(),NULL,0,NULL,NULL);
	//為多位元組字元陣列申請空間，陣列大小為按位元組計算的寬位元組位元組大小
	char * pFileName = new char[len+1];   //以位元組為單位
	//寬位元組編碼轉換成多位元組編碼
	WideCharToMultiByte(CP_ACP,0,StrData,StrData.GetLength(),pFileName,len,NULL,NULL);
	pFileName[len] = '\0'; //添加字串結尾，注意不是len+1
	
	//************************
	//sscanf
	char Schrd01[500],Schrd02[500];
	double fltp01;
	sscanf (pFileName ,"%[^','],%s",Schrd01,Schrd02);
	fltp01=atof(Schrd02);
	delete [] pFileName;

	//**********************
	*dblPosition=fltp01;
	StrData=Schrd01;

	return StrData;
}
void CShopPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	TCHAR *ch1;
	if(ginttimerrun==1)
	{
		KillTimer(1);
		ginttimerrun=0;
		m_dblPosition=0.0;
		m_sourceFile=GetMapMemory(&m_dblPosition);
		if(m_sourceFile=="No Data")
			m_sourceFile = _T("D:\\11.wmv");
		m_mediaFileName =GetFileTitleFromFileName(m_sourceFile,1);
		SetWindowText(_T("Media File Name: ") + m_mediaFileName);
		/////////////////////////////////////////////////////////
		CreateGraph();
		ch1 = m_sourceFile.GetBuffer(m_sourceFile.GetLength());
		if (!m_pFilterGraph->RenderFile(ch1))//馺�壅諒槸躁�ㄛ凳膘薦疏ん蟈桶
		{
			MessageBox(_T("拸楊馺�噹佸諒槸躁�ㄐ③�溜珅Й騋終區鉆婼熆踰鰷撘�ㄐ\n 麼氪森羸极恅璃眒囷輓ㄐ"),_T("炵苀枑尨"),MB_ICONWARNING); 
		}
		m_sourceFile.ReleaseBuffer();
		//扢离芞砉珆尨敦諳
		m_pFilterGraph->SetDisplayWindow(m_videoWindow.GetSafeHwnd());
		//扢离敦諳秏洘籵眭
		m_pFilterGraph->SetNotifyWindow(this->GetSafeHwnd());
		//珆尨菴珨痋芞砉
		m_pFilterGraph->Pause();
		/////////////////////////////////////////////////////////////
		m_pFilterGraph->SetCurrentPosition(m_dblPosition);
		m_pFilterGraph->Run();
		SetTimer(1,100,NULL);
	}

	CDialog::OnTimer(nIDEvent);
}
