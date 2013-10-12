
// ScreenCapDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ScreenCap.h"
#include "ScreenCapDlg.h"
#include "afxdialogex.h"

#define WM_SHOWTASK (WM_USER + 1)
#define TIMER_INIT_MIN 1

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//获取指定图片格式的 CLSID
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	// CLSID encoderClsid;
	// GetEncoderClsid(L"image/png", &encoderClsid);
	// GetEncoderClsid(L"image/bmp", &encoderClsid);
	// GetEncoderClsid(L"image/gif", &encoderClsid);
	// GetEncoderClsid(L"image/jpeg", &encoderClsid);
	// GetEncoderClsid(L"image/tiff", &encoderClsid);
	UINT num= 0;
	UINT size= 0;

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if(size== 0)
	{
		return -1;
	}
	pImageCodecInfo= (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo== NULL)
	{
		return -1;
	}

	GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j=0; j< num; ++j)
	{
		if(wcscmp(pImageCodecInfo[j].MimeType, format)== 0)
		{
			*pClsid= pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	} 
	free(pImageCodecInfo);
	return -1;
}

//保存屏幕截图
void SaveScreenToJpg()
{
	// 得到屏幕DC
	HWND hDeskWnd = ::GetDesktopWindow();					//获得屏幕的HWND. 
	CDC *pDestDC = CDC::FromHandle(::GetDC(hDeskWnd));		//获取当前整个屏幕DC
	int screenWidth = pDestDC->GetDeviceCaps(HORZRES);		//屏幕宽
	int screenHeight = pDestDC->GetDeviceCaps(VERTRES);     //屏幕高
	// 创建与屏幕兼容的Bitmap 
	CBitmap memBitmap;   
	memBitmap.CreateCompatibleBitmap(pDestDC, screenWidth, screenHeight);
	// 创建屏幕的内存DC
	CDC memDC;     
	memDC.CreateCompatibleDC(pDestDC);
	// 将兼容Bitmap格式选入内存DC
	memDC.SelectObject(&memBitmap);
	// 将屏幕DC数据copy至内存DC
	memDC.BitBlt(0, 0, screenWidth, screenHeight, pDestDC, 0, 0, SRCCOPY);
	// 获得位图信息
	BITMAP bmpInfo;
	memBitmap.GetBitmap(&bmpInfo);
	// 生成BITMAPINFO
	BITMAPINFO m_BITMAPINFO;
	memset(&m_BITMAPINFO, 0, sizeof(BITMAPINFO));
	m_BITMAPINFO.bmiHeader.biSize     = sizeof(BITMAPINFOHEADER);
	m_BITMAPINFO.bmiHeader.biPlanes     = 1;
	m_BITMAPINFO.bmiHeader.biBitCount    = bmpInfo.bmBitsPixel;
	m_BITMAPINFO.bmiHeader.biCompression   = BI_RGB;
	m_BITMAPINFO.bmiHeader.biWidth     = bmpInfo.bmWidth;
	m_BITMAPINFO.bmiHeader.biHeight     = bmpInfo.bmHeight;
	// 获取位图数据
	BYTE * pBuffer = new BYTE[bmpInfo.bmWidthBytes * bmpInfo.bmHeight];
	GetDIBits(memDC.m_hDC, (HBITMAP) memBitmap.m_hObject, 0, screenHeight, pBuffer,
		(LPBITMAPINFO) &m_BITMAPINFO, DIB_RGB_COLORS);
	// 生成位图
	Bitmap *pSrcBmp = Bitmap::FromBITMAPINFO(&m_BITMAPINFO, (void*)pBuffer);
	// 保存成jpg文件
	CLSID encoderClsid;
	GetEncoderClsid(L"image/jpeg", &encoderClsid);
	pSrcBmp->Save(L"destop.jpg", &encoderClsid);
	// 释放内存
	delete pSrcBmp;
	delete pBuffer;

	ReleaseDC(NULL,pDestDC->GetSafeHdc());
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CScreenCapDlg 对话框


CScreenCapDlg::CScreenCapDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CScreenCapDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CScreenCapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CScreenCapDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_MENU_NEW_CAP, &CScreenCapDlg::OnMenuNewCap)
	ON_COMMAND(ID_MENU_SAVE_CAP, &CScreenCapDlg::OnMenuSaveCap)
	ON_WM_SIZE()
	ON_MESSAGE(WM_SHOWTASK,OnShowTask)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_HOTKEY,OnHotKey)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CScreenCapDlg 消息处理程序

BOOL CScreenCapDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//初始时最小化
	//SendMessage(WM_SYSCOMMAND,SC_MINIMIZE,NULL);
	//ShowWindow(SW_MINIMIZE);
	SetTimer(TIMER_INIT_MIN,10,NULL);

	//设置全局热键
	if(!::RegisterHotKey(GetSafeHwnd(),WM_HOTKEY,MOD_ALT|MOD_CONTROL,VK_F12))
	{
		AfxMessageBox(L"热键设置失败");
	}
	
	
	// 将“关于...”菜单项添加到系统菜单中。
	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CScreenCapDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CScreenCapDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		//使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CScreenCapDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//菜单：新建截屏
void CScreenCapDlg::OnMenuNewCap()
{
	// TODO: Add your command handler code here
}

//菜单：
void CScreenCapDlg::OnMenuSaveCap()
{
	SaveScreenToJpg();
}

//用来截获最小化消息
void CScreenCapDlg::OnSize(UINT nType, int cx, int cy)
{
	//最小化消息
	if(nType == SIZE_MINIMIZED)
	{
		HideToTask();
	}
	CDialogEx::OnSize(nType, cx, cy);
}


//托盘消息的处理函数
LRESULT CScreenCapDlg::OnShowTask(WPARAM wParam,LPARAM lParam) 
	//wParam接收的是图标的ID，而lParam接收的是鼠标的行为
{ 
	if(wParam!=IDR_MAINFRAME) 
		return 1; 
	switch(lParam) 
	{    
	case WM_RBUTTONUP://右键起来时弹出快捷菜单 
		{ 
			LPPOINT lpoint=new tagPOINT; 
			::GetCursorPos(lpoint);//得到鼠标位置 
			CMenu menu; 
			menu.CreatePopupMenu();//声明一个弹出式菜单 
			//增加菜单项“关闭”，点击则发送消息WM_DESTROY给主窗口（已 
			//隐藏），将程序结束。 
			menu.AppendMenu(MF_STRING,WM_DESTROY,L"关闭"); 
			menu.AppendMenu(MF_STRING,WM_SHOWWINDOW,L"打开");
			//确定弹出式菜单的位置 
			menu.TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this); 
			//资源回收 
			HMENU hmenu=menu.Detach(); 
			menu.DestroyMenu();			
			delete lpoint; 
		} 
		break; 
	case WM_LBUTTONUP:	//单机左键的处理 
		{ 		
			ShowWindow(SW_SHOWNORMAL);//显示主窗口
			SetForegroundWindow();	  //并将主窗体置于顶层	
		} 
		break; 
	} 
	return 0; 
} 


//关闭窗口时，删除托盘区图标和热键
void CScreenCapDlg::OnClose()
{
	//将图标从托盘区删除
	NOTIFYICONDATA nd;
	nd.cbSize   = sizeof (NOTIFYICONDATA);
	nd.hWnd		= m_hWnd;
	nd.uID		= IDR_MAINFRAME;
	Shell_NotifyIcon(NIM_DELETE,&nd);

	//删除热键
	::UnregisterHotKey(GetSafeHwnd(),WM_HOTKEY);

	CDialogEx::OnClose();
}


//最小到托盘区
void CScreenCapDlg::HideToTask(void)
{
	//最小到托盘区
	NOTIFYICONDATA nid; 
	nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA); 
	nid.hWnd=this->m_hWnd; 
	nid.uID=IDR_MAINFRAME; 
	nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ; 
	nid.uCallbackMessage=WM_SHOWTASK;	//自定义的消息名称 
	nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)); 
	wcscpy(nid.szTip,L"ScreenCap 1.0");		//信息提示条
	Shell_NotifyIcon(NIM_ADD,&nid);			//在托盘区添加图标 
	// 
	ShowWindow(SW_HIDE);					//隐藏主窗口
}

//热键消息处理函数
LRESULT CScreenCapDlg::OnHotKey(WPARAM wp, LPARAM lp)
{
	switch(wp)
	{
	case WM_HOTKEY:
		AfxMessageBox(L"HotKey Pressed");
	default:
		break;
	}
	return TRUE;
}


//计时器：程序启动时，将对话框最小化
void CScreenCapDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case TIMER_INIT_MIN:
		//ShowWindow(SW_MINIMIZE);
		KillTimer(TIMER_INIT_MIN);
		//AfxMessageBox(L"开始截屏");
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CScreenCapDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	//用整个桌面填充全屏对话框背景
	BITMAP bmp;
	CBitmap*  m_pBitmap;
	m_pBitmap->GetBitmap(&bmp);

	CDC dcCompatible;
	dcCompatible.CreateCompatibleDC(pDC);

	dcCompatible.SelectObject(m_pBitmap);

	CRect rect;
	GetClientRect(&rect);
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&dcCompatible,0,0,SRCCOPY);
	return CDialogEx::OnEraseBkgnd(pDC);
}
