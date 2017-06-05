
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
    if (argc > 1) serverIp = argv[1];
    if (argc > 2) serverPort = std::stoi (argv[2]);

    // Dependency Injection
    ProtocolClient::Init (serverIp, (unsigned short) serverPort);

    // Start the view
    ClientViewManager viewManager;
    viewManager.ToAuthView ();
    viewManager.Start ();

    return 0;
}