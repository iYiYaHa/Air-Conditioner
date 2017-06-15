﻿#ifndef CLIENTVIEWGUI_H
#define CLIENTVIEWGUI_H

#include <QWidget>
#include <QCloseEvent>
#include <QMessageBox>
#include <QTimer>

#include <functional>
#include "client-model.h"

namespace Ui {
class AuthWindow;
class ControlWindow;
}

class AuthWindow : public QWidget
{
    Q_OBJECT
   using OnAuth = std::function<void (const Air_Conditioner::GuestInfo &)>;

public:
    explicit AuthWindow(QWidget *parent = 0);
    ~AuthWindow();
    void SetOnAuth(OnAuth && onAuth){
        _onAuth = onAuth;
    }
private slots:
    void on_LogOnBtn_clicked();

    void on_QuitBtn_clicked();

private:
    Ui::AuthWindow *ui;
    OnAuth _onAuth;
};

class ControlWindow : public QWidget
{
    Q_OBJECT
    using OnTempChanged = std::function<void (const Air_Conditioner::Temperature _temp)>;
    using OnWindChanged = std::function<void (const Air_Conditioner::Wind _wind)>;
    using OnClock = std::function<void ()>;
public:
    explicit ControlWindow(QWidget *parent = 0);
    ~ControlWindow();

    void SetOnTempChanged(OnTempChanged && onTempChanged){
        _onTempChanged = onTempChanged;
    }

    void SetOnWindChanged(OnWindChanged && onWindChanged){
        _onWindChanged = onWindChanged;
    }

    void Message(QString message);

    void ShowState(Air_Conditioner::ServerInfo,
                   Air_Conditioner::ClientInfo,
                   Air_Conditioner::RoomRequest);
    void LoadGuestInfo(Air_Conditioner::GuestInfo guest);

    void SetOnClock(OnClock && onClock){
        QObject::connect(&_timer,SIGNAL(timeout()),this,SLOT(clockAlarmed()));
        _onClock = onClock;
    }

    void start(){
        _timer.setInterval(1000);
        _timer.start();
    }
protected slots:
    void clockAlarmed(){
        _onClock();
    }

private slots:

    void on_StopWind_clicked();
    
    void on_LowWind_clicked();
    
    void on_MidWind_clicked();
    
    void on_StrongWind_clicked();
    
    void on_UpBtn_clicked();

    void on_DownBtn_clicked();

    void on_QuitBtn_clicked();

private:
    Ui::ControlWindow *ui;
    OnTempChanged _onTempChanged;
    OnWindChanged _onWindChanged;
    OnClock _onClock;
    QTimer _timer;
};
#endif // CLIENTVIEWGUI_H