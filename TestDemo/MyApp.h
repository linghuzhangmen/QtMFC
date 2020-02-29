#pragma once
#include <QApplication>

#define QTWINMIGRATE_WITHMFC
class CWinApp;

#if defined(Q_OS_WIN)
#  if !defined(QT_QTWINMIGRATE_EXPORT) && !defined(QT_QTWINMIGRATE_IMPORT)
#    define QT_QTWINMIGRATE_EXPORT
#  elif defined(QT_QTWINMIGRATE_IMPORT)
#    if defined(QT_QTWINMIGRATE_EXPORT)
#      undef QT_QTWINMIGRATE_EXPORT
#    endif
#    define QT_QTWINMIGRATE_EXPORT __declspec(dllimport)
#  elif defined(QT_QTWINMIGRATE_EXPORT)
#    undef QT_QTWINMIGRATE_EXPORT
#    define QT_QTWINMIGRATE_EXPORT __declspec(dllexport)
#  endif
#else
#  define QT_QTWINMIGRATE_EXPORT
#endif

#if QT_VERSION >= 0x050000
#include <QAbstractNativeEventFilter>

class QT_QTWINMIGRATE_EXPORT QMfcAppEventFilter : public QAbstractNativeEventFilter
{
public:
	QMfcAppEventFilter();
	bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
};
#endif

class CMyApp : public QApplication
{
	Q_OBJECT

public:
	static bool pluginInstance(Qt::HANDLE plugin = 0);

#ifdef QTWINMIGRATE_WITHMFC
	static int run(CWinApp *mfcApp);
	static QApplication *instance(CWinApp *mfcApp);
	CMyApp(CWinApp *mfcApp, int &argc, char **argv);
#endif

public:
	CMyApp(int &argc, char **argv);
	~CMyApp();

	bool winEventFilter(MSG *msg, long *result);

	void enterModalLoop();
	void exitModalLoop();

private:
#ifdef QTWINMIGRATE_WITHMFC
	static char ** mfc_argv;
	static int mfc_argc;
	static CWinApp *mfc_app;
#endif

	int idleCount;
	bool doIdle;
};

