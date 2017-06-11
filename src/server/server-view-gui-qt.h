
#ifndef SERVER_VIEW_GUI_H
#define SERVER_VIEW_GUI_H
#include <QWidget>
#include <functional>
#include <QCloseEvent>
#include <QMessageBox>

#include "server-view.h"

namespace Ui {
class WelcomeWindow;
class StatisticWindow;
class ClientWindow;
class GuestWindow;
class ConfigWindow;
}

using namespace Air_Conditioner;

class WelcomeWindow : public QWidget
{
    Q_OBJECT
    using OnNav = std::function<void()>;
public:
    explicit WelcomeWindow(QWidget *parent = 0);
    ~WelcomeWindow();

    void setOnQuit(OnNav && onQuit){
        _onQuit = onQuit;
    }
    void setOnConfig(OnNav && onConfig){
        _onConfig = onConfig;
    }
    void setOnClient(OnNav && onClient){
        _onClient = onClient;
    }
    void setOnGuest(OnNav && onGuest){
        _onGuest = onGuest;
    }
    void setOnLog(OnNav && onLog){
        _onLog = onLog;
    }
private slots:
    void on_QuitBtn_clicked();

    void on_ConfigBtn_clicked();

    void on_GuestBtn_clicked();

    void on_StatisticBtn_clicked();

    void on_ClientBtn_clicked();

private:
    OnNav _onQuit;
    OnNav _onConfig;
    OnNav _onClient;
    OnNav _onGuest;
    OnNav _onLog;
    Ui::WelcomeWindow *ui;
};

class StatisticWindow : public QWidget
{
    Q_OBJECT
    using OnBack = std::function<void ()>;
public:
    explicit StatisticWindow(QWidget *parent = 0);
    ~StatisticWindow();
    void SetOnBack(OnBack && onBack){
        _onBack = onBack;
    }

protected:
    void closeEvent(QCloseEvent *event);
private:
    Ui::StatisticWindow *ui;
    OnBack _onBack;
};

class ClientWindow : public QWidget
{
    Q_OBJECT
    using OnBack = std::function<void ()>;
public:
    explicit ClientWindow(QWidget *parent = 0);
    ~ClientWindow();
    void SetOnBack(OnBack && onBack){
        _onBack = onBack;
    }
protected:
    void closeEvent(QCloseEvent *event);
private:
    Ui::ClientWindow *ui;
    OnBack _onBack;
};

class GuestWindow : public QWidget
{
    Q_OBJECT
    using OnBack = std::function<void()>;

public:
    explicit GuestWindow(QWidget *parent = 0);
    ~GuestWindow();
    void SetOnBack(OnBack && onBack){
        _onBack = onBack;
    }

protected:
    void closeEvent(QCloseEvent *event);
private:
    Ui::GuestWindow *ui;
    OnBack _onBack;
};


class ConfigWindow : public QWidget
{
    Q_OBJECT
    using OnSet = std::function<void (const ServerInfo &)>;
    using OnBack = std::function<void ()>;
public:
    explicit ConfigWindow(QWidget *parent = 0);
    ~ConfigWindow();
    void LoadConfig(ServerInfo config){
        _config = config;
        ShowConfig();
    }
    void ShowConfig();
    void SetConfig(){
        _onSet(_config);
    }
    void SetOnBack(OnBack && onBack){
        _onBack = onBack;
    }
    void SetOnSet(OnSet && onSet){
        _onSet = onSet;
    }

protected:
    void closeEvent(QCloseEvent *event);
private slots:
    void on_ConfirmBtn_clicked();

    void on_CancelBtn_clicked();

private:
    Ui::ConfigWindow *ui;
    OnBack _onBack;
    OnSet _onSet;
    ServerInfo _config;
};

#endif SERVER_VIEW_GUI_H
