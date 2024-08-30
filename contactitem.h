#pragma once

#include <QWidget>
#include "ui_contactitem.h"

class ContactItem : public QWidget
{
	Q_OBJECT

public:
	ContactItem(QWidget *parent = nullptr);
	~ContactItem();

	void SetUserName(const QString& username);
	void SetSignName(const QString& signname);
	void SetHeadPixmap(const QPixmap& headpath);
	QString GetUserName1() const;
	QSize GetHeadLabelSize()const;
private:
	void InitControl();
private:
	Ui::ContactItemClass ui;
};
