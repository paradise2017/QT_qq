#pragma once

#include <QWidget>
#include "TalkWindowShell.h"
#include "ui_TalkWindow.h"

class TalkWindow : public QWidget
{
	Q_OBJECT

public:
	TalkWindow(QWidget *parent,const QString& uid);
	~TalkWindow();

public:
	void addEmotionImage(int emotionNum);
	void setWindowName(const QString& name);
	QString getTalkId();

private slots:
	void onFileOpenBtnClicked(bool);
	void onSendBtnClicked(bool);
	void onItemDoubleClicked(QTreeWidgetItem* item, int column);

private:
	void initControl();
	void initGroupTalkStatus();
	int getCompDepID();

	void initTalkWindow();		//初始化群聊
	void initPtoPTalk();		//初始化单聊
	void addPeopInfo(QTreeWidgetItem* pRootGroupItem,int employeeID);

private:
	Ui::TalkWindow ui;
	QString m_talkId;
	bool m_isGroupTalk;	//是否为群聊
	QMap<QTreeWidgetItem*, QString> m_groupPeopleMap;//所有分组联系人姓名
	friend class TalkWindowShell;
};
