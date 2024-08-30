#include "contactitem.h"

ContactItem::ContactItem(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	InitControl();
}

ContactItem::~ContactItem()
{
}

void ContactItem::SetUserName(const QString& username)
{
	ui.userName->setText(username);
}

void ContactItem::SetSignName(const QString& signname)
{
	ui.signName->setText(signname);
}

void ContactItem::SetHeadPixmap(const QPixmap& headpath)
{
	ui.label->setPixmap(headpath);
}

QString ContactItem::GetUserName1() const
{
	return ui.userName->text();
}

QSize ContactItem::GetHeadLabelSize() const
{
	return ui.label->size();
}

void ContactItem::InitControl()
{

}
