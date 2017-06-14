#include "client-view-gui-qt.h"

#include "ui_authwindow.h"
#include "ui_controlwindow.h"

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
        }
    }
    catch(std::exception &ex){
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
    if(ret == 0)
        this->close();
}

ControlWindow::ControlWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlWindow)
{
    ui->setupUi(this);
    ui->LowWind->setChecked(true);
}

ControlWindow::~ControlWindow()
{
    delete ui;
}

void ControlWindow::Message(QString message)
{
    QMessageBox::information(this,QStringLiteral(""),message,QStringLiteral("确定"));
}

void ControlWindow::ShowState(Air_Conditioner::ServerInfo _serverInfo,
                              Air_Conditioner::ClientInfo _clientInfo,
                              Air_Conditioner::RoomRequest _roomRequest)
{
    ui->Cost->setText(QString::number(_clientInfo.cost));
    ui->Energy->setText(QString::number(_clientInfo.energy));

    if (!_serverInfo.isOn)
        ui->Wind->setText(QStringLiteral("主控机关机"));
    else if (!_clientInfo.hasWind)
        ui->Wind->setText(QStringLiteral("停止送风"));
    else{
        switch(_roomRequest.wind){
            case 0:
                ui->Wind->setText(QStringLiteral("停止送风"));
                break;
            case 1:
                ui->Wind->setText(QStringLiteral("低风速"));
                break;
            case 2:
                ui->Wind->setText(QStringLiteral("中风速"));
                break;
            case 3:
                ui->Wind->setText(QStringLiteral("高风速"));
                break;
            default:
                break;
        }
   }
   ui->CurrTemp->setText(QString::number(_roomRequest.current,'g',3));
   ui->TargetTemp->setText(QString::number(_roomRequest.target,'g',3));
}
void ControlWindow::LoadGuestInfo(Air_Conditioner::GuestInfo guest)
{
     ui->RoomId->setText(QString::fromStdString(guest.room));
}

void ControlWindow::on_StopWind_clicked()
{
    if(ui->StopWind->isChecked())
        _onWindChanged(Air_Conditioner::Wind{0});
}

void ControlWindow::on_LowWind_clicked()
{
    if(ui->LowWind->isChecked())
        _onWindChanged(Air_Conditioner::Wind{1});
    
}

void ControlWindow::on_MidWind_clicked()
{
    if(ui->MidWind->isChecked())
        _onWindChanged(Air_Conditioner::Wind{2});   
}

void ControlWindow::on_StrongWind_clicked()
{
    if(ui->StrongWind->isChecked())
        _onWindChanged(Air_Conditioner::Wind{3});
}

void ControlWindow::on_UpBtn_clicked()
{
    Air_Conditioner::Temperature temp = ui->TargetTemp->text().toDouble();
    temp += 0.1;
    ui->TargetTemp->setText(QString::number(temp,'g',3));
    _onTempChanged(Air_Conditioner::Temperature{temp});
}

void ControlWindow::on_DownBtn_clicked()
{
     Air_Conditioner::Temperature temp = ui->TargetTemp->text().toDouble();
     temp -= 0.1;
     ui->TargetTemp->setText(QString::number(temp,'g',3));
     _onTempChanged(Air_Conditioner::Temperature{temp});
}

void ControlWindow::on_QuitBtn_clicked()
{
    this->close();
}
