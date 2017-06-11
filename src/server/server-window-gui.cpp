#include "server-window-gui.h"

#include "ui_welcomewindow.h"
#include "ui_statisticwindow.h"
#include "ui_logwindow.h"
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
    if(_onBack) _onBack();
}

LogWindow::LogWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogWindow)
{
    ui->setupUi(this);
}

LogWindow::~LogWindow()
{
    delete ui;
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
    if(_onStatistic) _onStatistic();
}

void WelcomeWindow::on_ClientBtn_clicked()
{
    if(_onClient) _onClient();
}
