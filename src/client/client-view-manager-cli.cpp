
//
// Air Conditioner - Client MVC View (CLI View Manager)
// BOT Man, 2017
//

#include <memory>
#include <functional>

#include "client-controller.h"
#include "client-view-cli.h"

namespace Air_Conditioner
{
    void ClientViewManager::ToAuthView ()
    {
        using namespace std::placeholders;
        auto controller = std::make_shared<ClientFacadeController> (*this);
        _Navigate<AuthViewCLI> (
            std::bind (&ClientFacadeController::Auth,
            std::move (controller), _1));
    }

    void ClientViewManager::ToControlView (
        const GuestInfo &guestInfo)
    {
        using namespace std::placeholders;
        auto controller = std::make_shared<ClientFacadeController> (*this);
        _Navigate<ControlViewCLI> (
            guestInfo,
            std::bind (&ClientFacadeController::Request, controller, _1),
            std::bind (&ClientFacadeController::Pulse, controller, _1),
            std::bind (&ClientFacadeController::Simulate, controller, _1, _2, _3));
    }
}