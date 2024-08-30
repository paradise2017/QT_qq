#include "qmsgtextedit.h"
#include <qmovie.h>
void QmsgTextEdit::AddEmotionUrl(int emotion_num){
	const QString& image_name = QString("qrc:/Resources/MainWindow/emotion/%1.png").arg(emotion_num);
	const QString& flagname = QString("%1").arg(image_name);
	this->insertHtml(QString("<img src='%1' />").arg(flagname));
	if (list_emotion_url_.contains(image_name)) {
		return;
	}
	else {
		list_emotion_url_.append(image_name);
	}
	QMovie* apng_movie = new QMovie(image_name, "apng", this);
	emotion_map_.insert(apng_movie, flagname);
	//数据改变发生信号
	connect(apng_movie, SIGNAL(frameChanged(int)),this, SLOT(OnEmotionImageFrameChange(int)));
	apng_movie->start();
	updateGeometry();

}

void QmsgTextEdit::DeleteAllEmotionImage(){
	for (auto iter = emotion_map_.constBegin();iter!= emotion_map_.constEnd();++iter)
	{
		delete iter.key();
	}
	emotion_map_.clear(); 
}

QmsgTextEdit::QmsgTextEdit(QWidget *parent)
	: QTextEdit(parent)
{
}

QmsgTextEdit::~QmsgTextEdit()
{
	DeleteAllEmotionImage();
}
void QmsgTextEdit::OnEmotionImageFrameChange(int frame) {
	//那个动画改变？
	QMovie* movie = qobject_cast<QMovie*>(sender());
	this->document()->addResource(QTextDocument::ImageResource,QUrl(emotion_map_.value(movie)),movie->currentPixmap());
}