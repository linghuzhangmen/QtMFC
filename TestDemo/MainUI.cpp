#include "stdafx.h"
#include "MainUI.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>

CMainUI::CMainUI(QWidget *parent)
	: QFrame(parent)
{
	setWindowTitle(QString::fromLocal8Bit("MFC Qt混合编程，这是Qt窗口"));
	setFixedSize(800, 600);

	QPushButton *button1 = new QPushButton("One");
	QPushButton *button2 = new QPushButton("Two");
	QPushButton *button3 = new QPushButton("Three");
	QPushButton *button4 = new QPushButton("Four");
	QPushButton *button5 = new QPushButton("Five");

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(button1);
	layout->addWidget(button2);
	layout->addWidget(button3);
	layout->addWidget(button4);
	layout->addWidget(button5);

	setLayout(layout);
}

CMainUI::~CMainUI()
{
}
