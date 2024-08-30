#include "systray.h"
#include "custommenu.h"
#include "basicwindow.h"
SysTray::SysTray(QWidget *parent)
	:parent_(parent),
	QSystemTrayIcon(parent)
{
	InitSystemTray();
	this->show(); 
}

SysTray::~SysTray()
{}

void SysTray::InitSystemTray(){
	this->setToolTip(QStringLiteral("QQ-骑牛科技"));
	this->setIcon(QIcon(":/Resources/MainWindow/app/logo.ico"));
	connect(this, &QSystemTrayIcon::activated, this, &SysTray::OnIconActivated);
}
//添加系统菜单
void SysTray::AdSystrayMenu(){
	//菜单的父窗口
	//菜单先找到 窗口的托盘？ 有托盘，那么直接加到托盘上
	CustomMenu* custom_menu = new CustomMenu(parent_);
	custom_menu->AddCustomMenu("onShow", ":/Resources/MainWindow/app/logo.ico",QStringLiteral("显示"));
	custom_menu->AddCustomMenu("onQuit", ":/Resources/MainWindow/app/page_close_btn_hover.png",QStringLiteral("退出"));

	connect(custom_menu->GetAction("onShow"), SIGNAL(triggered(bool)), parent_, SLOT(onShowNormal(bool)));
	connect(custom_menu->GetAction("onQuit"), SIGNAL(triggered(bool)), parent_, SLOT(onShowQuit(bool)));
	//当前坐标系的位置  ，在哪里执行？ ，当前坐标值位置
	custom_menu->exec(QCursor::pos());
	//custom_menu->exec();
	delete custom_menu;
	custom_menu = nullptr;
	//connect(custom_menu->GetAction("onShow"),&QAction::triggered,parent_,&BasicWindow::onShowNormal);
	//connect(custom_menu->GetAction("onQuit"),&QAction::triggered,parent_,&BasicWindow::onShowQuit);
	//connect(custom_menu->GetAction("onShow"), &QAction::triggered, parent_, &BasicWindow::onShowNormal);
	//connect(custom_menu->GetAction("onQuit"), &QAction::triggered, parent_, &BasicWindow::onShowQuit);
}

//托盘窗口触发
//注意父类对象
void SysTray::OnIconActivated(QSystemTrayIcon::ActivationReason reason) {
	if (reason == QSystemTrayIcon::Trigger) {
		parent_->show();
		parent_->activateWindow();
	}
	else if (reason == QSystemTrayIcon::Context) {
		AdSystrayMenu();
	}
}