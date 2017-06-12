
//
// Air Conditioner - Server MVC View (CLI View)
// BOT Man, 2017
//

#ifndef AC_SERVER_VIEW_CLI_H
#define AC_SERVER_VIEW_CLI_H

#include <iostream>
#include <iomanip>
#include <string>
#include <unordered_map>
#include <thread>
#include <chrono>

#include "server-view.h"

namespace Air_Conditioner
{
    class WelcomeViewCLI : public WelcomeView
    {
        ViewType _GetViewType () const
        {
            static std::unordered_map<std::string, ViewType> viewStr
            {
                { "config", ViewType::ConfigView },
                { "guest", ViewType::GuestView },
                { "log", ViewType::LogView },
                { "client", ViewType::ClientView },
                { "quit", ViewType::Quit }
            };
            while (true)
            {
                std::string cmd;
                try
                {
                    std::cout << "$ ";
                    std::cin >> cmd;
                    return viewStr.at (cmd);
                }
                catch (...)
                {
                    std::cerr << "Invalid command\n";
                }
            }
        }

        OnNav _onNav;

    public:
        WelcomeViewCLI (OnNav &&onNav)
            : _onNav (onNav)
        {}

        virtual void Show () override
        {
            std::cout << "Welcome to Air Conditioner System :-)\n"
                "What you wanna do? Enter command to goto the page\n"
                " - 'config': Config the master machine\n"
                " - 'guest': Manage the guest list of the rooms\n"
                " - 'log': View logs\n"
                " - 'client': View logs\n"
                " - 'quit': Quit\n";
            try
            {
                if (_onNav) _onNav (_GetViewType ());
            }
            catch (const std::exception &ex)
            {
                std::cerr << ex.what () << std::endl;
            }
        }
    };

    class ConfigViewCLI : public ConfigView
    {
        void _PrintInfo () const
        {
            std::cout << "Current Config:"
                << "\n - Master is " << (_config.isOn ? "ON" : "OFF")
                << "\n - Mode: " << (!_config.mode ? "Summer" : "Winter")
                << "\n";
        }

        ServerInfo _config;
        OnSet _onSet;
        OnBack _onBack;

    public:
        ConfigViewCLI (const ServerInfo &config,
                       OnSet &&onSet, OnBack &&onBack)
            : _config (config), _onSet (onSet), _onBack (onBack)
        {}

        virtual void Show () override
        {
            _PrintInfo ();
            std::cout << "\nWhat you wanna do? Enter command to update config or back\n"
                " - 'on' / 'off' to turn on/off the master\n"
                " - 'summer' / 'winter' to update working mode\n"
                " - 'back' to go back to welcome page\n";

            while (true)
            {
                std::cout << "$ ";
                std::string cmd;
                std::cin >> cmd;
                if (cmd == "on")
                    _config.isOn = true;
                else if (cmd == "off")
                    _config.isOn = false;
                else if (cmd == "summer")
                    _config.mode = 0;
                else if (cmd == "winter")
                    _config.mode = 1;
                else if (cmd == "back")
                {
                    if (_onBack) _onBack ();
                    std::cout << std::endl;
                    break;
                }
                else
                {
                    std::cerr << "Invalid command\n";
                    continue;
                }
                if (_onSet) _onSet (_config);
                std::cout << "Config has been updated :-)\n";
                _PrintInfo ();
                std::cout << std::endl;
            }
        }
    };

    class GuestViewCLI : public GuestView
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
        GuestViewCLI (const std::list<GuestInfo> &list,
                      OnAdd &&onAdd, OnDel &&onDel,
                      OnBack &&onBack)
            : _list (list),
            _onAdd (onAdd), _onDel (onDel), _onBack (onBack)
        {}

        virtual void Show () override
        {
            std::cout << "\nWhat you wanna do? Enter command to update guest info or back\n"
                " - 'list' to view current list of guests\n"
                " - 'add' to add new guest\n"
                " - 'del' to delete existing guest\n"
                " - 'back' to go back to welcome page\n";

            while (true)
            {
                std::cout << "$ ";
                std::string cmd;
                std::cin >> cmd;
                if (cmd == "list")
                    _PrintList ();
                else if (cmd == "add")
                {
                    try
                    {
                        auto guest = _GetGuestInfo ();
                        if (_onDel) _onAdd (guest);
                        _list.emplace_back (std::move (guest));
                        std::cout << "Adding Done\n";
                    }
                    catch (const std::exception &ex)
                    {
                        std::cerr << ex.what () << std::endl;
                    }
                }
                else if (cmd == "del")
                {
                    try
                    {
                        auto room = _GetRoomId ();
                        if (_onDel) _onDel (room);
                        std::cout << "Deleting Done\n";
                    }
                    catch (const std::exception &ex)
                    {
                        std::cerr << ex.what () << std::endl;
                    }
                }
                else if (cmd == "back")
                {
                    if (_onBack) _onBack ();
                    std::cout << std::endl;
                    break;
                }
                else
                {
                    std::cerr << "Invalid command\n";
                    continue;
                }
            }
        }
    };

    class LogViewCLI : public LogView
    {
        OnQueryOnOff _onQueryOnOff;
        OnQueryRequest _onQueryRequest;
        OnBack _onBack;

    public:
        LogViewCLI (OnQueryOnOff &&onQueryOnOff,
                    OnQueryRequest &&onQueryRequest,
                    OnBack &&onBack)
            : _onQueryOnOff (onQueryOnOff), _onQueryRequest (onQueryRequest),
            _onBack (onBack)
        {}

        virtual void Show () override
        {
            if (_onBack) _onBack ();
        }
    };

    class ClientViewCLI : public ClientView
    {
        int _GetRefreshRate () const
        {
            // TODO: Handle invalid input
            while (true)
            {
                int ret = 0;
                std::cout << "Refresh Rate: ";
                std::cin >> ret;
                if (ret > 0) return ret;
                std::cout << "Invalid Refresh Rate\n";
            }
        }

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
                const auto &roomId = client.first;
                const auto &roomState = client.second;

                std::cout << std::fixed
                    << std::setprecision (2)
                    << " - Room: " << roomId
                    << " Guest: " << roomState.guest
                    << " Cur Temp: " << roomState.current
                    << " Target Temp: " << roomState.target
                    << " Wind: " << roomState.wind
                    << " Energy: " << roomState.energy
                    << " Cost: " << roomState.cost
                    << "\n";
            }
        }

        ClientList _clients;
        OnUpdate _onUpdate;
        OnBack _onBack;

    public:
        ClientViewCLI (OnUpdate &&onUpdate, OnBack &&onBack)
            : _onUpdate (onUpdate), _onBack (onBack)
        {}

        virtual void Show () override
        {
            std::cout << "Press 'Enter' to Back to the Welcome Page\n";

            auto refreshRate = _GetRefreshRate ();
            auto sleepTime = std::chrono::seconds { refreshRate };
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

            // TODO: handle invalid input
            getchar (); getchar ();
            if (_onBack) _onBack ();
            isQuit = true;
            if (thread.joinable ()) thread.join ();
        }
    };
}

#endif AC_SERVER_VIEW_CLI_H