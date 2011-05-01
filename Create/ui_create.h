/********************************************************************************
** Form generated from reading UI file 'create.ui'
**
** Created: Sun May 1 00:13:29 2011
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATE_H
#define UI_CREATE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CreateClass
{
public:

    void setupUi(QWidget *CreateClass)
    {
        if (CreateClass->objectName().isEmpty())
            CreateClass->setObjectName(QString::fromUtf8("CreateClass"));
        CreateClass->resize(400, 300);

        retranslateUi(CreateClass);

        QMetaObject::connectSlotsByName(CreateClass);
    } // setupUi

    void retranslateUi(QWidget *CreateClass)
    {
        CreateClass->setWindowTitle(QApplication::translate("CreateClass", "Create", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CreateClass: public Ui_CreateClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATE_H
