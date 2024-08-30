#include "CCMainWindow.h"
#include <QProxyStyle>
#include <Qpainter>
#include "skinwindow.h"
#include <QTimer>
#include "systray.h"
#include "notifymanager.h"
#include <QTreeWidgetItem>
#include "rootcontactitem.h"
#include "contactitem.h"
#include "windowmanager.h"
#include "talkwindowshell.h"
#include <QmouseEvent>
#include <qapplication.h>
#include <qsqlquery.h>
QString global_str_login_head_path;
extern QString login_account_id;

class CustomProxyStyle :public QProxyStyle
{
public:
    virtual void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const {
       //取消边框绘制
        if (element == PE_FrameFocusRect) {
            return;
        }
        else{
            QProxyStyle::drawPrimitive(element, option, painter, widget);
        }
    }
};
CCMainWindow::CCMainWindow(QString account, bool is_account_login, QWidget *parent)
    : BasicWindow(parent)
    ,is_account_(is_account_login)
    ,account_(account)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() | Qt::Tool);
    loadStyleSheet("CCMainWindow");
    SetHeadPixmap(GetHeadPicturePath());
    InitControl();
    InitTimer();

}
//初始化控件
void CCMainWindow::InitControl()
{
    //重写父类，取消树绘制焦点时，绘制边框
    ui.treeWidget->setStyle(new CustomProxyStyle);
    SetLevelPixmap(0);
   // SetHeadPixmap(":/Resources/MainWindow/girl.png");
    SetStatusMenuIcon(":/Resources/MainWindow/StatusSucceeded.png");

    QHBoxLayout* appup_layout = new QHBoxLayout;
    //布局间隙
    appup_layout->setContentsMargins(0, 0, 0, 0);
    appup_layout->addWidget(AddOtherAppExtension(":/Resources/MainWindow/app/app_7.png", "app_7"));
    appup_layout->addWidget(AddOtherAppExtension(":/Resources/MainWindow/app/app_2.png", "app_2"));
    appup_layout->addWidget(AddOtherAppExtension(":/Resources/MainWindow/app/app_3.png", "app_3"));
    appup_layout->addWidget(AddOtherAppExtension(":/Resources/MainWindow/app/app_4.png", "app_4"));
    appup_layout->addWidget(AddOtherAppExtension(":/Resources/MainWindow/app/app_5.png", "app_5"));
    appup_layout->addWidget(AddOtherAppExtension(":/Resources/MainWindow/app/app_6.png", "app_6"));
    appup_layout->addWidget(AddOtherAppExtension(":/Resources/MainWindow/app/skin.png", "app_skin"));
    appup_layout->addStretch();
    //部件间隙
    appup_layout->setSpacing(2);
    ui.appWidget->setLayout(appup_layout);
    ui.bottomLayout_up->addWidget(AddOtherAppExtension(":/Resources/MainWindow/app/app_8.png", "app_8"));
    ui.bottomLayout_up->addWidget(AddOtherAppExtension(":/Resources/MainWindow/app/app_10.png", "app_10"));
    ui.bottomLayout_up->addWidget(AddOtherAppExtension(":/Resources/MainWindow/app/app_11.png", "app_11"));
    ui.bottomLayout_up->addWidget(AddOtherAppExtension(":/Resources/MainWindow/app/app_9.png", "app_9"));
    //部件紧凑
    ui.bottomLayout_up->addStretch();
    //个性签名  安装事件过滤器，  当前对象监视  
    ui.lineEdit->installEventFilter(this);
    //好友搜索
    ui.searchLineEdit->installEventFilter(this);

    connect(ui.sysmin,&QPushButton::clicked,this,&BasicWindow::onShowHide);
    connect(ui.sysclose,&QPushButton::clicked,this,&BasicWindow::onShowClose);
    //系统托盘
    SysTray* sys_tray = new SysTray(this);

    connect(NotifyManager::getInstance(), &NotifyManager::signalSkinChanged,[this]() {
        UpdateSearchStyle();
    });
    //初始化树空间
    InitContactTree();
}
void CCMainWindow::SetLevelPixmap(int level) {
    QPixmap levelPixmap(ui.levelBtn->size());
    //透明
    levelPixmap.fill(Qt::transparent);
    QPainter painter(&levelPixmap);
    //等级图片绘制  0,4 代表绘制的起始位置x,y
    painter.drawPixmap(0,4,QPixmap(":/Resources/MainWindow/lv.png"));
    //个位数
    int unit_num = level % 10; 
    //十位数
    int ten_num = level / 10;

    //十位绘制，截图图片中的部分绘制       绘制点(x,y,图片左上角x,y,拷贝的长度，宽度)    每个数字的长度   ten_num *6  
    painter.drawPixmap(10,4,QPixmap(":/Resources/MainWindow/levelvalue.png"),ten_num * 6,0,6,7);
    //个位
    painter.drawPixmap(16, 4, QPixmap(":/Resources/MainWindow/levelvalue.png"), unit_num * 6, 0, 6, 7);
    //图片大小和填充（和按键一样大小）
    ui.levelBtn->setIcon(levelPixmap);
    ui.levelBtn->setIconSize(ui.levelBtn->size());
}

void CCMainWindow::SetHeadPixmap(const QString& headPath) {
    QPixmap pix;
    pix.load(":/Resources/MainWindow/head_mask.png");
    //绘制圆头像
    ui.headLabel->setPixmap(getRoundImage(QPixmap(headPath),pix,ui.headLabel->size()));
}

void CCMainWindow::SetStatusMenuIcon(const QString& statusPath) {
    QPixmap status_btn_pixmap(ui.statusBtn->size());
    status_btn_pixmap.fill(Qt::transparent);
    //在哪里绘图
    QPainter painter(&status_btn_pixmap);
    painter.drawPixmap(4, 4, QPixmap(statusPath));
    ui.statusBtn->setIcon(status_btn_pixmap);
    ui.statusBtn->setIconSize(ui.statusBtn->size());
}
void CCMainWindow::OnAppIconClicked() {
    //如果信号的发送者是app_skin,前边已经定义
    if (sender()->objectName() == "app_skin") {
        SkinWindow* skin_window = new SkinWindow;
        skin_window->show();
    }
}
//基类指针
QWidget* CCMainWindow::AddOtherAppExtension(const QString& app_path, const QString& app_name)
{
    QPushButton* btn = new QPushButton(this);
    btn->setFixedSize(20, 20);
    //空白图片
    QPixmap pixmap(btn->size());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    //导入图片
    QPixmap app_pixmap(app_path);
    //长宽都有间距
    painter.drawPixmap((btn->width() - app_pixmap.width()) / 2,(btn->height()-app_pixmap.height())/2,app_pixmap);
    btn->setIcon(pixmap);
    btn->setIconSize(btn->size());
    //设置对象名
    btn->setObjectName(app_name);
    //设置边框属性
    btn->setProperty("hasborder",true);
    connect(btn, &QPushButton::clicked, this, &CCMainWindow::OnAppIconClicked);
    return btn;
}



void CCMainWindow::InitTimer()
{
    QTimer* timer = new QTimer(this);
    timer->setInterval(500);
    connect(timer, &QTimer::timeout, [this]() {
        static int level = 0;
        if (level == 99) {
            level = 0;
        }
        level++;
        this->SetLevelPixmap(level);
        });
    timer->start();
}

void CCMainWindow::SetUserName(const QString& username){
    ui.nameLabel->adjustSize();
    //文本过长则进行省略  fontMetrics(); QFontmetrics  省略设置
    QString name = ui.nameLabel->fontMetrics().elidedText(username,Qt::ElideRight,ui.nameLabel->width());
    ui.nameLabel->setText(name);
}
void CCMainWindow::resizeEvent(QResizeEvent* event){
    SetUserName(QString::fromLocal8Bit("骑牛科技科技科技"));
    BasicWindow::resizeEvent(event);
}

bool CCMainWindow::eventFilter(QObject* obj, QEvent* event)
{
    if (ui.searchLineEdit == obj) {
       //如果是键盘焦点事件
        if (event->type() == QEvent::FocusIn) {
            ui.searchWidget->setStyleSheet(QString("QWidget#searchWidget{background-color:rgb(255,255,255);\
                border-bottom:1px solid rbga(%1,%2,%3,100)}QPushButton#searchBtn{border-image:url(:/Resources/MainWindow/search/main_search_deldown.png)}\
                QPushButton#searchBtn:hover{border-image:url(:/Resources/MainWindow/search/main_search_delhighlight.png)}\
                QPushButton#searchBtn:pressed{border-image:url(:/Resources/MainWindow/search/main_search_delhighdown.png)}").arg(m_colorBackGround.red()).arg(m_colorBackGround.green()).arg(m_colorBackGround.blue()));
        }
        else if(event->type() == QEvent::FocusOut){
            UpdateSearchStyle();
        }
            
    }
    return false;
}

void CCMainWindow::UpdateSearchStyle()
{
    ui.searchWidget->setStyleSheet(QString("QWidget#searchWidget{background-color:rgba(%1,%2,%3,50);\
                border-bottom:1px solid rbga(%1,%2,%3,30)}QPushButton#searchBtn{border-image:url(:/Resources/MainWindow/search/search_icon.png)}\
              ").arg(m_colorBackGround.red()).arg(m_colorBackGround.green()).arg(m_colorBackGround.blue()));
}



void CCMainWindow::InitContactTree(){
    //展开与收缩信号

 /*   connect(ui.treeWidget, &QTreeWidget::clicked, this, &CCMainWindow::OnItemClicked);
    connect(ui.treeWidget, &QTreeWidget::itemExpanded, this, &CCMainWindow::OnItemExpanded);
    connect(ui.treeWidget, &QTreeWidget::itemCollapsed, this, &CCMainWindow::OnItemCollapsed);
    connect(ui.treeWidget, &QTreeWidget::itemDoubleClicked, this, &CCMainWindow::OnItemDoubleClicked);*/
    
    connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(OnItemClicked(QTreeWidgetItem*, int)));
    connect(ui.treeWidget, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(OnItemExpanded(QTreeWidgetItem*)));
    connect(ui.treeWidget, SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(OnItemCollapsed(QTreeWidgetItem*)));
    connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(OnItemDoubleClicked(QTreeWidgetItem*, int)));
    
    //根节点
    QTreeWidgetItem* root_group_item = new QTreeWidgetItem();
    //默认显示所有节点
    root_group_item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
    //  0列，userrole 用户角色， 值 0
    root_group_item->setData(0, Qt::UserRole, 0);
    RootContactItem* item_name = new RootContactItem(true, ui.treeWidget);


    //获取公司部门ID
    QSqlQuery part_id(QString("SELECT departmentID FROM qt_qq.tab_department WHERE department_name = '%1'").arg(QString::fromLocal8Bit("公司群")));
    part_id.first();
    int compony_dep_id = part_id.value(0).toInt();
    int a = 10;
    //获取QQ登录所在部门ID (部门群号)
    QSqlQuery self_id(QString("SELECT departmentID FROM qt_qq.tab_employees WHERE employeeID = %1").arg(login_account_id));
    self_id.first();
    if (self_id.first()) {
        int a = 10;
    }
    int self_dep_id = self_id.value(0).toInt();

 
    //初始化公司群 部门群
    AddCompanyParts(root_group_item,compony_dep_id);
    AddCompanyParts(root_group_item,self_dep_id);

    QString str_group_name = QString::fromLocal8Bit("骑牛科技");
    item_name->SetText(str_group_name);

    //插入分组节点   先有节点，再有窗口
    ui.treeWidget->addTopLevelItem(root_group_item);
    ui.treeWidget->setItemWidget(root_group_item, 0, item_name);

}
void CCMainWindow::AddCompanyParts(QTreeWidgetItem* root_group_item, int Dep_id)
{
    QTreeWidgetItem* child = new QTreeWidgetItem;
    //添加子节点
    child->setData(0, Qt::UserRole, 1);  //子项数据设为1
    //窗口的唯一识别
    child->setData(0, Qt::UserRole + 1,Dep_id);
    //获取公司
    QPixmap group_pix;
    QSqlQuery query_picture(QString("SELECT picture FROM qt_qq.tab_department WHERE departmentID = %1").arg(Dep_id));
    //query_picture.exec();
    query_picture.first();
    group_pix.load(query_picture.value(0).toString());
    //部门名称
    QString str_department_name;
    QSqlQuery department_name(QString("SELECT department_name FROM qt_qq.tab_department WHERE departmentID = %1").arg(Dep_id));
    //department_name.exec();
    department_name.first();
    str_department_name = department_name.value(0).toString();
    ContactItem* contact_item = new ContactItem(ui.treeWidget);
    QPixmap pix;
    pix.load(":/Resources/MainWindow/head_mask.png");
    contact_item->SetHeadPixmap(getRoundImage(group_pix,pix,contact_item->GetHeadLabelSize()));
    contact_item->SetUserName(str_department_name);
    root_group_item->addChild(child); 
    ui.treeWidget->setItemWidget(child, 0, contact_item);
}


void CCMainWindow::OnItemClicked(QTreeWidgetItem* event, int col)
{
    //数据为0是根项，数据为1是子项
    bool is_child = event->data(0, Qt::UserRole).toBool();
    if (!is_child) {
        event->setExpanded(!event->isExpanded());
    }
}

void CCMainWindow::OnItemExpanded(QTreeWidgetItem* event)
{
    bool is_child = event->data(0, Qt::UserRole).toBool();
    if (!is_child) {
        //Qwidget 转为Qlabel  基类转为派生类
        RootContactItem* root_item = dynamic_cast<RootContactItem*> (ui.treeWidget->itemWidget(event,0));
        //指针不空
        if (root_item) {
            root_item->SetExpanded(true);
        }
    }
}

void CCMainWindow::OnItemCollapsed(QTreeWidgetItem* event)
{
    bool is_child = event->data(0, Qt::UserRole).toBool();
    if (!is_child) {
        //Qwidget 转为Qlabel  基类转为派生类
        RootContactItem* root_item = dynamic_cast<RootContactItem*> (ui.treeWidget->itemWidget(event, 0));
        //指针不空
        if (root_item) {
            root_item->SetExpanded(false);
        }
    }
}

void CCMainWindow::OnItemDoubleClicked(QTreeWidgetItem* event, int col)
{
    bool ischild = event->data(0, Qt::UserRole).toBool();
    //当前子节点的那个群？
    if (ischild) {  
     WindowManager::GetInstance()->AddNewTalkWindow(event->data(0, Qt::UserRole + 1).toString());
    }
}
void CCMainWindow::mousePressEvent(QMouseEvent* event)
{
   //清除焦点
    if (qApp->widgetAt(event->pos())!= ui.searchLineEdit && ui.searchLineEdit->hasFocus()) {
        ui.searchLineEdit->clearFocus();
    }
    else if (qApp->widgetAt(event->pos()) != ui.lineEdit && ui.lineEdit->hasFocus()) {
        ui.lineEdit->clearFocus();
    }
    BasicWindow::mousePressEvent(event);
}

QString CCMainWindow::GetHeadPicturePath()
{
    QString str_picture_path;

    if (!is_account_) {//qq login
        QSqlQuery query_picture(QString("SELECT picture FROM qt_qq.tab_employees WHERE employeeID = %1").arg(login_account_id));
        query_picture.first();
        str_picture_path = query_picture.value(0).toString();
    }
    else { //账号登录
        QSqlQuery query_employee_id(QString("SELECT employeeID FROM qt_qq.tab_account WHERE account = '%1'").arg(account_));
        query_employee_id.first();
        int employee_id = query_employee_id.value(0).toInt();
        QSqlQuery query_picture(QString("SELECT picture FROM qt_qq.tab_employees WHERE employeeID = %1").arg(employee_id));
        query_picture.first();
        str_picture_path = query_picture.value(0).toString();
    }
    global_str_login_head_path = str_picture_path;
    return str_picture_path;
}

CCMainWindow::~CCMainWindow(){

}
