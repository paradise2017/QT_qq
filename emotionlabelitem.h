#pragma once
#include "qclicklabel.h"
class QMovie;

class EmotionLabelItem  : public QClickLabel
{
	Q_OBJECT

public:
	EmotionLabelItem(QWidget *parent);
	~EmotionLabelItem();
	void SetEmotionName(int emotion_name);
private:
	void InitControl();
	int emotion_name_;
	QMovie* png_movie_;
signals:
	void EmotionClicked(int emotion_num);

};
