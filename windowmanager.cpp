#include "windowmanager.h"
#include "talkwindow.h"
#include "talkwindowitem.h"
#include "qsqlquery.h"
#include <QSqlQueryModel>
//单例模式，创建全局静态对象
Q_GLOBAL_STATIC(WindowManager,theInstance)

WindowManager::WindowManager()
	:QObject(nullptr),
	talk_window_shell_(nullptr)
{

}

WindowManager::~WindowManager()
{}

QWidget* WindowManager::FindWindowName(const QString& qs_window_name)
{
	if (talk_window_map_.contains(qs_window_name)) {
		return talk_window_map_.value(qs_window_name);
	}
	return nullptr;	
}

void WindowManager::DeleteWindowName(const QString& qs_window_name)
{
	talk_window_map_.remove(qs_window_name);
}

void WindowManager::AddWindowName(const QString& qs_window_name, QWidget* qWidget)
{
	if(!talk_window_map_.contains(qs_window_name)) {
		talk_window_map_.insert(qs_window_name, qWidget);
	}
}

WindowManager* WindowManager::GetInstance()
{
	return theInstance;
}

void WindowManager::AddNewTalkWindow(const QString& user_id)
{
	if (talk_window_shell_ == nullptr) {
		talk_window_shell_ = new TaikWindowShell;
		connect(talk_window_shell_, &TaikWindowShell::destroyed, [this](QObject* obj) {
			talk_window_shell_ = nullptr;
			});
	}
	//qq 号 还是 群聊ID
	QWidget* widget = FindWindowName(user_id);
	if (!widget) {
		str_creating_talk_id_ = user_id;
		TalkWindow* talk_window = new TalkWindow(talk_window_shell_,user_id);
		TalkWindowItem* talk_window_item = new TalkWindowItem(talk_window);
		str_creating_talk_id_ = "";
		//判断是群聊还是单聊
		QSqlQueryModel sql_dep_model;
		QString str_sql = QString("SELECT department_name,sign FROM qt_qq.tab_department WHERE departmentID = %1").arg(user_id);
		sql_dep_model.setQuery(str_sql);
		int rows = sql_dep_model.rowCount();
		QString str_window_name, str_msg_label;
		if (rows == 0) {//单聊
			QString sql = QString("SELECT employee_name,employee_sign FROM qt_qq.tab_employees WHERE employeeID = %1").arg(user_id);
			sql_dep_model.setQuery(sql);
		}
		QModelIndex index_dep_index, sign_index;
		index_dep_index = sql_dep_model.index(0, 0);  //0 行 0 列
		sign_index = sql_dep_model.index(0, 1);
		str_window_name = sql_dep_model.data(sign_index).toString();
		str_msg_label = sql_dep_model.data(index_dep_index).toString();


		talk_window->SetWindowName(str_window_name);  //窗口名称
		talk_window_item->SetMsgLabelContent(str_msg_label);//左侧联系人文本显示
		talk_window_shell_->AddTalkWindow(talk_window, talk_window_item,user_id);



		//std::string chat_flag = "SELECT * FROM tab_department WHERE departmentID = ";
		//chat_flag.append(user_id.toStdString());

		//std::string judgeflag = DataBaseConnect::getInstance().SQLQuery(chat_flag, "department_name");
		////单聊
		//if (judgeflag == "") {
		//	std::string single_sql = "SELECT * FROM tab_employees WHERE employeeID = ";
		//	single_sql.append(user_id.toStdString());
		//	std::string str_window_name = DataBaseConnect::getInstance().SQLQuery(single_sql, "employee_name");
		//	std::string str_window_sign = DataBaseConnect::getInstance().SQLQuery(single_sql, "employee_sign");
		//	//窗口名称 左侧联系人文本显示
		//	talk_window->SetWindowName(QString::fromLocal8Bit(str_window_name.c_str()));  
		//	talk_window_item->SetMsgLabelContent(QString::fromLocal8Bit(str_window_sign.c_str()));
		//	talk_window_shell_->AddTalkWindow(talk_window, talk_window_item, user_id);
		//}
		//else {
		////窗口名称 左侧联系人文本显示
		//talk_window->SetWindowName(QString::fromLocal8Bit(judgeflag.c_str()));
		//std::string chat_flag = "SELECT * FROM tab_department WHERE departmentID = ";
		//chat_flag.append(user_id.toStdString());
		//std::string signflag = DataBaseConnect::getInstance().SQLQuery(chat_flag, "sign");
		//talk_window_item->SetMsgLabelContent(QString::fromLocal8Bit(signflag.c_str()));
		//talk_window_shell_->AddTalkWindow(talk_window, talk_window_item, user_id);
		//}
		
	}
	else {
		//左侧聊天设为选中
		QListWidgetItem* item = talk_window_shell_->GetTalkWindowItemMap().key(widget);
		item->setSelected(true);
		//右侧聊天窗口
		talk_window_shell_->SetCurrentWidget(widget);   
	}

	talk_window_shell_->show();
	talk_window_shell_->activateWindow();
}

TaikWindowShell* WindowManager::GetTaikWindowShell()
{
	return talk_window_shell_;
}

QString WindowManager::GetCreatingTalkId()
{
	return str_creating_talk_id_;
}
