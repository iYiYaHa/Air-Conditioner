
//
// Air Conditioner - Client MVC Controller
// BOT Man, 2017
//

#ifndef AC_CLIENT_CONTROLLER_H
#define AC_CLIENT_CONTROLLER_H

#include "client-model.h"
#include "client-view.h"
#include "client-protocol.h"

namespace Air_Conditioner
{
    class ClientFacadeController
    {
        ProtocolClient &_client;
        ClientViewManager &_viewManager;

    public:
        ClientFacadeController (ClientViewManager &viewManager)
            : _client (ProtocolClient::Instance ()), _viewManager (viewManager)
        {}

        void Auth (const GuestInfo &guest)
        {
            _client.Auth (guest);
            _viewManager.ToControlView (guest);
        }

        std::pair<ClientInfo, ServerInfo> Request (const RoomRequest &req)
        {
            return _client.Req (req);
        }

        std::pair<ClientInfo, ServerInfo> Pulse (const RoomInfo &info)
        {
            return _client.Pulse (info);
        }

        void Simulate (RoomInfo &room,
                       const RoomRequest &request,
                       const bool hasWind)
        {
            // TODO: impl sim algorithm
            // using static variables to store states

            if (hasWind)
                room.temp = request.temp + Temperature { 1 };
            else
                room.temp = request.temp - Temperature { 1 };
        }
    };
}

#endif AC_CLIENT_CONTROLLER_H