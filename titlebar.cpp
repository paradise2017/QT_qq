#include "titlebar.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QEvent>
#include <QMouseEvent>
#include <QFile>
#define BUTTON_HEIGHT 27 //按钮高度
#define BUTTON_WIDTH 27 //按钮高度
#define TITLE_HEIGHT 27 //标题栏高度

TitleBar::TitleBar(QWidget* parent)
	: QWidget(parent),
	m_isPressed(false),
	m_buttonType(MIN_MAX_BUTTON)
{
	initControl();
	initConnections();
	loadStyleSheet("Title");

}

TitleBar::~TitleBar()
{
}
//设置标题栏图标
void TitleBar::setTitleIcon(const QString& filePath)
{
	QPixmap titleIcon(filePath);
	m_pIcon->setFixedSize(titleIcon.size());
	m_pIcon->setPixmap(titleIcon);
}
//设置标题栏内容
void TitleBar::setTitleContent(const QString& titleConent)
{
	m_pTitleContent->setText(titleConent);
	m_titleContent = titleConent;
}
//设置标题栏长度
void TitleBar::setTitleWidth(int width)
{
	this->setFixedWidth(width);
}
//设置标题栏按钮类型
void TitleBar::setButtonType(ButtonType buttonType)
{
	m_buttonType = buttonType;
	switch (m_buttonType)
	{
	case MIN_BUTTON:
		m_pButtonRestore->setVisible(false);
		m_pButtonMax->setVisible(false);
		break;
	case MIN_MAX_BUTTON:
		m_pButtonRestore->setVisible(false);
		break;
	case ONLY_CLOSE_BUTTON:
		m_pButtonRestore->setVisible(false);
		m_pButtonMax->setVisible(false);
		m_pButtonMin->setVisible(false);
		break;
	default:
		break;
	}
}
//保存窗口最大化前窗口的大小和位置
void TitleBar::saveRestoreInfo(const QPoint& point, const QSize& size)
{
	m_restorePos = point;
	m_restoreSize = size;
}
//获取窗口最大化前窗口大小和位置
void TitleBar::getRestoreInfo(QPoint& point, QSize& size)
{
	point = m_restorePos;
	size = m_restoreSize;
}
//绘制标题栏
void TitleBar::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QPainterPath pathBack;
	pathBack.setFillRule(Qt::WindingFill);  //设置填充规则
	pathBack.addRoundedRect(QRect(0, 0, width(), height()),3,3);  //添加圆角矩形到绘图路径

	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
	//当窗口最大化，或者还原，窗口长度改变，标题栏相应改变   parentWidget()返回父部件
	if (width() != parentWidget()->width())
	{
		this->setFixedWidth(parentWidget()->width());
	}
	QWidget::paintEvent(event);
}
//双击响应事件，实现标题栏最大化最小化的操作
void TitleBar::mouseDoubleClickEvent(QMouseEvent* event)
{
	//只有最大化，最小化按钮才有效
	if (m_buttonType == MIN_MAX_BUTTON)
	{
		if (m_pButtonMax->isVisible())
		{
			onButtonMaxClicked();
		}
		else {
			onButtonRestoreClicked();
		}
	}
	return QWidget::mouseDoubleClickEvent(event);
}

//通过鼠标按下，鼠标移动，鼠标释放，实现拖动标题栏，达到移动窗口效果
void TitleBar::mousePressEvent(QMouseEvent* event)
{
	if (m_buttonType == MIN_MAX_BUTTON)
	{
		//窗口最大化禁止拖动
		if (m_pButtonMax->isVisible())
		{
			m_isPressed = true;
			m_startMovePos = event->globalPos();  //globalPos 返回事件发生时，鼠标所在的全局位置。
		}
	}
	else {
		m_isPressed = true;
		m_startMovePos = event->globalPos();
	}
	return QWidget::mousePressEvent(event);
}

void TitleBar::mouseMoveEvent(QMouseEvent* event)
{
	if (m_isPressed = true)
	{
		QPoint movePoint = event->globalPos() - m_startMovePos;
		QPoint widgetPos = parentWidget()->pos();
		m_startMovePos = event->globalPos();
		parentWidget()->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
	}
	return QWidget::mouseMoveEvent(event);
}

void TitleBar::mouseReleaseEvent(QMouseEvent* event)
{
	m_isPressed = false;
	return QWidget::mouseReleaseEvent(event);
}

//初始化控件
void TitleBar::initControl()
{
	m_pIcon = new QLabel(this);
	m_pTitleContent = new QLabel(this);

	m_pButtonMin = new QPushButton(this);
	m_pButtonRestore = new QPushButton(this);
	m_pButtonMax = new QPushButton(this);
	m_pButtonClose = new QPushButton(this);
	//设置按键大小
	m_pButtonMin->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonRestore->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonMax->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonClose->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	//设置对象名
	m_pTitleContent->setObjectName("TitleContent");
	m_pButtonMin->setObjectName("ButtonMin");
	m_pButtonRestore->setObjectName("ButtonRestore");
	m_pButtonMax->setObjectName("ButtonMax");
	m_pButtonClose->setObjectName("ButtonClose");

	//设置布局
	QHBoxLayout* mylayout = new QHBoxLayout(this);
	mylayout->addWidget(m_pIcon);
	mylayout->addWidget(m_pTitleContent);
	mylayout->addWidget(m_pButtonMin);
	mylayout->addWidget(m_pButtonRestore);
	mylayout->addWidget(m_pButtonMax);
	mylayout->addWidget(m_pButtonClose);

	mylayout->setContentsMargins(5, 0, 0, 0);  //设置布局的间隙
	mylayout->setSpacing(0);                   //设置布局里部件间的间隙
	//标题的尺寸测量  水平扩展， 垂直固定
	m_pTitleContent->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	this->setFixedHeight(TITLE_HEIGHT);
	this->setWindowFlags(Qt::FramelessWindowHint);  //无边框
}

void TitleBar::initConnections()
{
	connect(m_pButtonMin,&QPushButton::clicked,this,&TitleBar::onButtonMinClicked);
	connect(m_pButtonRestore,&QPushButton::clicked,this, &TitleBar::onButtonRestoreClicked);
	connect(m_pButtonMax,&QPushButton::clicked,this, &TitleBar::onButtonMaxClicked);
	connect(m_pButtonClose,&QPushButton::clicked,this, &TitleBar::onButtonCloseClicked);
}

void TitleBar::loadStyleSheet(const QString& sheetName)
{
	QFile file(":/Resources/QSS/" + sheetName + ".css");
	file.open(QFile::ReadOnly); 
	if (file.isOpen()) {
		QString styleSheet = this->styleSheet();
		styleSheet += QLatin1String(file.readAll());
		this->setStyleSheet(styleSheet);
	}
}


void TitleBar::onButtonRestoreClicked()
{
	m_pButtonRestore->setVisible(false);
	m_pButtonMax->setVisible(true);
	emit signalButtonRestoreClicked();
}

void TitleBar::onButtonMaxClicked()
{
	m_pButtonMax->setVisible(false);
	m_pButtonRestore->setVisible(true);
	emit signalButtonMaxClicked();
}

void TitleBar::onButtonCloseClicked() {
	emit signalButtonCloseClicked();
}
void TitleBar::onButtonMinClicked() {
	emit signalButtonMinClicked();
}
