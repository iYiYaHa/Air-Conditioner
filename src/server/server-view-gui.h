
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

        void _PrintTime (const TimePoint &timePoint) const
        {
            auto timeT = std::chrono::system_clock::to_time_t (timePoint);
            auto timeTm = std::localtime (&timeT);
            std::cout << timeTm->tm_year + 1900
                << "-" << timeTm->tm_mon + 1
                << "-" << timeTm->tm_mday;
        }

        void _PrintTimeRange () const
        {
            std::cout << "Log starts from ";
            _PrintTime (_timeBeg);
            std::cout << " to ";
            _PrintTime (_timeEnd);
            std::cout << std::endl;
        }

//        TimePoint _GetTimeBeg () const
//        {
//            std::cout << "Select a time point to start with"
//                " (Time Format: 'Year-Month-Day')\n";

//            while (true)
//            {
//                auto str = InputHelper::Get<std::string> ("Time String");
//                for (auto &ch : str) if (ch == '-') ch = ' ';
//                std::istringstream iss (str);

//                try
//                {
//                    std::tm timeTm { 0 };
//                    auto count = 0;
//                    while (iss >> str)
//                    {
//                        auto num = std::stoi (str);
//                        if (!num) throw 0;

//                        if (!timeTm.tm_year) timeTm.tm_year = num - 1900;
//                        else if (!timeTm.tm_mon) timeTm.tm_mon = num - 1;
//                        else if (!timeTm.tm_mday) timeTm.tm_mday = num;

//                        ++count;
//                    }

//                    // Validation
//                    if (count != 3 ||
//                        timeTm.tm_year < 0 ||
//                        timeTm.tm_mon < 0 ||
//                        timeTm.tm_mon > 11 ||
//                        timeTm.tm_mday < 1 ||
//                        timeTm.tm_mday > 31)
//                        throw 0;
//                    auto timeT = mktime (&timeTm);
//                    if (localtime (&timeT) == nullptr)
//                        throw 0;

//                    auto ret = std::chrono::system_clock::from_time_t (timeT);

//                    // Range Validation
//                    if (ret < _timeBeg - std::chrono::hours { 24 } ||
//                        ret > _timeEnd + std::chrono::hours { 24 })
//                    {
//                        std::cout << "Time ";
//                        _PrintTime (ret);
//                        std::cout << " out of range\n";
//                        _PrintTimeRange ();
//                    }
//                    else return ret;
//                }
//                catch (...)
//                {
//                    std::cout << "Invalid Time Format\n";
//                }
//            }
//        }

//        TimePoint _GetTimeEnd (const TimePoint &tBeg) const
//        {
//            std::cout << "Select a Log Type (day/week/month)\n";
//            while (true)
//            {
//                auto type = InputHelper::Get<std::string> ("Log Mode");
//                if (type == "day")
//                    return tBeg + std::chrono::hours { 24 };
//                else if (type == "week")
//                    return tBeg + std::chrono::hours { 24 * 7 };
//                else if (type == "month")
//                    return tBeg + std::chrono::hours { 24 * 30 };
//                else
//                    std::cout << "Invalid Log Mode\n";
//            }
//        }

        void _PrintLog (const LogOnOffList &onOffList,
                        const LogRequestList &requestList)
        {
            std::cout << std::endl;
            if (onOffList.empty ())
                std::cout << "No On-Off records\n";
            else
            {
                std::cout << "On-Off records:\n";
                for (const auto &item : onOffList)
                {
                    std::cout << " - Room " << item.first
                        << " has " << item.second.size ()
                        << " On-Off records\n";
                }
            }

            std::cout << std::endl;
            if (requestList.empty ())
                std::cout << "No Request records\n";
            else
            {
                std::cout << "Request records:\n";
                for (const auto &item : requestList)
                {
                    std::cout << " - Room " << item.first
                        << " has " << item.second.size ()
                        << " Request records\n";
                    for (const auto &entry : item.second)
                    {
                        std::cout << "  - [";
                        _PrintTime (entry.timeBeg);
                        std::cout << " ~ ";
                        _PrintTime (entry.timeEnd);
                        std::cout << "] Temp: " << entry.tempBeg
                            << " -> " << entry.tempEnd
                            << " Wind: " << entry.wind
                            << " Cost: " << entry.cost
                            << std::endl;
                    }
                }
            }
        }

        TimePoint _timeBeg, _timeEnd;
        OnQueryOnOff _onQueryOnOff;
        OnQueryRequest _onQueryRequest;
        OnBack _onBack;

    public:
        LogViewGUI (TimePoint timeBeg,
                    TimePoint timeEnd,
                    OnQueryOnOff &&onQueryOnOff,
                    OnQueryRequest &&onQueryRequest,
                    OnBack &&onBack)
            : _timeBeg (timeBeg), _timeEnd (timeEnd),
            _onQueryOnOff (onQueryOnOff),
            _onQueryRequest (onQueryRequest),
            _onBack (onBack)
        {}

        virtual void Show () override
        {
            std::cout << "\n";
            _PrintTimeRange ();
            auto timeBeg = _GetTimeBeg ();
            auto timeEnd = _GetTimeEnd (timeBeg);

            auto onOffList = _onQueryOnOff (timeBeg, timeEnd);
            auto requestList = _onQueryRequest (timeBeg, timeEnd);

            _PrintLog (onOffList, requestList);
            std::cout << std::endl;

            if (_onBack) _onBack ();
//            int tmpArgc = 0;
//            char ** tmpArgv = nullptr;
//            QApplication app(tmpArgc,tmpArgv);
//            StatisticWindow statistic;
//            statistic.SetOnBack(std::move(_onBack));
//            statistic.show();
//            app.exec();
        }

    };

    class ClientViewGUI : public ClientView
    {
        ClientList _clients;
        OnUpdate _onUpdate;
        OnBack _onBack;

    public:
        ClientViewGUI (OnUpdate &&onUpdate, OnBack &&onBack)
            : _onUpdate (onUpdate), _onBack (onBack)
        {}

        virtual void Show () override
           {
               int tmpArgc = 0;
               char ** tmpArgv = nullptr;
               QApplication app(tmpArgc,tmpArgv);
               ClientWindow client;
               client.SetOnBack(std::move(_onBack));
               client.SetOnUpdate(std::move(_onUpdate));
               client.show();
               app.exec();
           }


    };
}

#endif AC_SERVER_VIEW_GUI_H
