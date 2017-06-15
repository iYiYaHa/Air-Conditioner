
//
// Air Conditioner - Server MVC Service
// BOT Man, 2017
//

#ifndef AC_SERVER_SERVICE_H
#define AC_SERVER_SERVICE_H

#include <exception>
#include <queue>

#define MAXCLIENT 2
#define THRESHOLD 1.0
#define DEADTIME 3
#define DBNAME "ac.db"

#include "ormlite/ormlite.h"
#include "server-model.h"
#include "time-helper.h"

namespace Air_Conditioner
{
    class LogManager
    {
        struct OnOffEntity
        {
            int id; RoomId room;
            time_t timeBeg, timeEnd;

            ORMAP ("OnOffLog", id, room, timeBeg, timeEnd);
        };

        struct RequestEntity
        {
            int id; RoomId room;
            time_t timeBeg, timeEnd;
            Temperature tempBeg, tempEnd;
            Cost costBeg, costEnd;
            Wind wind;

            ORMAP ("RequestLog", id, room, timeBeg, timeEnd,
                   tempBeg, tempEnd, wind, costBeg, costEnd);
        };

        // In Database
        static BOT_ORM::ORMapper &_mapper ()
        {
            static BOT_ORM::ORMapper mapper (DBNAME);
            static auto hasInit = false;

            if (!hasInit)
            {
                try { mapper.CreateTbl (OnOffEntity {}); }
                catch (...) {}
                try { mapper.CreateTbl (RequestEntity {}); }
                catch (...) {}
                hasInit = true;
            }
            return mapper;
        }

    public:
        static void WriteOnOff (const RoomId &room,
                                const LogOnOff &entry)
        {
            // TODO: time conversion
            auto &mapper = _mapper ();
            mapper.Insert (OnOffEntity {
                0, room,
                std::chrono::system_clock::to_time_t (entry.timeBeg),
                std::chrono::system_clock::to_time_t (entry.timeEnd)
            }, false);
        }

        static void WriteRequest (const RoomId &room,
                                  const LogRequest &entry)
        {
            // TODO: time conversion
            auto &mapper = _mapper ();
            mapper.Insert (RequestEntity {
                0, room,
                std::chrono::system_clock::to_time_t (entry.timeBeg),
                std::chrono::system_clock::to_time_t (entry.timeEnd),
                entry.tempBeg, entry.tempEnd,
                entry.costBeg, entry.costEnd,
                entry.wind
            }, false);
        }

        static std::pair<TimePoint, TimePoint> GetTimeRange ()
        {
            static OnOffEntity onOffEntity;
            static RequestEntity requestEntity;
            static auto field = BOT_ORM::FieldExtractor {
                onOffEntity, requestEntity };

            auto &mapper = _mapper ();
            auto minTime = mapper.Query (onOffEntity)
                .Aggregate (BOT_ORM::Expression::Min (
                    field (onOffEntity.timeBeg)));
            auto maxTime = mapper.Query (onOffEntity)
                .Aggregate (BOT_ORM::Expression::Max (
                    field (onOffEntity.timeEnd)));

            auto timeBeg = (minTime == nullptr) ?
                std::chrono::system_clock::now () :
                std::chrono::system_clock::from_time_t (minTime.Value ());
            auto timeEnd = (maxTime == nullptr) ?
                std::chrono::system_clock::now () + std::chrono::hours { 24 } :
                std::chrono::system_clock::from_time_t (maxTime.Value ());

            return std::make_pair (std::move (timeBeg), std::move (timeEnd));
        }

        static LogOnOffList GetOnOff (const TimePoint &from,
                                      const TimePoint &to)
        {
            static OnOffEntity entity;
            static auto field = BOT_ORM::FieldExtractor { entity };

            auto &mapper = _mapper ();
            auto result = mapper.Query (entity)
                .Where (
                    field (entity.timeBeg) >= std::chrono::system_clock::to_time_t (from) &&
                    field (entity.timeEnd) < std::chrono::system_clock::to_time_t (to)
                )
                .ToList ();

            LogOnOffList ret;
            for (const auto &entry : result)
            {
                ret[entry.room].emplace_back (LogOnOff {
                    std::chrono::system_clock::from_time_t (entry.timeBeg),
                    std::chrono::system_clock::from_time_t (entry.timeEnd)
                });
            }
            return ret;
        }

        static LogRequestList GetRequest (const TimePoint &from,
                                          const TimePoint &to)
        {
            static RequestEntity entity;
            static auto field = BOT_ORM::FieldExtractor { entity };

            auto &mapper = _mapper ();
            auto result = mapper.Query (entity)
                .Where (
                    field (entity.timeBeg) >= std::chrono::system_clock::to_time_t (from) &&
                    field (entity.timeEnd) < std::chrono::system_clock::to_time_t (to)
                )
                .ToList ();

            LogRequestList ret;
            for (const auto &entry : result)
            {
                ret[entry.room].emplace_back (LogRequest {
                    std::chrono::system_clock::from_time_t (entry.timeBeg),
                    std::chrono::system_clock::from_time_t (entry.timeEnd),
                    entry.tempBeg, entry.tempEnd,
                    entry.costBeg, entry.costEnd,
                    entry.wind
                });
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
        static ClientList &_clients ()
        {
            static ClientList clients;
            return clients;
        }

        static bool HasWind (const ClientState &state,
                             const ServerInfo &config)
        {
            // Case: Server Off
            if (!config.isOn) return false;

            // Case: Enough already
            if (config.mode == 0 &&
                state.current <= state.target) return false;
            if (config.mode == 1 &&
                state.current >= state.target) return false;

            // Case: Need to send wind
            if (config.mode == 0 &&
                state.current - state.target >= THRESHOLD) return true;
            if (config.mode == 1 &&
                state.target - state.current >= THRESHOLD) return true;

            // Case: Keep the state
            return state.hasWind;
        }

        static void Schedule ()
        {
            auto &clients = _clients ();
            const auto &config = ConfigManager::GetConfig ();

            auto count = 0;
            std::unordered_map<RoomId, bool> hasWindList;
            for (auto &client : clients)
                if (HasWind (client.second, config) && count < 3)
                {
                    hasWindList[client.first] = true;
                    ++count;
                }
                else
                    hasWindList[client.first] = false;

            for (auto &client : clients)
                client.second.hasWind = hasWindList[client.first];
        }

        static void HandleReqBeg (const RoomId &room,
                                  const TimePoint &time,
                                  ClientState &state)
        {
            state.lastRequest.timeBeg = time;
            state.lastRequest.tempBeg = state.current;
            state.lastRequest.costBeg = state.cost;
            state.lastRequest.wind = state.wind;
        }

        static void HandleReqEnd (const RoomId &room,
                                  const TimePoint &time,
                                  ClientState &state)
        {
            state.lastRequest.timeEnd = time;
            state.lastRequest.tempEnd = state.current;
            state.lastRequest.costEnd = state.cost;

            LogManager::WriteRequest (room, state.lastRequest);
        }

        static void HandleTurnOn (const RoomId &room,
                                  const TimePoint &time,
                                  ClientState &state)
        {
            state.lastOnOff.timeBeg = time;
        }

        static void HandleTurnOff (const RoomId &room,
                                   const TimePoint &time,
                                   ClientState &state)
        {
            if (state.lastRequest.costBeg != state.cost)
                HandleReqEnd (room, time, state);

            state.lastOnOff.timeEnd = state.pulse;

            LogManager::WriteOnOff (room, state.lastOnOff);
        }

        static void CheckAlive ()
        {
            auto now = std::chrono::system_clock::now ();
            auto deadTime = now - std::chrono::seconds { DEADTIME };

            auto &clients = _clients ();
            for (auto p = clients.begin (); p != clients.end ();)
                if (p->second.pulse < deadTime)
                {
                    HandleTurnOff (p->first, now, p->second);
                    p = clients.erase (p);
                }
                else ++p;
        }

    public:
        static void AddClient (const GuestInfo &room)
        {
            // Check Alive
            CheckAlive ();

            auto &clients = _clients ();

            // Login already
            if (clients.find (room.room) != clients.end ())
                throw std::runtime_error ("Login already");

            // New Login
            auto now = std::chrono::system_clock::now ();
            auto state = ClientState {
                room.guest, Temperature { 0 }, Temperature { 0 }, Wind { 0 },
                false, Energy { 0 }, Cost { 0 }, now
            };
            HandleTurnOn (room.room, now, state);
            clients.emplace (room.room, std::move (state));
        }

        static void RemoveClient (const RoomId &room)
        {
            try
            {
                auto &clients = _clients ();
                auto &roomState = GetClient (room);
                auto now = std::chrono::system_clock::now ();

                HandleTurnOff (room, now, roomState);
                clients.erase (room);
            }
            catch (...) {}
        }

        static void Pulse (const RoomRequest &req)
        {
            // Check Alive
            CheckAlive ();

            auto &roomState = GetClient (req.room);

            // Track traits for Beg/End Request
            auto isChanged =
                roomState.target != req.target ||
                roomState.wind != req.wind;
            auto hasWindBefore = roomState.hasWind;

            // Update Client State
            roomState.current = req.current;
            roomState.target = req.target;
            roomState.wind = req.wind;

            // Schedule
            Schedule ();

            // Get Delta Time and Pulse
            auto now = std::chrono::system_clock::now ();
            std::chrono::duration<double> deltaTime = now - roomState.pulse;
            roomState.pulse = now;

            // Handle Beg/End Request
            if (!hasWindBefore && roomState.hasWind)
                HandleReqBeg (req.room, now, roomState);
            else if (hasWindBefore && roomState.hasWind)
                HandleReqEnd (req.room, now, roomState);
            else if (isChanged)
            {
                HandleReqEnd (req.room, now, roomState);
                HandleReqBeg (req.room, now, roomState);
            }

            // Calc Energy and Cost
            if (roomState.hasWind)
            {
                // Get Delta Energy
                auto deltaEnergy = Energy { deltaTime.count () / 60.0 };
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
            catch (...) { throw std::runtime_error ("Logout already"); }
        }

        static const ClientList &GetClientList ()
        {
            // Check Alive
            CheckAlive ();

            return _clients ();
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

            ScheduleManager::RemoveClient (room);
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

            ScheduleManager::AddClient (guest);
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
}

#endif AC_SERVER_SERVICE_H