#include "msgwebview.h"
#include "qfile.h"
#include "qmessagebox.h"
#include "qjsonobject.h"
#include "qjsondocument.h"
#include "qwebchannel.h"
#include "talkwindow.h"
#include "windowmanager.h"
#include <qsqlquerymodel.h>
extern QString global_str_login_head_path;
MsgWebView::MsgWebView(QWidget *parent)
	: QWebEngineView(parent),
	channel_(new QWebChannel(this))
{
	//构造网页
	MsgWebPage* page = new MsgWebPage(this);
	this->setPage(page);

	msg_html_obj_ = new MsgHtmlObject(this);
	channel_->registerObject("external0", msg_html_obj_);
	

	TaikWindowShell* talk_window_shell = WindowManager::GetInstance()->GetTaikWindowShell();
	connect(this,&MsgWebView::SignalSendMsg,talk_window_shell,&TaikWindowShell::UpdateSendTcpMsg);
	//当前正在构建的聊天窗口ID（QQ号）
	QString str_talk_id = WindowManager::GetInstance()->GetCreatingTalkId();
	
	//获取公司群ID
	QSqlQueryModel query_employee_model;
	QString str_employee_id, str_picture_path;
	QString str_external;
	bool is_group_talk = false;
	//获取公司群ID
	query_employee_model.setQuery(QString("SELECT departmentID FROM qt_qq.tab_department WHERE department_name = '%1'").arg(QStringLiteral("公司群")));
	QModelIndex company_index = query_employee_model.index(0, 0);
	QString str_company_id = query_employee_model.data(company_index).toString();
	if (str_talk_id == str_company_id) {  //公司群聊
		is_group_talk = true;
		query_employee_model.setQuery("SELECT employeeID,picture FROM qt_qq.tab_employees WHERE status = 1");
	}
	else {
		if (str_talk_id.length() == 4) { //其他群聊
			is_group_talk = true;
			query_employee_model.setQuery(QString("SELECT employeeID,picture FROM qt_qq.tab_employees WHERE status = 1 AND departmentID = %1").arg(str_talk_id));
		}
		else {							  //单独聊天	
			query_employee_model.setQuery(QString("SELECT picture FROM qt_qq.tab_employees WHERE status = 1 AND employeeID = %1").arg(str_talk_id));
			QModelIndex index = query_employee_model.index(0,0);
			str_picture_path = query_employee_model.data(index).toString();
			str_external = "external_" + str_talk_id;
			MsgHtmlObject* msghtml_obj = new MsgHtmlObject(this,str_picture_path);
			channel_->registerObject(str_external,msghtml_obj);
		}
	}
	if (is_group_talk) {
		QModelIndex employee_model_index, picture_model_index;
		int rows = query_employee_model.rowCount();
		for (int i = 0; i < rows; i++){
			employee_model_index = query_employee_model.index(i, 0);
			picture_model_index = query_employee_model.index(i, 1);
			str_employee_id = query_employee_model.data(employee_model_index).toString();
			str_picture_path = query_employee_model.data(picture_model_index).toString();
			str_external = "external_" + str_employee_id;
			MsgHtmlObject* msghtml_obj = new MsgHtmlObject(this, str_picture_path);
			channel_->registerObject(str_external, msghtml_obj);
		}
	}


	this->page()->setWebChannel(channel_);
	//初始化 收信息网页
	this->load(QUrl("qrc:/Resources/MainWindow/MsgHtml/msgTmpl.html"));
}

void MsgWebView::AppendMsg(const QString& html, QString str_obj)
{
	QJsonObject msg_obj;
	QString qs_msg;
	//解析html
	const QList<QStringList> msg_list = ParseHtml(html);
	int image_num = 0;
	int msg_type = 1;  //信息类型，0 表情信息， 1 文本信息， 2 文件
	bool is_image_msg = false;
	QString str_data; //055 008

	for (size_t i = 0; i < msg_list.size(); i++)
	{
		if (msg_list.at(i).at(0) == "img") {
			QString imagePath = msg_list.at(i).at(1);
			QPixmap pixmap;
			//获取表情名称的位置
			QString str_emotion_path = "qrc:/Resources/MainWindow/emotion/";
			int pos = str_emotion_path.size();
			is_image_msg = true;
			QString str_emotion_name = imagePath.mid(pos);
			str_emotion_name.replace(".png","");
			//根据表情名称的长度进行设置表情数据，不足3位则补足3位
			int emotion_name_length = str_emotion_name.length();
			if (emotion_name_length == 1) {
				str_data = str_data + "00" + str_emotion_name;
			}
			else if(emotion_name_length == 2){
				str_data = str_data + "0" + str_emotion_name;
			}
			else if (emotion_name_length == 3) {
				str_data = str_data + str_emotion_name;
			}
			msg_type = 0; //表情信息
			image_num++; 
			if (imagePath.left(3) == "qrc") { //qrc:/Mainwindow  0  1  2 3
				pixmap.load(imagePath.mid(3));  //去掉表情中的qrc
			}
			else {
				pixmap.load(imagePath);
			}

			//表情图片html格式组合
			QString imgPath = QString("<img src=\"%1\" width=\"%2\" height=\"%3\"/>").
				arg(imagePath).arg(pixmap.width()).arg(pixmap.height());
			qs_msg += imgPath;
		}
		else if(msg_list.at(i).at(0) == "text"){
			//1  文本的真实内容
			qs_msg += msg_list.at(i).at(1);
			str_data = qs_msg;
		}
	}
	msg_obj.insert("MSG",qs_msg);
	const QString& msg = QJsonDocument(msg_obj).toJson(QJsonDocument::Compact);
	if (str_obj == "0") {	//发信息
		this->page()->runJavaScript(QString("appendHtml0(%1)").arg(msg));
		if (is_image_msg) {
			str_data = QString::number(image_num) + "images" + str_data;
		}
		emit SignalSendMsg(str_data, msg_type);
	}
	else {  //来信
		this->page()->runJavaScript(QString("recvHtml_%1(%2)").arg(str_obj).arg(msg));
	}
}

MsgWebView::~MsgWebView()
{
}

QList<QStringList> MsgWebView::ParseHtml(const QString & html)
{
	//节点元素
	QDomDocument doc;
	doc.setContent(html);
	const QDomElement& root = doc.documentElement(); //节点元素
	
	const QDomNode& node = root.firstChildElement("body");


	return ParseDocNode(node);
}

QList<QStringList> MsgWebView::ParseDocNode(const QDomNode& node)
{
	QList<QStringList> attribute;
	
	//返回所有子节点
	const QDomNodeList& list = node.childNodes();
	for (size_t i = 0; i < list.count(); i++)
	{
		const QDomNode& node = list.at(i);
		//是否元素
		if (node.isElement()) {
			//转换元素
			const QDomElement& element = node.toElement();

			if (element.tagName() == "img") {
				QStringList attribute_list;
				attribute_list << "img" << element.attribute("src");
				attribute << attribute_list;
			}
			if (element.tagName() == "span") {
				QStringList attribute_list;
				attribute_list << "text" << element.text();
				attribute << attribute_list;
			}
			if (node.hasChildNodes()) {
				attribute << ParseDocNode(node);
			}
		}
	}

	return attribute;
}

MsgHtmlObject::MsgHtmlObject(QObject* parent, QString msg_leftpic_path)
	:QObject(parent){
	msg_leftpic_path_ = msg_leftpic_path;
	InitHtmlTmpl();
}

void MsgHtmlObject::InitHtmlTmpl()
{
	m_msgLHtmlTmpl = GetMsgTmplHtml("msgleftTmpl");
	m_msgLHtmlTmpl.replace("%1", msg_leftpic_path_);

	m_msgRHtmlTmpl = GetMsgTmplHtml("msgrightTmpl");
	m_msgRHtmlTmpl.replace("%1", global_str_login_head_path);
}

QString MsgHtmlObject::GetMsgTmplHtml(const QString& code)
{
	QFile file(":/Resources/MainWindow/MsgHtml/" + code + ".html");
	file.open(QFile::ReadOnly);
	QString str_data;
	if (file.isOpen()) {
		str_data = QLatin1String(file.readAll());
	}
	else {
		QMessageBox::information(nullptr, "Tips", "Failed to init html!");
		return QString("");
	}
	file.close();
	return str_data;
}

bool MsgWebPage::accpetNavigationRequest(const QUrl& url, NavigationType type, bool isMainFrame)
{
	//仅接受 qrc:/*.html  判断url类型
	if (url.scheme() == QString("qrc")) {
		return true;
	}
	else{
		return false;
	}

}

MsgWebPage::~MsgWebPage()
{
}
