/********************************************************************************
** Form generated from reading UI file 'sendfile.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SENDFILE_H
#define UI_SENDFILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SendFileClass
{
public:
    QWidget *title_Widget;
    QWidget *body_Widget;

    void setupUi(QWidget *SendFileClass)
    {
        if (SendFileClass->objectName().isEmpty())
            SendFileClass->setObjectName(QString::fromUtf8("SendFileClass"));
        SendFileClass->resize(519, 169);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SendFileClass->sizePolicy().hasHeightForWidth());
        SendFileClass->setSizePolicy(sizePolicy);
        SendFileClass->setMinimumSize(QSize(519, 169));
        SendFileClass->setMaximumSize(QSize(519, 619));
        title_Widget = new QWidget(SendFileClass);
        title_Widget->setObjectName(QString::fromUtf8("title_Widget"));
        title_Widget->setGeometry(QRect(0, 0, 519, 50));
        title_Widget->setProperty("titleskin", QVariant(true));
        body_Widget = new QWidget(SendFileClass);
        body_Widget->setObjectName(QString::fromUtf8("body_Widget"));
        body_Widget->setGeometry(QRect(0, 50, 519, 119));

        retranslateUi(SendFileClass);

        QMetaObject::connectSlotsByName(SendFileClass);
    } // setupUi

    void retranslateUi(QWidget *SendFileClass)
    {
        SendFileClass->setWindowTitle(QCoreApplication::translate("SendFileClass", "SendFile", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SendFileClass: public Ui_SendFileClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SENDFILE_H
