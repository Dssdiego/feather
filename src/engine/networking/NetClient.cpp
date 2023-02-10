//
// Created by Diego S. Seabra on 24/11/22.
//

#include "NetClient.h"

void NetClient::Init()
{
    Logger::Info("Initializing yojimbo client...");

    if (!InitializeYojimbo())
        Logger::Error("error: failed to initialize Yojimbo!", "");
    else
        Logger::Info("Yojimbo initialized!");

    yojimbo_log_level(YOJIMBO_LOG_LEVEL_INFO);

    ClientServerConfig config;

    client = std::make_unique<yojimbo::Client>(GetDefaultAllocator(), Address("0.0.0.0"), config, adapter, advanceTime);

    Address serverAddress("127.0.0.1", ServerPort);

    uint8_t privateKey[KeyBytes];
    memset(privateKey, 0, KeyBytes);

    uint64_t clientId = rand(); // generating a random id for the client every time
    client->InsecureConnect( privateKey, clientId, serverAddress);

    char addressString[256];
    client->GetAddress().ToString( addressString, sizeof(addressString));
    printf("client address is %s\n", addressString);

    const int clientIndex = client->GetClientIndex();
    std::cout << "client Index: " << clientIndex << std::endl;
}

void NetClient::Run()
{
    client->SendPackets();
    client->ReceivePackets();

    if (client->IsConnecting())
        connectionStatus = ConnectionStatus::NET_CONNECTING;

    if (client->IsConnected())
        connectionStatus = ConnectionStatus::NET_CONNECTED;

    if (client->IsDisconnected())
        connectionStatus = ConnectionStatus::NET_DISCONNECTED;

    advanceTime += deltaTime;

    client->AdvanceTime(advanceTime);

    if (client->ConnectionFailed())
    {
        Logger::Error("Client->Server connection failed", "");
        return;
    }

    // FIXME: This reduces the FPS too much! Networking should be running separated from the drawing!
//    yojimbo_sleep(deltaTime);
}

void NetClient::Shutdown()
{
    // FIXME: Leaking memory! Disconnect and destroy client before exiting
    client->Disconnect();
//    client->DisconnectLoopback();

    Logger::Info("Shutting down yojimbo...");
    ShutdownYojimbo();

    // REVIEW: Destroy client! How?
    client = nullptr;
}

ConnectionStatus NetClient::GetConnectionStatus()
{
    return connectionStatus;
}

std::string NetClient::GetConnectionStatusStr()
{
    switch (connectionStatus)
    {
        case NET_CONNECTING: return "Connecting...";
        case NET_CONNECTED: return "Connected";
        case NET_DISCONNECTED: return "Disconnected";
        case NET_UNKNOWN: break;
    }

    return "Unknown";
}
