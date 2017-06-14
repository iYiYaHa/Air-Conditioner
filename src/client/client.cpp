
//
// Air Conditioner - Client
// BOT Man, 2017
//

#include <iostream>
#include <string>

#include "client-protocol.h"
#include "client-view.h"

#define IPADDR "127.0.0.1"
#define PORT 5768

int main (int argc, char *argv[])
{
    using namespace Air_Conditioner;

    // Read from start-up args to Init Protocol
    auto serverIp = IPADDR;
    auto serverPort = PORT;

    try
    {
        if (argc > 1) serverIp = argv[1];
        if (argc > 2) serverPort = std::stoi (argv[2]);
    }
    catch (...)
    {
        // TODO: prompt in Qt
        std::cerr << "Invalid command line args" << std::endl;
        return 1;
    }

    try
    {
        // Dependency Injection
        ProtocolClient::Init (serverIp, (unsigned short) serverPort);

        // Start the view
        ClientViewManager viewManager;
        viewManager.ToAuthView ();
        viewManager.Start ();
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what () << std::endl;
        return 2;
    }
    catch (...)
    {
        std::cerr << "Unknown Error" << std::endl;
        return 3;
    }

    return 0;
}