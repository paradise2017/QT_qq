#include "CCMainWindow.h"
#include "SkinWindow.h"
#include "SysTray.h"
#include "NotifyManager.h"
#include "RootContatItem.h"
#include "ContactItem.h"
#include "WindowManager.h"
#include "TalkWindowShell.h"

#include <QHBoxLayout>
#include <QProxyStyle>
#include <QPainter>
#include <QTimer>
#include <QEvent>
#include <QTreeWidgetItem>
#include <QMouseEvent>
#include <QApplication>

#include <QSqlQuery>

QString gstrLoginHeadPath;
extern QString gLoginEmployeeID;

class CustomProxyStyle :public QProxyStyle
{
public:
	virtual void drawPrimitive(PrimitiveElement element, const QStyleOption* option,
		QPainter* painter, const QWidget* widget = nullptr) const
	{
		if (element == PE_FrameFocusRect)
		{
			return;
		}
		else
		{
			QProxyStyle::drawPrimitive(element, option, painter, widget);
		}
	}
};

CCMainWindow::CCMainWindow(QString account, bool isAccountLogin, QWidget *parent)
	: BasicWindow(parent)
	,m_isAccountLogin(isAccountLogin)
	,m_account(account)
{
	ui.setupUi(this);
	setWindowFlags(windowFlags() | Qt::Tool);
	loadStyleSheet("CCMainWindow");
	setHeadPixmap(getHeadPicturePath());
	initControl();
	initTimer();
}

CCMainWindow::~CCMainWindow()
{
}

void CCMainWindow::initTimer()
{
	QTimer *timer = new QTimer(this);
	timer->setInterval(500);

	connect(timer, &QTimer::timeout,[this]{
		static int level = 0;
		if (level == 99)
		{
			level = 0;
		}
		level++;
		setLevelPixmap(level);
	});

	timer->start();
}

void CCMainWindow::initControl()
{
	//树获取焦点时不绘制边框
	ui.treeWidget->setStyle(new CustomProxyStyle);
	setLevelPixmap(0);
	setStatusMenuIcon(":/Resources/MainWindow/StatusSucceeded.png");

	QHBoxLayout *appupLayout = new QHBoxLayout;
	appupLayout->setContentsMargins(0, 0, 0, 0);
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_7.png", "app_7"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_2.png", "app_2"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_3.png", "app_3"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_4.png", "app_4"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_5.png", "app_5"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_6.png", "app_6"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/skin.png", "app_skin"));
	appupLayout->addStretch();
	appupLayout->setSpacing(2);
	ui.appWidget->setLayout(appupLayout);

	ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_10.png", "app_10"));
	ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_8.png", "app_8"));
	ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_11.png", "app_11"));
	ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_9.png", "app_9"));
	ui.bottomLayout_up->addStretch();

	initContactTree();

	//个性签名
	ui.lineEdit->installEventFilter(this);
	//好友搜索
	ui.searchLineEdit->installEventFilter(this);

	connect(ui.sysmin, SIGNAL(clicked(bool)), this, SLOT(onShowHide(bool)));
	connect(ui.sysclose, SIGNAL(clicked(bool)), this, SLOT(onShowClose(bool)));

	connect(NotifyManager::getInstance(), &NotifyManager::signalSkinChanged, [this]() {
		updateSeachStyle();
	});

	SysTray* systray = new SysTray(this);
}

QString CCMainWindow::getHeadPicturePath()
{
	QString strPicturePath;

	if (!m_isAccountLogin)//QQ号登录
	{
		QSqlQuery queryPicture(QString("SELECT picture FROM tab_employees WHERE employeeID = %1").arg(gLoginEmployeeID));
		queryPicture.exec();
		queryPicture.next();

		strPicturePath = queryPicture.value(0).toString();
	}
	else//账号登录
	{
		QSqlQuery queryEmployeeID(QString("SELECT employeeID FROM tab_accounts WHERE account = '%1'").arg(m_account));
		queryEmployeeID.exec();
		queryEmployeeID.next();

		int employeeID = queryEmployeeID.value(0).toInt();

		QSqlQuery queryPicture(QString("SELECT picture FROM tab_employees WHERE employeeID = %1").arg(employeeID));
		queryPicture.exec();
		queryPicture.next();

		strPicturePath = queryPicture.value(0).toString();
	}

	gstrLoginHeadPath = strPicturePath;
	return strPicturePath;
}

void CCMainWindow::updateSeachStyle()
{
	ui.searchWidget->setStyleSheet(QString("QWidget#searchWidget{background-color:rgba(%1,%2,%3,50);border-bottom:1px solid rgba(%1,%2,%3,30)}\
											QPushButton#searchBtn{border-image:url(:/Resources/MainWindow/search/search_icon.png)}")
										.arg(m_colorBackGround.red())
										.arg(m_colorBackGround.green())
										.arg(m_colorBackGround.blue()));
}

void CCMainWindow::addCompanyDeps(QTreeWidgetItem * pRootGroupItem,int DepID)
{
	QTreeWidgetItem* pChild = new QTreeWidgetItem;

	QPixmap pix;
	pix.load(":/Resources/MainWindow/head_mask.png");

	//添加子节点
	pChild->setData(0, Qt::UserRole, 1);//子项数据设为1
	pChild->setData(0, Qt::UserRole + 1, DepID);

	//获取公司、部门头像
	QPixmap groupPix;
	QSqlQuery queryPicture(QString("SELECT picture FROM tab_department WHERE departmentID = %1").arg(DepID));
	queryPicture.exec();
	queryPicture.next();
	groupPix.load(queryPicture.value(0).toString());

	//获取部门名称
	QString strDepName;
	QSqlQuery querDepName(QString("SELECT department_name FROM tab_department WHERE departmentID = %1").arg(DepID));
	querDepName.exec();
	querDepName.first();
	strDepName = querDepName.value(0).toString();


	ContactItem* pContactItem = new ContactItem(ui.treeWidget);
	pContactItem->setHeadPixmap(getRoundImage(groupPix,pix,pContactItem->getHeadLabelSize()));
	pContactItem->setUserName(strDepName);

	pRootGroupItem->addChild(pChild);
	ui.treeWidget->setItemWidget(pChild, 0, pContactItem);
}

void CCMainWindow::setUserName(const QString & username)
{
	ui.nameLabel->adjustSize();

	//文本过长则进行省略...
	//fontMetrics()返回QFontMetrics类对象
 	QString name =  ui.nameLabel->fontMetrics().elidedText(username, Qt::ElideRight, ui.nameLabel->width());

	ui.nameLabel->setText(name);
}

void CCMainWindow::setLevelPixmap(int level)
{
	QPixmap levelPixmap(ui.levelBtn->size());
	levelPixmap.fill(Qt::transparent);

	QPainter painter(&levelPixmap);
	painter.drawPixmap(0, 4, QPixmap(":/Resources/MainWindow/lv.png"));

	int unitNum = level % 10;//个位数
	int tenNum = level / 10; //十位数

	//十位,截取图片中的部分进行绘制
	//drawPixmap(绘制点x，绘制点y,图片，图片左上角x,图片左上角y,拷贝的宽度，拷贝的高度)
	painter.drawPixmap(10, 4, QPixmap(":/Resources/MainWindow/levelvalue.png"), tenNum * 6, 0, 6, 7);

	//个位
	painter.drawPixmap(16, 4, QPixmap(":/Resources/MainWindow/levelvalue.png"), unitNum * 6, 0, 6, 7);

	ui.levelBtn->setIcon(levelPixmap);
	ui.levelBtn->setIconSize(ui.levelBtn->size());
}

void CCMainWindow::setHeadPixmap(const QString & headPath)
{
	QPixmap pix;
	pix.load(":/Resources/MainWindow/head_mask.png");
	ui.headLabel->setPixmap(getRoundImage(QPixmap(headPath), pix, ui.headLabel->size()));
}

void CCMainWindow::setStatusMenuIcon(const QString & statusPath)
{
	QPixmap statusBtnPixmap(ui.stausBtn->size());
	statusBtnPixmap.fill(Qt::transparent);

	QPainter painter(&statusBtnPixmap);
	painter.drawPixmap(4, 4, QPixmap(statusPath));

	ui.stausBtn->setIcon(statusBtnPixmap);
	ui.stausBtn->setIconSize(ui.stausBtn->size());
}

QWidget * CCMainWindow::addOtherAppExtension(const QString & appPath, const QString & appName)
{
	QPushButton* btn = new QPushButton(this);
	btn->setFixedSize(20, 20);

	QPixmap pixmap(btn->size());
	pixmap.fill(Qt::transparent);

	QPainter painter(&pixmap);
	QPixmap appPixmap(appPath);
	painter.drawPixmap( (btn->width() - appPixmap.width()) / 2,
		(btn->height() - appPixmap.height()) / 2, appPixmap);
	btn->setIcon(pixmap);
	btn->setIconSize(btn->size());
	btn->setObjectName(appName);
	btn->setProperty("hasborder", true);

	connect(btn, &QPushButton::clicked, this, &CCMainWindow::onAppIconClicked);
	return btn;
}

void CCMainWindow::initContactTree()
{
	//展开与收缩时的信号
	connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(onItemClicked(QTreeWidgetItem*, int)));
	connect(ui.treeWidget, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(onItemExpanded(QTreeWidgetItem*)));
	connect(ui.treeWidget, SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(onItemCollapsed(QTreeWidgetItem*)));
	connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(onItemDoubleClicked(QTreeWidgetItem*, int)));

	//根节点
	QTreeWidgetItem* pRootGroupItem = new QTreeWidgetItem;
	pRootGroupItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	pRootGroupItem->setData(0, Qt::UserRole, 0);//根项数据设为0
	RootContatItem* pItemName = new RootContatItem(true, ui.treeWidget);
	
	//获取公司部门ID(公司群号)
	QSqlQuery queryCompDepID(QString("SELECT departmentID FROM tab_department WHERE department_name = '%1'").arg(QString::fromLocal8Bit("公司群")));
	queryCompDepID.exec();
	queryCompDepID.first();
	int CompDepID = queryCompDepID.value(0).toInt();

	//获取QQ登录者所在的部门ID（部门群号）
	QSqlQuery querySelfDepID(QString("SELECT departmentID FROM tab_employees WHERE employeeID = %1").arg(gLoginEmployeeID));
	querySelfDepID.exec();
	querySelfDepID.first();
	int SelfDepID = querySelfDepID.value(0).toInt();

	//初始化公司群及登录者所在的群
	addCompanyDeps(pRootGroupItem, CompDepID);
	addCompanyDeps(pRootGroupItem, SelfDepID);

	QString strGroupName = QString::fromLocal8Bit("奇牛科技");
	pItemName->setText(strGroupName);

	//插入分组节点
	ui.treeWidget->addTopLevelItem(pRootGroupItem);
	ui.treeWidget->setItemWidget(pRootGroupItem, 0, pItemName);
}

void CCMainWindow::resizeEvent(QResizeEvent * event)
{
	setUserName(QString::fromLocal8Bit("奇牛科技-越分享越拥有"));
	BasicWindow::resizeEvent(event);
}

bool CCMainWindow::eventFilter(QObject * obj, QEvent * event)
{
	if (ui.searchLineEdit == obj)
	{
		//键盘焦点事件
		if (event->type() == QEvent::FocusIn)
		{
			ui.searchWidget->setStyleSheet(QString("QWidget#searchWidget{background-color:rgb(255,255,255);border-bottom:1px solid rgba(%1,%2,%3,100)}\
													QPushButton#searchBtn{border-image:url(:/Resources/MainWindow/search/main_search_deldown.png)} \
													QPushButton#searchBtn:hover{border-image:url(:/Resources/MainWindow/search/main_search_delhighlight.png)} \
													QPushButton#searchBtn:pressed{border-image:url(:/Resources/MainWindow/search/main_search_delhighdown.png)}")
												.arg(m_colorBackGround.red())
												.arg(m_colorBackGround.green())
												.arg(m_colorBackGround.blue()));
		}
		else if (event->type() == QEvent::FocusOut)
		{
			updateSeachStyle();
		}
	}
	return false;
}

void CCMainWindow::mousePressEvent(QMouseEvent * event)
{
	if (qApp->widgetAt(event->pos()) != ui.searchLineEdit && ui.searchLineEdit->hasFocus())
	{
		ui.searchLineEdit->clearFocus();
	}
	else if (qApp->widgetAt(event->pos()) != ui.lineEdit && ui.lineEdit->hasFocus())
	{
		ui.lineEdit->clearFocus();
	}

	BasicWindow::mousePressEvent(event);
}

void CCMainWindow::onItemClicked(QTreeWidgetItem * item, int column)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (!bIsChild)
	{
		item->setExpanded(!item->isExpanded());//未展开则展开子项
	}
}

void CCMainWindow::onItemExpanded(QTreeWidgetItem * item)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (!bIsChild)
	{
		//dynamic_cast 将基类对象指针(或引用)转换到继承类指针
		RootContatItem* prootItem = dynamic_cast<RootContatItem *>(ui.treeWidget->itemWidget(item, 0));
		if(prootItem)
		{ 
			prootItem->setExpanded(true);
		}
			
	}
}

void CCMainWindow::onItemCollapsed(QTreeWidgetItem * item)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (!bIsChild)
	{
		//dynamic_cast 将基类对象指针(或引用)转换到继承类指针
		RootContatItem* prootItem = dynamic_cast<RootContatItem *>(ui.treeWidget->itemWidget(item, 0));
		if (prootItem)
		{
			prootItem->setExpanded(false);

		}

	}
}

void CCMainWindow::onItemDoubleClicked(QTreeWidgetItem * item, int column)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (bIsChild)
	{
		WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString());
	}
}

void CCMainWindow::onAppIconClicked()
{
	//判断信号发送者的对象名是否是app_skin
	if (sender()->objectName() == "app_skin")
	{
		SkinWindow* skinWindow = new SkinWindow;
		skinWindow->show();
	}
}