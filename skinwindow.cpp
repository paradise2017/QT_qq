#include "skinwindow.h"
#include "qclicklabel.h"
#include "notifymanager.h"
SkinWindow::SkinWindow(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	loadStyleSheet("SkinWindow");
	//当窗体收到关闭事件，就会关闭
	setAttribute(Qt::WA_DeleteOnClose);
	InitControl();
}

SkinWindow::~SkinWindow()
{}

void SkinWindow::InitControl(){		
	QList<QColor> colorlist = { QColor(22,154,218) ,QColor(40,138,221), QColor(49,166,107),QColor(218,67,68),
								QColor(177,99,158),QColor(107,81,92),QColor(89,92,160),QColor(21,156,199),
								QColor(79,169,172),QColor(155,183,154),QColor(128,77,77),QColor(240,188,189),
	};
	for (size_t row = 0; row < 3; row++){
		for (size_t column = 0; column < 4; column++){
			QClickLabel* label = new QClickLabel(this);
			//设置标题形状， 这里是继承父类
			label->setCursor(Qt::PointingHandCursor);
			//C++11新支持 lambda表达式
			connect(label, &QClickLabel::clicked, [=]() {                                //查找当前颜色
				NotifyManager::getInstance()->notifyOtherWindowChangeSkin(colorlist.at(row * 4 + column));
				});
			label->setFixedSize(84, 84);
			//设置背景
			QPalette palette;
			palette.setColor(QPalette::Background, colorlist.at(row * 4 + column));
			label->setAutoFillBackground(true);
			label->setPalette(palette);
			ui.gridLayout->addWidget(label, row, column);
		}
	}
	
	connect(ui.sysmin, &QPushButton::clicked, this, [this]() {
		onShowMin(true);
		});
	connect(ui.sysclose,&QPushButton::clicked,this,&BasicWindow::onShowMin);
}
void SkinWindow::onShowClose() {
	this->close();
}