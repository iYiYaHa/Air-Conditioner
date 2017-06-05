
//
// Air Conditioner - Server MVC View (CLI View Manager)
// BOT Man, 2017
//

#include <memory>
#include <functional>

#include "server-view-controller.h"
#include "server-view-cli.h"

namespace Air_Conditioner
{
    void ServerViewManager::ToWelcomeView ()
    {
        using namespace std::placeholders;
        auto controller = std::make_shared<WelcomeController> (*this);
        _Navigate<WelcomeViewCLI> (
            std::bind (&WelcomeController::Nav,
            std::move (controller), _1));
    }

    void ServerViewManager::ToConfigView ()
    {
        using namespace std::placeholders;
        auto controller = std::make_shared<ConfigController> ();
        _Navigate<ConfigViewCLI> (
            controller->GetConfig (),
            std::bind (&ConfigController::SetConfig, controller, _1),
            [&] { ToWelcomeView (); });
    }

    void ServerViewManager::ToGuestView ()
    {
        using namespace std::placeholders;
        auto controller = std::make_shared<GuestInfoController> ();
        _Navigate<GuestViewCLI> (
            controller->GetGuestList (),
            std::bind (&GuestInfoController::AddGuest, controller, _1),
            std::bind (&GuestInfoController::RemoveGuest, controller, _1),
            [&] { ToWelcomeView (); });
    }

    void ServerViewManager::ToLogView ()
    {
        using namespace std::placeholders;
        auto controller = std::make_shared<LogController> ();
        // TODO: impl navigate to log view
    }

    void ServerViewManager::ToClientView ()
    {
        using namespace std::placeholders;
        auto controller = std::make_shared<ClientController> ();
        _Navigate<ClientViewCLI> (
            ConfigController ().GetConfig ().pulseFreq,
            std::bind (&ClientController::GetClientList,
            std::move (controller)),
            [&] { ToWelcomeView (); });
    }
}