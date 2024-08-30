#pragma once

#include "ui_talkwindowshell.h"
#include "basicwindow.h"
#include <QMap>
#include <qtcpsocket.h>
#include <qudpsocket.h>
class TalkWindow;
class TalkWindowItem;
class QListWidgetItem;
class EmotionWindow;

enum class GroupType {
	COMPANY = 0,
	PERSONGROUP = 1,
	DEVELOPGROUP = 2,
	MARKETGROUP = 3,
	PERSONTOPERSON = 4,
};
class TaikWindowShell : public BasicWindow
{
	Q_OBJECT

public:
	TaikWindowShell(QWidget *parent = nullptr);
	~TaikWindowShell();

public:
	//添加新的聊天窗口
	void AddTalkWindow(TalkWindow* talkWindow,TalkWindowItem* talkWindowItem, const QString& id);
	//设置当前聊天窗口
	void SetCurrentWidget(QWidget* widget);

	//获取列表
	const QMap<QListWidgetItem*, QWidget*>& GetTalkWindowItemMap() const;

private:
	void InitControl();		//初始化控件
	void InitTcpSocket();	//初始化Tcp
	void InitUdpSocket();	//初始化Udp
	void GetEmployeeID(QStringList& employee_list); //获取所有员工qq号
	bool CreateJSfile(QStringList& employee_list);	//创建JS文件
	void HandleReceiveMsg(int sender_employeeid,int msg_type,QString str_msg);
public slots:
	//表情按钮
	void OnEmotionBtnClicked(bool);
	// 客户端发送tcp 数据，数据类型，文件
	void UpdateSendTcpMsg(QString& str_data,int& msg_type,QString file_name ="");
private slots:
	//左侧列表点击执行的槽函数
	void OnTalkWindowItemClicked(QListWidgetItem* item);
	//表情窗口
	void OnEmotionItemClicked(int emotion_num);
	void ProcessPendingData();		//处理广播收到的数据
private:
	Ui::TaikWindowShellClass ui;
	//聊天窗口映射
	QMap<QListWidgetItem*, QWidget*> talk_window_item_map_;
	EmotionWindow* emotion_window_;

private:
	QTcpSocket* tcp_client_socket_; //客户端TCP
	QUdpSocket* udp_receiver_;	   //udp接收端
};
