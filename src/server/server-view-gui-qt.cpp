#include "server-view-gui-qt.h"

#include "ui_welcomewindow.h"
#include "ui_statisticwindow.h"
#include "ui_clientwindow.h"
#include "ui_configwindow.h"
#include "ui_guestwindow.h"

GuestWindow::GuestWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GuestWindow)
{
    ui->setupUi(this);
}

GuestWindow::~GuestWindow()
{
    delete ui;
}

void GuestWindow::closeEvent(QCloseEvent * event){
    if(_onBack) _onBack();
    else{
        event->accept();
    }
}

ConfigWindow::ConfigWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigWindow)
{
    ui->setupUi(this);
}

ConfigWindow::~ConfigWindow()
{
    delete ui;
}

void ConfigWindow::closeEvent(QCloseEvent *event){
    if(_onBack)
        _onBack();
    else{
        event->accept();
    }
}

ClientWindow::ClientWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientWindow)
{
    ui->setupUi(this);
}

ClientWindow::~ClientWindow()
{
    delete ui;
}

void ClientWindow::closeEvent(QCloseEvent *event){
    if(_onBack)
        _onBack();
    else{
        event->accept();
    }
}

StatisticWindow::StatisticWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticWindow)
{
    ui->setupUi(this);
}

StatisticWindow::~StatisticWindow()
{
    delete ui;
}

void StatisticWindow::closeEvent(QCloseEvent * event){
    if(_onBack)
        _onBack();
    else{
        event->accept();
    }
}

WelcomeWindow::WelcomeWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomeWindow)
{
    ui->setupUi(this);
}

WelcomeWindow::~WelcomeWindow()
{
    delete ui;
}

void WelcomeWindow::on_QuitBtn_clicked()
{
    if(_onQuit) _onQuit();
}

void WelcomeWindow::on_ConfigBtn_clicked()
{
    if(_onConfig) _onConfig();
}

void WelcomeWindow::on_GuestBtn_clicked()
{
    if(_onGuest) _onGuest();
}

void WelcomeWindow::on_StatisticBtn_clicked()
{
    if(_onLog) _onLog();
}

void WelcomeWindow::on_ClientBtn_clicked()
{
    if(_onClient) _onClient();
}
void ConfigWindow::ShowConfig(){

   if(_config.isOn)
       ui->OnBtn->setChecked(true);
   else
       ui->OffBtn->setChecked(true);

   if(_config.mode == 0)
       ui->SummerBtn->setChecked(true);
   else
       ui->WinterBtn->setChecked(true);
}

void ConfigWindow::on_ConfirmBtn_clicked()
{
    _config.isOn = ui->OnBtn->isChecked();
    _config.mode = ui->SummerBtn->isChecked() == true ? 0 : 1;
    SetConfig();
    this->close();
    _onBack();

}

void ConfigWindow::on_CancelBtn_clicked()
{
    this->close();
    _onBack();
}
