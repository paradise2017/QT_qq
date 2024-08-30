#pragma once

#include <QWidget>
#include "ui_talkwindowitem.h"

class TalkWindowItem : public QWidget
{
	Q_OBJECT

public:
	TalkWindowItem(QWidget *parent = nullptr);
	~TalkWindowItem();

	void SetHeadPixmap(const QPixmap& pixmap);
	void SetMsgLabelContent(const QString& msg);
	QString GetMsgLabelText();

signals:
	void SignalCloseClicked();
private:
	void InitControl();

	void enterEvent(QEvent* event)override;
	void leaveEvent(QEvent* event)override;
	void resizeEvent(QResizeEvent* event)override;

private:
	Ui::TalkWindowItemClass ui;
};
