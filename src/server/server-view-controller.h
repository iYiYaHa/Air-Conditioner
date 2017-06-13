
//
// Air Conditioner - Server MVC View Controller
// BOT Man, 2017
//

#ifndef AC_SERVER_VIEW_CONTROLLER_H
#define AC_SERVER_VIEW_CONTROLLER_H

#include <utility>

#include "server-service.h"
#include "server-view.h"

namespace Air_Conditioner
{
    class WelcomeController
    {
        ServerViewManager &_viewManager;

    public:
        WelcomeController (ServerViewManager &viewManager)
            : _viewManager (viewManager)
        {}

        void Nav (ViewType view)
        {
            switch (view)
            {
            case ViewType::ConfigView:
                _viewManager.ToConfigView ();
                break;
            case ViewType::GuestView:
                _viewManager.ToGuestView ();
                break;
            case ViewType::LogView:
                _viewManager.ToLogView ();
                break;
            case ViewType::ClientView:
                _viewManager.ToClientView ();
                break;
            case ViewType::Quit: default: break;
            }
        }
    };

    class ConfigController
    {
    public:
        void SetConfig (const ServerInfo &config)
        {
            ScheduleManager::SetConfig (config);
        }

        // For ConfigView
        const ServerInfo &GetConfig () const
        {
            return ScheduleManager::GetConfig ();
        }
    };

    class GuestInfoController
    {
    public:
        void AddGuest (const GuestInfo &guest)
        {
            GuestManager::AddGuest (guest);
        }
        void RemoveGuest (const RoomId &guest)
        {
            GuestManager::RemoveGuest (guest);
        }

        // For GuestView
        std::list<GuestInfo> GetGuestList () const
        {
            return GuestManager::GetGuestList ();
        }
    };

    class LogController
    {
    public:
        std::pair<TimePoint, TimePoint> GetTimeRange () const
        {
            return LogManager::GetTimeRange ();
        }
        LogOnOffList GetLogOnOff (const TimePoint &from, const TimePoint &to)
        {
            return LogManager::GetOnOff (from, to);
        }
        LogRequestList GetLogRequest (const TimePoint &from, const TimePoint &to)
        {
            return LogManager::GetRequest (from, to);
        }
    };

    class ClientController
    {
    public:
        // For ClientView
        const ClientList &GetClientList () const
        {
            return ScheduleManager::GetClientList ();
        }
    };
}

#endif AC_SERVER_VIEW_CONTROLLER_H