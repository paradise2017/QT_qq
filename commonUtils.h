#pragma once
#include <QPixmap>
#include <QSize>
#include <QProxyStyle>

//改变默认的部件风格  例如默认的边框属性   
class CustomProxyStyle :public QProxyStyle 
{
public:
	CustomProxyStyle(QObject* parent) 
	{
		this->setParent(parent);
	}
	virtual void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter,
		const QWidget* widget = 0) const 
	{
		if (PE_FrameFocusRect == element) {
			//去掉Windows中部件默认的边框或虚线宽 ，  部件获取焦点时，直接返回  不进行绘制
			return;
		}
		else {
			QProxyStyle::drawPrimitive(element, option, painter, widget);
		};
	};
};
class CommonUtils
{
public:
	CommonUtils();
public:
	static QPixmap getRoundImage(const QPixmap& src, QPixmap& mask, QSize masksize = QSize(0, 0));
	static void loadStyleSheet(QWidget* widget, const QString& sheetName);
	static void setDefaultSkinColor(const QColor& color);
	static QColor getDefaultSkinColor();
};
