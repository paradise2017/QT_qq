#pragma once

#include <QObject>
#include "TalkWindowShell.h"


class WindowManager : public QObject
{
	Q_OBJECT

public:
	WindowManager();
	~WindowManager();

public:
	QWidget* findWindowName(const QString& qsWindowName);
	void deleteWindowName(const QString& qsWindowName);
	void addWindowName(const QString& qsWindowName, QWidget* qWidget);
	
	static WindowManager* getInstance();
	TalkWindowShell* getTalkWindowShell();
	QString getCreatingTalkId();
	void addNewTalkWindow(const QString&uid);

private:
	TalkWindowShell* m_talkwindowshell;
	QMap<QString, QWidget*> m_windowMap;
	QString m_strCreatingTalkId = "";//正在创建的聊天窗口（QQ号）
};
