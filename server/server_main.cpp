//
// Created by Diego S. Seabra on 16/11/22.
//

#include <exception>
#include <iostream>
#include "Server.h"

int main()
{
    Feather::Server server;
    try
    {
        server.Init();
        server.Run();
        server.Shutdown();
    } catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}