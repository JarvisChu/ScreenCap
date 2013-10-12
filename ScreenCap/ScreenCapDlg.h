
// ScreenCapDlg.h : ͷ�ļ�
//

#pragma once


// CScreenCapDlg �Ի���
class CScreenCapDlg : public CDialogEx
{
// ����
public:
	CScreenCapDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SCREENCAP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	afx_msg LRESULT OnHotKey(WPARAM wp, LPARAM lp);  //�ȼ���Ϣӳ�亯��
	afx_msg void OnClose();
	void HideToTask(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
