//
// Created by Diego S. Seabra on 31/01/23
//

#ifndef FEATHER_INSPECTOR_PANEL_H
#define FEATHER_INSPECTOR_PANEL_H

// ECS components
class IdentityComponent;
class TransformComponent;
class AreaComponent;
class SpriteRendererComponent;
class DraggableComponent;
class GravityComponent;
class JoystickComponent;
class AIComponent;
class CutsceneComponent;

#include <entt/entt.hpp>

#include "../../components/Components.h"
#include "../EditorDraw.h"

namespace Feather
{
	class InspectorPanel
	{
	public:
		void Draw(entt::entity selectedEntity);

		static void DrawComponentStart();
		static void DrawComponentEnd();

		static entt::entity GetSelectedEntity() { return currentEntity; }

		template <typename T>
		static void DisplayAddComponentEntry(const std::string& entryName)
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

	private:
		void DrawIdentityComponent(IdentityComponent& identity);
		void DrawTransformComponent(TransformComponent& transform);
		void DrawAreaComponent(AreaComponent& area);
		void DrawDraggableComponent(DraggableComponent& draggable);
		void DrawRendererComponent(SpriteRendererComponent& renderer);
		void DrawGravityComponent(GravityComponent& gravity);
		void DrawJoystickComponent(JoystickComponent& joystick);
		void DrawAIComponent(AIComponent& ai);
		void DrawCutsceneComponent(CutsceneComponent& cutscene);

		void DrawAddComponentPopup();

		bool noSelection = false;
		inline static entt::entity currentEntity{};

		template <typename T>
		void DrawOptionsPopup(T t);
	};
}

#endif //FEATHER_INSPECTOR_PANEL_H

