#include "emotionlabelitem.h"
#include <QMovie>
EmotionLabelItem::EmotionLabelItem(QWidget *parent)
	: QClickLabel(parent)
{
	InitControl();
	connect(this, &QClickLabel::clicked, [this]() {
		emit EmotionClicked(emotion_name_);
		});
}

EmotionLabelItem::~EmotionLabelItem()
{}

void EmotionLabelItem::SetEmotionName(int emotion_name)
{
	emotion_name_ = emotion_name;
	QString image_name = QString(":/Resources/MainWindow/emotion/%1.png").arg(emotion_name);
	png_movie_ = new QMovie(image_name,"apng",this);
	png_movie_->start();
	png_movie_->stop();
	this->setMovie(png_movie_);
}

void EmotionLabelItem::InitControl()
{
	this->setAlignment(Qt::AlignCenter);
	this->setObjectName("emotionLabelItem");
	this->setFixedSize(32,32);

}
