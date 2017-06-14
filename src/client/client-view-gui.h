
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
#include <QTimer>

namespace Air_Conditioner
{
    class AuthViewGUI : public AuthView
    {  
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

        void _UpdateTemp (const Air_Conditioner::Temperature temp)
        {
            std::lock_guard<std::mutex> lg { _mtxData };
            auto tempRange = _GetTempRange ();
            if (temp < tempRange.first || temp > tempRange.second)
            {
                QString info = "Temperature should be in range ["
                    + QString().setNum(tempRange.first) + ", "
                        + QString().setNum(tempRange.second) + "]\n";
                throw std::runtime_error(info.toStdString());
            }
            _roomRequest.target = temp;
          //  std::cout << "Updated\n";
        }

        void _UpdateWind (const Air_Conditioner::Wind wind)
        {
            constexpr auto minWind = Wind { 1 };
            constexpr auto maxWind = Wind { 3 };

            if (wind < minWind || wind > maxWind)
            {
                QString info =  "Wind should be in range ["
                    + QString().setNum(minWind) + ", " + QString().setNum(maxWind) + "]\n";
                throw std::runtime_error(info.toStdString());
            }

            std::lock_guard<std::mutex> lg { _mtxData };
            _roomRequest.wind = wind;
           // std::cout << "Updated\n";
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

        OnPulse _onPulse;
        OnSim _onSim;


    public:
        ControlViewGUI (const GuestInfo &guestInfo,
                        OnPulse &&onPulse,
                        OnSim &&onSim)
            : _guestInfo (guestInfo), _onPulse (onPulse), _onSim (onSim),
            _roomRequest { guestInfo.room, DefaultRoomTemp, 0, Wind { 2 } }
        {}

        virtual void Show () override
        {
            constexpr auto sleepTime = std::chrono::seconds { 1 };

            int tmpArgc = 0;
            char ** tmpArgv = nullptr;
            QApplication app(tmpArgc,tmpArgv);
            ControlWindow control;

            auto isQuit = false;

            std::thread thread ([&] {
                auto lastHit = std::chrono::system_clock::now ();
                while (!isQuit)
                {
                    try
                    {
                      std::lock_guard<std::mutex> lg { _mtxData };
                      if(_onPulse) _Pulse();
                      if (_onSim) _onSim (_roomRequest, _clientInfo.hasWind);
                      control.ShowState(_serverInfo,_clientInfo,_roomRequest);
                    }
                    catch (const std::exception &ex)
                    {
                      std::cerr << ex.what () << std::endl;
                      control.Message(QString(ex.what()));
                    }

                    // Prevent over sleep :-)
                    auto timeWasted = std::chrono::system_clock::now () - lastHit;
                    if (timeWasted < sleepTime)
                        std::this_thread::sleep_for (sleepTime - timeWasted);
                    lastHit = std::chrono::system_clock::now ();
                }
            });

            control.LoadGuestInfo(_guestInfo);
            control.SetOnTempChanged([&](const Temperature temp){
                try{
                    _UpdateTemp(temp);
                }
                catch (std::exception &ex){
                    control.Message(ex.what());
                }
            });

            control.SetOnWindChanged([&](const Wind wind){
                try{
                    _UpdateWind(wind);
                }
                catch (std::exception &ex){
                    control.Message(ex.what());
                }
            });

            control.show();
            app.exec();
            isQuit = true;

            if (thread.joinable ()) thread.join ();

        }
    };
}
#endif AC_CLIENT_VIEW_GUI_H
