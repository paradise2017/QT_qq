#pragma once
#include "basicwindow.h"
#include "ui_CCMainWindow.h"
class CCMainWindow : public BasicWindow
{
    Q_OBJECT

public:
    CCMainWindow(QString account,bool is_account_login,QWidget *parent = nullptr);
    ~CCMainWindow();

public:

    //设置用户名
    void SetUserName(const QString& username);
    //设置等级
    void SetLevelPixmap(int level);
    //设置头像
    void SetHeadPixmap(const QString& headPath);
    //设置状态
    void SetStatusMenuIcon(const QString& headPath);

    //添加应用部件(app图片路径，app部件对象名,)
    QWidget* AddOtherAppExtension(const QString& app_path, const QString& app_name);

    //初始化联系人
    void InitContactTree();

private:
   
    //初始化计时器
    void InitTimer();
    void InitControl();
    //更新搜索样式
    void UpdateSearchStyle();
    void AddCompanyParts(QTreeWidgetItem* root_group_item, int Dep_id);
    void resizeEvent(QResizeEvent* event)override;
    bool eventFilter(QObject* obj, QEvent* event);
    void mousePressEvent(QMouseEvent* event)override;
    //获取头像
    QString GetHeadPicturePath();
private slots:
    void OnAppIconClicked();
    void OnItemClicked(QTreeWidgetItem* event, int col);
    void OnItemExpanded(QTreeWidgetItem* event);
    void OnItemCollapsed(QTreeWidgetItem* event);
    void OnItemDoubleClicked(QTreeWidgetItem* event, int col);
private:
    Ui::CCMainWindowClass ui;
    bool is_account_;
    QString account_; //登录的账号或QQ号
};
