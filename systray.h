#pragma once

#include <QSystemTrayIcon>
#include <QWidget>
class SysTray  : public QSystemTrayIcon
{
	Q_OBJECT

public:
	SysTray(QWidget *parent);
	~SysTray();

public slots:
	void OnIconActivated(QSystemTrayIcon::ActivationReason reason);
private:
	void InitSystemTray();
	void AdSystrayMenu();
private:
	QWidget* parent_;
};
