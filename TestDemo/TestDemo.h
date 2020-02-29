
// TestDemo.h : TestDemo 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include <QSplashScreen>
#include "MainUI.h"


// CTestDemoApp:
// 有关此类的实现，请参阅 TestDemo.cpp
//

class CTestDemoApp : public CWinAppEx
{
public:
	CTestDemoApp();

	//重写Run方法，截断MFC消息流程
	virtual BOOL Run();

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

	afx_msg void onMainUI(WPARAM wParam, LPARAM lParam);
	afx_msg void onMainExit(WPARAM wParam, LPARAM lParam);

private:
	QSplashScreen *m_SplashScreen;
	CMainUI *m_UI;
};

extern CTestDemoApp theApp;
