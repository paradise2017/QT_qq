#pragma once

#include <QTextEdit>

class QmsgTextEdit  : public QTextEdit
{
	Q_OBJECT
public slots:
	void OnEmotionImageFrameChange(int frame);
	void AddEmotionUrl(int emotion_num);
	void DeleteAllEmotionImage();
public:
	QmsgTextEdit(QWidget* parent =nullptr);
	~QmsgTextEdit();

private:
	QList<QString> list_emotion_url_;
	QMap<QMovie*, QString> emotion_map_;
};
