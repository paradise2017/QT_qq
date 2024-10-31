#include "TalkWindowItem.h"
#include "CommonUtils.h"

TalkWindowItem::TalkWindowItem(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initControl();
}

TalkWindowItem::~TalkWindowItem()
{
}

void TalkWindowItem::setHeadPixmap(const QPixmap& pixmap)
{
	QPixmap mask = QPixmap(":/Resources/MainWindow/head_mask.png");
//	QPixmap head = QPixmap(":/Resources/MainWindow/girl.png");

	const QPixmap& headpixmap = CommonUtils::getRoundImage(pixmap, mask, ui.headlabel->size());
	ui.headlabel->setPixmap(headpixmap);
}

void TalkWindowItem::setMsgLabelContent(const QString & msg)
{
	ui.msgLabel->setText(msg);
}

QString TalkWindowItem::getMsgLabelText()
{
	return ui.msgLabel->text();
}

void TalkWindowItem::initControl()
{
	ui.tclosebtn->setVisible(false);
	connect(ui.tclosebtn, SIGNAL(clicked(bool)), this, SIGNAL(signalCloseClicked()));
}

void TalkWindowItem::enterEvent(QEvent * event)
{
	ui.tclosebtn->setVisible(true);
	__super::enterEvent(event);
}

void TalkWindowItem::leaveEvent(QEvent * event)
{
	ui.tclosebtn->setVisible(false);
	__super::enterEvent(event);
}

void TalkWindowItem::resizeEvent(QResizeEvent * event)
{
	__super::resizeEvent(event);
}
