﻿#pragma once

#include <QDialog>
#include "titlebar.h"

class BasicWindow  : public QDialog
{
	Q_OBJECT

public:
	BasicWindow(QWidget *parent = nullptr);
	virtual ~BasicWindow();

public:
	//加载样式表
	void loadStyleSheet(const QString& sheetName);
	//获取圆头像
	QPixmap getRoundImage(const QPixmap & src, QPixmap &mask, QSize masksize = QSize(0,0));

private:
	void initBackGroundColor();  //初始化背景

protected:
	void paintEvent(QPaintEvent* event) override;			//绘图事件
	void mousePressEvent(QMouseEvent* event) override;		 //鼠标按下事件
	void mouseMoveEvent(QMouseEvent* event) override;		//鼠标移动事件
	void mouseReleaseEvent(QMouseEvent* event) override;	//鼠标松开事件

protected:
	void initTitleBar(ButtonType buttontype = MIN_BUTTON);
	void setTitleBarTitle(const QString& title, const QString& icon = "");

public slots:
	void onShowClose(bool);
	void onShowMin(bool);
	void onShowHide(bool);
	void onShowNormal(bool);
	void onShowQuit(bool);
	void onSignalSkinChanged(const QColor &color);

	void onButtonMinClicked();
	void onButtonRestoreClicked();
	void onButtonMaxClicked();
	void onButtonCloseClicked();

protected:
	QPoint m_mousePoint;	//鼠标位置
	bool m_mousePressed; //鼠标是否按下
	QColor m_colorBackGround;  //背景色
	QString m_styleName;    //样式文件名称
	TitleBar* _titleBar;	//标题栏
};
