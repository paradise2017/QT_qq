#pragma once

#include "basicwindow.h"
#include "ui_sendfile.h"

class SendFile : public BasicWindow
{
	Q_OBJECT

public:
	SendFile(QWidget *parent = nullptr);
	~SendFile();

private:
	Ui::SendFileClass ui;
};
