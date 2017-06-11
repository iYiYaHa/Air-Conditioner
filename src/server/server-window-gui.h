
#include <QWidget>
#include <functional>
#include <QCloseEvent>
#include <QMessageBox>

namespace Ui {
class WelcomeWindow;
class StatisticWindow;
class LogWindow;
class GuestWindow;
class ConfigWindow;
}

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
    void setOnStatistic(OnNav && onStatistic){
        _onStatistic = onStatistic;
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
    OnNav _onStatistic;
    Ui::WelcomeWindow *ui;
};

class StatisticWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticWindow(QWidget *parent = 0);
    ~StatisticWindow();

private:
    Ui::StatisticWindow *ui;
};

class LogWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LogWindow(QWidget *parent = 0);
    ~LogWindow();

private:
    Ui::LogWindow *ui;
};

class GuestWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GuestWindow(QWidget *parent = 0);
    ~GuestWindow();

private:
    Ui::GuestWindow *ui;
};


class ConfigWindow : public QWidget
{
    Q_OBJECT
    using OnBack = std::function<void ()>;
public:
    explicit ConfigWindow(QWidget *parent = 0);
    ~ConfigWindow();
    void SetOnBack(OnBack && onBack){
        _onBack = onBack;
    }

protected:
    void closeEvent(QCloseEvent *event);
private:
    Ui::ConfigWindow *ui;
    OnBack _onBack;
};
//#endif // SERVERVIEWGUI_H
