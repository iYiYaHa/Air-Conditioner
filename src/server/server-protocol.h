
//
// Air Conditioner - Server Protocol Module
// BOT Man, 2017
//

#ifndef AC_SERVER_PROTOCOL_H
#define AC_SERVER_PROTOCOL_H

#include "../common/bot-cs.h"
#include "../common/common-protocol.h"

#include "server-protocol-controller.h"

namespace Air_Conditioner
{
    class ProtocolServer : public BOT_CS::Server
    {
    public:
        ProtocolServer (unsigned short port)
            : BOT_CS::Server (port)
        {
            // Do not use dependency inversion here ...
            ProtocolController controller;

            SetHandler (AUTH, [&] (json &response, const json &request)
            {
                auto req = Protocol::JsonToGuestInfo (request);
                controller.Auth (req);
                auto ret = controller.GetInfo (req.room);
                response = Protocol::ClientServerInfoToJson (ret.first, ret.second);
            });

            SetHandler (PULSE, [&] (json &response, const json &request)
            {
                auto req = Protocol::JsonToRoomRequest (request);
                controller.Pulse (req);
                auto ret = controller.GetInfo (req.room);
                response = Protocol::ClientServerInfoToJson (ret.first, ret.second);
            });
        }
    };
}

#endif AC_SERVER_PROTOCOL_H