/********************************************************************************
** Form generated from reading UI file 'welcomewindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WELCOMEWINDOW_H
#define UI_WELCOMEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WelcomeWindow
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *ConfigBtn;
    QPushButton *GuestBtn;
    QPushButton *StatisticBtn;
    QPushButton *ClientBtn;
    QPushButton *QuitBtn;

    void setupUi(QWidget *WelcomeWindow)
    {
        if (WelcomeWindow->objectName().isEmpty())
            WelcomeWindow->setObjectName(QStringLiteral("WelcomeWindow"));
        WelcomeWindow->resize(400, 300);
        verticalLayoutWidget = new QWidget(WelcomeWindow);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(110, 80, 160, 141));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        ConfigBtn = new QPushButton(verticalLayoutWidget);
        ConfigBtn->setObjectName(QStringLiteral("ConfigBtn"));

        verticalLayout->addWidget(ConfigBtn);

        GuestBtn = new QPushButton(verticalLayoutWidget);
        GuestBtn->setObjectName(QStringLiteral("GuestBtn"));

        verticalLayout->addWidget(GuestBtn);

        StatisticBtn = new QPushButton(verticalLayoutWidget);
        StatisticBtn->setObjectName(QStringLiteral("StatisticBtn"));

        verticalLayout->addWidget(StatisticBtn);

        ClientBtn = new QPushButton(verticalLayoutWidget);
        ClientBtn->setObjectName(QStringLiteral("ClientBtn"));

        verticalLayout->addWidget(ClientBtn);

        QuitBtn = new QPushButton(verticalLayoutWidget);
        QuitBtn->setObjectName(QStringLiteral("QuitBtn"));

        verticalLayout->addWidget(QuitBtn);


        retranslateUi(WelcomeWindow);

        QMetaObject::connectSlotsByName(WelcomeWindow);
    } // setupUi

    void retranslateUi(QWidget *WelcomeWindow)
    {
        WelcomeWindow->setWindowTitle(QApplication::translate("WelcomeWindow", "WelcomeWindow", Q_NULLPTR));
        ConfigBtn->setText(QApplication::translate("WelcomeWindow", "\351\205\215\347\275\256\347\256\241\347\220\206", Q_NULLPTR));
        GuestBtn->setText(QApplication::translate("WelcomeWindow", "\346\210\277\345\256\242\347\256\241\347\220\206", Q_NULLPTR));
        StatisticBtn->setText(QApplication::translate("WelcomeWindow", "\346\212\245\350\241\250", Q_NULLPTR));
        ClientBtn->setText(QApplication::translate("WelcomeWindow", "\344\273\216\346\216\247\346\234\272\347\233\221\346\216\247", Q_NULLPTR));
        QuitBtn->setText(QApplication::translate("WelcomeWindow", "\351\200\200\345\207\272", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class WelcomeWindow: public Ui_WelcomeWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WELCOMEWINDOW_H
