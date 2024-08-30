#include "userlogin.h"
#include "CCMainWindow.h"
#include "qmessagebox.h"
#include <qdebug.h>
#include "qsqlquery.h"
QString login_account_id;//登录者QQ
UserLogin::UserLogin(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->initTitleBar();
	this->setTitleBarTitle("", ":/Resources/MainWindow/qqlogoclassic.png");
	this->loadStyleSheet("UserLogin");
	this->initControl();

}

UserLogin::~UserLogin()
{
}

void UserLogin::initControl()
{
	QLabel* headlabel = new QLabel(this);
	headlabel->setFixedSize(68, 68);
	QPixmap pix(":/Resources/MainWindow/head_mask.png");
	headlabel->setPixmap(getRoundImage(QPixmap(":/Resources/MainWindow/app/logo.ico"),pix,headlabel->size()));
	headlabel->move(width()/2 - 34,ui.titleWidget->height()-34);

	connect(ui.loginBtn,&QPushButton::clicked,this,&UserLogin::onLoginBtnClicked);


	if (!ConnectDatabase()) {
		QMessageBox::information(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("连接数据库失败"));
		this->close();
	}
	else{

	}
}

bool UserLogin::ConnectDatabase()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
	db.setDatabaseName("mysql"); //数据库名称  
	db.setHostName("localhost");
	db.setUserName("root");
	db.setPassword("123456");
	db.setPort(3306);

	if (db.open()) {
		return true;
	}
	else {
		return false;
	}
}

bool UserLogin::IdentifyAccount(bool& is_account_login, QString& str_account)
{
	QString str_account_input = ui.editUserAccount->text();
	QString str_password_input = ui.editPassword->text();
	//输入员工号（QQ号登录）
	QString str_sql_password = QString("SELECT password FROM qt_qq.tab_account WHERE employeeID = %1").arg(str_account_input);
	QSqlQuery query_employee_id(str_sql_password);
	//id 登录
	if (query_employee_id.first()) {
		// 0列 查询code QQ号对应的密码
		QString str_password = query_employee_id.value(0).toString();
		if (str_password == str_password_input) {
			login_account_id = str_account_input;
			is_account_login = false;
			str_account = str_account_input;
			return true;
		}
		else{
			return false;
		}
	}
	// 账号登录
	str_sql_password = QString("SELECT password,employeeID FROM qt_qq.tab_account WHERE account = '%1'").arg(str_account_input);
	QSqlQuery query_employee_account(str_sql_password);
	if (query_employee_account.first()) {
		QString str_password = query_employee_id.value(0).toString();
		if (str_password == str_password_input) {
			login_account_id = query_employee_id.value(1).toString();
			is_account_login = true;
			str_account = str_account_input;
			return true;
		}
	}
	else {
		return false;
	}
}

void UserLogin::onLoginBtnClicked() {
	bool is_account_login;
	QString str_account; //账号或QQ号
	if (!IdentifyAccount(is_account_login,str_account)) {
		QMessageBox::information(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("密码输入错误"));
		ui.editPassword->setText("");
		ui.editUserAccount->setText("");
		return;
	}
	this->close();
	CCMainWindow* mainwindow = new CCMainWindow(str_account,is_account_login);
	mainwindow->show();
}
