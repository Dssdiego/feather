//
// Created by Diego S. Seabra on 31/01/23
//

#include "InspectorPanel.h"

#include <imgui.h>
#include <imgui/IconsFontAwesome6.h>
#include "../../../../game/src/editor/GameInspectorPanel.h"

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

			auto identity = registry->try_get<IdentityComponent>(selectedEntity);
			auto transform = registry->try_get<TransformComponent>(selectedEntity);
			auto renderer = registry->try_get<SpriteRendererComponent>(selectedEntity);
			auto area = registry->try_get<AreaComponent>(selectedEntity);
			auto draggable = registry->try_get<DraggableComponent>(selectedEntity);
			auto gravity = registry->try_get<GravityComponent>(selectedEntity);
			auto joystick = registry->try_get<JoystickComponent>(selectedEntity);
			auto ai = registry->try_get<AIComponent>(selectedEntity);
			auto cutscene = registry->try_get<CutsceneComponent>(selectedEntity);

			// TODO: Make this better/automatic
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
			if (gravity != nullptr)
				DrawGravityComponent(*gravity);
			if (joystick != nullptr)
				DrawJoystickComponent(*joystick);
			if (ai != nullptr)
				DrawAIComponent(*ai);
			if (cutscene != nullptr)
				DrawCutsceneComponent(*cutscene);

			GameInspectorPanel::DrawComponents();

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

	void InspectorPanel::DrawIdentityComponent(IdentityComponent& identity)
	{
		DrawComponentStart();

		auto opened = ImGui::TreeNodeEx(ICON_FA_TAG " Identity Component", ImGuiTreeNodeFlags_AllowItemOverlap);
		
		if (opened)
		{
			std::string idStr = "ID: " + identity.id;
			std::string nameStr = "Name: " + identity.name;

			ImGui::Text(idStr.c_str());
			ImGui::Text(nameStr.c_str());

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

	void InspectorPanel::DrawDraggableComponent(DraggableComponent& draggable)
	{
		DrawComponentStart();

		auto opened = ImGui::TreeNode(ICON_FA_HAND_BACK_FIST " Draggable Component");

		DrawOptionsPopup<DraggableComponent>(draggable);

		if (opened)
		{
			ImGui::Checkbox("Dragging", &draggable.dragging);
			EditorDraw::Vec3Control("Drag Offset", draggable.offset);

			ImGui::TreePop();
		}

		DrawComponentEnd();
	}

	void InspectorPanel::DrawRendererComponent(SpriteRendererComponent& renderer)
	{
		DrawComponentStart();

		auto opened = ImGui::TreeNode(ICON_FA_IMAGE " Renderer Component");

		DrawOptionsPopup<SpriteRendererComponent>(renderer);

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

	void InspectorPanel::DrawGravityComponent(GravityComponent& gravity)
	{
		DrawComponentStart();

		auto opened = ImGui::TreeNode(ICON_FA_ANGLES_DOWN " Gravity Component");

		DrawOptionsPopup<GravityComponent>(gravity);

		if (opened)
		{
			ImGui::DragFloat("Value", &gravity.value);
			//ImGui::Checkbox("Dragging", &draggable.dragging);
			//EditorDraw::Vec3Control("Drag Offset", draggable.offset);

			ImGui::TreePop();
		}

		DrawComponentEnd();
	}

	void InspectorPanel::DrawJoystickComponent(JoystickComponent& joystick)
	{
		DrawComponentStart();

		auto opened = ImGui::TreeNode(ICON_FA_GAMEPAD " Joystick Component");

		DrawOptionsPopup<JoystickComponent>(joystick);

		if (opened)
		{
			//ImGui::DragFloat("Value", &gravity.value);
			//ImGui::Checkbox("Dragging", &draggable.dragging);
			//EditorDraw::Vec3Control("Drag Offset", draggable.offset);

			ImGui::TreePop();
		}

		DrawComponentEnd();
	}

	void InspectorPanel::DrawAIComponent(AIComponent& ai)
	{
		DrawComponentStart();

		auto opened = ImGui::TreeNode(ICON_FA_BRAIN " AI Component");

		DrawOptionsPopup<AIComponent>(ai);

		if (opened)
		{
			//ImGui::DragFloat("Value", &gravity.value);
			//ImGui::Checkbox("Dragging", &draggable.dragging);
			//EditorDraw::Vec3Control("Drag Offset", draggable.offset);

			ImGui::TreePop();
		}

		DrawComponentEnd();
	}

	void InspectorPanel::DrawCutsceneComponent(CutsceneComponent& cutscene)
	{
		DrawComponentStart();

		auto opened = ImGui::TreeNode(ICON_FA_CLAPPERBOARD " Cutscene Component");

		DrawOptionsPopup<CutsceneComponent>(cutscene);

		if (opened)
		{
			//ImGui::DragFloat("Value", &gravity.value);
			//ImGui::Checkbox("Dragging", &draggable.dragging);
			//EditorDraw::Vec3Control("Drag Offset", draggable.offset);

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
			ImGui::Text("Engine:");

			DisplayAddComponentEntry<IdentityComponent>(ICON_FA_TAG "  Identity");
			DisplayAddComponentEntry<TransformComponent>(ICON_FA_ARROWS_UP_DOWN_LEFT_RIGHT "  Transform");
			DisplayAddComponentEntry<SpriteRendererComponent>(ICON_FA_IMAGE "  Renderer");
			DisplayAddComponentEntry<AreaComponent>(ICON_FA_TABLE_CELLS "  Area");
			DisplayAddComponentEntry<DraggableComponent>(ICON_FA_HAND_BACK_FIST "  Draggable");
			DisplayAddComponentEntry<GravityComponent>(ICON_FA_ANGLES_DOWN "  Gravity");
			DisplayAddComponentEntry<JoystickComponent>(ICON_FA_GAMEPAD "  Joystick");
			DisplayAddComponentEntry<AIComponent>(ICON_FA_BRAIN "  AI");
			DisplayAddComponentEntry<CutsceneComponent>(ICON_FA_CLAPPERBOARD "  Cutscene");
			//DisplayAddComponentEntry<CollisionComponent>("Collision");

			ImGui::Separator();
			ImGui::Spacing();

			GameInspectorPanel::DrawAddComponentPopup();
			// NOTE: Can't add empty components! entt won't allow it!

			ImGui::EndPopup();
		}
	}

	//template<typename T>
	//void InspectorPanel::DisplayAddComponentEntry(const std::string& entryName)
	//{
	//	auto registry = ECS::GetRegistry();
	//	auto comp = registry->try_get<T>(currentEntity);

	//	// if we don't have the component, we show the menu to be able to add it to the entity
	//	if (comp == nullptr)
	//	{
	//		if (ImGui::MenuItem(entryName.c_str()))
	//		{
	//			T newComp = T();

	//			ECS::AddComponent<T>(currentEntity, newComp);
	//			ImGui::CloseCurrentPopup();
	//		}
	//	}
	//}

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
