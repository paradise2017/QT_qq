#include "talkwindow.h"
#include "rootcontactitem.h"
#include "contactitem.h"
#include "commonUtils.h"
#include "windowmanager.h"
#include "qtooltip.h"
#include "qfile.h"
#include "qmessagebox.h"
#include "qsqlquery.h"
#include "qsqlquerymodel.h"
TalkWindow::TalkWindow(QWidget* parent, const QString& user_id)
	: QWidget(parent),
	talk_id_(user_id)
{
	ui.setupUi(this);

	this->setAttribute(Qt::WA_DeleteOnClose);
	InitGroupTalkStatus();
	InitControl();
	WindowManager::GetInstance()->AddWindowName(talk_id_,this);
}

TalkWindow::~TalkWindow()
{
	WindowManager::GetInstance()->DeleteWindowName(talk_id_);
}

void TalkWindow::AddEmotionImage(int emotion_num)
{
	//焦点选择
	ui.textEdit->setFocus();
	ui.textEdit->AddEmotionUrl(emotion_num);

}

void TalkWindow::SetWindowName(const QString& name)
{
	ui.nameLabel->setText(name);
}

void TalkWindow::InitControl()
{
	QList<int> rightWidgetSize;
	rightWidgetSize << 600 << 138;
	ui.bodySplitter->setSizes(rightWidgetSize);

	ui.textEdit->setFontPointSize(10);
	//设置焦点，不需要用户点击
	ui.textEdit->setFocus();

	//connect(ui.sysmin,&QPushButton::clicked,this->parent(),&OnShowMin);
	//connect(ui.sysclose, &QPushButton::clicked, this->parent(), &OnShowClose);
	//connect(ui.closeBtn, &QPushButton::clicked, this->parent(), &OnShowClose);
	//connect(ui.faceBtn, &QPushButton::clicked,this->parent(), &OnEmotionBtnClicked);
	//connect(ui.sendBtn, &QPushButton::clicked, this, &OnSendBtnClicked);
	//connect(ui.treeWidget,&QTreeWidget::itemDoubleClicked,this,&OnItemDoubleClicked);
	connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(OnItemDoubleClicked(QTreeWidgetItem*, int)));
	connect(ui.sysmin,SIGNAL(clicked(bool)),this->parent(),SLOT(onShowMin(bool)));
	connect(ui.sysclose, SIGNAL(clicked(bool)), this->parent(), SLOT(onShowClose(bool)));
	connect(ui.closeBtn, SIGNAL(clicked(bool)), this->parent(), SLOT(onShowClose(bool)));
	connect(ui.faceBtn, SIGNAL(clicked(bool)), this->parent(), SLOT(OnEmotionBtnClicked(bool)));
	connect(ui.sendBtn, SIGNAL(clicked(bool)), this, SLOT(OnSendBtnClicked(bool)));
	
	if (is_group_talk_) {
		InitTalkWindow();
	}
	else{
		InitPtoTalk();
	}
	/*switch (group_type_)
	{
	case GroupType::COMPANY:{
		InitCompanyTalk();
		break; }
	case GroupType::PERSONGROUP:{
		InitPersonTalk();
		break; }
	case GroupType::DEVELOPGROUP:
		InitDevelopTalk();
		break;
	case GroupType::MARKETGROUP:
		InitMarketTalk();
		break;
	case GroupType::PERSONTOPERSON:
		InitPtoTalk();
		break;
	default:
		break;
	}*/
}



void TalkWindow::InitGroupTalkStatus(){

	QSqlQueryModel sql_dep_model;
	QString str_sql = QString("SELECT * FROM qt_qq.tab_department WHERE departmentID = %1").arg(talk_id_);

	sql_dep_model.setQuery(str_sql);

	int rows = sql_dep_model.rowCount();

	if (rows == 0) {//单聊
		is_group_talk_ = false;
	}
	else {
		is_group_talk_ = true;
	}
}
void TalkWindow::InitTalkWindow()
{
	QTreeWidgetItem* p_root_item = new QTreeWidgetItem();
	p_root_item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	//设置data 区分根项，子项
	p_root_item->setData(0, Qt::UserRole, 0);
	RootContactItem* p_item_name = new RootContactItem(false, ui.treeWidget);
	//shell - shell height
	ui.treeWidget->setFixedHeight(646);

	//当前聊天的群组名

	QString str_group_name;
	QSqlQuery query_group_name(QString("SELECT department_name FROM qt_qq.tab_department WHERE departmentID = %1").arg(talk_id_));
	
	if (query_group_name.first()) {
		str_group_name = query_group_name.value(0).toString();
	}

	QSqlQueryModel query_employee_model;
	if (GetCompanyId() == talk_id_.toInt()) { //公司群
		query_employee_model.setQuery("SELECT employeeID FROM qt_qq.tab_employees WHERE status = 1");
	}
	else {
		query_employee_model.setQuery(QString("SELECT employeeID FROM qt_qq.tab_employees WHERE status = 1 AND departmentID = %1").arg(talk_id_));
	}	
	//当前员工数量
	int employee_num = query_employee_model.rowCount();

	QString qs_group_name = QString::fromLocal8Bit("%1,%2/%3").arg(str_group_name).
		arg(0)
		.arg(employee_num);
	p_item_name->SetText(qs_group_name);
	//插入分组节点
	ui.treeWidget->addTopLevelItem(p_root_item);
	ui.treeWidget->setItemWidget(p_root_item, 0, p_item_name);
	//默认展开
	p_root_item->setExpanded(true);

	for (size_t i = 0; i < employee_num; i++)
	{
		QModelIndex model_index = query_employee_model.index(i,0);
		int employee_id = query_employee_model.data(model_index).toInt();
		//添加子节点
		AddPeopleInfo(p_root_item,employee_id);
	}
}
/*
void TalkWindow::InitCompanyTalk()
{
	QTreeWidgetItem* p_root_item = new QTreeWidgetItem();
	p_root_item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	//设置data 区分根项，子项
	p_root_item->setData(0, Qt::UserRole, 0);
	RootContactItem* p_item_name = new RootContactItem(false,ui.treeWidget);
	//shell - shell height
	ui.treeWidget->setFixedHeight(646);

	int employee_num = 50;
	QString qs_group_name = QString::fromLocal8Bit("公司群%1,%2").arg(0).arg(1);
	p_item_name->SetText(qs_group_name);

	//插入分组节点

	ui.treeWidget->addTopLevelItem(p_root_item);
	ui.treeWidget->setItemWidget(p_root_item,0, p_item_name);
	//默认展开
	p_root_item->setExpanded(true);

	for (size_t i = 0; i < employee_num; i++)
	{
		AddPeopleInfo(p_root_item);
	}
}
void TalkWindow::InitPersonTalk()
{
	QTreeWidgetItem* p_root_item = new QTreeWidgetItem();
	p_root_item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	//设置data 区分根项，子项
	p_root_item->setData(0, Qt::UserRole, 0);
	RootContactItem* p_item_name = new RootContactItem(false, ui.treeWidget);
	//shell - shell height
	ui.treeWidget->setFixedHeight(646);

	int employee_num = 5;
	QString qs_group_name = QString::fromLocal8Bit("人事部%1,%2").arg(0).arg(1);
	p_item_name->SetText(qs_group_name);

	//插入分组节点

	ui.treeWidget->addTopLevelItem(p_root_item);
	ui.treeWidget->setItemWidget(p_root_item, 0, p_item_name);
	//默认展开
	p_root_item->setExpanded(true);

	for (size_t i = 0; i < employee_num; i++)
	{
		AddPeopleInfo(p_root_item);
	}
}

void TalkWindow::InitMarketTalk()
{
	QTreeWidgetItem* p_root_item = new QTreeWidgetItem();
	p_root_item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	//设置data 区分根项，子项
	p_root_item->setData(0, Qt::UserRole, 0);
	RootContactItem* p_item_name = new RootContactItem(false, ui.treeWidget);
	//shell - shell height
	ui.treeWidget->setFixedHeight(646);

	int employee_num = 8;
	QString qs_group_name = QString::fromLocal8Bit("市场部%1,%2").arg(0).arg(1);
	p_item_name->SetText(qs_group_name);

	//插入分组节点

	ui.treeWidget->addTopLevelItem(p_root_item);
	ui.treeWidget->setItemWidget(p_root_item, 0, p_item_name);
	//默认展开
	p_root_item->setExpanded(true);

	for (size_t i = 0; i < employee_num; i++)
	{
		AddPeopleInfo(p_root_item);
	}
}
void TalkWindow::InitDevelopTalk()
{
	QTreeWidgetItem* p_root_item = new QTreeWidgetItem();
	p_root_item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	//设置data 区分根项，子项
	p_root_item->setData(0, Qt::UserRole, 0);
	RootContactItem* p_item_name = new RootContactItem(false, ui.treeWidget);
	//shell - shell height
	ui.treeWidget->setFixedHeight(646);

	int employee_num = 10;
	QString qs_group_name = QString::fromLocal8Bit("研发部%1,%2").arg(0).arg(1);
	p_item_name->SetText(qs_group_name);

	//插入分组节点

	ui.treeWidget->addTopLevelItem(p_root_item);
	ui.treeWidget->setItemWidget(p_root_item, 0, p_item_name);
	//默认展开
	p_root_item->setExpanded(true);

	for (size_t i = 0; i < employee_num; i++)
	{
		AddPeopleInfo(p_root_item);
	}
}
*/
void TalkWindow::InitPtoTalk(){
	QPixmap pixskin;
	pixskin.load(":/Resources/MainWindow/skin.png");
	ui.widget->setFixedSize(pixskin.size());
	QLabel* skinlabel = new QLabel(ui.widget);
	skinlabel->setPixmap(pixskin);
	skinlabel->setFixedSize(ui.widget->size());
}
void TalkWindow::AddPeopleInfo(QTreeWidgetItem* p_root_item, int employ_id)
{
	QTreeWidgetItem* p_child = new QTreeWidgetItem();
	//添加子节点
	p_child->setData(0, Qt::UserRole, 1);
	p_child->setData(0, Qt::UserRole + 1, employ_id);
	ContactItem* p_contact_item = new ContactItem(ui.treeWidget);

	QPixmap pix1;
	pix1.load(":/Resources/MainWindow/head_mask.png");
	//获取名，签名，头像
	QString	str_name, str_sign, str_picture;
	QSqlQueryModel query_info_model;
	query_info_model.setQuery(QString("SELECT employee_name,employee_sign,picture FROM qt_qq.tab_employees WHERE employeeID = %1").arg(employ_id));

	QModelIndex name_index, sign_index, picture_index;
	name_index = query_info_model.index(0,0);//行 列
	sign_index = query_info_model.index(0, 1);
	picture_index = query_info_model.index(0, 2);

	str_name = query_info_model.data(name_index).toString();
	str_sign = query_info_model.data(sign_index).toString();
	str_picture = query_info_model.data(picture_index).toString();

	QImage image_head;
	image_head.load(str_picture);
	 
	p_contact_item->SetHeadPixmap(CommonUtils::getRoundImage(QPixmap::fromImage(image_head), pix1, p_contact_item->GetHeadLabelSize()));
	p_contact_item->SetUserName(str_name);
	p_contact_item->SetSignName(str_sign);
	p_root_item->addChild(p_child);
	ui.treeWidget->setItemWidget(p_child,0,p_contact_item);
	QString str = p_contact_item->GetUserName1();
	group_people_map_.insert(p_child,str);
}
int TalkWindow::GetCompanyId()
{
	QSqlQuery company_id(QString("SELECT departmentID FROM qt_qq.tab_department WHERE department_name = '%1'").arg(QString::fromLocal8Bit("公司群")));
	company_id.first();
	return company_id.value(0).toInt();

}
QString TalkWindow::GetTalkId()
{
	return talk_id_;
}
//聊天界面用户点击实现
void TalkWindow::OnItemDoubleClicked(QTreeWidgetItem* item, int col)
{
	//子项
	bool is_child = item->data(0, Qt::UserRole).toBool();
	if (is_child) {
		////双击的那个人？
		//QString str_people_name = group_people_map_.value(item);
		//窗口标识 （0,Qt::UserRole+1）
		WindowManager::GetInstance()->AddNewTalkWindow(item->data(0,Qt::UserRole+1).toString());

	}
}
void TalkWindow::OnSendBtnClicked(bool flag) {
	if (ui.textEdit->toPlainText().isEmpty()) {
		//提示信息的坐标   父窗体 this  2000 显示时间  2S
		QToolTip::showText(this->mapToGlobal(QPoint(630, 660)), QString::fromLocal8Bit("发送的信息不能为空"), this, QRect(0, 0, 120, 100),2000); 
		return;
	 }

	QString html = ui.textEdit->document()->toHtml();
	//文本HTML 如果没有添加字体则添加字体   没有字体 字号
	if (!html.contains(".png") && !html.contains("</span>")) {
		QString font_html;
		QString text = ui.textEdit->toPlainText();
		QFile file(":/Resources/MainWindow/MsgHtml/msgFont.txt");
		if (file.open(QIODevice::ReadOnly)) {
			font_html = file.readAll();
			font_html.replace("%1", text);
			file.close();
		}
		else{
			QMessageBox::information(this,QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("文件msgFont.txt不存在"));
			return;
		}

		if (!html.contains(font_html)) {
			html.replace(text, font_html);
		}
	}

	ui.textEdit->clear();
	// 清空表情
	ui.textEdit->DeleteAllEmotionImage();
	ui.msgWidget->AppendMsg(html);// 收信息窗口
}
