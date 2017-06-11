/********************************************************************************
** Form generated from reading UI file 'logwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGWINDOW_H
#define UI_LOGWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LogWindow
{
public:
    QTableView *LogTable;
    QPushButton *QuitBtn;

    void setupUi(QWidget *LogWindow)
    {
        if (LogWindow->objectName().isEmpty())
            LogWindow->setObjectName(QStringLiteral("LogWindow"));
        LogWindow->resize(400, 300);
        LogTable = new QTableView(LogWindow);
        LogTable->setObjectName(QStringLiteral("LogTable"));
        LogTable->setGeometry(QRect(50, 80, 256, 192));
        QuitBtn = new QPushButton(LogWindow);
        QuitBtn->setObjectName(QStringLiteral("QuitBtn"));
        QuitBtn->setGeometry(QRect(290, 40, 75, 23));

        retranslateUi(LogWindow);

        QMetaObject::connectSlotsByName(LogWindow);
    } // setupUi

    void retranslateUi(QWidget *LogWindow)
    {
        LogWindow->setWindowTitle(QApplication::translate("LogWindow", "Form", Q_NULLPTR));
        QuitBtn->setText(QApplication::translate("LogWindow", "\351\200\200\345\207\272", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class LogWindow: public Ui_LogWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGWINDOW_H
