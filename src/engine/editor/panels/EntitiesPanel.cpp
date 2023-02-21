//
// Created by Diego S. Seabra on 31/01/23
//

#include "EntitiesPanel.h"

#include <imgui.h>
#include <imgui/IconsFontAwesome6.h>

// FIXME: all of this is required to be included before including the "Components.h" header
#include <glm/glm.hpp>
#include "../../rendering/shapes/Shape.h"
#include "../../common/structs.h"
#include "../../common/Color.h"
#include "../../entities/ECS.h"
// all... END
#include "../../components/Components.h"

#include "../EditorDraw.h"

namespace Feather
{
	void EntitiesPanel::Draw()
	{
		ImGui::Begin(ICON_FA_GLOBE " Entities");

		auto registry = ECS::GetRegistry();
		auto view = registry->view<const IdentityComponent, TransformComponent>();

		//selected.resize(view.size_hint());

		view.each([this, registry](const auto entity, const auto& identity, auto& transform)
			{
				// REVIEW: Probably there is a better way to do this!
				std::string entityName = ICON_FA_FEATHER_POINTED "  ";
		entityName.append(identity.name);

		ImGui::AlignTextToFramePadding();
		auto opened = ImGui::Selectable(entityName.c_str(), false, ImGuiSelectableFlags_AllowItemOverlap);

		ImGui::SameLine(ImGui::GetWindowWidth() - 30);
		if (ImGui::Button(ICON_FA_EYE))
			Logger::Warn("TODO: Change the visibility of this entity!");

		EditorDraw::SetTooltip("Show/Hide");

		if (opened)
		{
			selected = entity;
		}
			});

		if (EditorDraw::AlignedButton(ICON_FA_CIRCLE_PLUS " Add Entity"))
			ECS::Create();

		ImGui::End();
	}
}
