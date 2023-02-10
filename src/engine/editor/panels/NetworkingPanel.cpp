//
// Created by Diego S. Seabra on 30/01/23
//

#include "NetworkingPanel.h"

#include <imgui.h>

#include "../../networking/NetClient.h"

void NetworkingPanel::Draw()
{
    ImGui::Begin("Networking");

    auto connectionStatus = NetClient::GetConnectionStatusStr();
    ImGui::Text("Connection Status: %s", connectionStatus.c_str());

    ImGui::End();
}
