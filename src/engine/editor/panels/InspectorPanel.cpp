//
// Created by Diego S. Seabra on 31/01/23
//

#include "InspectorPanel.h"

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
	void InspectorPanel::Draw(entt::entity selectedEntity)
	{
		currentEntity = selectedEntity;

		if (noSelection)
			ImGui::Text("No entity selected...");
		else
		{
			ImGui::Begin(ICON_FA_CUBE " Inspector");

			auto registry = ECS::GetRegistry();

			auto identity = registry->try_get<Identity>(selectedEntity);
			auto transform = registry->try_get<TransformComponent>(selectedEntity);
			auto renderer = registry->try_get<AtlasSpriteRenderer>(selectedEntity);
			auto area = registry->try_get<AreaComponent>(selectedEntity);
			auto draggable = registry->try_get<Draggable>(selectedEntity);
			auto card = registry->try_get<CardComponent>(selectedEntity);

			if (identity != nullptr)
				DrawIdentityComponent(*identity);
			if (transform != nullptr)
				DrawTransformComponent(*transform);
			if (renderer != nullptr)
				DrawRendererComponent(*renderer);
			if (area != nullptr)
				DrawAreaComponent(*area);
			if (draggable != nullptr)
				DrawDraggableComponent(*draggable);
			if (card != nullptr)
				DrawCardComponent(*card);

			if (EditorDraw::AlignedButton(ICON_FA_CIRCLE_PLUS " Add Component"))
				ImGui::OpenPopup("AddComponent");

			DrawAddComponentPopup();

			/*if (ImGui::TreeNode("World"))
			{
				ImGui::ColorEdit4("background color", (float *) World::GetWorldColorRef());
				ImGui::TreePop();
			}*/

			ImGui::End();
		}
	}

	void InspectorPanel::DrawIdentityComponent(Identity& identity)
	{
		DrawComponentStart();

		if (ImGui::TreeNodeEx(ICON_FA_TAG " Identity Component", ImGuiTreeNodeFlags_AllowItemOverlap))
		{
			ImGui::Text(identity.id.c_str());
			ImGui::Text(identity.name.c_str());

			ImGui::TreePop();
		}

		DrawComponentEnd();
	}

	// reference: https://github.com/TheCherno/Hazel/blob/master/Hazelnut/src/Panels/SceneHierarchyPanel.cpp
	void InspectorPanel::DrawTransformComponent(TransformComponent& transform)
	{
		DrawComponentStart();

		auto opened = ImGui::TreeNodeEx(ICON_FA_ARROWS_UP_DOWN_LEFT_RIGHT " Transform Component", ImGuiTreeNodeFlags_AllowItemOverlap);

		DrawOptionsPopup<TransformComponent>(transform);

		if (opened)
		{
			EditorDraw::Vec3Control("Position", transform.position);
			EditorDraw::Vec3Control("Offst", transform.offset);
			EditorDraw::Vec3Control("Scale", transform.scale, 1.f);
			EditorDraw::Vec3Control("Rotation", transform.rotation);

			ImGui::TreePop();
		}

		//if (ImGui::BeginPopupContextItem())
		//{
		//	if (ImGui::MenuItem("Delete Entity"))
		//		Logger::Info("Delete this entity!");

		//	ImGui::EndPopup();
		//}

		DrawComponentEnd();
	}

	void InspectorPanel::DrawAreaComponent(AreaComponent& area)
	{
		DrawComponentStart();

		auto opened = ImGui::TreeNode(ICON_FA_TABLE_CELLS " Area Component");

		DrawOptionsPopup<AreaComponent>(area);

		if (opened)
		{
			EditorDraw::Vec3Control("Area Min", area.min);
			EditorDraw::Vec3Control("Area Min", area.max);

			ImGui::TreePop();
		}

		DrawComponentEnd();
	}

	void InspectorPanel::DrawDraggableComponent(Draggable& draggable)
	{
		DrawComponentStart();

		auto opened = ImGui::TreeNode(ICON_FA_HAND_BACK_FIST " Draggable Component");

		DrawOptionsPopup<Draggable>(draggable);

		if (opened)
		{
			ImGui::Checkbox("Dragging", &draggable.dragging);
			EditorDraw::Vec3Control("Drag Offset", draggable.offset);

			ImGui::TreePop();
		}

		DrawComponentEnd();
	}

	void InspectorPanel::DrawRendererComponent(AtlasSpriteRenderer& renderer)
	{
		DrawComponentStart();

		auto opened = ImGui::TreeNode(ICON_FA_IMAGE " Renderer Component");

		DrawOptionsPopup<AtlasSpriteRenderer>(renderer);

		if (opened)
		{
			std::string pipelineStr = "Pipeline: " + renderer.pipeline;
			std::string textureStr = "Texture: " + renderer.textureName;
			std::string tileStr = "Tile: " + std::to_string(renderer.tile);

			ImGui::Text(pipelineStr.c_str());
			ImGui::Text(textureStr.c_str());
			ImGui::Text(tileStr.c_str());
			ImGui::Checkbox("Visible", &renderer.visible);

			ImGui::TreePop();
		}
		DrawComponentEnd();
	}

	void InspectorPanel::DrawCardComponent(CardComponent& card)
	{
		DrawComponentStart();

		if (ImGui::TreeNode(ICON_FA_HEART " Card Component"))
		{
			std::string valueStr = "Value: " + std::to_string(card.value);
			ImGui::Text(valueStr.c_str());

			ImGui::TreePop();
		}
		DrawComponentEnd();
	}

	void InspectorPanel::DrawComponentStart()
	{
		ImGui::Spacing();
	}

	void InspectorPanel::DrawComponentEnd()
	{
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
	}

	// TODO: Refactor
	void InspectorPanel::DrawAddComponentPopup()
	{
		if (ImGui::BeginPopup("AddComponent"))
		{
			DisplayAddComponentEntry<Identity>("Identity");
			DisplayAddComponentEntry<TransformComponent>("Transform");
			DisplayAddComponentEntry<AtlasSpriteRenderer>("Renderer");
			DisplayAddComponentEntry<CardComponent>("Card");
			DisplayAddComponentEntry<AreaComponent>("Area");
			DisplayAddComponentEntry<Draggable>("Draggable");

			// can't add empty components! entt won't allow it!
			//DisplayAddComponentEntry<JoystickComponent>("Joystick");
			//DisplayAddComponentEntry<AIComponent>("AI");

			ImGui::EndPopup();
		}
	}

	template<typename T>
	void InspectorPanel::DisplayAddComponentEntry(const std::string& entryName)
	{
		auto registry = ECS::GetRegistry();
		auto comp = registry->try_get<T>(currentEntity);

		// if we don't have the component, we show the menu to be able to add it to the entity
		if (comp == nullptr)
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				T newComp = T();

				ECS::AddComponent<T>(currentEntity, newComp);
				ImGui::CloseCurrentPopup();
			}
		}
	}

	template<typename T>
	void InspectorPanel::DrawOptionsPopup(T t)
	{
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
			ImGui::OpenPopup("OptionsPopup");

		if (ImGui::BeginPopup("OptionsPopup"))
		{
			if (ImGui::MenuItem(ICON_FA_TRASH "  Delete"))
				ECS::RemoveComponent<T>(currentEntity, t);

			ImGui::EndPopup();
		}

		//EditorDraw::SetTooltip("Remove component");
	}
}
