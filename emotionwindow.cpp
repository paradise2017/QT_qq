#include "emotionwindow.h"
#include "commonUtils.h"
#include "emotionlabelitem.h"
#include <QStyleOption>
#include <qpainter.h>
const int emotion_row = 12;
const int emotion_col = 14;
EmotionWindow::EmotionWindow(QWidget *parent)
	: QWidget(parent)
{

	//子窗口和透明属性,资源回收
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setAttribute(Qt::WA_DeleteOnClose);
	ui.setupUi(this);
	InitControl();
}

EmotionWindow::~EmotionWindow()
{}

void EmotionWindow::InitControl()
{
	CommonUtils::loadStyleSheet(this, "EmotionWindow");
	for (size_t row = 0; row < emotion_row; row++)
	{
		for (size_t col = 0; col < emotion_col; col++)
		{
			//添加表情
			EmotionLabelItem* label = new EmotionLabelItem(this);
			label->SetEmotionName(row*col+col);
			connect(label, &EmotionLabelItem::EmotionClicked, this, &EmotionWindow::AddEmotion);
			ui.gridLayout->addWidget(label,row,col);
		}
	}
}

void EmotionWindow::AddEmotion(int emotion_num) {
	this->hide();
	emit SignalEmotionWindowHide();
	emit SignalEmotionItemClicked(emotion_num);
}

void EmotionWindow::paintEvent(QPaintEvent* event) {
	QStyleOption opt;
	opt.init(this);

	QPainter painter(this);

	this->style()->drawPrimitive(QStyle::PE_Widget,&opt,&painter,this);
	__super::paintEvent(event);
}
