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
	void setUserName(const QString& username);	//�����û���
	void setLevelPixmap(int level);				//���ö���
	void setHeadPixmap(const QString& headPath);//����ͷ��
	void setStatusMenuIcon(const QString& statusPath);//����״̬
	//���Ӧ�ò���(appͼƬ·����app����������)
	QWidget* addOtherAppExtension(const QString& appPath, const QString& appName);
	void initContactTree();

private:
	void initTimer();	//��ʼ����ʱ��
	void initControl();
	QString getHeadPicturePath();
	void updateSeachStyle(); //����������ʽ
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
	QString m_account;		//��¼���˺Ż�QQ��
};
