
//
// Air Conditioner - Server MVC View (GUI View)
// Youjie Zhang, 2017
//

#ifndef AC_SERVER_VIEW_GUI_H
#define AC_SERVER_VIEW_GUI_H

#include <iostream>
#include <iomanip>
#include <string>
#include <unordered_map>
#include <thread>
#include <chrono>

#include <QApplication>
#include "server-view-gui-qt.h"

namespace Air_Conditioner
{
    class WelcomeViewGUI : public WelcomeView
    {     
        OnNav _onNav;
    public:
        WelcomeViewGUI (OnNav &&onNav)
            : _onNav (onNav)
        {}

        virtual void Show () override
        {

            int tmpArgc = 0;
            char ** tmpArgv = nullptr;
            QApplication app(tmpArgc,tmpArgv);
            WelcomeWindow welcome;
            welcome.setOnQuit([&]{
                welcome.close();
                _onNav(ViewType::Quit);

            });

            welcome.setOnConfig([&]{
                welcome.close();
                _onNav(ViewType::ConfigView);
            });

            welcome.setOnLog([&]{
                welcome.close();
                _onNav(ViewType::LogView);
            });

            welcome.setOnClient([&]{
                welcome.close();
                _onNav(ViewType::ClientView);
            });

            welcome.setOnGuest([&]{
                welcome.close();
                _onNav(ViewType::GuestView);
            });

            welcome.show();
            app.exec();

        }
    };

    class ConfigViewGUI : public ConfigView
    {
        ServerInfo _config;
        OnSet _onSet;
        OnBack _onBack;
    public:
        ConfigViewGUI (const ServerInfo &config,
                       OnSet &&onSet, OnBack &&onBack)
            : _config (config), _onSet (onSet), _onBack (onBack)
        {}

        virtual void Show () override
        {                    
            int tmpArgc = 0;
            char ** tmpArgv = nullptr;
            QApplication app(tmpArgc,tmpArgv);
            ConfigWindow configWin;
            configWin.LoadConfig(_config);
            configWin.SetOnBack(std::move(_onBack));
            configWin.SetOnSet(std::move(_onSet));
            configWin.show();
            app.exec();
        }
    };

    class GuestViewGUI : public GuestView
    {
        GuestInfo _GetGuestInfo () const
        {
            RoomId roomId;
            GuestId guestId;

            std::cout << "Room ID: ";
            std::cin >> roomId;
            std::cout << "Guest ID: ";
            std::cin >> guestId;

            return GuestInfo {
                roomId, guestId
            };
        }

        RoomId _GetRoomId () const
        {
            RoomId roomId;

            std::cout << "Room ID: ";
            std::cin >> roomId;

            return roomId;
        }

        void _PrintList () const
        {
            if (_list.empty ())
            {
                std::cout << "No guest is registered...\n";
                return;
            }

            std::cout << "Guest on the list:\n";
            for (const auto &guest : _list)
            {
                std::cout
                    << " - Room: " << guest.room
                    << " Guest: " << guest.guest
                    << "\n";
            }
        }

        std::list<GuestInfo> _list;
        OnAdd _onAdd;
        OnDel _onDel;
        OnBack _onBack;

    public:
        GuestViewGUI (const std::list<GuestInfo> &list,
                      OnAdd &&onAdd, OnDel &&onDel,
                      OnBack &&onBack)
            : _list (list),
            _onAdd (onAdd), _onDel (onDel), _onBack (onBack)
        {}

        virtual void Show () override
        {        
            int tmpArgc = 0;
            char ** tmpArgv = nullptr;
            QApplication app(tmpArgc,tmpArgv);
            GuestWindow guest;
            guest.SetOnBack(std::move(_onBack));
            guest.LoadGuest(_list);
            guest.SetOnAdd(std::move(_onAdd));
            guest.SetOnDel(std::move(_onDel));
            guest.SetOnBack(std::move(_onBack));
            guest.show();
            app.exec();
        }
    };

    class LogViewGUI : public LogView
    {
        OnQueryOnOff _onQueryOnOff;
        OnQueryRequest _onQueryRequest;
        OnBack _onBack;

    public:
        LogViewGUI (OnQueryOnOff &&onQueryOnOff,
                    OnQueryRequest &&onQueryRequest,
                    OnBack &&onBack)
            : _onQueryOnOff (onQueryOnOff), _onQueryRequest (onQueryRequest),
            _onBack (onBack)
        {}

        virtual void Show () override
        {
            //if (_onBack) _onBack ();
            int tmpArgc = 0;
            char ** tmpArgv = nullptr;
            QApplication app(tmpArgc,tmpArgv);
            StatisticWindow statistic;
            statistic.SetOnBack(std::move(_onBack));
            statistic.show();
            app.exec();
        }
    };

    class ClientViewGUI : public ClientView
    {
        void _PrintInfo () const
        {
            if (_clients.empty ())
            {
                std::cout << "No slave is connecting...\n";
                return;
            }

            static std::unordered_map<Wind, std::string> windStr
            {
                { 0, "Stop" },
                { 1, "Weak" },
                { 2, "Mid" },
                { 3, "Strong" }
            };

            std::cout << "Client List:\n";
            for (const auto &client : _clients)
            {
                std::cout << std::fixed
                    << std::setprecision (2)
                    << " - Room: " << client.first
                    << " Guest: " << client.second.guest
                    << " Wind: " << client.second.wind
                    << " Energy: " << client.second.energy
                    << " Cost: " << client.second.cost
                    << "\n";
            }
        }

        ClientList _clients;
        OnUpdate _onUpdate;
        OnBack _onBack;

    public:
        ClientViewGUI (OnUpdate &&onUpdate, OnBack &&onBack)
            : _onUpdate (onUpdate), _onBack (onBack)
        {}

        virtual void Show () override
           {
               std::cout << "Press 'Enter' to Back to the Welcome Page\n";

               // TODO: config refresh rate
               auto sleepTime = std::chrono::seconds { 1 };
               auto isQuit = false;

               std::thread thread ([&] {
                   auto lastHit = std::chrono::system_clock::now ();
                   while (!isQuit)
                   {
                       try
                       {
                           if (_onUpdate) _clients = _onUpdate ();
                           _PrintInfo ();
                       }
                       catch (const std::exception &ex)
                       {
                           std::cerr << ex.what () << std::endl;
                       }

                       // To prevent over sleep :-)
                       auto timeWasted = std::chrono::system_clock::now () - lastHit;
                       if (timeWasted < sleepTime)
                           std::this_thread::sleep_for (sleepTime - timeWasted);
                       lastHit = std::chrono::system_clock::now ();
                   }
               });

               int tmpArgc = 0;
               char ** tmpArgv = nullptr;
               QApplication app(tmpArgc,tmpArgv);
               ClientWindow client;
               client.SetOnBack(std::move(_onBack));
               client.SetOnUpdate(std::move(_onUpdate));
               client.show();
               app.exec();

               // TODO: handle invalid input
               //getchar (); getchar ();
               if (_onBack) _onBack ();
               isQuit = true;
               if (thread.joinable ()) thread.join ();
           }


    };
}

#endif AC_SERVER_VIEW_GUI_H
