
//
// Air Conditioner - Server MVC Service
// BOT Man, 2017
//

#ifndef AC_SERVER_SERVICE_H
#define AC_SERVER_SERVICE_H

#include <exception>

#define DEADTIME 3
#define DBNAME "ac.db"

#include "ormlite/ormlite.h"
#include "server-model.h"

namespace Air_Conditioner
{
    class LogManager
    {
    public:
        static void TurnOn (const RoomId &room)
        {
            // impl turn on
        }
        static void TurnOff (const RoomId &room)
        {
            // impl turn off
        }

        static void BegRequest (const RoomId &room,
                                const ClientState &state)
        {
            // impl beg req
        }
        static void EndRequest (const RoomId &room)
        {
            // impl end req
        }

        static std::pair<TimePoint, TimePoint> GetTimeRange ()
        {
            // impl here
            return std::make_pair (std::chrono::system_clock::now (),
                                   std::chrono::system_clock::now () + std::chrono::hours { 24 });
        }
        static LogOnOffList GetOnOff (const TimePoint &from, const TimePoint &to)
        {
            // impl here
            LogOnOffList ret;
            ret["john"].emplace_back (LogOnOff {
                std::chrono::system_clock::now (),
                std::chrono::system_clock::now () + std::chrono::hours { 1 }
            });
            ret["lee"];
            return ret;
        }
        static LogRequestList GetRequest (const TimePoint &from, const TimePoint &to)
        {
            // impl here
            return LogRequestList {};
        }
    };

    class GuestManager
    {
        struct GuestEntity
        {
            RoomId room;
            GuestId guest;
            ORMAP ("Guest", room, guest);
        };

        static BOT_ORM::ORMapper &_mapper ()
        {
            static BOT_ORM::ORMapper mapper (DBNAME);
            static auto hasInit = false;

            if (!hasInit)
            {
                try { mapper.CreateTbl (GuestEntity {}); }
                catch (...) {}
                hasInit = true;
            }
            return mapper;
        }

    public:
        static void AddGuest (const GuestInfo &guest)
        {
            auto &mapper = _mapper ();
            try
            {
                mapper.Insert (GuestEntity { guest.room, guest.guest });
            }
            catch (...)
            {
                throw std::runtime_error (
                    "The room has already been registered");
            }
        }
        static void RemoveGuest (const RoomId &room)
        {
            auto &mapper = _mapper ();
            mapper.Delete (GuestEntity { room, GuestId {} });
        }

        static void AuthGuest (const GuestInfo &guest)
        {
            static GuestEntity entity;
            static auto field = BOT_ORM::FieldExtractor { entity };

            auto &mapper = _mapper ();
            auto count = mapper.Query (entity)
                .Where (
                    field (entity.room) == guest.room &&
                    field (entity.guest) == guest.guest)
                .Aggregate (
                    BOT_ORM::Expression::Count ());

            if (count == nullptr || count == size_t { 0 })
                throw std::runtime_error ("Invalid Room ID or Guest ID");
        }

        static std::list<GuestInfo> GetGuestList ()
        {
            static GuestEntity entity;
            static auto field = BOT_ORM::FieldExtractor { entity };

            auto &mapper = _mapper ();
            auto list = mapper.Query (entity).ToList ();

            std::list<GuestInfo> ret;
            for (auto &entry : list)
            {
                ret.emplace_back (GuestInfo {
                    std::move (entry.room),
                    std::move (entry.guest) });
            }
            return ret;
        }
    };

    class EnergyCostManager
    {
    public:
        static void AddEnergy (const RoomId &room, const Energy &energy);
        static Energy GetEnergy (const RoomId &room);
        static Cost GetCost (const RoomId &room);
    };

    class ScheduleManager
    {
        static void AdjustSchedule ()
        {
            // TODO: impl scheduling scheme
            // EnergyCostManager::*
        }

        static void CheckAlive ()
        {
            // TODO: impl check alive and write to log

            const auto deadTime = std::chrono::system_clock::now () -
                std::chrono::seconds { DEADTIME };
            auto &clients = _clients ();
            for (auto p = clients.begin (); p != clients.end ();)
                if (p->second.pulse < deadTime)
                    p = clients.erase (p);
                else ++p;
        }

    private:
        static ServerInfo &_config ()
        {
            static ServerInfo config;
            return config;
        }

    public:
        static void SetConfig (const ServerInfo &config)
        {
            _config () = config;
        }
        static const ServerInfo &GetConfig ()
        {
            return _config ();
        }

    private:
        static ClientList &_clients ()
        {
            static ClientList clients;
            return clients;
        }

    public:
        static void AddClient (const GuestInfo &room)
        {
            _clients ().emplace (room.room, ClientState {
                room.guest, Temperature { 0 }, Temperature { 0 },
                Wind { 0 }, Energy { 0 }, Cost { 0 },
                std::chrono::system_clock::now ()
            });
        }

        static void Pulse (const RoomRequest &req)
        {
            // TODO: impl pulse
            // LogManager::BegRequest

            CheckAlive ();

            auto &roomState = GetClient (req.room);
            roomState.current = req.current;
            roomState.target = req.target;
            roomState.wind = req.wind;
            roomState.pulse = std::chrono::system_clock::now ();
        }

        static ClientState &GetClient (const RoomId &room)
        {
            try { return _clients ().at (room); }
            catch (...) { throw std::runtime_error ("No Such Room"); }
        }

        static const ClientList &GetClientList ()
        {
            return _clients ();
        }
    };
}

#endif AC_SERVER_SERVICE_H