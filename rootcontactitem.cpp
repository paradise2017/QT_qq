#include "rootcontactitem.h"
#include <QPainter>
RootContactItem::RootContactItem(bool has_arrow,QWidget *parent)
	: QLabel(parent),
	rotation_(0),
	has_arrow_(has_arrow){
	this->setFixedHeight(32);
	//设置属性
	this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	//属性动画初始化
	animation_ = new QPropertyAnimation(this,"rotation");
	//设置动画时长
	animation_->setDuration(100);   //100ms
	//动画曲率
	animation_->setEasingCurve(QEasingCurve::InQuad);

}

RootContactItem::~RootContactItem()
{}

void RootContactItem::SetText(const QString & title)
{
	title_text_ = title;
	update();
}

void RootContactItem::SetExpanded(bool expand)
{
	if (expand) {
		//如果展开，箭头旋转90度  ,设置属性动画的结束值
		animation_->setEndValue(90);
	}
	else {
		animation_->setEndValue(0);
	}
	animation_->start();
}

int RootContactItem::rotation()
{
	return rotation_;
}

void RootContactItem::SetRotation(int rotation)
{
	rotation_ = rotation;
	update();
}

void RootContactItem::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	//文本反锯齿
	painter.setRenderHint(QPainter::TextAntialiasing,true);
	QFont font;
	font.setPointSize(10);
	painter.setFont(font);
	painter.drawText(24,0,width()-24,height(),Qt::AlignLeft|Qt::AlignVCenter,title_text_);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
	//保存画家设置
	painter.save();
	if (has_arrow_) {
		QPixmap pixmap;
		pixmap.load(":/Resources/MainWindow/arrow.png");

		QPixmap temp_pixmap(pixmap.size());
		temp_pixmap.fill(Qt::transparent);
		QPainter temp_painter(&temp_pixmap);
		temp_painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
		//坐标系的移动到图片的中间 
		temp_painter.translate(pixmap.width()/2,pixmap.height()/2);
		//坐标系旋转(顺时针)
		temp_painter.rotate(rotation_);
		temp_painter.drawPixmap(0-pixmap.width()/2,0-pixmap.height()/2,pixmap);

		painter.drawPixmap(6,(pixmap.height()-3),temp_pixmap);
		
		//恢复画家设置  对应save
		painter.restore();
	}
	QLabel::paintEvent(event);
}
