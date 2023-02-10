//
// Created by Diego S. Seabra on 31/01/23
//

#include "WorldEditorPanel.h"

#include <imgui.h>

void WorldEditorPanel::Draw()
{
    //ImGui::Begin("World Editor");
    //auto imageId = SpriteAtlas::GetImGuiImage();

    //// create image button grid
    //ImGuiStyle& style = ImGui::GetStyle();
    //ImVec2 buttonSize = ImVec2(48, 48);
    //auto tiles = Grid::GetTiles();
    //int buttonCount = (int) tiles.size();
    //float windowVisibleXPos = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
    //for (int n = 0; n < buttonCount; n++)
    //{
    //    ImGui::PushID(n);

    //    auto tilePos = tiles[n].tilePosition;
    //    auto uvCoords = SpriteAtlas::GetUVCoordinate((int) tilePos.x, (int) tilePos.y);
    //    if (ImGui::ImageButton(imageId, buttonSize, ImVec2(uvCoords.x, uvCoords.y), ImVec2(uvCoords.z, uvCoords.w)))
    //        std::cout << "(X: " << tilePos.x << ", Y: " << tilePos.y << ")" << std::endl;

    //    float lastButtonXPos = ImGui::GetItemRectMax().x;
    //    float nextButtonXPos = lastButtonXPos + style.ItemSpacing.x + buttonSize.x; // Expected position if next button was on same line

    //    if (n + 1 < buttonCount && nextButtonXPos < windowVisibleXPos)
    //        ImGui::SameLine();

    //    ImGui::PopID();
    //}

    //ImGui::End();
}
