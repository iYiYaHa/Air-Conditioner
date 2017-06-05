
//
// Air Conditioner - Client MVC View (CLI View)
// BOT Man, 2017
//

#ifndef AC_CLIENT_VIEW_CLI_H
#define AC_CLIENT_VIEW_CLI_H

#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <string>
#include <sstream>
#include <thread>
#include <mutex>
#include <chrono>

#include "client-view.h"

namespace Air_Conditioner
{
    class AuthViewCLI : public AuthView
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
        AuthViewCLI (OnAuth &&onAuth) : _onAuth (onAuth) {}

        virtual void Show () override
        {
            std::cout << "Welcome to Air Conditioner System :-)\n"
                "Please login to enter the system...\n";
            if (!_onAuth) return;

            while (true)
            {
                try
                {
                    _onAuth (_GetGuestInfo ());
                    break;
                }
                catch (const std::exception &ex)
                {
                    std::cerr << ex.what () << std::endl;
                }
            }
        }
    };

    class ControlViewCLI : public ControlView
    {
        GuestInfo _guestInfo;
        RoomInfo _roomInfo;
        RoomRequest _roomRequest;
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

                std::cout << "Target Temp (18 - 30): ";
                std::cin.getline (buf, 1024);
                std::istringstream iss (buf); iss >> temp;
                if (temp < 18 || temp > 30)
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

                _roomRequest.temp = temp;
                _roomRequest.wind = wind;
                return;
            }
        }

        void _PrintInfo () const
        {
            if (!_serverInfo.isOn)
            {
                std::cout << "\rServer is OFF      "
                    "                    "
                    "                    "
                    "                    "
                    "                    "
                    "                    ";
                return;
            }

            static std::unordered_map<Wind, std::string> windStr
            {
                { 0, "Stop" },
                { 1, "Weak" },
                { 2, "Mid" },
                { 3, "Strong" }
            };

            std::cout << std::fixed
                << std::setprecision (2)
                << "\rRoom: " << _guestInfo.room
                << " Current: " << _roomInfo.temp
                << " Target: " << _roomRequest.temp
                << " Wind: " << windStr.at (_clientInfo.hasWind ? _roomRequest.wind : 0)
                << " Energy: " << _clientInfo.energy
                << " Cost: " << _clientInfo.cost
                << "        ";
        }

        // Allow callback only once at a time
        void _Callback (const std::function<void ()> &fn)
        {
            static std::mutex mtxCallback;
            try
            {
                std::lock_guard<std::mutex> lg (mtxCallback);
                fn ();
            }
            catch (const std::exception &ex)
            {
                std::cerr << ex.what () << std::endl;
            }
        }

        OnRequest _onRequest;
        OnPulse _onPulse;
        OnSim _onSim;

    public:
        ControlViewCLI (const GuestInfo &guestInfo,
                        OnRequest &&onRequest,
                        OnPulse &&onPulse,
                        OnSim &&onSim)
            : _guestInfo (guestInfo),
            _onRequest (onRequest), _onPulse (onPulse), _onSim (onSim),
            _roomInfo { guestInfo.room, 26 },
            _roomRequest { guestInfo.room, 0, 0 }
        {}

        virtual void Show () override
        {
            std::chrono::seconds sleepTime;
            auto isQuit = false;
            auto isPause = false;

            auto updateState = [&] (const ResponseFmt &ret)
            {
                _clientInfo = ret.first;
                _serverInfo = ret.second;

                if (_roomRequest.temp == 0)
                    _roomRequest.temp = _serverInfo.mode == 0 ?
                    Temperature { 22 } : Temperature { 28 };

                sleepTime = std::chrono::seconds (_serverInfo.pulseFreq);
            };

            std::cout << "Welcom " << _guestInfo.guest
                << " to Room " << _guestInfo.room
                << " :-)\n (Press 'Enter' to pause)\n";

            std::thread thread ([&] {
                auto lastHit = std::chrono::system_clock::now ();
                while (!isQuit)
                {
                    _Callback ([&] {
                        if (_onPulse)
                            updateState (_onPulse (_roomInfo));
                        if (_onSim)
                            _onSim (_roomInfo, _roomRequest, _clientInfo.hasWind);
                        if (!isPause)
                            _PrintInfo ();
                    });

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
                getchar (); getchar (); isPause = true;
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
                        _Callback ([&] {
                            if (_onRequest)
                                updateState (_onRequest (_roomRequest));
                        });
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

#endif AC_CLIENT_VIEW_CLI_H