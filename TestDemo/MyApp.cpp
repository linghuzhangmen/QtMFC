#include "stdafx.h"
#include "MyApp.h"
#include <QEventLoop>
#include <QAbstractEventDispatcher>
#include <QWidget>

#ifdef QTWINMIGRATE_WITHMFC
#include <afxwin.h>
#else
#include <qt_windows.h>
#endif

#ifdef QTWINMIGRATE_WITHMFC
CWinApp *CMyApp::mfc_app = 0;
char **CMyApp::mfc_argv = 0;
int CMyApp::mfc_argc = 0;
#endif

#if QT_VERSION >= 0x050000
#define QT_WA(unicode, ansi) unicode

QMfcAppEventFilter::QMfcAppEventFilter() : QAbstractNativeEventFilter()
{
}

bool QMfcAppEventFilter::nativeEventFilter(const QByteArray &, void *message, long *result)
{
	return static_cast<CMyApp*>(qApp)->winEventFilter((MSG*)message, result);
}
#endif

static int modalLoopCount = 0;

HHOOK hhook;

LRESULT CALLBACK QtFilterProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (qApp) 
	{
		// don't process deferred-deletes while in a modal loop
		if (modalLoopCount)
			qApp->sendPostedEvents();
		else
			qApp->sendPostedEvents(0, -1);
	}

	return CallNextHookEx(hhook, nCode, wParam, lParam);
}

void CMyApp::enterModalLoop()
{
	++modalLoopCount;
}

void CMyApp::exitModalLoop()
{
	--modalLoopCount;
	Q_ASSERT(modalLoopCount >= 0);
}

bool CMyApp::pluginInstance(Qt::HANDLE plugin)
{
	if (qApp)
		return FALSE;

	QT_WA({
		hhook = SetWindowsHookExW(WH_GETMESSAGE, QtFilterProc, 0, GetCurrentThreadId());
	}, {
		hhook = SetWindowsHookExA(WH_GETMESSAGE, QtFilterProc, 0, GetCurrentThreadId());
	});

	int argc = 0;
	(void)new QApplication(argc, 0);

	if (plugin) 
	{
		char filename[256];
		if (GetModuleFileNameA((HINSTANCE)plugin, filename, 255))
			LoadLibraryA(filename);
	}

	return TRUE;
}

#if QT_VERSION >= 0x050000
Q_GLOBAL_STATIC(QMfcAppEventFilter, qmfcEventFilter);
#endif

#ifdef QTWINMIGRATE_WITHMFC

int CMyApp::run(CWinApp *mfcApp)
{
	bool ownInstance = !qApp;
	if (ownInstance)
		instance(mfcApp);
	int result = qApp->exec();

	if (mfcApp) 
	{
		int mfcRes = mfcApp->ExitInstance();
		if (mfcRes && !result)
			result = mfcRes;
	}

	if (ownInstance)
		delete qApp;
	exit(1);
	return result;
}

QApplication *CMyApp::instance(CWinApp *mfcApp)
{
	mfc_app = mfcApp;
	if (mfc_app) 
	{
#if defined(UNICODE)
		QString exeName((QChar*)mfc_app->m_pszExeName, (int)wcslen(mfc_app->m_pszExeName));
		QString cmdLine((QChar*)mfc_app->m_lpCmdLine, (int)wcslen(mfc_app->m_lpCmdLine));
#else
		QString exeName = QString::fromLocal8Bit(mfc_app->m_pszExeName);
		QString cmdLine = QString::fromLocal8Bit(mfc_app->m_lpCmdLine);
#endif
		QStringList arglist = QString(exeName + " " + cmdLine).split(' ');

		mfc_argc = arglist.count();
		mfc_argv = new char*[mfc_argc + 1];
		int a;
		for (a = 0; a < mfc_argc; ++a)
		{
			QString arg = arglist[a];
			mfc_argv[a] = new char[arg.length() + 1];
			qstrcpy(mfc_argv[a], arg.toLocal8Bit().data());
		}
		mfc_argv[a] = 0;
	}

	return new CMyApp(mfcApp, mfc_argc, mfc_argv);
}

static bool qmfc_eventFilter(void *message)
{
	long result = 0;
	return static_cast<CMyApp*>(qApp)->winEventFilter((MSG*)message, &result);
}

CMyApp::CMyApp(CWinApp *mfcApp, int &argc, char **argv)
	: QApplication(argc, argv), idleCount(0), doIdle(FALSE)
{
	mfc_app = mfcApp;
#if QT_VERSION >= 0x050000
	QAbstractEventDispatcher::instance()->installNativeEventFilter(qmfcEventFilter());
#else
	QAbstractEventDispatcher::instance()->setEventFilter(qmfc_eventFilter);
#endif
	setQuitOnLastWindowClosed(true);
}
#endif

CMyApp::CMyApp(int &argc, char **argv) : QApplication(argc, argv)
{
#if QT_VERSION >= 0x050000
	QAbstractEventDispatcher::instance()->installNativeEventFilter(qmfcEventFilter());
#endif
}

CMyApp::~CMyApp()
{
	if (hhook) 
	{
		UnhookWindowsHookEx(hhook);
		hhook = 0;
	}

#ifdef QTWINMIGRATE_WITHMFC
	for (int a = 0; a < mfc_argc; ++a) 
	{
		char *arg = mfc_argv[a];
		delete[] arg;
	}

	delete[]mfc_argv;

	mfc_argc = 0;
	mfc_argv = 0;
	mfc_app = 0;
	
	HANDLE hself = GetCurrentProcess();
	TerminateProcess(hself, 0);
#endif
}

bool CMyApp::winEventFilter(MSG *msg, long *result)
{
	static bool recursion = false;
	if (recursion)
		return false;

	recursion = true;

	QWidget *widget = QWidget::find((WId)msg->hwnd);
	HWND toplevel = 0;
	if (widget) 
	{
		HWND parent = (HWND)widget->winId();
		while (parent) 
		{
			toplevel = parent;
			parent = GetParent(parent);
		}

		HMENU menu = toplevel ? GetMenu(toplevel) : 0;
		if (menu && GetFocus() == msg->hwnd) 
		{
			if (msg->message == WM_SYSKEYUP && msg->wParam == VK_MENU) 
			{
				// activate menubar on Alt-up and move focus away
				SetFocus(toplevel);
				SendMessage(toplevel, msg->message, msg->wParam, msg->lParam);
				widget->setFocus();
				recursion = false;
				return TRUE;
			}
			else if (msg->message == WM_SYSKEYDOWN && msg->wParam != VK_MENU) 
			{
				SendMessage(toplevel, msg->message, msg->wParam, msg->lParam);
				SendMessage(toplevel, WM_SYSKEYUP, VK_MENU, msg->lParam);
				recursion = false;
				return TRUE;
			}
		}
	}
#ifdef QTWINMIGRATE_WITHMFC
	else if (mfc_app)
	{
		MSG tmp;
		while (doIdle && !PeekMessage(&tmp, 0, 0, 0, PM_NOREMOVE)) 
		{
			if (!mfc_app->OnIdle(idleCount++))
				doIdle = FALSE;
		}

		if (mfc_app->IsIdleMessage(msg)) 
		{
			doIdle = TRUE;
			idleCount = 0;
		}
	}

	if (mfc_app && mfc_app->PreTranslateMessage(msg))
	{
		recursion = false;
		return TRUE;
	}
#endif

	recursion = false;
#if QT_VERSION < 0x050000
	return QApplication::winEventFilter(msg, result);
#else
	Q_UNUSED(result);
	return false;
#endif
}
