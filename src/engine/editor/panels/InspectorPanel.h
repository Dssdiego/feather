//
// Created by Diego S. Seabra on 31/01/23
//

#ifndef FEATHER_INSPECTOR_PANEL_H
#define FEATHER_INSPECTOR_PANEL_H

// ecs components
class Identity;
class TransformComponent;
class AreaComponent;
class AtlasSpriteRenderer;
class Draggable;
class Gravity;

#include <entt/entt.hpp>

namespace Feather
{
	class InspectorPanel
	{
	public:
		void Draw(entt::entity selectedEntity);

	private:
		void DrawIdentityComponent(Identity& identity);
		void DrawTransformComponent(TransformComponent& transform);
		void DrawAreaComponent(AreaComponent& area);
		void DrawDraggableComponent(Draggable& draggable);
		void DrawRendererComponent(AtlasSpriteRenderer& renderer);
		void DrawGravityComponent(Gravity& gravity);

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

