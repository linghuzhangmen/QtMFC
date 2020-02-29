/*

创建Qt主界面

*/

#pragma once
#include <QFrame>

class CMainUI : public QFrame
{
	Q_OBJECT

public:
	CMainUI(QWidget *parent = 0);
	~CMainUI();
};

