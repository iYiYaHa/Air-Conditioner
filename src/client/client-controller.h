
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

        std::pair<ClientInfo, ServerInfo> Pulse (const RoomRequest &req)
        {
            return _client.Pulse (req);
        }

        void Simulate (RoomRequest &request,
                       const bool hasWind)
        {
            // TODO: impl sim algorithm
            // using static variables to store states

            if (hasWind)
                request.current= request.target + Temperature { 1 };
            else
                request.current = request.target - Temperature { 1 };
        }
    };
}

#endif AC_CLIENT_CONTROLLER_H