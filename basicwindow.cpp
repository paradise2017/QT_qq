#include "basicwindow.h"
#include <QFile>
#include <QStyleOption>
#include <QPainter>
#include <QApplication>
#include <QMouseEvent>
#include <QDesktopWidget>
#include "commonUtils.h"
#include "NotifyManager.h"
BasicWindow::BasicWindow(QWidget *parent)
	: QDialog(parent)
{
	this->setWindowFlag(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground,true);    //透明效果
	m_colorBackGround = CommonUtils::getDefaultSkinColor();
	connect(NotifyManager::getInstance(), SIGNAL(signalSkinChanged(const QColor&)),this,SLOT(onSignalSkinChanged(const QColor&)));
}

BasicWindow::~BasicWindow()
{
}

void BasicWindow::loadStyleSheet(const QString& sheetName)
{
	m_styleName = sheetName;
	QFile file(":/Resources/QSS/" + sheetName + ".css");
	file.open(QFile::ReadOnly);
	if (file.isOpen()) {
		this->setStyleSheet("");
		QString qssstyleSheet = QLatin1String(file.readAll());
		//获取当前皮肤的RGB值
		QString r = QString::number(m_colorBackGround.red());
		QString g = QString::number(m_colorBackGround.green());
		QString b = QString::number(m_colorBackGround.blue());

		qssstyleSheet += QString("QWidget[titleskin=true]\
								{background-color:rgb(%1,%2,%3);\
								border-top-left-radius:4px;}\
								QWidget[bottomskin=true]\
								{border-top:1px solid rgba(%1,%2,%3,100);background-color:rgba(%1,%2,%3,50);\
								border-bottom-left-radius:4px;\
								border-bottom-right-radius:4px;}")
			.arg(r).arg(g).arg(b);
		this->setStyleSheet(qssstyleSheet);
	}
	file.close();
}
//获取头像转圆头像									//更改图像		初始空头像  初始头像大小			
QPixmap BasicWindow::getRoundImage(const QPixmap& src, QPixmap& mask, QSize masksize)
{
	if (masksize ==QSize(0,0)) {
		masksize = mask.size();
	}
	else {						//最大化的缩放			//平滑转换
		mask = mask.scaled(masksize,Qt::KeepAspectRatio,Qt::SmoothTransformation);
	}
	//保存转换后的图像，清晰头像获取
	QImage resultImage(masksize,QImage::Format_ARGB32_Premultiplied);
	//画家画 resultImage
	QPainter painter(&resultImage);
	//叠加模式  3种
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(resultImage.rect(),Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawPixmap(0,0,mask);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.drawPixmap(0, 0, src.scaled(masksize,Qt::KeepAspectRatio, Qt::SmoothTransformation));
	painter.end();
	return QPixmap::fromImage(resultImage);
}
//初始化背景图
void BasicWindow::initBackGroundColor()
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
}
//子类部件初始化，需要重写绘图事件设置背景图
void BasicWindow::paintEvent(QPaintEvent* event)
{
	initBackGroundColor();
	QDialog::paintEvent(event);
}
//鼠标按下事件
void BasicWindow::mousePressEvent(QMouseEvent* event)
{
	if (event->button() && Qt::LeftButton) {
		m_mousePressed = true;
		//全局坐标  pos 相对于窗口左上角的坐标
		m_mousePoint = event->globalPos() - pos();
		event->accept();
	}
}
//鼠标移动事件
void BasicWindow::mouseMoveEvent(QMouseEvent* event)
{

	if (m_mousePressed == true && (event->buttons() & Qt::LeftButton)) {
		
		this->move(event->globalPos() - m_mousePoint);
		event->accept();
	}
}

void BasicWindow::mouseReleaseEvent(QMouseEvent* event)
{
	m_mousePressed = false; 
}

void BasicWindow::onSignalSkinChanged(const QColor& color)
{
	m_colorBackGround = color;
	this->loadStyleSheet(m_styleName);

}

void BasicWindow::onButtonMinClicked()
{
	//判断是否为工具风格
	if (Qt::Tool == (windowFlags() & Qt::Tool)) {
		this->hide();
	}
	else {
		this->showMinimized();
	}
}

void BasicWindow::onButtonRestoreClicked()
{
	QPoint windowPos;
	QSize windowSize;
	//获取窗口栏的大小及信息
	_titleBar->getRestoreInfo(windowPos,windowSize);
	this->setGeometry(QRect(windowPos, windowSize));
}

void BasicWindow::onButtonMaxClicked()
{
	//宽度及高度
	_titleBar->saveRestoreInfo(pos(), QSize(width(), height()));
	QRect desktopRect = QApplication::desktop()->availableGeometry();
	QRect factrect = QRect(desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6);
	this->setGeometry(factrect); 
}

void BasicWindow::onButtonCloseClicked()
{
	this->close();
}

void BasicWindow::initTitleBar(ButtonType buttontype)
{
	_titleBar = new TitleBar(this);
	_titleBar->setButtonType(buttontype);
	_titleBar->move(0, 0);
	connect(_titleBar,&TitleBar::signalButtonCloseClicked,this,&BasicWindow::onButtonCloseClicked);
	connect(_titleBar, &TitleBar::signalButtonRestoreClicked, this, &BasicWindow::onButtonRestoreClicked);
	connect(_titleBar, &TitleBar::signalButtonMaxClicked, this, &BasicWindow::onButtonMaxClicked);
	connect(_titleBar, &TitleBar::signalButtonMinClicked, this, &BasicWindow::onButtonMinClicked); 
}

void BasicWindow::setTitleBarTitle(const QString& title, const QString& icon)
{
	_titleBar->setTitleIcon(icon);
	_titleBar->setTitleContent(title);
}

void BasicWindow::onShowMin(bool)
{
	showMinimized();
}

void BasicWindow::onShowHide(bool)
{
	this->hide();
}

void BasicWindow::onShowNormal(bool)
{
	this->show();
	this->activateWindow();
}

void BasicWindow::onShowQuit(bool)
{
	QApplication::quit();
}

void BasicWindow::onShowClose(bool) {
	this->close();
}
