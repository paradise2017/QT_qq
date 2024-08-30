#pragma once

#include <QWebEngineView>
#include <QDomNode>
//显示网页内容
class MsgHtmlObject : public QObject {
	Q_OBJECT
	//发来的信息   //动态属性
	Q_PROPERTY(QString msgLHtmlTmpl MEMBER m_msgLHtmlTmpl NOTIFY signalMsgHtml)
	//我发的信息
	Q_PROPERTY(QString msgRHtmlTmpl MEMBER m_msgRHtmlTmpl NOTIFY signalMsgHtml)

public:
	MsgHtmlObject(QObject* parent,QString msg_leftpic_path = ""); //发信息来的头像路径

signals:
	void signalMsgHtml(const QString& html);
private:
	//初始化聊天网页
	void InitHtmlTmpl(); 
	QString GetMsgTmplHtml(const QString& code);

private:
	QString msg_leftpic_path_;
	QString m_msgLHtmlTmpl; //别人发来的信息
	QString m_msgRHtmlTmpl; //我发来的信息
};
//QWebEngineView视图用于显示网页内容
class MsgWebView  : public QWebEngineView
{
	Q_OBJECT

public:
	MsgWebView(QWidget *parent);
	~MsgWebView();
	void AppendMsg(const QString& html, QString str_obj = "0");  //external 0; external_xxx


private:
	QList<QStringList> ParseHtml(const QString& html); //解析html
	//QT中的所有DOM节点(属性，说明，文本)都可以使用QDomNode
	QList<QStringList> ParseDocNode(const QDomNode& node);
signals:
	void SignalSendMsg(QString& str_data, int& msg_type, QString file = "");
private:
	MsgHtmlObject* msg_html_obj_;
	QWebChannel* channel_;
};

class MsgWebPage :public QWebEnginePage {
	Q_OBJECT
public:
	//只接受QRC
	MsgWebPage(QObject* parent = nullptr) :QWebEnginePage(parent) {};
protected:
	bool accpetNavigationRequest(const QUrl& url,NavigationType type,bool isMainFrame);
	~MsgWebPage();
};
