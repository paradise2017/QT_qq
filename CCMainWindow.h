#pragma once

#include "basicwindow.h"
#include "ui_CCMainWindow.h"

class QTreeWidgetItem;

class CCMainWindow : public BasicWindow
{
	Q_OBJECT

public:
	CCMainWindow(QString account,bool isAccountLogin, QWidget *parent = Q_NULLPTR);
	~CCMainWindow();

public:
	void setUserName(const QString& username);	//设置用户名
	void setLevelPixmap(int level);				//设置顶级
	void setHeadPixmap(const QString& headPath);//设置头像
	void setStatusMenuIcon(const QString& statusPath);//设置状态
	//添加应用部件(app图片路径，app部件对象名)
	QWidget* addOtherAppExtension(const QString& appPath, const QString& appName);
	void initContactTree();

private:
	void initTimer();	//初始化计时器
	void initControl();
	QString getHeadPicturePath();
	void updateSeachStyle(); //更新搜索样式
	void addCompanyDeps(QTreeWidgetItem* pRootGroupItem, int DepID);
	
private:
	void resizeEvent(QResizeEvent* event);
	bool eventFilter(QObject* obj, QEvent* event);
	void mousePressEvent(QMouseEvent* event);

private slots:
	void onItemClicked(QTreeWidgetItem* item, int column);
	void onItemExpanded(QTreeWidgetItem* item);
	void onItemCollapsed(QTreeWidgetItem* item);
	void onItemDoubleClicked(QTreeWidgetItem* item, int column);
	void onAppIconClicked();
private:
	Ui::CCMainWindowClass ui;
	bool m_isAccountLogin;
	QString m_account;		//登录的账号或QQ号
};
