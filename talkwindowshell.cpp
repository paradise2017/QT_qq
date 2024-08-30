#include "talkwindowshell.h"
#include "commonUtils.h"
#include "emotionwindow.h"
#include "QListWidget"
#include "talkwindow.h"
#include "talkwindowitem.h"
#include <qsqlquery.h>
#include <qsqlquerymodel.h>
#include "qmessagebox.h"
#include "windowmanager.h"
#include <qfile.h>
const int global_tcp_port = 6666;
const int global_udp_port = 8888;
extern QString login_account_id;
TaikWindowShell::TaikWindowShell(QWidget*parent)
	:BasicWindow(parent)
{
	ui.setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	InitControl();
	InitTcpSocket();
	InitUdpSocket();
	QFile file(":/Resources/MainWindow/MsgHtml/msgtmpl.js");
	if (!file.size()) {
		QStringList employeeid_list;
		GetEmployeeID(employeeid_list);
		if (!CreateJSfile(employeeid_list)) {
			QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("js文件写入数据失败"));
		}
	}

}

TaikWindowShell::~TaikWindowShell()
{
	delete emotion_window_;
	emotion_window_ = nullptr;
}

void TaikWindowShell::AddTalkWindow(TalkWindow* talkWindow, TalkWindowItem* talkWindowItem,const QString& id)
{
	ui.rightStackedWidget->addWidget(talkWindow); 
	connect(emotion_window_, SIGNAL(SignalEmotionWindowHide()),
		talkWindow, SLOT(OnSetEmotionBtnStatus()));


	QListWidgetItem* item = new QListWidgetItem(ui.listWidget);
	talk_window_item_map_.insert(item,talkWindow);
	
	item->setSelected(true);
	//设置头像
	//判断群聊还是单聊
	QSqlQueryModel sql_dep_model;
	QString str_query = QString("SELECT picture FROM qt_qq.tab_department WHERE departmentID = %1").arg(id);
	sql_dep_model.setQuery(str_query);
	int row = sql_dep_model.rowCount();
	if (row == 0) {//单聊
		str_query = QString("SELECT picture FROM qt_qq.tab_employees WHERE employeeID = %1").arg(id);
		sql_dep_model.setQuery(str_query);
	}
	QModelIndex index;
	index = sql_dep_model.index(0, 0);

	QImage img;
	img.load(sql_dep_model.data(index).toString());
	
	talkWindowItem->SetHeadPixmap(QPixmap::fromImage(img));

	ui.listWidget->addItem(item);
	ui.listWidget->setItemWidget(item, talkWindowItem);
	//设置被点击
	OnTalkWindowItemClicked(item);
	//关闭释放资源
	connect(talkWindowItem, &TalkWindowItem::SignalCloseClicked, [talkWindowItem,talkWindow,item,this]() {
		talk_window_item_map_.remove(item);
		talkWindow->close();
		ui.listWidget->takeItem(ui.listWidget->row(item));
		delete talkWindowItem;
		ui.rightStackedWidget->removeWidget(talkWindow);
		if (ui.rightStackedWidget->count() < 1) {
			this->close();
		}
		});
}

void TaikWindowShell::SetCurrentWidget(QWidget* widget)
{
	ui.rightStackedWidget->setCurrentWidget(widget);
}

const QMap<QListWidgetItem*, QWidget*>& TaikWindowShell::GetTalkWindowItemMap() const
{
	return talk_window_item_map_;
}

void TaikWindowShell::InitControl()
{
	this->loadStyleSheet("TalkWindow");
	this->setWindowTitle(QString::fromLocal8Bit("奇奇-聊天窗口"));
	emotion_window_ = new EmotionWindow();
	//隐藏表情窗口
	emotion_window_->hide();
	QList<int> leftWindgetSize;
	leftWindgetSize << 154 << this->width() - 154;
	//分类器设置尺寸
	ui.splitter->setSizes(leftWindgetSize);
	ui.listWidget->setStyle(new CustomProxyStyle(this));

	connect(ui.listWidget,&QListWidget::itemClicked,this,&TaikWindowShell::OnTalkWindowItemClicked);
	connect(emotion_window_, SIGNAL(SignalEmotionItemClicked(int)),this,SLOT(OnEmotionItemClicked(int)));

}
void TaikWindowShell::InitTcpSocket(){
	tcp_client_socket_ = new QTcpSocket(this);
	// ip 端口号
	tcp_client_socket_->connectToHost("127.0.0.1",global_tcp_port);
}
void TaikWindowShell::InitUdpSocket(){
	udp_receiver_ = new QUdpSocket(this);
	for (qint16 port = global_udp_port; port < global_udp_port + 200; ++port) {
		if (udp_receiver_->bind(port,QUdpSocket::ShareAddress)) {
			break;
		}
	}
	connect(udp_receiver_,&QUdpSocket::readyRead,this,&TaikWindowShell::ProcessPendingData);
}
void TaikWindowShell::GetEmployeeID(QStringList& employee_list){
	QSqlQueryModel query_model;
	query_model.setQuery("SELECT employeeID FROM qt_qq.tab_employees WHERE status = 1");
	//返回模型的总行数（员工的总数）
	int employee_num = query_model.rowCount();
	QModelIndex index;
	for (int i = 0; i < employee_num; i++) {
		index = query_model.index(i, 0); //行 ； 列
		employee_list << query_model.data(index).toString();
	}
}
bool TaikWindowShell::CreateJSfile(QStringList& employee_list){
	//读取文件数据 txt
	QString str_file_txt = "Resources/MainWindow/MsgHtml/msgtmpl.txt";
	QFile file_read(str_file_txt);
	QString str_file;
	if (file_read.open(QIODevice::ReadOnly)) {
		str_file = file_read.readAll();
		file_read.close();
	}
	else {
		QMessageBox::information(this,QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("读取 msgtmpl.txt 失败"));
	}
	//替换（external0,appendhtml0） 用作发信息使用
	QFile file_write("Resources/MainWindow/MsgHtml/msgtmpl.js");
	if (file_write.open(QIODevice::WriteOnly| QIODevice::Truncate)) {
		//更新空值
		QString str_source_init_null = "var external = null;";
		//更新初始化
		QString str_source_init = "external = channel.objects.external;";
		//更新new QWebChannel
		QString str_source_new =
			"new QWebChannel(qt.webChannelTransport,\
			function(channel) {\
			external = channel.objects.external;\
		}\
		); \
		";
		//更新追加recvHtml,脚本中有双引号 无法直接构造
		QString str_source_recv_html;
		QFile file_recv_html("Resources/MainWindow/MsgHtml/recvHtml.txt");
		if (file_recv_html.open(QIODevice::ReadOnly)) {
			str_source_recv_html = file_recv_html.readAll();
			file_recv_html.close();
		}
		else {
			QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("读取 recvHtml.txt 失败")); 
			return false;
		}
		//保存替换后的脚本
		QString str_replace_init_null;
		QString str_replace_init;
		QString str_replace_new;
		QString str_replace_recv_html;
		
		for (int i = 0; i < employee_list.length();i++) {
			//编辑替换后的空值
			QString str_init_null = str_source_init_null;
			str_init_null.replace("external",QString("external_%1").arg(employee_list.at(i)));
			str_replace_init_null += str_init_null;
			str_replace_init_null += "\n";
			//编辑替换后的初始值
			QString str_init = str_source_init;
			str_init.replace("external", QString("external_%1").arg(employee_list.at(i)));
			str_replace_init += str_init;
			str_replace_init += "\n";

			//编辑替换后的new QWebChannel
			QString str_new = str_source_new;
			str_new.replace("external", QString("external_%1").arg(employee_list.at(i)));
			str_replace_new += str_new; 
			str_replace_new += "\n";
			//编辑替换后 recvHtml
			QString str_recv_html = str_source_recv_html;
			str_recv_html.replace("external", QString("external_%1").arg(employee_list.at(i)));
			str_recv_html.replace("recvHtml",QString("recvHtml_%1").arg(employee_list.at(i)));
			str_replace_recv_html += str_recv_html;
			str_replace_recv_html += "\n";
		}
		str_file.replace(str_source_init_null,str_replace_init_null);
		str_file.replace(str_source_init,str_replace_init);
		str_file.replace(str_source_new,str_replace_new);
		str_file.replace(str_source_recv_html,str_replace_recv_html);
		
		QTextStream stream(&file_write);
		stream << str_file;
		file_write.close();
		return true;
	}
	else {
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("写 msgtemp.js 失败"));
		return false;
	}
}
void TaikWindowShell::HandleReceiveMsg(int sender_employeeid, int msg_type, QString str_msg){
	QmsgTextEdit msg_text_edit;
	msg_text_edit.setText(str_msg);
	if (msg_type == 1) { //文本信息
		msg_text_edit.document()->toHtml();
	}
	else if(msg_type == 0){ //表情信息
		const int emotion_width = 3;
		//表情总数
		int emotion_num = str_msg.length() / emotion_width;

		for (int i = 0; i < emotion_num; i++){
			msg_text_edit.AddEmotionUrl(str_msg.mid(i * emotion_width, emotion_width).toInt());
		} 
	}
	QString html = msg_text_edit.document()->toHtml();
	//文本HTML 如果没有添加字体则添加字体   没有字体 字号
	if (!html.contains(".png") && !html.contains("</span>")) {
		QString font_html;
		QFile file(":/Resources/MainWindow/MsgHtml/msgFont.txt");
		if (file.open(QIODevice::ReadOnly)) {
			font_html = file.readAll();
			font_html.replace("%1", str_msg);
			file.close();
		}
		else {
			QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("文件msgFont.txt不存在"));
			return;
		}

		if (!html.contains(font_html)) {
			html.replace(str_msg, font_html);
		}
	}
	TalkWindow* talk_window =  dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());
	talk_window->ui.msgWidget->AppendMsg(html,QString::number(sender_employeeid));
}
void TaikWindowShell::UpdateSendTcpMsg(QString& str_data, int& msg_type, QString file_name){
	//文本数据包的格式：群聊标志，发信息员工QQ号 +收信息员工QQ号 + 信息类型 + 数据长度 + 数据（hello
	//表情数据表格式：群聊标志+发信息员工QQ号+收信息员工QQ号 + 信息类型 + 表情个数 image + 数据
	//msg_type   0表情 1文本信息 2文件信息

	//当前活动窗口
	TalkWindow* cur_talk_window = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());
	QString talk_id = cur_talk_window->GetTalkId();
	//群聊标志
	QString str_group_flag;  
	QString str_send;
	if (talk_id.length() == 4) {//群QQ号长度
		str_group_flag = "1";
	}
	else {
		str_group_flag = "0";
	}
	int origin_str_data_length = str_data.length();
	int data_length = QString::number(origin_str_data_length).length();  //"10" 2
	QString str_data_length;
	if (msg_type == 1) {  //发送文本信息
		//文本信息的长度约定为5位
		if (data_length == 1) {//
			str_data_length = "0000" + QString::number(origin_str_data_length);
		}
		else if (data_length == 2) {
			str_data_length = "000" + QString::number(origin_str_data_length);
		}
		else if (data_length == 3) {
			str_data_length = "00" + QString::number(origin_str_data_length);
		}
		else if (data_length == 4) {
			str_data_length = "0" + QString::number(origin_str_data_length);
		}
		else if (data_length == 5) {
			str_data_length = QString::number(origin_str_data_length);
		}
		else {
			QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("不合理的数据长度"));
		}

		str_send = str_group_flag + login_account_id + talk_id + "1" + str_data_length + str_data;
	}
	else if(msg_type == 0){		//表情信息
		//表情数据包格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号(群QQ号) 
		//+ 信息类型 + 表情个数 + images + 数据
		str_send = str_group_flag + login_account_id + talk_id + "0" + str_data;
	}
	else if (msg_type == 2) { //文件信息	
		//文件数据表格式： 群聊标志 +  发信息员工QQ号 + 收信息员工QQ号（群QQ号） + 信息类型（2）+ 文件长度 + "bytes"+ 文件名称 + "data_begin"+ 文件内容 
		//Qbytearray
		QByteArray bt = str_data.toUtf8();
		QString str_length = QString::number(bt.length());
		str_send = str_group_flag + login_account_id + talk_id + "2" + str_length + "bytes" + file_name + "data_begin" + str_data;
	}
	QByteArray data_bt;
	data_bt.resize(str_send.length());
	data_bt = str_send.toUtf8();
	//客户端向socket 写入数据
	tcp_client_socket_->write(data_bt);
}
void TaikWindowShell::OnEmotionBtnClicked(bool) {
	emotion_window_->setVisible(!emotion_window_->isVisible());
	//将当前控件的相对位置转化为屏幕的绝对位置
	QPoint emotion_Point = this->mapToGlobal(QPoint(0, 0));
	emotion_Point.setX(emotion_Point.x() + 170);
	emotion_Point.setY(emotion_Point.y() + 220);
	emotion_window_->move(emotion_Point);
}

void TaikWindowShell::OnEmotionItemClicked(int emotion_num)
{
	TalkWindow* current_talk_window = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());
	if (current_talk_window) {
		current_talk_window->AddEmotionImage(emotion_num);
	}
}
//通信协议
//数据包格式：群聊标识 + 发信息员工QQ号 + 收信息员工QQ号（群QQ号） + 信息类型（1） + 数据长度 + 文本数据
//表情数据包格式： 群聊标识+ 发信息员工QQ号+ 收信息员工QQ号 + 信息类型（0）+ 表情个数 + image + 表情名称
//文件数据包格式： 群聊标识+ 发信息员工QQ号+ 收信息员工QQ号 + 信息类型（2）+ 文件字节数 +byte + 文件名 + data_begin+ 文件数据

//群聊标识1 位  0单聊1群聊  信息类型1位 0表情信息 1文本信息 2 文件信息
//QQ号站5位  群QQ号站4位   数据长度占5位 表情名称占3位 （群聊标识为1） 数据包没有收信息员工QQ号而是 信息群QQ号	
//群聊表示为0  数据包没有收信息群QQ号而是 收信息员工QQ号
// 群聊文本信息如： 1100012001100005Hello   QQ10001向群2001发送文本信息hello 5
// 单聊图片信息如： 0100011000201imgaes060		//QQ 10001 向 QQ10002发送1个表情60.png
// 群聊文件信息： 1100052000210bytestest.txtdata_beginhelloworld   QQ 10005 向群 2000发送文件信息文件test.txt 文件内容长度10，内容helloworld

//群聊文件解析			 1 10001 2001 1 00005 hello
//单聊图片信息解析		 0 10001 10002 0 060
//群聊文件信息			 1 10005 2000 2 10 bytes test.txt data_begin helloworld
void TaikWindowShell::ProcessPendingData(){
	//端口中有未处理的数据

	while (udp_receiver_->hasPendingDatagrams()) {

		const static int group_flag_width = 1;		//群聊标志站位
		const static int group_width = 4;		    //群QQ号宽度
		const static int employee_width = 5;		//员工QQ号宽度
		const static int msgtype_width = 1;			//信息类型宽度
		const static int msg_length_width = 5;	    //文本信息长度的宽度
		const static int picture_width = 3;        //表情图片的宽度
		//读取UDP数据
		QByteArray bt_data;
		bt_data.resize(udp_receiver_->pendingDatagramSize());
		udp_receiver_->readDatagram(bt_data.data(),bt_data.size());
		QString str_data = bt_data.data();
		QString str_window_id; //聊天窗口ID，群聊是群号，单聊是员工QQ号
		QString str_send_employee_id, str_receive_employeeid;	//发送及接收端的QQ号
		QString str_msg;		//数据
		int msg_len;			//数据长度
		int msg_type;			//数据类型
		
		str_send_employee_id = str_data.mid(group_flag_width,employee_width);
		//自己发的信息不做处理
		if (str_send_employee_id == login_account_id) {
			return;
		}
		if (bt_data[0] == '1') {  //群聊
			//群QQ号
			str_window_id = str_data.mid(group_flag_width+employee_width,group_width);
			QChar char_msg_type = bt_data[group_flag_width + employee_width + group_width];
			if (char_msg_type == '1') {  //文本信息
				msg_type = 1;
				msg_len = str_data.mid(group_flag_width + employee_width +group_width + msgtype_width,msg_length_width).toInt();
				str_msg = str_data.mid(group_flag_width+employee_width+group_width+msg_type+msg_length_width,msg_len);
			}
			else if (char_msg_type == '0') { //表情信息
				msg_type = 0;
				int pos_images = str_data.indexOf("images");
				str_msg = str_data.right(str_data.length() - pos_images - QString("images").length());
			}
			else if (char_msg_type == '2') { //文件信息
				msg_type = 2;
				int bytes_width = QString("bytes").length();
				int pos_bytes = str_data.indexOf("bytes");
				int pos_data_begin = str_data.indexOf("data_begin");
				//文件名称
				QString file_name = str_data.mid(pos_bytes+bytes_width,pos_data_begin-pos_bytes-bytes_width);
				//文件内容
				int data_length_width;
				int pos_data = pos_data_begin + QString("data_begin").length();
				str_msg = str_data.mid(pos_data);
				//根据 employeeID 获取发送者姓名
				QString sender;
				int employeeID = str_send_employee_id.toInt();
				QSqlQuery query_group_name(QString("SELECT employee_name FROM qt_qq.tab_employees WHERE employeeID = %1").arg(employeeID));
				if (query_group_name.first()) {
					sender = query_group_name.value(0).toString();
				}
				//接受文件的后续操作。。。
			} 
		}
		else {  //单聊
			str_receive_employeeid = str_data.mid(group_flag_width + employee_width,employee_width);
			str_window_id = str_send_employee_id;
			//获取信息的类型
			QChar char_msg_type = bt_data[group_flag_width + employee_width + employee_width];
			if (char_msg_type =='1') {	//文本信息
				msg_type = 1;				//发送者QQ号宽度 接受者QQ号宽度
				msg_len = str_data.mid(group_flag_width+employee_width+employee_width + msgtype_width,msg_length_width).toInt();//文本信息长度
				//文本信息
				str_msg = str_data.mid(group_flag_width + employee_width + employee_width + msgtype_width + msg_length_width,msg_len);

			}
			else if(char_msg_type == '0'){  //表情信息
				msg_type = 0;
				int pos_image = str_data.indexOf("images");
				int images_width = QString("images").length();
				str_msg = str_data.mid(pos_image + images_width);
			}
			else if (char_msg_type == '2') {  //文件信息
				msg_type = 2;
				int bytes_width = QString("bytes").length();
				int pos_bytes = str_data.indexOf("bytes");
				int data_begin_width = QString("data_begin").length();
				int pos_data_begin = str_data.indexOf("data_begin");
				//文件名称
				QString file_name = str_data.mid(pos_bytes + bytes_width, pos_data_begin - pos_bytes - bytes_width);
				//文件内容
				str_msg = str_data.mid(pos_data_begin + data_begin_width);
			}
		}
		//将聊天窗口设为活动的窗口
		QWidget* widget = WindowManager::GetInstance()->FindWindowName(str_window_id);
		if (widget) {
			this->SetCurrentWidget(widget);
			//同步左侧聊天窗口
			QListWidgetItem* item = talk_window_item_map_.key(widget);
			item->setSelected(true);
		}
		else {
			return;
		}
		//文件信息另做处理
		if (msg_type != 2) {
			int send_employee_id = str_send_employee_id.toInt();
			HandleReceiveMsg(send_employee_id, msg_type, str_msg);
		}
	}

}

void TaikWindowShell::OnTalkWindowItemClicked(QListWidgetItem* item) {
	QWidget* talk_window_widget = talk_window_item_map_.find(item).value();
	ui.rightStackedWidget->setCurrentWidget(talk_window_widget);
}