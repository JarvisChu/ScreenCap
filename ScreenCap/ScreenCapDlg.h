
// ScreenCapDlg.h : 头文件
//

#pragma once


// CScreenCapDlg 对话框
class CScreenCapDlg : public CDialogEx
{
// 构造
public:
	CScreenCapDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SCREENCAP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMenuNewCap();
	afx_msg void OnMenuSaveCap();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnShowTask(WPARAM wParam,LPARAM lParam); 
	afx_msg LRESULT OnHotKey(WPARAM wp, LPARAM lp);  //热键消息映射函数
	afx_msg void OnClose();
	void HideToTask(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
