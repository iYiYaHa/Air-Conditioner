
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
#include <thread>
#include <mutex>
#include <chrono>

#include "client-view.h"
#include "../common/cli-helper.h"

namespace Air_Conditioner
{
    class AuthViewCLI : public AuthView
    {
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
                auto roomId = InputHelper::Get<RoomId> ("Room ID");
                auto guestId = InputHelper::Get<GuestId> ("Guest ID");
                try
                {
                    _onAuth (GuestInfo {
                        std::move (roomId), std::move (guestId)
                    });
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
        std::mutex _mtxData;
        GuestInfo _guestInfo;
        RoomRequest _roomRequest;
        ClientInfo _clientInfo;
        ServerInfo _serverInfo;

        std::pair<Temperature, Temperature> _GetTempRange () const
        {
            if (_serverInfo.mode == 0)  // Summer
                return std::make_pair (MinTemp, DefaultRoomTemp);
            else                        // Winter
                return std::make_pair (DefaultRoomTemp, MaxTemp);
        }

        void _UpdateTemp ()
        {
            auto temp = InputHelper::Get<Temperature> ("Target Temp");

            std::lock_guard<std::mutex> lg { _mtxData };
            auto tempRange = _GetTempRange ();
            if (temp < tempRange.first || temp > tempRange.second)
            {
                std::cout << "Temperature should be in range ["
                    << tempRange.first << ", " << tempRange.second << "]\n";
                return;
            }
            _roomRequest.target = temp;
            std::cout << "Updated\n";
        }

        void _UpdateWind ()
        {
            constexpr auto minWind = Wind { 1 };
            constexpr auto maxWind = Wind { 3 };

            auto wind = InputHelper::Get<Wind> ("Target Wind");
            if (wind < minWind || wind > maxWind)
            {
                std::cout << "Wind should be in range ["
                    << minWind << ", " << maxWind << "]\n";
                return;
            }

            std::lock_guard<std::mutex> lg { _mtxData };
            _roomRequest.wind = wind;
            std::cout << "Updated\n";
        }

        void _Pulse ()
        {
            if (!_onPulse) return;

            auto ret = _onPulse (_roomRequest);

            _clientInfo = ret.first;
            _serverInfo = ret.second;

            // Handle server update working mode
            auto tempRange = _GetTempRange ();
            if (_roomRequest.target < tempRange.first ||
                _roomRequest.target > tempRange.second)
            {
                _roomRequest.target = _serverInfo.mode == 0 ?
                    DefaultSummerTemp : DefaultWinterTemp;
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
        ControlViewCLI (const GuestInfo &guestInfo,
                        OnPulse &&onPulse,
                        OnSim &&onSim)
            : _guestInfo (guestInfo), _onPulse (onPulse), _onSim (onSim),
            _roomRequest { guestInfo.room, DefaultRoomTemp, 0, Wind { 2 } }
        {}

        virtual void Show () override
        {
            constexpr auto sleepTime = std::chrono::seconds { 1 };

            auto isQuit = false;
            auto isPause = false;

            std::cout << "Welcom " << _guestInfo.guest
                << " to Room " << _guestInfo.room
                << " :-)\n (Press 'Enter' to pause)\n";

            std::thread thread ([&] {
                auto lastHit = std::chrono::system_clock::now ();
                while (!isQuit)
                {
                    try
                    {
                        std::lock_guard<std::mutex> lg { _mtxData };
                        _Pulse ();
                        if (_onSim) _onSim (_roomRequest, _clientInfo.hasWind);
                        if (!isPause) _PrintInfo ();
                    }
                    catch (const std::exception &ex)
                    {
                        std::cerr << ex.what () << std::endl;
                    }

                    // Prevent over sleep :-)
                    auto timeWasted = std::chrono::system_clock::now () - lastHit;
                    if (timeWasted < sleepTime)
                        std::this_thread::sleep_for (sleepTime - timeWasted);
                    lastHit = std::chrono::system_clock::now ();
                }
            });

            while (!isQuit)
            {
                InputHelper::GetLine ();
                isPause = true;
                std::cout << "What you wanna do? Enter command to update request or quit\n"
                    " - 'temp' to update temperature\n"
                    " - 'wind' to update wind\n"
                    " - 'quit' to quit\n"
                    " - Press Enter to resume\n"
                    "$ ";

                std::string cmd;
                if (!InputHelper::GetByRef (cmd))
                {
                    isPause = false;
                    continue;
                }

                if (cmd == "quit")
                {
                    isQuit = true;
                    break;
                }
                else if (cmd == "temp") _UpdateTemp ();
                else if (cmd == "wind") _UpdateWind ();
                else std::cout << "Invalid Command\n";
                isPause = false;
            }
            if (thread.joinable ()) thread.join ();
        }
    };
}

#endif AC_CLIENT_VIEW_CLI_H