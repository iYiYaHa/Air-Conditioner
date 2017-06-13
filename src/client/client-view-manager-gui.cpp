
//
// Air Conditioner - Client MVC View (GUI View Manager)
// Youjie Zhang, 2017
//

#include <memory>
#include <functional>

#include "client-controller.h"
#include "client-view-gui.h"

namespace Air_Conditioner
{
    void ClientViewManager::ToAuthView ()
    {
        using namespace std::placeholders;
        auto controller = std::make_shared<ClientFacadeController> (*this);
        _Navigate<AuthViewGUI> (
            std::bind (&ClientFacadeController::Auth, controller, _1));
    }

    void ClientViewManager::ToControlView (
        const GuestInfo &guestInfo)
    {
        using namespace std::placeholders;
        auto controller = std::make_shared<ClientFacadeController> (*this);
        _Navigate<ControlViewGUI> (
            guestInfo,
            std::bind (&ClientFacadeController::Pulse, controller, _1),
            std::bind (&ClientFacadeController::Simulate, controller, _1, _2));
    }
}
