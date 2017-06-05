
//
// Air Conditioner - Common Protocol Module
// BOT Man, 2017
//

#ifndef AC_COMMON_PROTOCOL_H
#define AC_COMMON_PROTOCOL_H

#include "json/json.hpp"
#include "common-model.h"

// Data

#define ROOM "room"
#define GUEST "guest"

#define TEMP "temp"
#define WIND "wind"

#define HASWIND "hasWind"
#define ENERGY "energy"
#define COST "cost"

#define SERVERON "on"
#define WORKINGMODE "mode"
#define PULSEFREQ "freq"

// Action

#define AUTH "auth"
#define REQUEST "request"
#define PULSE "pulse"

// Helper

namespace Air_Conditioner
{
    class Protocol
    {
        using json = nlohmann::json;

    public:
        static json GuestInfoToJson (const GuestInfo &guest)
        {
            return json {
                { ROOM, guest.room },
                { GUEST, guest.guest }
            };
        }
        static GuestInfo JsonToGuestInfo (const json &j)
        {
            return GuestInfo {
                j.at (ROOM).get<RoomId> (),
                j.at (GUEST).get<GuestId> ()
            };
        }

        static json RoomRequestToJson (const RoomRequest &request)
        {
            return json {
                { ROOM, request.room },
                { TEMP, request.temp },
                { WIND, request.wind }
            };
        }
        static RoomRequest JsonToRoomRequest (const json &j)
        {
            return RoomRequest {
                j.at (ROOM).get<RoomId> (),
                j.at (TEMP).get<Temperature> (),
                j.at (WIND).get<Wind> ()
            };
        }

        static json RoomInfoToJson (const RoomInfo &info)
        {
            return json {
                { ROOM, info.room },
                { TEMP, info.temp }
            };
        }
        static RoomInfo JsonToRoomInfo (const json &j)
        {
            return RoomInfo {
                j.at (ROOM).get<RoomId> (),
                j.at (TEMP).get<Temperature> ()
            };
        }

        static json ClientServerInfoToJson (const ClientInfo &client,
                                            const ServerInfo &server)
        {
            return json {
                { HASWIND, client.hasWind },
                { ENERGY, client.energy },
                { COST, client.cost },
                { SERVERON, server.isOn },
                { WORKINGMODE, server.mode },
                { PULSEFREQ, server.pulseFreq }
            };
        }
        static ClientInfo JsonToClientInfo (const json &j)
        {
            return ClientInfo {
                j.at (HASWIND).get<bool> (),
                j.at (ENERGY).get<Energy> (),
                j.at (COST).get<Cost> ()
            };
        }
        static ServerInfo JsonToServerInfo (const json &j)
        {
            return ServerInfo {
                j.at (SERVERON).get<bool> (),
                j.at (WORKINGMODE).get<WorkingMode> (),
                j.at (PULSEFREQ).get<PulseFreq> ()
            };
        }
    };
}

#undef ROOM
#undef GUEST

#undef TEMP
#undef WIND
#undef ENERGY
#undef COST

#undef SERVERON
#undef WORKINGMODE
#undef PULSEFREQ

#endif AC_COMMON_PROTOCOL_H