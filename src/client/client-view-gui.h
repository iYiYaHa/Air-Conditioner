
//
// Air Conditioner - Client MVC View (GUI View)
// Youjie Zhang, 2017
//

#ifndef AC_CLIENT_VIEW_GUI_H
#define AC_CLIENT_VIEW_GUI_H

#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <string>
#include <sstream>
#include <thread>
#include <mutex>
#include <chrono>

#include "client-view.h"
#include "client-controller.h"
#include "client-view-gui-qt.h"

#include <QApplication>

namespace Air_Conditioner
{
    class AuthViewGUI : public AuthView
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

        OnAuth _onAuth;

    public:
        AuthViewGUI (OnAuth &&onAuth) : _onAuth (onAuth) {}

        virtual void Show () override
        {
            int tmpArgc = 0;
            char ** tmpArgv = nullptr;
            if (!_onAuth) return;
            QApplication app(tmpArgc,tmpArgv);
            AuthWindow auth;
            auth.SetOnAuth(std::move(_onAuth));
            auth.show();
            app.exec();
        }
    };

    class ControlViewGUI : public ControlView
    {
        GuestInfo _guestInfo;
        RoomRequest _roomRequest;
        std::mutex _mtxRoomRequest;
        ClientInfo _clientInfo;
        ServerInfo _serverInfo;

        void _GetRequest ()
        {
            getchar ();
            char buf[1024];
            while (true)
            {
                Temperature temp = 31;
                Wind wind = 4;

                std::cout << "Target Temp (" << MinTemp
                    << " - " << MaxTemp << "): ";
                std::cin.getline (buf, 1024);
                std::istringstream iss (buf); iss >> temp;
                if (temp < MinTemp || temp > MaxTemp)
                {
                    std::cerr << "Invalid Temperature Input :-(\n";
                    continue;
                }

                std::cout << "Target Wind (0 - 3): ";
                std::cin.getline (buf, 1024);
                std::istringstream iss2 (buf); iss2 >> wind;
                if (wind < 0 || wind > 3)
                {
                    std::cerr << "Invalid Wind Input :-(\n";
                    continue;
                }

                std::lock_guard<std::mutex> lg { _mtxRoomRequest };
                _roomRequest.target = temp;
                _roomRequest.wind = wind;
                break;
            }
        }

        void _PrintInfo () const
        {
            static std::unordered_map<Wind, std::string> windStr
            {
                { -1, "Off" },
                { 0, "Stop" },
                { 1, "Weak" },
                { 2, "Mid" },
                { 3, "Strong" }
            };

            Wind wind;
            if (!_serverInfo.isOn)
                wind = -1;
            else if (!_clientInfo.hasWind)
                wind = 0;
            else
                wind = _roomRequest.wind;

            std::cout << std::fixed
                << std::setprecision (2)
                << "\rRoom: " << _guestInfo.room
                << " Current: " << _roomRequest.current
                << " Target: " << _roomRequest.target
                << " Energy: " << _clientInfo.energy
                << " Cost: " << _clientInfo.cost
                << " Wind: " << windStr.at (wind)
                << "        ";
        }

        OnPulse _onPulse;
        OnSim _onSim;

    public:
        ControlViewGUI (const GuestInfo &guestInfo,
                        OnPulse &&onPulse,
                        OnSim &&onSim)
            : _guestInfo (guestInfo), _onPulse (onPulse), _onSim (onSim),
            _roomRequest { guestInfo.room, DefaultRoomTemp, 0, 0 }
        {}

        virtual void Show () override
        {
            constexpr auto sleepTime = std::chrono::seconds { 1 };

            auto isQuit = false;
            auto isPause = false;

            auto updateState = [&] (const ResponseFmt &ret)
            {
                _clientInfo = ret.first;
                _serverInfo = ret.second;

                if (_roomRequest.target == 0)
                    _roomRequest.target = _serverInfo.mode == 0 ?
                    DefaultSummerTemp : DefaultWinterTemp;
                if (_roomRequest.wind == 0)
                    _roomRequest.wind = 2;
            };

            std::cout << "Welcom " << _guestInfo.guest
                << " to Room " << _guestInfo.room
                << " :-)\n (Press 'Enter' to pause)\n";

            std::thread thread ([&] {
                auto lastHit = std::chrono::system_clock::now ();
                while (!isQuit)
                {
                    try
                    {
                        std::lock_guard<std::mutex> lg { _mtxRoomRequest };
                        if (_onPulse) updateState (_onPulse (_roomRequest));
                        if (_onSim) _onSim (_roomRequest, _clientInfo.hasWind);
                        if (!isPause) _PrintInfo ();
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

            while (!isQuit)
            {
                // TODO: handle invalid input
                //getchar (); getchar (); isPause = true;
                std::cout << "What you wanna do? Enter command to send request or quit\n"
                    " - 'req' to send request\n"
                    " - 'quit' to quit\n"
                    " - 'resume' to resume\n";

                std::string cmd;
                while (true)
                {
                    std::cout << "$ ";
                    std::cin >> cmd;
                    if (cmd == "req")
                    {
                        _GetRequest ();
                        std::cout << "Request Done\n";
                    }
                    else if (cmd == "quit")
                    {
                        isQuit = true;
                        break;
                    }
                    else if (cmd == "resume")
                    {
                        std::cout << std::endl;
                        isPause = false;
                        break;
                    }
                    else
                        std::cerr << "Invalid command\n";
                }
            }
            if (thread.joinable ()) thread.join ();
        }
    };
}

#endif AC_CLIENT_VIEW_GUI_H
