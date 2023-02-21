//
// Created by Diego S. Seabra on 31/01/23
//

#ifndef FEATHER_INSPECTOR_PANEL_H
#define FEATHER_INSPECTOR_PANEL_H

// ecs components
class IdentityComponent;
class TransformComponent;
class AreaComponent;
class SpriteRendererComponent;
class DraggableComponent;
class GravityComponent;

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

