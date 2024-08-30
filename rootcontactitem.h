#pragma once

#include <QLabel>
#include <QPropertyAnimation>

class RootContactItem  : public QLabel
{
	Q_OBJECT
		//箭头角度  动态属性
		Q_PROPERTY(int rotation READ rotation WRITE SetRotation);

public:
	RootContactItem(bool has_arrow = true,QWidget *parent = nullptr);
	~RootContactItem();

public:
	void SetText(const QString& title);
	void SetExpanded(bool expand);
private:
	int rotation();
	void SetRotation(int rotation);
protected:
	void  paintEvent(QPaintEvent* event)override;

private:
	QPropertyAnimation* animation_;
	QString title_text_;   //文本
	int rotation_;		//箭头角度
	bool has_arrow_;	//箭头标志
};
