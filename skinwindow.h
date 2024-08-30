#pragma once

#include "basicwindow.h"
#include "ui_skinwindow.h"

class SkinWindow : public BasicWindow
{
	Q_OBJECT

public:
	SkinWindow(QWidget *parent = nullptr);
	~SkinWindow();
public:
	void InitControl();
public slots:
	//重写父类关闭
	void onShowClose();

private:
	Ui::SkinWindow ui;
};
