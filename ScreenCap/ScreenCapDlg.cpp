
// ScreenCapDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ScreenCap.h"
#include "ScreenCapDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//��ȡָ��ͼƬ��ʽ�� CLSID
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

//������Ļ��ͼ
void SaveScreenToJpg()
{
	// �õ���ĻDC
	HWND hDeskWnd = ::GetDesktopWindow();       //�����Ļ��HWND. 
	CDC *pDestDC = CDC::FromHandle(::GetDC(hDeskWnd));    //��ȡ��ǰ������ĻDC
	int screenWidth = pDestDC->GetDeviceCaps(HORZRES);     //��Ļ��
	int screenHeight = pDestDC->GetDeviceCaps(VERTRES);     //��Ļ��
	// ��������Ļ���ݵ�Bitmap 
	CBitmap memBitmap;   
	memBitmap.CreateCompatibleBitmap(pDestDC, screenWidth, screenHeight);
	// ������Ļ���ڴ�DC
	CDC memDC;     
	memDC.CreateCompatibleDC(pDestDC);
	// ������Bitmap��ʽѡ���ڴ�DC
	memDC.SelectObject(&memBitmap);
	// ����ĻDC����copy���ڴ�DC
	memDC.BitBlt(0, 0, screenWidth, screenHeight, pDestDC, 0, 0, SRCCOPY);
	// ���λͼ��Ϣ
	BITMAP bmpInfo;
	memBitmap.GetBitmap(&bmpInfo);
	// ����BITMAPINFO
	BITMAPINFO m_BITMAPINFO;
	memset(&m_BITMAPINFO, 0, sizeof(BITMAPINFO));
	m_BITMAPINFO.bmiHeader.biSize     = sizeof(BITMAPINFOHEADER);
	m_BITMAPINFO.bmiHeader.biPlanes     = 1;
	m_BITMAPINFO.bmiHeader.biBitCount    = bmpInfo.bmBitsPixel;
	m_BITMAPINFO.bmiHeader.biCompression   = BI_RGB;
	m_BITMAPINFO.bmiHeader.biWidth     = bmpInfo.bmWidth;
	m_BITMAPINFO.bmiHeader.biHeight     = bmpInfo.bmHeight;
	// ��ȡλͼ����
	BYTE * pBuffer = new BYTE[bmpInfo.bmWidthBytes * bmpInfo.bmHeight];
	GetDIBits(memDC.m_hDC, (HBITMAP) memBitmap.m_hObject, 0, screenHeight, pBuffer,
		(LPBITMAPINFO) &m_BITMAPINFO, DIB_RGB_COLORS);
	// ����λͼ
	Bitmap *pSrcBmp = Bitmap::FromBITMAPINFO(&m_BITMAPINFO, (void*)pBuffer);
	// �����jpg�ļ�
	CLSID encoderClsid;
	GetEncoderClsid(L"image/jpeg", &encoderClsid);
	pSrcBmp->Save(L"destop.jpg", &encoderClsid);
	// �ͷ��ڴ�
	delete pSrcBmp;
	delete pBuffer;

	ReleaseDC(NULL,pDestDC->GetSafeHdc());
}

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CScreenCapDlg �Ի���




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
END_MESSAGE_MAP()


// CScreenCapDlg ��Ϣ�������

BOOL CScreenCapDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CScreenCapDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CScreenCapDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//�˵����½�����
void CScreenCapDlg::OnMenuNewCap()
{
	// TODO: Add your command handler code here
}


//�˵���
void CScreenCapDlg::OnMenuSaveCap()
{
	SaveScreenToJpg();
}
