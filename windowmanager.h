#pragma once

#include <QObject>
#include "talkwindowshell.h"

class WindowManager  : public QObject
{
	Q_OBJECT

public:
	WindowManager();
	~WindowManager();

public:
	//通过指针地址转化为ID
	QWidget* FindWindowName(const QString& qs_window_name);
	void DeleteWindowName(const QString& qs_window_name);
	void AddWindowName(const QString& qs_window_name,QWidget* qWidget);
	static WindowManager* GetInstance();
	void AddNewTalkWindow(const QString& user_id);
	TaikWindowShell* GetTaikWindowShell();
	QString GetCreatingTalkId();
private:
	TaikWindowShell* talk_window_shell_;
	QMap<QString, QWidget*> talk_window_map_;
	QString str_creating_talk_id_ = "";  //正在创建的聊天窗口(QQ号)
};
