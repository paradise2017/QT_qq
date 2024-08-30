#pragma once

#include <QWidget>
#include "ui_emotionwindow.h"

class EmotionWindow : public QWidget
{
	Q_OBJECT

public:
	EmotionWindow(QWidget *parent = nullptr);
	~EmotionWindow();

private:
	void InitControl();

signals:
	void SignalEmotionWindowHide();
	void SignalEmotionItemClicked(int emotion_num);

private:
	void paintEvent(QPaintEvent* event) override;
private slots: 
	void AddEmotion(int emotion_num);


private:
	Ui::EmotionWindowClass ui;
};
