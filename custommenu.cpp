#include "custommenu.h"
#include "commonUtils.h"
CustomMenu::CustomMenu(QWidget *parent)
	: QMenu(parent)
{
	//背景透明
	this->setAttribute(Qt::WA_TranslucentBackground);
	CommonUtils::loadStyleSheet(this,"Menu");
}
	
CustomMenu::~CustomMenu()
{}

void CustomMenu::AddCustomMenu(const QString & text, const QString & icon, const QString & name){
	QAction* action = addAction(QIcon(icon),name);
	menu_action_map.insert(text, action);
}

QAction* CustomMenu::GetAction(const QString& text)
{
	return menu_action_map[text];
}
