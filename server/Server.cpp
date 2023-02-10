//
// Created by Diego S. Seabra on 16/11/22.
//

#include "Server.h"

using namespace Feather;

namespace Feather
{
    void Server::Init()
    {
        std::cout << "Initializing yojimbo server" << std::endl;

        if (!InitializeYojimbo())
            std::cout << "error: failed to initialize Yojimbo!" << std::endl;
        else
            std::cout << "yojimbo initialized!" << std::endl;

        yojimbo_log_level(YOJIMBO_LOG_LEVEL_INFO);
    }

    void Server::Run()
    {
        // TODO: Make a way to configure the server properly
        ClientServerConfig config;

        uint8_t privateKey[KeyBytes];
        memset(privateKey, 0, KeyBytes);

        yojimbo::Server server(GetDefaultAllocator(), privateKey, Address("127.0.0.1", ServerPort), config, adapter, advanceTime);

        server.Start( MaxClients);

        char addressString[256];
        server.GetAddress().ToString(addressString, sizeof(addressString));
        printf( "started server @ %s (max clients: %d)\n", addressString, MaxClients);

        while (!shouldCloseServer)
        {
            server.SendPackets();
            server.ReceivePackets();

//            if (server.IsClientConnected(0))
//            {
//                std::cout << "Client 0 connected!" << std::endl;
//                break;
//            }

            advanceTime += deltaTime;

            server.AdvanceTime(advanceTime);

            if (!server.IsRunning())
            {
                std::cout << "Closing server, something went wrong" << std::endl;
                break;
            }

            yojimbo_sleep(deltaTime);
        }

        std::cout << "stopping server" << std::endl;
        server.Stop();
    }

    void Server::Shutdown()
    {
        std::cout << "Shutting down yojimbo" << std::endl;
        ShutdownYojimbo();
    }
}