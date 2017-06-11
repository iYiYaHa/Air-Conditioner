/********************************************************************************
** Form generated from reading UI file 'guestwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUESTWINDOW_H
#define UI_GUESTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GuestWindow
{
public:
    QPushButton *AddBtn;
    QPushButton *DelBtn;
    QTableView *tableView;
    QPushButton *QuitBtn;

    void setupUi(QWidget *GuestWindow)
    {
        if (GuestWindow->objectName().isEmpty())
            GuestWindow->setObjectName(QStringLiteral("GuestWindow"));
        GuestWindow->resize(400, 300);
        AddBtn = new QPushButton(GuestWindow);
        AddBtn->setObjectName(QStringLiteral("AddBtn"));
        AddBtn->setGeometry(QRect(50, 70, 75, 23));
        DelBtn = new QPushButton(GuestWindow);
        DelBtn->setObjectName(QStringLiteral("DelBtn"));
        DelBtn->setGeometry(QRect(170, 70, 75, 23));
        tableView = new QTableView(GuestWindow);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setGeometry(QRect(50, 100, 256, 192));
        QuitBtn = new QPushButton(GuestWindow);
        QuitBtn->setObjectName(QStringLiteral("QuitBtn"));
        QuitBtn->setGeometry(QRect(270, 60, 75, 23));

        retranslateUi(GuestWindow);

        QMetaObject::connectSlotsByName(GuestWindow);
    } // setupUi

    void retranslateUi(QWidget *GuestWindow)
    {
        GuestWindow->setWindowTitle(QApplication::translate("GuestWindow", "Form", Q_NULLPTR));
        AddBtn->setText(QApplication::translate("GuestWindow", "\345\242\236\345\212\240", Q_NULLPTR));
        DelBtn->setText(QApplication::translate("GuestWindow", "\345\210\240\351\231\244", Q_NULLPTR));
        QuitBtn->setText(QApplication::translate("GuestWindow", "\351\200\200\345\207\272", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class GuestWindow: public Ui_GuestWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUESTWINDOW_H
