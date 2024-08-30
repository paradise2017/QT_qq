#include "talkwindowitem.h"
#include <qpushbutton.h>
#include "commonUtils.h"
TalkWindowItem::TalkWindowItem(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	InitControl();
}

TalkWindowItem::~TalkWindowItem()
{}

void TalkWindowItem::SetHeadPixmap(const QPixmap & pixmap)
{
	QPixmap mask = QPixmap(":/Resources/MainWindow/head_mask.png");
	const QPixmap& head_pixmap = CommonUtils::getRoundImage(pixmap,mask,ui.headlabel->size());
	ui.headlabel->setPixmap(head_pixmap);

}

void TalkWindowItem::SetMsgLabelContent(const QString& msg)
{
	ui.msgLabel->setText(msg);

}

QString TalkWindowItem::GetMsgLabelText()
{
	return ui.msgLabel->text();
}

void TalkWindowItem::InitControl()
{
	ui.tclosebtn->setVisible(false);
	connect(ui.tclosebtn, &QPushButton::clicked, this, &TalkWindowItem::SignalCloseClicked);
}

void TalkWindowItem::enterEvent(QEvent* event)
{
	ui.tclosebtn->setVisible(true);
	//其他的默认操作
	__super::enterEvent(event);
}

void TalkWindowItem::leaveEvent(QEvent* event)
{
	ui.tclosebtn->setVisible(false);
	//其他的默认操作
	__super::enterEvent(event);
}

void TalkWindowItem::resizeEvent(QResizeEvent* event)
{
	__super::resizeEvent(event);
}
