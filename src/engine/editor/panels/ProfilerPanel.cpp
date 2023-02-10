//
// Created by Diego S. Seabra on 30/01/23
//

#include "ProfilerPanel.h"

#include <imgui.h>
#include "../../rendering/Window.h"

void ProfilerPanel::Draw()
{
    ImGui::Begin("Profiler");
    auto arr = Window::GetFPSHistory();
    ImGui::PlotLines("FPS", arr.data(), arr.size(), 0, nullptr, 0.f, 1000.f, ImVec2(0, 80.0f));
    ImGui::End();
}
