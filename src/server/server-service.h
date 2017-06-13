
//
// Air Conditioner - Server MVC Service
// BOT Man, 2017
//

#ifndef AC_SERVER_SERVICE_H
#define AC_SERVER_SERVICE_H

#include <exception>
#include <queue>

#define MAXCLIENT 2
#define THRESHOLD 1
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
            // TODO: impl turn on
        }
        static void TurnOff (const RoomId &room)
        {
            // TODO: impl turn off
        }

        static void BegRequest (const RoomId &room,
                                const ClientState &state)
        {
            // TODO: impl beg req
        }
        static void EndRequest (const RoomId &room)
        {
            // TODO: impl end req
        }

        static std::pair<TimePoint, TimePoint> GetTimeRange ()
        {
            // TODO: impl here
            return std::make_pair (std::chrono::system_clock::now (),
                                   std::chrono::system_clock::now () + std::chrono::hours { 24 });
        }
        static LogOnOffList GetOnOff (const TimePoint &from, const TimePoint &to)
        {
            // TODO: impl here
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
            // TODO: impl here
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

        // In Database
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

    class ConfigManager
    {
        // In Memory
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
    };

    class ScheduleManager
    {
        static void Schedule ()
        {
            auto &clients = _clients ();

            // Handle Server Off
            if (!ConfigManager::GetConfig ().isOn)
            {
                for (auto &client : clients)
                    client.second.hasWind = false;
                return;
            }

            using Entry = std::pair<RoomId, ClientState>;

            // Comp strategy (hard coded)
            auto cmp = [] (const Entry &a, const Entry &b)
            {
                auto scoreA = std::abs (a.second.current - a.second.target);
                auto scoreB = std::abs (b.second.current - b.second.target);
                return scoreA < scoreB;
            };

            // Need to Change strategy (hard coded)
            auto needToChange = [] (const ClientState &a)
            {
                return std::abs (a.target - a.current) >
                    Temperature { THRESHOLD };
            };

            // Add clients that need to change and Sort in heap
            std::priority_queue<Entry, std::vector<Entry>,
                decltype (cmp)> queue (cmp);
            for (auto &client : clients)
                if (needToChange (client.second))
                    queue.push (client);
                else
                    client.second.hasWind = false;

            // Select top 'MAXCLIENT
            auto count = 0;
            std::unordered_map<RoomId, bool> hasWindList;
            while (!queue.empty ())
            {
                if (++count > MAXCLIENT)
                    break;

                hasWindList[queue.top ().first] = true;
                queue.pop ();
            }

            // Set their hasWind
            for (auto &client : clients)
                if (hasWindList[client.first])
                    client.second.hasWind = true;
        }

        static void CheckAlive ()
        {
            const auto deadTime = std::chrono::system_clock::now () -
                std::chrono::seconds { DEADTIME };

            auto &clients = _clients ();
            for (auto p = clients.begin (); p != clients.end ();)
                if (p->second.pulse < deadTime)
                {
                    auto room = p->first;
                    p = clients.erase (p);

                    // Write to log
                    LogManager::TurnOff (room);
                    LogManager::EndRequest (room);
                }
                else ++p;
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
                Wind { 0 }, Energy { 0 }, Cost { 0 }, false,
                std::chrono::system_clock::now ()
            });

            // Write to log
            LogManager::TurnOn (room.room);
        }

        static void Pulse (const RoomRequest &req)
        {
            // Check Alive
            CheckAlive ();

            auto &roomState = GetClient (req.room);
            auto isChanged =
                roomState.current != req.current ||
                roomState.target != req.target ||
                roomState.wind != req.wind;

            roomState.current = req.current;
            roomState.target = req.target;
            roomState.wind = req.wind;

            // Write to log
            if (isChanged)
                LogManager::BegRequest (req.room, roomState);

            // Schedule
            Schedule ();

            // Get Delta Time and Pulse
            auto now = std::chrono::system_clock::now ();
            std::chrono::duration<double> deltaTime = now - roomState.pulse;
            roomState.pulse = now;

            // Calc Energy and Cost
            if (roomState.hasWind)
            {
                // Get Delta Energy
                auto deltaEnergy = Energy { deltaTime.count () };
                if (roomState.wind == 1)
                    deltaEnergy = deltaEnergy * 0.8;
                else if (roomState.wind == 3)
                    deltaEnergy = deltaEnergy * 1.3;

                // Add up energy
                roomState.energy += deltaEnergy;
                roomState.cost = roomState.energy * 5;
            }
        }

        static ClientState &GetClient (const RoomId &room)
        {
            try { return _clients ().at (room); }
            catch (...) { throw std::runtime_error ("No such active room, try login again"); }
        }

        static const ClientList &GetClientList ()
        {
            return _clients ();
        }
    };
}

#endif AC_SERVER_SERVICE_H