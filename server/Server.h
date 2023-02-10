//
// Created by Diego S. Seabra on 16/11/22.
//

#ifndef FEATHER_SERVER_H
#define FEATHER_SERVER_H

#include <iostream>
#include <yojimbo.h>
#include <shared.h>

enum class ServerMessageType {
    TEST,
    COUNT
};

// two channels, one for each type that Yojimbo supports
enum class ServerChannel {
    RELIABLE,
    UNRELIABLE,
    COUNT
};

// the client and server config
struct ServerConnectionConfig : yojimbo::ClientServerConfig {
    ServerConnectionConfig()  {
        numChannels = 2;
        channel[(int)ServerChannel::RELIABLE].type = yojimbo::CHANNEL_TYPE_RELIABLE_ORDERED;
        channel[(int)ServerChannel::UNRELIABLE].type = yojimbo::CHANNEL_TYPE_UNRELIABLE_UNORDERED;
    }
};

namespace Feather
{
    class Server
    {
    public:
        void Init();
        void Run();
        void Shutdown();

    private:
        bool shouldCloseServer = false;
        double advanceTime = 100.f;
        double deltaTime = 1.f/60.f; // 60 ticks
    };
}


#endif //FEATHER_SERVER_H
