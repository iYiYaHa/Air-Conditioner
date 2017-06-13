
//
// Air Conditioner - Server MVC Protocol Controller
// BOT Man, 2017
//

#ifndef AC_SERVER_PROTOCOL_CONTROLLER_H
#define AC_SERVER_PROTOCOL_CONTROLLER_H

#include <utility>

#include "server-service.h"

namespace Air_Conditioner
{
    class ProtocolController
    {
    public:
        void Auth (const GuestInfo &guest)
        {
            GuestManager::AuthGuest (guest);
            ScheduleManager::AddClient (guest);
        }

        std::pair<ClientInfo, ServerInfo> Pulse (const RoomRequest &req)
        {
            ScheduleManager::Pulse (req);

            const auto &serverInfo = ConfigManager::GetConfig ();
            const auto &clientState = ScheduleManager::GetClient (req.room);
            ClientInfo clientInfo {
                clientState.hasWind, clientState.energy, clientState.cost
            };
            return std::make_pair (std::move (clientInfo),
                                   serverInfo);
        }
    };
}

#endif AC_SERVER_PROTOCOL_CONTROLLER_H