//
// Created by Diego S. Seabra on 24/01/23
//

#include "DragSystem.h"
#include "../entities/ECS.h"

namespace Feather
{
	// REVIEW: This should be in a EntitySystem Update()?
	void DragSystem::Update()
	{
		auto mousePositionScreen = Input::GetMouseWorldPosition();
		glm::vec3 mousePos = glm::vec3(mousePositionScreen.x, mousePositionScreen.y, 0.f);

		auto view = ECS::GetRegistry()->view<const IdentityComponent, AreaComponent, TransformComponent, DraggableComponent>();
		view.each([mousePos](const auto& identity, auto& area, auto& transform, auto& draggable)
			{
				auto inside = AreaSystem::IsInside(area, mousePos);

		if (inside && Input::IsMouseButtonPressed(MouseButton::BUTTON_1))
		{
			draggable.offset = transform.position - mousePos;
			transform.position = mousePos;
		}
			});
	}
}
