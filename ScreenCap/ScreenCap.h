
// ScreenCap.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CScreenCapApp:
// �йش����ʵ�֣������ ScreenCap.cpp
//

class CScreenCapApp : public CWinApp
{
public:
	CScreenCapApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	GdiplusStartupInput m_GdiplusStartupInput;
	ULONG_PTR m_GdiplusToken;
	virtual int ExitInstance();
};

extern CScreenCapApp theApp;