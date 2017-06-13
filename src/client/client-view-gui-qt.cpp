#include "client-view-gui-qt.h"

#include "ui_authwindow.h"
#include "ui_controlwindow.h"

#include <iostream>

AuthWindow::AuthWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AuthWindow)
{
    ui->setupUi(this);
}

AuthWindow::~AuthWindow()
{
    delete ui;
}

void AuthWindow::on_LogOnBtn_clicked()
{
    try{
        QString roomId = ui->RoomId->text();
        QString guestId = ui->GuestId->text();
        if(_onAuth){
            _onAuth(Air_Conditioner::GuestInfo{roomId.toStdString(),
                                          guestId.toStdString()});
            this->close();
            //this->
        }
    }
    catch(std::exception &ex){
        std::cerr << ex.what() << std::endl;
        QMessageBox::warning(this,
                         QStringLiteral("登录失败"),
                         QString(ex.what()));
    }

}

void AuthWindow::on_QuitBtn_clicked()
{
    int ret = QMessageBox::information(this,QStringLiteral("退出？"),
                   QStringLiteral("确定退出从控机系统？"),QStringLiteral("确定"),
                                       QStringLiteral("取消"));
    if(ret == 1)
        this->close();
}

ControlWindow::ControlWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlWindow)
{
    ui->setupUi(this);
}

ControlWindow::~ControlWindow()
{
    delete ui;
}
