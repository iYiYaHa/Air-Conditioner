#ifndef CLIENTVIEWGUI_H
#define CLIENTVIEWGUI_H

#include <QWidget>
#include <QCloseEvent>
#include <QMessageBox>

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

public:
    explicit ControlWindow(QWidget *parent = 0);
    ~ControlWindow();

private:
    Ui::ControlWindow *ui;
};
#endif // CLIENTVIEWGUI_H
