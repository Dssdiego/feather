//
// Created by Diego S. Seabra on 31/01/23
//

#include "EngineStatusPanel.h"

#include <imgui.h>
#include <imgui/IconsFontAwesome6.h>

#include "../../core/Time.h"
#include "../../rendering/Window.h"
#include "../../rendering/SpriteBatcher.h"

namespace Feather
{
	void EngineStatusPanel::Draw()
	{
		ImGui::Begin(ICON_FA_GEAR " Engine Status");

		deltaStr = "Delta (ms): " + std::to_string(static_cast<double>(Time::Delta));
		fpsStr = "FPS: " + std::to_string(static_cast<uint32_t>(Window::GetFPS()));
		frameStr = "Frame: " + std::to_string(Window::GetFrameNumber());
		drawCallStr = "Draw Calls: " + std::to_string(SpriteBatcher::GetDrawCallCount());
		verticesStr = "Vertices: " + std::to_string(SpriteBatcher::GetVerticesCount());
		indicesStr = "Indices: " + std::to_string(SpriteBatcher::GetIndicesCount());
		quadsStr = "Quads: " + std::to_string(SpriteBatcher::GetQuadCount());

		ImGui::Text("%s", deltaStr.c_str());
		ImGui::Text("%s", fpsStr.c_str());
		ImGui::Text("%s", frameStr.c_str());
		ImGui::Text("%s", drawCallStr.c_str());
		ImGui::Text("%s", verticesStr.c_str());
		ImGui::Text("%s", indicesStr.c_str());
		ImGui::Text("%s", quadsStr.c_str());

		ImGui::End();
	}
}
