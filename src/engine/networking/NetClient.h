//
// Created by Diego S. Seabra on 24/11/22.
//

#ifndef FEATHER_NETCLIENT_H
#define FEATHER_NETCLIENT_H

#include "../profiling/Logger.h"

#include <yojimbo/yojimbo.h>
#include <yojimbo/shared.h>

// REVIEW: Why this conflicts with Input.h?
enum ConnectionStatus
{
    NET_UNKNOWN,
    NET_CONNECTING,
    NET_CONNECTED,
    NET_DISCONNECTED
};

class NetClient
{
public:
    static void Init();
    static void Run();
    static void Shutdown();

    static ConnectionStatus GetConnectionStatus();
    static std::string GetConnectionStatusStr();

private:
    inline static std::unique_ptr<yojimbo::Client> client = nullptr;
    inline static double advanceTime = 100.f;
    inline static double deltaTime = 1.f/60.f; // 60 ticks

    inline static ConnectionStatus connectionStatus = ConnectionStatus::NET_UNKNOWN;
};


#endif //FEATHER_NETCLIENT_H
