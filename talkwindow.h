#pragma once

#include <QWidget>
#include "ui_talkwindow.h"
#include "talkwindowshell.h"
class TalkWindow : public QWidget
{
	Q_OBJECT

public:
	TalkWindow(QWidget *parent,const QString& user_id);
	~TalkWindow();

public:
	void AddEmotionImage(int emotion_num);
	void SetWindowName(const QString& name);
	void InitControl();
	void InitGroupTalkStatus();

	//初始化群聊
	void InitTalkWindow();
	//初始化单聊
	void InitPtoTalk();
	//公司群添加人员
	void AddPeopleInfo(QTreeWidgetItem* p_root_item,int employ_id);
	//获取公司部门的QQ号
	int GetCompanyId();
	QString GetTalkId();

private slots:
	void OnSendBtnClicked(bool flag);
	void OnItemDoubleClicked(QTreeWidgetItem* item, int col);
private:
	Ui::TalkWindowClass ui;
	QString talk_id_;
	bool is_group_talk_; //是否群聊
	QMap<QTreeWidgetItem*, QString> group_people_map_;//所有分组联系人姓名
	friend class TaikWindowShell;
};
