/********************************************************************************
** Form generated from reading UI file 'statisticwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STATISTICWINDOW_H
#define UI_STATISTICWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StatisticWindow
{
public:
    QComboBox *MonthBox;
    QTableView *StatisticTable;
    QComboBox *DayBox;
    QPushButton *QueryBtn;
    QComboBox *YearBox;
    QPushButton *QuitBtn;

    void setupUi(QWidget *StatisticWindow)
    {
        if (StatisticWindow->objectName().isEmpty())
            StatisticWindow->setObjectName(QStringLiteral("StatisticWindow"));
        StatisticWindow->resize(400, 300);
        MonthBox = new QComboBox(StatisticWindow);
        MonthBox->setObjectName(QStringLiteral("MonthBox"));
        MonthBox->setGeometry(QRect(130, 40, 69, 22));
        StatisticTable = new QTableView(StatisticWindow);
        StatisticTable->setObjectName(QStringLiteral("StatisticTable"));
        StatisticTable->setGeometry(QRect(60, 70, 256, 192));
        DayBox = new QComboBox(StatisticWindow);
        DayBox->setObjectName(QStringLiteral("DayBox"));
        DayBox->setGeometry(QRect(220, 40, 69, 22));
        QueryBtn = new QPushButton(StatisticWindow);
        QueryBtn->setObjectName(QStringLiteral("QueryBtn"));
        QueryBtn->setGeometry(QRect(300, 40, 75, 23));
        YearBox = new QComboBox(StatisticWindow);
        YearBox->setObjectName(QStringLiteral("YearBox"));
        YearBox->setGeometry(QRect(30, 40, 69, 22));
        QuitBtn = new QPushButton(StatisticWindow);
        QuitBtn->setObjectName(QStringLiteral("QuitBtn"));
        QuitBtn->setGeometry(QRect(300, 270, 75, 23));

        retranslateUi(StatisticWindow);

        QMetaObject::connectSlotsByName(StatisticWindow);
    } // setupUi

    void retranslateUi(QWidget *StatisticWindow)
    {
        StatisticWindow->setWindowTitle(QApplication::translate("StatisticWindow", "Form", Q_NULLPTR));
        QueryBtn->setText(QApplication::translate("StatisticWindow", "\346\237\245\350\257\242", Q_NULLPTR));
        QuitBtn->setText(QApplication::translate("StatisticWindow", "\351\200\200\345\207\272", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class StatisticWindow: public Ui_StatisticWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATISTICWINDOW_H
