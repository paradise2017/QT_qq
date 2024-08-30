/********************************************************************************
** Form generated from reading UI file 'contactitem.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTACTITEM_H
#define UI_CONTACTITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ContactItemClass
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QLabel *userName;
    QLabel *signName;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *ContactItemClass)
    {
        if (ContactItemClass->objectName().isEmpty())
            ContactItemClass->setObjectName(QString::fromUtf8("ContactItemClass"));
        ContactItemClass->resize(483, 54);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ContactItemClass->sizePolicy().hasHeightForWidth());
        ContactItemClass->setSizePolicy(sizePolicy);
        ContactItemClass->setMinimumSize(QSize(0, 54));
        ContactItemClass->setMaximumSize(QSize(16777215, 54));
        horizontalLayout = new QHBoxLayout(ContactItemClass);
        horizontalLayout->setSpacing(12);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(6, 0, 6, 0);
        label = new QLabel(ContactItemClass);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setMinimumSize(QSize(40, 40));
        label->setMaximumSize(QSize(40, 40));

        horizontalLayout->addWidget(label);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer = new QSpacerItem(98, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        userName = new QLabel(ContactItemClass);
        userName->setObjectName(QString::fromUtf8("userName"));

        verticalLayout->addWidget(userName);

        signName = new QLabel(ContactItemClass);
        signName->setObjectName(QString::fromUtf8("signName"));

        verticalLayout->addWidget(signName);

        verticalSpacer_2 = new QSpacerItem(98, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(ContactItemClass);

        QMetaObject::connectSlotsByName(ContactItemClass);
    } // setupUi

    void retranslateUi(QWidget *ContactItemClass)
    {
        ContactItemClass->setWindowTitle(QCoreApplication::translate("ContactItemClass", "ContactItem", nullptr));
        label->setText(QString());
        userName->setText(QString());
        signName->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ContactItemClass: public Ui_ContactItemClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTACTITEM_H
