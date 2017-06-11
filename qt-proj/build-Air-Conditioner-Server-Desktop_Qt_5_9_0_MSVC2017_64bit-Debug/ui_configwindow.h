/********************************************************************************
** Form generated from reading UI file 'configwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGWINDOW_H
#define UI_CONFIGWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConfigWindow
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *StateText;
    QGroupBox *groupBox;
    QRadioButton *OnBtn;
    QRadioButton *OffBtn;
    QHBoxLayout *horizontalLayout_5;
    QLabel *StateText_2;
    QGroupBox *groupBox_3;
    QRadioButton *SummerBtn;
    QRadioButton *WinterBtn;
    QHBoxLayout *horizontalLayout_4;
    QLabel *PulseText;
    QSpinBox *spinBox;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *ConfirmBtn;
    QPushButton *CancelBtn;

    void setupUi(QWidget *ConfigWindow)
    {
        if (ConfigWindow->objectName().isEmpty())
            ConfigWindow->setObjectName(QStringLiteral("ConfigWindow"));
        ConfigWindow->resize(400, 300);
        verticalLayoutWidget = new QWidget(ConfigWindow);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(60, 40, 254, 131));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        StateText = new QLabel(verticalLayoutWidget);
        StateText->setObjectName(QStringLiteral("StateText"));

        horizontalLayout->addWidget(StateText);

        groupBox = new QGroupBox(verticalLayoutWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setStyleSheet(QLatin1String("border:None\n"
""));
        OnBtn = new QRadioButton(groupBox);
        OnBtn->setObjectName(QStringLiteral("OnBtn"));
        OnBtn->setGeometry(QRect(0, 10, 80, 16));
        OnBtn->setChecked(true);
        OnBtn->setAutoExclusive(true);
        OffBtn = new QRadioButton(groupBox);
        OffBtn->setObjectName(QStringLiteral("OffBtn"));
        OffBtn->setGeometry(QRect(60, 10, 122, 16));
        OffBtn->setChecked(false);
        OffBtn->setAutoExclusive(true);

        horizontalLayout->addWidget(groupBox);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setSizeConstraint(QLayout::SetDefaultConstraint);
        StateText_2 = new QLabel(verticalLayoutWidget);
        StateText_2->setObjectName(QStringLiteral("StateText_2"));

        horizontalLayout_5->addWidget(StateText_2);

        groupBox_3 = new QGroupBox(verticalLayoutWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setStyleSheet(QStringLiteral("border:none"));
        SummerBtn = new QRadioButton(groupBox_3);
        SummerBtn->setObjectName(QStringLiteral("SummerBtn"));
        SummerBtn->setGeometry(QRect(0, 10, 51, 16));
        SummerBtn->setChecked(true);
        WinterBtn = new QRadioButton(groupBox_3);
        WinterBtn->setObjectName(QStringLiteral("WinterBtn"));
        WinterBtn->setGeometry(QRect(60, 10, 61, 16));
        WinterBtn->setAutoExclusive(true);

        horizontalLayout_5->addWidget(groupBox_3);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        PulseText = new QLabel(verticalLayoutWidget);
        PulseText->setObjectName(QStringLiteral("PulseText"));

        horizontalLayout_4->addWidget(PulseText);

        spinBox = new QSpinBox(verticalLayoutWidget);
        spinBox->setObjectName(QStringLiteral("spinBox"));

        horizontalLayout_4->addWidget(spinBox);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayoutWidget_2 = new QWidget(ConfigWindow);
        horizontalLayoutWidget_2->setObjectName(QStringLiteral("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(70, 190, 235, 80));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        ConfirmBtn = new QPushButton(horizontalLayoutWidget_2);
        ConfirmBtn->setObjectName(QStringLiteral("ConfirmBtn"));

        horizontalLayout_2->addWidget(ConfirmBtn);

        CancelBtn = new QPushButton(horizontalLayoutWidget_2);
        CancelBtn->setObjectName(QStringLiteral("CancelBtn"));

        horizontalLayout_2->addWidget(CancelBtn);


        retranslateUi(ConfigWindow);

        QMetaObject::connectSlotsByName(ConfigWindow);
    } // setupUi

    void retranslateUi(QWidget *ConfigWindow)
    {
        ConfigWindow->setWindowTitle(QApplication::translate("ConfigWindow", "Form", Q_NULLPTR));
        StateText->setText(QApplication::translate("ConfigWindow", "    \345\274\200\345\205\263\346\234\272", Q_NULLPTR));
        groupBox->setTitle(QString());
        OnBtn->setText(QApplication::translate("ConfigWindow", "\345\274\200\346\234\272", Q_NULLPTR));
        OffBtn->setText(QApplication::translate("ConfigWindow", "\345\205\263\346\234\272", Q_NULLPTR));
        StateText_2->setText(QApplication::translate("ConfigWindow", "   \345\267\245\344\275\234\346\250\241\345\274\217", Q_NULLPTR));
        groupBox_3->setTitle(QString());
        SummerBtn->setText(QApplication::translate("ConfigWindow", "\345\244\217\345\244\251", Q_NULLPTR));
        WinterBtn->setText(QApplication::translate("ConfigWindow", "\345\206\254\345\244\251", Q_NULLPTR));
        PulseText->setText(QApplication::translate("ConfigWindow", "\344\273\216\346\216\247\346\234\272\345\277\203\350\267\263\351\242\221\347\216\207", Q_NULLPTR));
        ConfirmBtn->setText(QApplication::translate("ConfigWindow", "\347\241\256\345\256\232", Q_NULLPTR));
        CancelBtn->setText(QApplication::translate("ConfigWindow", "\345\217\226\346\266\210", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ConfigWindow: public Ui_ConfigWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGWINDOW_H
