#pragma once
#include "basicwindow.h"
#include "ui_userlogin.h"
#include <qsqldatabase.h>
class UserLogin : public BasicWindow
{
	Q_OBJECT

public:
	UserLogin(QWidget *parent = nullptr);
	~UserLogin();
private slots:
	void onLoginBtnClicked();
private:
	void initControl();
	bool ConnectDatabase();
	bool IdentifyAccount(bool& is_account_login,QString& str_account);
private:
	Ui::UserLogin ui;
};
