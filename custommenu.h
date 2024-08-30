#pragma once

#include <QMenu>
#include <QMap>
class CustomMenu  : public QMenu
{
	Q_OBJECT

public:
	CustomMenu(QWidget *parent = nullptr);
	~CustomMenu();

public:
	void AddCustomMenu(const QString& text,const QString& icon, const QString& name);
	QAction* GetAction(const QString& text);
private:
	QMap<QString, QAction*>menu_action_map;
};
