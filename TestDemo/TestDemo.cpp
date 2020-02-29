
// TestDemo.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "TestDemo.h"
#include "MainFrm.h"
#include "TestDemoDoc.h"
#include "TestDemoView.h"
#include <QtWidgets/QApplication>
#include <QDateTime>
#include "MyApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//自定义消息，MFC调用Qt界面
#define MainUI     WM_USER + 1000

//自定义消息，异常退出
#define MainExit   WM_USER + 1001


// CTestDemoApp

BEGIN_MESSAGE_MAP(CTestDemoApp, CWinAppEx)
	//ON_COMMAND(ID_APP_ABOUT, &CTestDemoApp::OnAppAbout)
	//// 基于文件的标准文档命令
	//ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	//// 标准打印设置命令
	//ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)

	ON_THREAD_MESSAGE(MainUI, onMainUI)
	ON_THREAD_MESSAGE(MainExit, onMainExit)
END_MESSAGE_MAP()


// CTestDemoApp 构造

CTestDemoApp::CTestDemoApp()
{
	// 支持重新启动管理器
//	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
//#ifdef _MANAGED
//	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则: 
//	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
//	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
//	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
//#endif
//
//	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
//	//为 CompanyName.ProductName.SubProduct.VersionInformation
//	SetAppID(_T("TestDemo.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CTestDemoApp 对象

CTestDemoApp theApp;


// CTestDemoApp 初始化

BOOL CTestDemoApp::InitInstance()
{
	CWinApp::InitInstance();

	//Qt窗口显示的消息
	PostMessage(NULL, MainUI, 0, 0);

	return TRUE;
}

int CTestDemoApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	//AfxOleTerm(FALSE);
	SAFE_DELETE(m_UI);

	return CWinAppEx::ExitInstance();
}

// CTestDemoApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CTestDemoApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CTestDemoApp 消息处理程序
void CTestDemoApp::onMainUI(WPARAM wParam, LPARAM lParam)
{
	// 设置文字显示位置
	Qt::Alignment topRight = Qt::AlignBottom | Qt::AlignRight;
	QString startPng = qApp->applicationDirPath() + "\\start.png";

	//设置启动画面图片
	QSplashScreen *m_AppSplash = new QSplashScreen(QPixmap(startPng));
	m_AppSplash->show();

	//显示文字示例
	m_AppSplash->showMessage(QString::fromLocal8Bit("程序启动中，大概3秒"), topRight, Qt::white);

	((QApplication *)QApplication::instance())->processEvents();
	QDateTime n = QDateTime::currentDateTime();
	QDateTime now;

	//设置延时，或者不设置，当主程序加载完显示后，启动画面也会消失
	do
	{
		now = QDateTime::currentDateTime();
	} 
	while (n.secsTo(now) <= 3);  //3为需要延时的秒数

	m_UI = new CMainUI();
	m_UI->show();   //主界面显示

	m_AppSplash->finish(m_UI); //当mainwindow显示了自己就消失
	delete m_AppSplash;      //释放资源
	m_AppSplash = NULL;
}

void CTestDemoApp::onMainExit(WPARAM wParam, LPARAM lParam)
{

}

//截断MFC消息循环，进入Qt消息循环
BOOL CTestDemoApp::Run()
{
	return CMyApp::run(this);
}

