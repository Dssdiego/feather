//
// Created by Diego S. Seabra on 30/01/23
//

#include "DebugLogPanel.h"

#include <imgui.h>
#include <imgui/IconsFontAwesome6.h>

#include "../../profiling/Logger.h"
#include "../../common/Color.h"

void DebugLogPanel::Draw()
{
	ImGui::Begin(ICON_FA_CODE " Debug Log");
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
	auto logs = Logger::GetLogs();
	for (auto& log : logs)
	{
		if (log.find("DEBUG::") != std::string::npos)
			ImGui::PushStyleColor(ImGuiCol_Text, Color::ToImGuiColor(Color::Pink));

		if (log.find("INFO::") != std::string::npos)
			ImGui::PushStyleColor(ImGuiCol_Text, Color::ToImGuiColor(Color::Cyan));

		if (log.find("WARN::") != std::string::npos)
			ImGui::PushStyleColor(ImGuiCol_Text, Color::ToImGuiColor(Color::Yellow));

		if (log.find("ERROR::") != std::string::npos)
			ImGui::PushStyleColor(ImGuiCol_Text, Color::ToImGuiColor(Color::Red));

		ImGui::Text("%s", log.c_str());
		ImGui::PopStyleColor();

		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);
	}
	ImGui::EndChild();
	ImGui::End();
}
