#pragma once

#include <QLabel>

class QClickLabel : public QLabel
{
	Q_OBJECT

public:
	QClickLabel(QWidget *parent);
	~QClickLabel();

protected:
	void mousePressEvent(QMouseEvent* event);

signals:
	void clicked();
};
