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

namespace Feather
{
	class InspectorPanel
	{
	public:
		void Draw(entt::entity selectedEntity);

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

		void DrawComponentStart();
		void DrawComponentEnd();

		void DrawAddComponentPopup();

		bool noSelection = false;
		entt::entity currentEntity{};

		// templates
		template <typename T>
		void DisplayAddComponentEntry(const std::string& entryName);

		template <typename T>
		void DrawOptionsPopup(T t);
	};
}

#endif //FEATHER_INSPECTOR_PANEL_H

