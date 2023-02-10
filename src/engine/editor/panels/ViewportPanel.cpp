//
// Created by Diego S. Seabra on 31/01/23
//

#include "ViewportPanel.h"

#include <imgui.h>

void ViewportPanel::Draw()
{
	ImGui::Begin("Game Viewport");

	//ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	//ImGui::Image(gameDescriptorSet, ImVec2{ 100, 100 });

	ImGui::Text("Game viewport will be here");

	ImGui::End();
}
